#pragma once

#include <string>

namespace rwpm {

#define CONFIG_FILE_NAME ".rwpm"

#define CONFIG_STEAM_LIBRARY_PATH "steamLibraryPath"
#define DEFAULT_STEAM_LIBRARY_PATH "C:\\Program Files (x86)\\Steam\\steamapps\\common\\"

#define CONFIG_PACK_STORAGE_PATH "packStoragePath"
#define DEFAULT_PACK_STORAGE_PATH ".\\Packs\\"

class Config {
public:
    bool isValid = false;
    std::string steamLibraryPath;
    std::string packStoragePath;

    Config();
    ~Config();
    
    void load();
    void save() const;
private:
};

}
