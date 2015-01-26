#include "autoCurseMenu.h"
#include <stdlib.h>

#define WIN_NUM 4

int main()
{
    int c;
    WINDOW *win[WIN_NUM];
    PANEL *panel[WIN_NUM];

    //initialize curses
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    //initialize color pairs
    init_pair(1, COLOR_BLACK, COLOR_BLUE);

    wbkgd(stdscr, COLOR_PAIR(1));
    refresh();

    //create windows
    initWindows(win, WIN_NUM);

    //set new windows to panels
    initPanels(panel,win,WIN_NUM);
    update_panels();
    doupdate();

    //pause program
    c = getch();

    //clean up all curses items;
    freePanels(panel,win, WIN_NUM);

    refresh();
    endwin();

    system("clear");

    return 0;
}
