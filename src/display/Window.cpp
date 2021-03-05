#include "Window.h"

Window::Window() {}

Window::Window(int height, int width, int startY, int startX)
{
    this->height = height;
    this->width = width;
    this->startY = startY;
    this->startX = startX;
    
    window = newwin(height, width, startY, startX);
    box(window, 0, 0);
    
    refresh();
}

int Window::mvprintw(int height, int width, const char* text)
{
    return mvwprintw(window, height, width, text);
}

int Window::refresh() { return wrefresh(window); }

void Window::clear()
{
    wclear(window);
    box(window, 0, 0);
    refresh();
}

int Window::deleteWindow() { return delwin(window); }