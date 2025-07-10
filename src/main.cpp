#include "ansi.hpp"
#include <cstdint>
#include <vector>

void drawMenu(const int x, const int y, const std::vector<std::string_view>& options, const uint8_t& selection) {
    using namespace rwpm;
    
    for (size_t i = 0; i < options.size(); ++i) {
        ansi::moveCursor(x, y + i * 2);
        if (i == selection) {
            printf("%s%s%s", ANSI_BG_CYAN.c_str(), options[i].data(), ANSI_RESET.c_str());
        } else {
            printf("%s", options[i].data());
        }
    }
}

int main(void) {
    using namespace rwpm;

    ansi::setConsoleOutput();
    iVec2 consoleSize = ansi::getConsoleSize();
    consoleSize.y -= 2; // Adjust for scrolling

    ansi::clearScreen();
    ansi::drawBorder(1, 1, consoleSize.x, consoleSize.y);
    ansi::moveCursor(3, consoleSize.y);
    printf("%s[Press 'q' to exit]%s", ANSI_BOLD.c_str(), ANSI_RESET.c_str());
    
    ansi::setInputMode(true);

    const auto menuOptions = std::vector<std::string_view>{
        "Option 1",
        "Option 2",
        "Option 3"
    };

    int menuSelection = 0;
    drawMenu(3, 2, menuOptions, menuSelection);

    while (true) {

        char input = getchar();
        if (input == 'q' || input == 'Q') {
            break;
        } else if (input == 'w' || input == 'W') {
            menuSelection = (menuSelection - 1 + 3) % 3;
        } else if (input == 's' || input == 'S') {
            menuSelection = (menuSelection + 1) % 3;
        }

        drawMenu(3, 2, menuOptions, menuSelection);
    }
    
    rwpm::ansi::setInputMode(false);
    return 0;    
}
