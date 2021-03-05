#ifndef WINDOW_H
#define WINDOW_H

#include <ncurses.h>

class Window
{
    public:
        WINDOW* window;

        int height;
        int width;
        int startY;
        int startX;
        
        Window();
        Window(int height, int width, int startY, int startX);

        int mvprintw(int height, int width, const char* text);
        
        int refresh();
        void clear();

        int deleteWindow();
};

#endif