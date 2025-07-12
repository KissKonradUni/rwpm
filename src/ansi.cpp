#include "ansi.hpp"

namespace rwpm {

void ansi::setConsoleOutput() {
    SetConsoleOutputCP( 65001 );
}

HANDLE ansi::getInputHandle() {
    return GetStdHandle(STD_INPUT_HANDLE);
}

HANDLE ansi::getOutputHandle() {
    return GetStdHandle(STD_OUTPUT_HANDLE);
}

iVec2 ansi::getConsoleSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(getOutputHandle(), &csbi);
    return iVec2(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
}

void ansi::setInputMode(const bool raw) {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    
    if (raw) {
        mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
        printf("%s", ANSI_HIDE_CURSOR.c_str());
    } else {
        mode |= (ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
        printf("%s", ANSI_SHOW_CURSOR.c_str());
    }
    
    SetConsoleMode(hStdin, mode);
}

void ansi::clearScreen() {
    printf("%s", ANSI_CLEAR_SCREEN.c_str());
}

void ansi::moveCursor(const uint32_t x, const uint32_t y) {
    printf("\033[%d;%dH", y, x);
}

void ansi::drawBorder(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) {
    moveCursor(x, y);
    
    printf("%s", ANSI_BORDER_TOP_LEFT.c_str());
    for (uint32_t i = 0; i < width - 2; ++i) {
        printf("%s", ANSI_BORDER_TOP.c_str());
    }
    printf("%s\n", ANSI_BORDER_TOP_RIGHT.c_str());

    for (uint32_t i = 0; i < height - 2; ++i) {
        moveCursor(x, y + i + 1);

        printf("%s", ANSI_BORDER_LEFT.c_str());
        for (uint32_t j = 0; j < width - 2; ++j) {
            printf(" ");
        }
        printf("%s", ANSI_BORDER_RIGHT.c_str());
    }

    moveCursor(x, y + height - 1);

    printf("%s", ANSI_BORDER_BOTTOM_LEFT.c_str());
    for (uint32_t i = 0; i < width - 2; ++i) {
        printf("%s", ANSI_BORDER_BOTTOM.c_str());
    }
    printf("%s\n", ANSI_BORDER_BOTTOM_RIGHT.c_str());
}

void ansi::drawWindow(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const std::string_view title, const std::string_view description) {
    drawBorder(x, y, width, height);
    
    moveCursor(x + 2, y);
    printf("%s", title.data());
    
    if (!description.empty()) {
        moveCursor(x + 2, y + height - 1);
        printf("%s", description.data());
    }

    moveCursor(1, y + height + 1);
}

}
