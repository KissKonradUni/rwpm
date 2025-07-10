#include <cstdio>
#include <string>
#include <windows.h>

namespace rwpm {

static const std::basic_string<char> ANSI_RED =  "\033[31m";
static const std::basic_string<char> ANSI_GREEN =  "\033[32m";
static const std::basic_string<char> ANSI_YELLOW =  "\033[33m";
static const std::basic_string<char> ANSI_BLUE =  "\033[34m";
static const std::basic_string<char> ANSI_MAGENTA =  "\033[35m";
static const std::basic_string<char> ANSI_CYAN =  "\033[36m";
static const std::basic_string<char> ANSI_WHITE =  "\033[37m";
static const std::basic_string<char> ANSI_RESET =  "\033[0m";

static const std::basic_string<char> ANSI_BG_RED =  "\033[41m";
static const std::basic_string<char> ANSI_BG_GREEN =  "\033[42m";
static const std::basic_string<char> ANSI_BG_YELLOW =  "\033[43m";
static const std::basic_string<char> ANSI_BG_BLUE =  "\033[44m";
static const std::basic_string<char> ANSI_BG_MAGENTA =  "\033[45m";
static const std::basic_string<char> ANSI_BG_CYAN =  "\033[46m";
static const std::basic_string<char> ANSI_BG_WHITE =  "\033[47m";
static const std::basic_string<char> ANSI_BG_RESET =  "\033[49m";

static const std::basic_string<char> ANSI_BOLD =  "\033[1m";
static const std::basic_string<char> ANSI_UNDERLINE =  "\033[4m";

static const std::basic_string<char> ANSI_CLEAR_SCREEN =  "\033[2J\033[H";

static const std::basic_string<char> ANSI_HIDE_CURSOR =  "\033[?25l";
static const std::basic_string<char> ANSI_SHOW_CURSOR =  "\033[?25h";

static const std::basic_string<char> ANSI_BORDER_TOP =  "═";
static const std::basic_string<char> ANSI_BORDER_BOTTOM =  "═";
static const std::basic_string<char> ANSI_BORDER_LEFT =  "║";
static const std::basic_string<char> ANSI_BORDER_RIGHT =  "║";
static const std::basic_string<char> ANSI_BORDER_TOP_LEFT =  "╔";
static const std::basic_string<char> ANSI_BORDER_TOP_RIGHT =  "╗";
static const std::basic_string<char> ANSI_BORDER_BOTTOM_LEFT =  "╚";
static const std::basic_string<char> ANSI_BORDER_BOTTOM_RIGHT =  "╝";

struct iVec2 {
    int x, y;

    iVec2(int x = 0, int y = 0) : x(x), y(y) {}

    iVec2 operator+(const iVec2& other) const {
        return iVec2(x + other.x, y + other.y);
    }

    iVec2 operator-(const iVec2& other) const {
        return iVec2(x - other.x, y - other.y);
    }
};

class ansi {
public:
    static void setConsoleOutput();
    static iVec2 getConsoleSize();
    static void setInputMode(const bool raw);

    static void clearScreen();
    static void moveCursor(const int x, const int y);
    static void drawBorder(const int x, const int y, const int width, const int height);
};

}
