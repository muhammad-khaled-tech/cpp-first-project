#include "ui.hpp"
#include "ansi_terminal.hpp"
#include <iostream>
#include <string>

using namespace Ansi;
using namespace std;

static const string menuOptions[] = {"1- New", "2- Display", "3- Exit"};
static const int totalOptions = sizeof(menuOptions) / sizeof(menuOptions[0]);

int getTotalOptions()
{
    return totalOptions;
}

void drawLoadingBar(int percent)
{
    cout << move(10, 21);
    cout << FG_WHITE << "[";
    int bars_count = percent / 5;
    cout << FG_BRIGHT_GREEN << BOLD;
    for (int i = 0; i < 20; ++i)
    {
        if (i < bars_count)
            cout << "=";
        else
            cout << " ";
    }
    cout << FG_WHITE << "] " << FG_BRIGHT_YELLOW << percent << "%" << RESET;
    cout << flush;
}

void printMenu(int selectedOption)
{

    std::cout << CURSOR_HOME << std::flush;
    std::cout << move(7, 30) << BG_BLUE << FG_BRIGHT_WHITE << BOLD << "====MAIN MENU====" << RESET;
    for (int i = 0; i < totalOptions; i++)
    {
        std::cout << move(9 + 2 * i, 34);

        if (selectedOption == i)
        {
            cout << BG_WHITE << FG_BLACK << BOLD << menuOptions[i] << RESET;
        }
        else
        {
            cout << FG_WHITE << menuOptions[i] << RESET;
        }
    }

    std::cout << "\033[J";

    std::cout << std::flush;
}

void openScreen(int selectedOption)
{
    cout << CLEAR_SCREEN << CURSOR_HOME << flush;
    cout << move(7, 30);
    cout << BG_BLUE << FG_BRIGHT_WHITE << BOLD << "=====" << menuOptions[selectedOption] << "=====" << RESET << flush;
}