#pragma once

#include <string_view>
#include <cstdint>
#include <vector>

#include "input.hpp"

namespace rwpm {

typedef void (*MenuOptionCallbackFunction)();

struct MenuOption {
    std::string_view text;
    MenuOptionCallbackFunction callback;

    MenuOption(std::string_view t, MenuOptionCallbackFunction cb)
        : text(t), callback(cb) {}
};

class Menu {
public:
    Menu(rwpm::InputHandler* inputHandler, const uint32_t x, const uint32_t y, const std::vector<MenuOption>& options);
    ~Menu();

    void drawMenu();
    inline bool isFinished() const { return this->mIsFinished; }
private:
    rwpm::InputHandler* mInputHandler;
    uint32_t mX, mY;
    uint32_t mSelection;
    uint32_t mLength;
    bool mIsFinished = false;
    std::vector<MenuOption> mOptions;

    static void menuUp(Menu* context);
    static void menuDown(Menu* context);
    static void menuSelect(Menu* context);
    static void menuExit(Menu* context);
};

}
