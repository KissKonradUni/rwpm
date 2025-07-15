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
    uint8_t topMargin;

    MenuOption(std::string_view t, MenuOptionCallbackFunction cb, uint8_t margin = 0)
        : text(t), callback(cb), topMargin(margin) {}
};

class Menu {
public:
    Menu(rwpm::InputHandler* inputHandler, const uint32_t x, const uint32_t y, const std::vector<MenuOption>& options);
    ~Menu();

    void drawMenu();
    inline bool isFinished() const { return this->mIsFinished; }
    inline uint32_t getSelection() const { return mSelection; }
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

class TextInput {
public:
    TextInput(std::string_view label, std::string* input, const uint32_t x, const uint32_t y, const uint32_t width);
    ~TextInput();

    void updateInput();
    inline bool isFinished() const { return mIsFinished; }
private:
    std::string_view mLabel;
    std::string* mInput;
    uint32_t mX, mY, mWidth;
    bool mIsFinished = false;
};

}
