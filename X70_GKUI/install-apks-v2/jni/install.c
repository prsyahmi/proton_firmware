#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void installFile(char* path) {
    char command[256];
    sprintf(command, "/system/bin/pm install \"%s\"", path);
    system(command);
}

void traverseDir(char* dirPath, char* apks[], int* count) {
    DIR* dir;
    struct dirent* entry;

    if ((dir = opendir(dirPath)) == NULL) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Ignore directories ".", ".." and anything start with "."
        printf("%s\n", entry->d_name);
        if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
            char subDirPath[256];
            sprintf(subDirPath, "%s/%s", dirPath, entry->d_name);
            traverseDir(subDirPath, apks, count);
        } else if (entry->d_type == DT_REG) {
            char* fileName = entry->d_name;
            size_t len = strlen(fileName);
            if (len > 4 && strcmp(&fileName[len-4], ".apk") == 0) {
                char filePath[256];
                sprintf(filePath, "%s/%s", dirPath, fileName);
                apks[(*count)++] = strdup(filePath);
            }
        }
    }

    closedir(dir);
}

int main() {
    char* apks[1000];
    int count = 0;

    traverseDir("/storage/udisk/apk", apks, &count);
    traverseDir("/storage/udisk1/apk", apks, &count);
    traverseDir("/storage/udisk2/apk", apks, &count);

    printf("clear_display\n");

    printf("ui_print Total APKs %d\n", count);

    system("settings put global install_non_market_apps 1");

    for (int i = 0; i < count; i++) {
        printf("ui_print installing %s\n", apks[i]);
        installFile(apks[i]);
        free(apks[i]);

        float progress = 1.0f / count;
        printf("progress %.2f \n0 \n", 1.0f / count);

        // We don't want to restart, set upper limit to 90%
        //printf("progress %.2f\n", progress > 0.9f ? 0.9f : progress);
    }

    printf("ui_print Done\n");

    // Fail intentionally to avoid restart
    return 1;
}