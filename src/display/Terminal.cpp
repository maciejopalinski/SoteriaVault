#include "Terminal.h"

Terminal::Terminal(bool color)
{
    initscr();
    
    raw();
    keypad(stdscr, true);
    noecho();
    cbreak();

    if(color) start_color();

    refresh();
}

int Terminal::kill() { return endwin(); }