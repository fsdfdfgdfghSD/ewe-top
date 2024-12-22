#ifndef WIN_H_
#define WIN_H_

#include <ncurses.h>
#include <menu.h>
#include "../error.h"

void init_win(void);
void create_box(void);

ssize_t write_to_win(const char *m);

#endif /* WIN_H_ */
