#include "io.hpp"
#include "ui.hpp"
#include "ansi.hpp"
#include "input.hpp"

#include <filesystem>

bool shouldClose = false;

void init() {
    using namespace rwpm;

    ansi::setConsoleOutput();
    ansi::setInputMode(true);
    ansi::clearScreen();
}

void cleanup() {
    using namespace rwpm;

    ansi::setInputMode(false);
    ansi::clearScreen();
    FlushConsoleInputBuffer(ansi::getInputHandle());
}

void detectRimWorldFolders(rwpm::Config& config, std::vector<std::string>& rimWorldFolders) {
    using namespace std::filesystem;
    using namespace rwpm;
    
    if (exists(config.steamLibraryPath)) {        
        for (const auto& entry : directory_iterator(config.steamLibraryPath)) {
            if (entry.is_directory() && entry.path().filename().string().find("RimWorld") != std::string::npos) {
                rimWorldFolders.push_back(entry.path().filename().string());
            }
        }
        if (!rimWorldFolders.empty()) {
            ansi::moveCursor(4, 3);
            printf("%sChoose a RimWorld version:%s\n", ANSI_GREEN.c_str(), ANSI_RESET.c_str());
        } else {
            ansi::moveCursor(4, 3);
            printf("%sNo RimWorld folders found!%s\n", ANSI_RED.c_str(), ANSI_RESET.c_str());
            ansi::moveCursor(4, 4);
            printf("%sDelete the .rwpm file to reset the configuration.%s\n", ANSI_YELLOW.c_str(), ANSI_RESET.c_str());
        }
    } else {
        ansi::moveCursor(4, 3);
        printf("%sInvalid Steam library path!%s\n", ANSI_RED.c_str(), ANSI_RESET.c_str());
        ansi::moveCursor(4, 4);
        printf("%sDelete the .rwpm file to reset the configuration.%s\n", ANSI_YELLOW.c_str(), ANSI_RESET.c_str());
    }
}

uint32_t versionMenu(rwpm::InputHandler* inputHandler, std::vector<std::string>& rimWorldFolders) {
    using namespace rwpm;
    
    std::vector<MenuOption> options;    

    for (size_t i = 0; i < rimWorldFolders.size(); ++i) {
        options.push_back(MenuOption(rimWorldFolders[i], []() {
            // Do nothing. We use the id at the end.
        }));
    }
    
    options.push_back(MenuOption(ANSI_RED + "Exit" + ANSI_RESET, []() { shouldClose = true; }, 1));

    Menu versionMenu(
        inputHandler,
        4, 5,
        options
    );
    versionMenu.drawMenu();

    while (!versionMenu.isFinished() && !shouldClose) {
        inputHandler->updateEvents();
        Sleep(10);
    }

    uint32_t selection = versionMenu.getSelection();
    if (selection < rimWorldFolders.size()) {
        return selection;
    } else {
        return UINT32_MAX; // Exit option selected
    }
}

uint32_t modpacksMenu(rwpm::InputHandler* inputHandler, const std::vector<std::string>& modpacks) {
    using namespace rwpm;

    std::vector<MenuOption> options;

    for (size_t i = 0; i < modpacks.size(); ++i) {
        options.push_back(MenuOption(modpacks[i], []() {
            // Do nothing. We use the id at the end.
        }));
    }

    options.push_back(MenuOption(ANSI_YELLOW + "Create new modpack" + ANSI_RESET, []() {
        // TODO: Implement modpack creation logic
        ansi::moveCursor(4, 5);
        printf("%sCreating a new modpack...%s\n", ANSI_YELLOW.c_str(), ANSI_RESET.c_str());
    }, 1));

    options.push_back(MenuOption(ANSI_RED + "Exit" + ANSI_RESET, []() { shouldClose = true; }, 1));

    Menu modpackMenu(
        inputHandler,
        4, 5,
        options
    );
    modpackMenu.drawMenu();

    while (!modpackMenu.isFinished() && !shouldClose) {
        inputHandler->updateEvents();
        Sleep(10);
    }

    uint32_t selection = modpackMenu.getSelection();
    if (selection < modpacks.size()) {
        return selection;
    } else {
        return UINT32_MAX; // Exit option selected
    }
}

