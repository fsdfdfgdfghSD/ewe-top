#include "win.h"

void init_win(void)
{
    initcsr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE)
}

void create_box(void)
{
}

ssize_t write_to_win(const char *m)
{
}
