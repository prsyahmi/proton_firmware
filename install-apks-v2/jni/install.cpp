#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <dirent.h>
#include <cstdlib>

void installFile(std::string path) {
    std::string command = "/system/bin/pm install \"" + path + "\"";
    system(command.c_str());
}

void traverseDir(std::string dirPath, std::vector<std::string>& apks) {
    DIR* dir;
    struct dirent* entry;

    if ((dir = opendir(dirPath.c_str())) == NULL) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Ignore directories ".", ".." and anything start with "."
        if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
            std::string subDirPath = dirPath + "/" + entry->d_name;
            traverseDir(subDirPath, apks);
        } else if (entry->d_type == DT_REG) {
            std::string fileName = entry->d_name;
            size_t pos = fileName.rfind(".apk");
            if (pos != std::string::npos && pos == fileName.length() - 4) {
                std::string filePath = dirPath + "/" + fileName;
                apks.push_back(filePath);
            }
        }
    }

    closedir(dir);
}

int main() {
    float progress = 0.0f;
    std::vector<std::string> apks;

    traverseDir("/storage/udisk", apks);
    traverseDir("/storage/udisk1", apks);
    traverseDir("/storage/udisk2", apks);

    std::cout << "clear_display" << std::endl;

    for (std::string apk : apks) {
        std::cout << "ui_print installing " << apk << std::endl;
        installFile(apk);
        progress += 1.0f / apks.size();
        std::cout << "progress " << std::fixed << std::setprecision(2) << progress << std::endl;
    }

    return 0;
}