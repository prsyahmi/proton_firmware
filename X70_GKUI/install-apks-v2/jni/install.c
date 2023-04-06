#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

struct TParam {
    char* apks[1000];
    char* custom_script;
    int apk_count;
    int reboot_after_done;
};

void installFile(char* path) {
    char command[256];
    sprintf(command, "/system/bin/pm install \"%s\"", path);
    system(command);
}

void traverseDir(char* dirPath, struct TParam* param, int is_root) {
    DIR* dir;
    struct dirent* entry;
    char tmp[256];

    if ((dir = opendir(dirPath)) == NULL) {
        return;
    }

    if (is_root) {
        sprintf(tmp, "%s/_custom.sh", dirPath);
        if (access(tmp, R_OK) == 0) {
            param->custom_script = strdup(tmp);
        }

        sprintf(tmp, "%s/_reboot_after_done.txt", dirPath);
        if (access(tmp, F_OK) == 0) {
            param->reboot_after_done = 1;
        }
    }

    while ((entry = readdir(dir)) != NULL) {
        char* fileName = entry->d_name;

        if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
            // Ignore directories ".", ".." and anything start with "."
            sprintf(tmp, "%s/%s", dirPath, entry->d_name);
            traverseDir(tmp, param, 0);
        } else if (entry->d_type == DT_REG) {
            size_t len = strlen(fileName);
            if (len > 4 && strcmp(&fileName[len-4], ".apk") == 0) {
                sprintf(tmp, "%s/%s", dirPath, fileName);
                param->apks[param->apk_count++] = strdup(tmp);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char** argv) {
    struct TParam param;
    int fd;
    FILE* f;

    // 1 = version
    // 2 = fd
    // 3 = package name
    // 4 = retry
    fd = atoi(argv[2]);
    f = fdopen(fd, "wb");
    setlinebuf(f);
    
    memset(&param, 0, sizeof(struct TParam));

    traverseDir("/storage/udisk/apk", &param, 1);
    traverseDir("/storage/udisk1/apk", &param, 1);
    traverseDir("/storage/udisk2/apk", &param, 1);

    fprintf(f, "clear_display\n");

    fprintf(f, "ui_print Installation will say 'failed' after done\n");
    fprintf(f, "ui_print Total APKs: %d\n", param.apk_count);

    system("settings put global install_non_market_apps 1");

    for (int i = 0; i < param.apk_count; i++) {
        fprintf(f, "ui_print installing %s\n", param.apks[i]);
        installFile(param.apks[i]);
        free(param.apks[i]);

        float progress = 1.0f / (param.apk_count + (param.custom_script ? 1 : 0));
        fprintf(f, "progress %.2f 0\n", progress);
    }

    if (param.custom_script) {
        fprintf(f, "ui_print Executing custom script\n");
        system(param.custom_script);
        free(param.custom_script);
    }

    fprintf(f, "ui_print Done\n");

    return param.reboot_after_done ? 0 : 1;
}
