#ifndef TERMINAL_H
#define TERMINAL_H

#include <ncurses.h>

class Terminal
{
    public:
        Terminal(bool color = false);

        int kill();
};

#endif