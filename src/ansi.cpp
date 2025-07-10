#include "ansi.hpp"

namespace rwpm {

void ansi::setConsoleOutput() {
    SetConsoleOutputCP( 65001 );
}

iVec2 ansi::getConsoleSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return iVec2(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
}

void ansi::clearScreen() {
    printf("%s", ANSI_CLEAR_SCREEN.c_str());
}

void ansi::moveCursor(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void ansi::setInputMode(bool raw) {
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

void ansi::drawBorder(int x, int y, int width, int height) {
    moveCursor(x, y);
    
    printf("%s", ANSI_BORDER_TOP_LEFT.c_str());
    for (int i = 0; i < width - 2; ++i) {
        printf("%s", ANSI_BORDER_TOP.c_str());
    }
    printf("%s\n", ANSI_BORDER_TOP_RIGHT.c_str());

    for (int i = 0; i < height - 2; ++i) {
        moveCursor(x, y + i + 1);

        printf("%s", ANSI_BORDER_LEFT.c_str());
        for (int j = 0; j < width - 2; ++j) {
            printf(" ");
        }
        printf("%s", ANSI_BORDER_RIGHT.c_str());
    }

    moveCursor(x, y + height - 1);

    printf("%s", ANSI_BORDER_BOTTOM_LEFT.c_str());
    for (int i = 0; i < width - 2; ++i) {
        printf("%s", ANSI_BORDER_BOTTOM.c_str());
    }
    printf("%s\n", ANSI_BORDER_BOTTOM_RIGHT.c_str());
}

}