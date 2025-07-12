#include "io.hpp"

#include <filesystem>
#include <cstring>
#include <fstream>

namespace rwpm {

Config::Config()
    : steamLibraryPath(""), packStoragePath("")
{
    load();
    printf("Loaded configuration:\n");
    printf("Steam Library Path: %s\n", steamLibraryPath.c_str());
    printf("Pack Storage Path: %s\n", packStoragePath.c_str());
}

Config::~Config() {
    save();
}

void Config::load() {
    using namespace std::filesystem;
    
    path configPath = "./" CONFIG_FILE_NAME;
    if (!exists(configPath)) {
        std::ofstream configFile(configPath);
        if (configFile.is_open()) {
            configFile << "# RWPM Configuration File\n";
            configFile << CONFIG_STEAM_LIBRARY_PATH << "=" << DEFAULT_STEAM_LIBRARY_PATH << "\n";
            configFile << CONFIG_PACK_STORAGE_PATH << "=" << DEFAULT_PACK_STORAGE_PATH << "\n";
            configFile.close();
        }
        isValid = false;
        steamLibraryPath = DEFAULT_STEAM_LIBRARY_PATH;
        packStoragePath = DEFAULT_PACK_STORAGE_PATH;
        return;
    }

    std::ifstream configFile(configPath);
    if (!configFile.is_open()) {
        isValid = false;
        return;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        if (line.starts_with(CONFIG_STEAM_LIBRARY_PATH)) {
            steamLibraryPath = line.substr(strlen(CONFIG_STEAM_LIBRARY_PATH) + 1);
        } else if (line.starts_with(CONFIG_PACK_STORAGE_PATH)) {
            packStoragePath = line.substr(strlen(CONFIG_PACK_STORAGE_PATH) + 1);
        }
    }
    configFile.close();

    isValid = !steamLibraryPath.empty() && !packStoragePath.empty();
}

void Config::save() const {
    using namespace std::filesystem;

    path configPath = "./" CONFIG_FILE_NAME;
    std::ofstream configFile(configPath);
    if (configFile.is_open()) {
        // Clear existing content
        configFile.clear();
        configFile.seekp(0, std::ios::beg);
        // Write new configuration
        configFile << "# RWPM Configuration File\n";
        configFile << CONFIG_STEAM_LIBRARY_PATH << "=" << steamLibraryPath << "\n";
        configFile << CONFIG_PACK_STORAGE_PATH << "=" << packStoragePath << "\n";
        configFile.close();
    }
}

}