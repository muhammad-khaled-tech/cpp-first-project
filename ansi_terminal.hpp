#pragma once

#include <string>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
#include <unistd.h>
#endif

namespace Ansi
{

    constexpr const char *RESET = "\033[0m";
    constexpr const char *CLEAR_SCREEN = "\033[2J";
    constexpr const char *CURSOR_HOME = "\033[H";
    constexpr const char *HIDE_CURSOR = "\033[?25l";
    constexpr const char *SHOW_CURSOR = "\033[?25h";

    constexpr const char *BOLD = "\033[1m";
    constexpr const char *UNDERLINE = "\033[4m";

    constexpr const char *FG_BLACK = "\033[30m";
    constexpr const char *FG_RED = "\033[31m";
    constexpr const char *FG_GREEN = "\033[32m";
    constexpr const char *FG_YELLOW = "\033[33m";
    constexpr const char *FG_BLUE = "\033[34m";
    constexpr const char *FG_MAGENTA = "\033[35m";
    constexpr const char *FG_CYAN = "\033[36m";
    constexpr const char *FG_WHITE = "\033[37m";

    constexpr const char *FG_BRIGHT_RED = "\033[1;31m";
    constexpr const char *FG_BRIGHT_GREEN = "\033[1;32m";
    constexpr const char *FG_BRIGHT_YELLOW = "\033[1;33m";
    constexpr const char *FG_BRIGHT_BLUE = "\033[1;34m";
    constexpr const char *FG_BRIGHT_CYAN = "\033[1;36m";
    constexpr const char *FG_BRIGHT_WHITE = "\033[1;37m";

    constexpr const char *BG_BLACK = "\033[40m";
    constexpr const char *BG_RED = "\033[41m";
    constexpr const char *BG_GREEN = "\033[42m";
    constexpr const char *BG_YELLOW = "\033[43m";
    constexpr const char *BG_BLUE = "\033[44m";
    constexpr const char *BG_MAGENTA = "\033[45m";
    constexpr const char *BG_CYAN = "\033[46m";
    constexpr const char *BG_WHITE = "\033[47m";

    inline std::string move(int y, int x)
    {
        std::ostringstream oss;
        oss << "\033[" << y << ";" << x << "H";
        return oss.str();
    }

    inline void init()
    {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
#endif
    }

    inline void sleep_ms(int milliseconds)
    {
#ifdef _WIN32
        Sleep(milliseconds);
#else
        usleep(milliseconds * 1000);
#endif
    }

}
