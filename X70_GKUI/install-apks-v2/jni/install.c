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

int main(int argc, char** argv) {
    char* apks[1000];
    int count = 0;

    // 1 = version
    // 2 = fd
    // 3 = package name
    // 4 = retry
    int fd = atoi(argv[2]);
    FILE* f = fdopen(fd, "wb");

    setlinebuf(f);

    traverseDir("/storage/udisk/apk", apks, &count);
    traverseDir("/storage/udisk1/apk", apks, &count);
    traverseDir("/storage/udisk2/apk", apks, &count);

    fprintf(f, "clear_display\n");

    fprintf(f, "ui_print Installation will say 'failed' after done\n");
    fprintf(f, "ui_print Total APKs: %d\n", count);

    system("settings put global install_non_market_apps 1");

    for (int i = 0; i < count; i++) {
        fprintf(f, "ui_print installing %s\n", apks[i]);
        installFile(apks[i]);
        free(apks[i]);

        float progress = 1.0f / count;
        fprintf(f, "progress %.2f 0\n", 1.0f / count);

        // We don't want to restart, set upper limit to 90%
        //printf("progress %.2f\n", progress > 0.9f ? 0.9f : progress);
    }

    fprintf(f, "ui_print Done\n");

    // Fail intentionally to avoid restart
    return 1;
}