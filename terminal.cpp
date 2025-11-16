#include "terminal.hpp" 
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include "ansi_terminal.hpp" 

using namespace Ansi;
using namespace std;

static struct termios defaultMode;
void disableRawMode()
{

    tcsetattr(STDIN_FILENO, TCSANOW, &defaultMode);
    

    cout << CLEAR_SCREEN << CURSOR_HOME << RESET << SHOW_CURSOR << flush;
}


void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &defaultMode);
    atexit(disableRawMode);
    struct termios rawMode = defaultMode;
    rawMode.c_lflag &= ~(ICANON | ECHO | ISIG);
    rawMode.c_cc[VMIN] = 0;
    rawMode.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &rawMode);
}