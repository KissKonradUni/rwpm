#include "io.hpp"
#include "ui.hpp"
#include "ansi.hpp"
#include "input.hpp"

bool shouldClose = false;

void firstMenu(rwpm::InputHandler* inputHandler) {
    using namespace rwpm;
    
    Menu testMenu(
        inputHandler,
        4, 3,
        {
            MenuOption("Option 1", []() { 
                ansi::moveCursor(4, 18);
                printf("%sOption 1 selected%s\n", ANSI_GREEN.c_str(), ANSI_RESET.c_str()); 
            }),
            MenuOption("Option 2", []() { 
                ansi::moveCursor(4, 18);
                printf("%sOption 2 selected%s\n", ANSI_GREEN.c_str(), ANSI_RESET.c_str()); 
            }),
            MenuOption("Exit", []() { 
                shouldClose = true;
            })
        }
    );

    while (!testMenu.isFinished() && !shouldClose) {
        testMenu.drawMenu();
        inputHandler->updateEvents();

        Sleep(10);
    }
}

int main(void) {
    using namespace rwpm;

    ansi::setConsoleOutput();
    ansi::setInputMode(true);
    iVec2 consoleSize = ansi::getConsoleSize();
    consoleSize.y -= 2;

    ansi::clearScreen();
    
    std::string title = ANSI_RESET + "[ " + ANSI_YELLOW + "RWPM" + ANSI_RESET + " ]";
    std::string desc  = ANSI_RESET + "[ " + ANSI_CYAN + "Press ESC to exit." + ANSI_RESET + " ]";
    ansi::drawWindow(1, 1, 40, 20, title, desc);

    Config config;
    InputHandler inputHandler;

    firstMenu(&inputHandler);
    
    Sleep(1000);
    
    rwpm::ansi::setInputMode(false);
    rwpm::ansi::clearScreen();

    FlushConsoleInputBuffer(ansi::getInputHandle());

    return 0;    
}
