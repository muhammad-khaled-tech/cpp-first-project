#include <iostream>
#include <stdio.h>
#include "ansi_terminal.hpp"
#include "terminal.hpp"
#include "ui.hpp"
#include "input.hpp"
using namespace Ansi;
using namespace std;

int main()
{
    // --- 1. INITIALIZATION --- //
    init();
    enableRawMode();

    // --- 2. WELCOME SCREEN & LOADING --- //
    cout << CLEAR_SCREEN << CURSOR_HOME << flush;
    cout << move(5, 20) << BG_BLUE << FG_BRIGHT_WHITE << BOLD << "  WELCOME TO LINUX SYSTEM  " << RESET;
    cout << move(7, 21) << FG_CYAN << "Initializing components..." << RESET;
    cout << move(35, 1) << FG_CYAN << "BY:Mohamed Khaled" << RESET;
    cout << move(35, 50) << FG_CYAN << "Supervised by Eng/Mina Nagy <3" << RESET;

    for (int i = 0; i <= 100; i += 2)
    {
        drawLoadingBar(i);
        sleep_ms(50);
    }
    cout << move(12, 25) << FG_BRIGHT_GREEN << "Done! Starting App..." << RESET;
    sleep_ms(1000);

    // --- 3. MAIN MENU LOOP (LOGIC) --- //
    int selectedOption = 0;
    const int totalOptions = getTotalOptions();
    std::cout << CLEAR_SCREEN << HIDE_CURSOR << std::flush;

    bool isRunning = true;
    while (isRunning)
    {
        printMenu(selectedOption);

        KeyType key = readKey();

        switch (key)
        {
        case KEY_UP:
            selectedOption--;
            if (selectedOption < 0)
                selectedOption = totalOptions - 1;
            break;
        case KEY_DOWN:
            selectedOption++;
            if (selectedOption >= totalOptions)
                selectedOption = 0;
            break;
        case KEY_HOME:
            selectedOption = 0;
            break;
        case KEY_END:
            selectedOption = totalOptions - 1;
            break;
        case KEY_ENTER:
        {
            if (selectedOption == totalOptions - 1)
            {
                isRunning = false;
            }
            else
            {
                openScreen(selectedOption);
                string msg = "(Press any key...)";
                cout << move(15, 30) << FG_CYAN << msg << flush;
                while (readKey() == KEY_UNKNOWN);
            }
        }
        break;
        case KEY_ESCAPE:
            isRunning = false;
            break;
        case KEY_QUIT:
            isRunning = false;
            break;
        case KEY_UNKNOWN:
        default:
            break;
        }
    }
    std::cout << CLEAR_SCREEN << HIDE_CURSOR << std::flush;
    disableRawMode();
    return 0;
}