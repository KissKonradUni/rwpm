#include "ui.hpp"
#include "ansi.hpp"

#include <conio.h>

#define max(a, b) (a > b ? a : b)

namespace rwpm {

/// Menu

void Menu::drawMenu() {
    int pos = 0;
    for (size_t i = 0; i < mOptions.size(); ++i) {
        pos += mOptions[i].topMargin;
        ansi::moveCursor(mX, mY + pos++);
        if (i == mSelection) {
            printf("%s%s%s", ANSI_BG_CYAN.c_str(), mOptions[i].text.data(), ANSI_RESET.c_str());
        } else {
            printf("%s", mOptions[i].text.data());
        }
    }
    ansi::moveCursor(mX, mY + mOptions.size() + 1);
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
    FlushConsoleInputBuffer(ansi::getInputHandle());
}

void Menu::menuUp(Menu* context) {
    context->mSelection = (context->mSelection + context->mLength - 1) % context->mLength;
    context->drawMenu();
}

void Menu::menuDown(Menu* context) {
    context->mSelection = (context->mSelection + 1) % context->mLength;
    context->drawMenu();
}

void Menu::menuSelect(Menu* context) {
    if (context->mOptions[context->mSelection].callback) {
        context->mOptions[context->mSelection].callback();
    }
    context->mIsFinished = true;
}

void Menu::menuExit(Menu* context) {
    context->mIsFinished = true;
    context->mSelection = UINT32_MAX;
}

/// TextInput

TextInput::TextInput(std::string_view label, std::string* input, const uint32_t x, const uint32_t y, const uint32_t width) :
    mLabel(label), mInput(input), mX(x), mY(y), mWidth(width), mIsFinished(false)
{
    FlushConsoleInputBuffer(ansi::getInputHandle());
    ansi::setInputMode(true);

    // Print label on its own line
    ansi::moveCursor(mX, mY);
    printf("%s%s%s", ANSI_CYAN.c_str(), mLabel.data(), ANSI_RESET.c_str());
    // Print input field below label
    ansi::moveCursor(mX, mY + 1);
    printf("%s%s%*c", ANSI_BLUE.c_str(), ANSI_BG_WHITE.c_str(), mWidth, ' ');
    ansi::moveCursor(mX, mY + 1);
    printf("%s", mInput->c_str());

    this->updateInput();

    ansi::moveCursor(mX, mY + 2); // Move cursor down after input
    ansi::setInputMode(false);
    FlushConsoleInputBuffer(ansi::getInputHandle());
}

TextInput::~TextInput() {
    ansi::moveCursor(mX, mY);
    printf("%s%s%s", ANSI_CYAN.c_str(), mLabel.data(), ANSI_RESET.c_str());
    ansi::moveCursor(mX, mY + 1);
    printf("%s%s%s%s", ANSI_BLUE.c_str(), "_", ANSI_RESET.c_str(), ANSI_RESET.c_str());
    ansi::moveCursor(1, mY + 2); // Move cursor down after input
}

void TextInput::updateInput() {
    while (!mIsFinished) {
        int inputChar = _getch();

        if (inputChar == '\n' || inputChar == '\r') {
            mIsFinished = true;
            break;
        }
        else if (inputChar == VK_UP || inputChar == VK_DOWN) {
            continue;
        }
        else if (inputChar == 27) {
            mIsFinished = true;
            *mInput = "";
            break;
        }
        else if (inputChar == 8 || inputChar == 127) {
            if (!mInput->empty()) {
                mInput->pop_back();
            }
        }
        else if (inputChar < 32 || inputChar > 126) {
            continue;
        }
        else {
            *mInput += static_cast<char>(inputChar);
        }

        ansi::moveCursor(mX, mY + 1);
        printf("%s%s%*s", ANSI_BLUE.c_str(), ANSI_BG_WHITE.c_str(), mWidth, "");
        ansi::moveCursor(mX, mY + 1);
        std::string lastCharacters = "";
        if (mInput->length() > mWidth - 1) {
            lastCharacters = mInput->substr(mInput->size() - mWidth - 1);
        } else {
            lastCharacters = *mInput;
        }
        printf("%s_", lastCharacters.c_str());
        fflush(stdout);
    }

    printf("%s", ANSI_RESET.c_str());
}

}
