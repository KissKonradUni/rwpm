#include "ui.hpp"
#include "ansi.hpp"

namespace rwpm {

void Menu::drawMenu() {
    for (size_t i = 0; i < mOptions.size(); ++i) {
        ansi::moveCursor(mX, mY + i);
        if (i == mSelection) {
            printf("%s%s%s", ANSI_BG_CYAN.c_str(), mOptions[i].text.data(), ANSI_RESET.c_str());
        } else {
            printf("%s", mOptions[i].text.data());
        }
    }
}

Menu::Menu(rwpm::InputHandler* inputHandler, const uint32_t x, const uint32_t y, const std::vector<MenuOption>& options)
    : mInputHandler(inputHandler), mX(x), mY(y), mSelection(0), mLength(options.size()), mOptions(options)
{
    mInputHandler->addEvent(VK_UP    , ((InputCallbackFunction)&Menu::menuUp    ), this);
    mInputHandler->addEvent(VK_DOWN  , ((InputCallbackFunction)&Menu::menuDown  ), this);
    mInputHandler->addEvent(VK_RETURN, ((InputCallbackFunction)&Menu::menuSelect), this);
    mInputHandler->addEvent(VK_ESCAPE, ((InputCallbackFunction)&Menu::menuExit  ), this);
}

Menu::~Menu() {
    mInputHandler->reset();
}

void Menu::menuUp(Menu* context) {
    context->mSelection = (context->mSelection + context->mLength - 1) % context->mLength;
}

void Menu::menuDown(Menu* context) {
    context->mSelection = (context->mSelection + 1) % context->mLength;
}

void Menu::menuSelect(Menu* context) {
    if (context->mOptions[context->mSelection].callback) {
        context->mOptions[context->mSelection].callback();
    }
    context->mIsFinished = true;
}

void Menu::menuExit(Menu* context) {
    context->mIsFinished = true;
}

}
