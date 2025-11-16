#include "terminal.hpp"
#include <iostream>
#include "ansi_terminal.hpp"
using namespace Ansi;
using namespace std;

#ifdef _WIN32

#include <windows.h>

static HANDLE hStdInput;
static DWORD defaultMode;

void disableRawMode()
{
    SetConsoleMode(hStdInput, defaultMode);
    cout << RESET << SHOW_CURSOR << CURSOR_HOME << flush;
}

void enableRawMode()
{
    hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdInput, &defaultMode);
    atexit(disableRawMode);
    DWORD rawMode = defaultMode;
    rawMode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    SetConsoleMode(hStdInput, rawMode);
}

void flushInputBuffer() {
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

#else

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static struct termios defaultMode;

void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &defaultMode);
    cout << RESET << SHOW_CURSOR << CURSOR_HOME << flush;
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

void flushInputBuffer() {
    tcflush(STDIN_FILENO, TCIFLUSH);
}

#endif

