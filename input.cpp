#include "input.hpp"
#include "terminal.hpp"

#ifdef _WIN32
#include <conio.h>
#else
#include <stdio.h>
#include <unistd.h>
#endif

#ifdef _WIN32

KeyType readKey()
{
    if (_kbhit())
    {
        int c = _getch();

        if (c == 224 || c == 0)
        {
            int c2 = _getch();
            switch (c2)
            {
            case 72:
                return KEY_UP;
            case 80:
                return KEY_DOWN;
            case 71:
                return KEY_HOME;
            case 79:
                return KEY_END;
            default:
                return KEY_UNKNOWN;
            }
        }

        if (c == '\r')
        {
            return KEY_ENTER;
        }
        if (c == 27)
        {
            return KEY_ESCAPE;
        }
        if (c == 'q')
        {
            return KEY_QUIT;
        }

        return KEY_UNKNOWN;
    }
    else
    {
        return KEY_UNKNOWN;
    }
}

#else

KeyType readKey()
{
    int nread=0;
    char c= '\0';
    nread = read(STDIN_FILENO, &c, 1);

    if (nread <= 0)
    {
        return KEY_UNKNOWN;
    }

    if (c == '\033')
    {
        char seq[2] = {0};
        if (read(STDIN_FILENO, &seq[0], 1) == 0)
            return KEY_ESCAPE;
        if (read(STDIN_FILENO, &seq[1], 1) == 0)
            return KEY_UNKNOWN;

        if (seq[0] == '[')
        {
            switch (seq[1])
            {
            case 'A':
                return KEY_UP;
            case 'B':
                return KEY_DOWN;
            case 'H':
                return KEY_HOME;
            case 'F':
                return KEY_END;
            }
        }
        return KEY_UNKNOWN;
    }

    if (c == '\n')
    {
        return KEY_ENTER;
    }
    if (c == 'q')
    {
        return KEY_QUIT;
    }

    return KEY_UNKNOWN;
}

#endif