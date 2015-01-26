#include "autoCurseMenu.h"
#include <stdlib.h>

#define HEIGHT 50
#define WIDTH 40
#define WIN_NUM 3

int main()
{
    int c;
    WINDOW *win[WIN_NUM];

    //initialize curses
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    //initialize color pairs
    init_pair(1, COLOR_BLACK, COLOR_BLUE);

    wbkgd(stdscr, COLOR_PAIR(1));

    //create window
    initWindows(win, WIN_NUM);
    refresh();
    refreshAllWindows(win, WIN_NUM);


    //pause program
    c = getch();

    //clean up all curses items;
    freeWindows(win, WIN_NUM);

    refresh();
    endwin();

    system("clear");

    return 0;
}