int main(void) {
    using namespace rwpm;

    init();
    ansi::drawWindow(1, 1, 60, 20, "RWPM", "Press ESC to exit");

    Config config;
    InputHandler inputHandler;

    if (!config.isValid()) {
        ansi::drawWindow(1, 1, 64, 20, "Steam library path.", "Leave empty for default.");

        std::string inputLabel = "Default: " + std::string(DEFAULT_STEAM_LIBRARY_PATH);
        std::string inputText = "";
        TextInput textInput(inputLabel, &inputText, 4, 3, 58);

        if (inputText.empty()) {
            inputText = DEFAULT_STEAM_LIBRARY_PATH;
        }
        config.steamLibraryPath = inputText;

        ansi::drawWindow(1, 1, 64, 20, "Modpack storage path.", "Leave empty for default.");

        inputLabel = "Default: " + std::string(DEFAULT_PACK_STORAGE_PATH);
        inputText = "";
        TextInput textInput2(inputLabel, &inputText, 4, 3, 58);

        if (inputText.empty()) {
            inputText = DEFAULT_PACK_STORAGE_PATH;
        }
        config.packStoragePath = inputText;
        config.save();
    }

    ansi::drawWindow(1, 1, 64, 20, "RWPM", "Press ESC to exit");
    
    std::vector<std::string> rimWorldFolders;
    detectRimWorldFolders(config, rimWorldFolders);
    
    auto selectedVersion = versionMenu(&inputHandler, rimWorldFolders);
    if (selectedVersion == UINT32_MAX) {
        ansi::moveCursor(1, 22);
        printf("%sExiting...%s\n", ANSI_RED.c_str(), ANSI_RESET.c_str());
        cleanup();
        return 0;
    }

    ansi::clearScreen();
    ansi::moveCursor(1, 22);
    printf("%sSelected RimWorld version: %s%s%s\n", ANSI_GREEN.c_str(), ANSI_CYAN.c_str(), rimWorldFolders[selectedVersion].c_str(), ANSI_RESET.c_str());
    ansi::moveCursor(1, 23);

    ansi::drawWindow(1, 1, 64, 20, "RWPM", "Press ESC to exit");

    std::vector<std::string> modpacks;
    // Check the MODPACK_FOLDER/<VERSION>/ directory for modpacks
    // If the directory does not exist, create it
    // Make sure the path is valid and ends with a slash
    std::filesystem::path modpackPath = std::filesystem::path(config.packStoragePath) / rimWorldFolders[selectedVersion];
    if (!std::filesystem::exists(modpackPath)) {
        std::filesystem::create_directories(modpackPath);
    }

    for (const auto& entry : std::filesystem::directory_iterator(modpackPath)) {
        if (entry.is_directory()) {
            modpacks.push_back(entry.path().filename().string());
        }
    }

    Sleep(100);

    if (modpacks.empty()) {
        ansi::moveCursor(4, 3);
        printf("%sNo modpacks found!%s\n", ANSI_RED.c_str(), ANSI_RESET.c_str());
        ansi::moveCursor(4, 4);
        printf("%sCreate a new modpack to start.%s\n", ANSI_YELLOW.c_str(), ANSI_RESET.c_str());
    } else {
        ansi::moveCursor(4, 3);
        printf("%sChoose a modpack:%s\n", ANSI_GREEN.c_str(), ANSI_RESET.c_str());
    }

    auto selectedModpack = modpacksMenu(&inputHandler, modpacks);
    if (selectedModpack == UINT32_MAX) {
        ansi::moveCursor(1, 22);
        printf("%sExiting...%s\n", ANSI_RED.c_str(), ANSI_RESET.c_str());
        cleanup();
        return 0;
    }

    cleanup();

    return 0;    
}
