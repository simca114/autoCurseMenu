#include "autoCurseMenu.h"
#include <stdlib.h>

#define WIN_NUM 8

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
    if(initWindows(win, WIN_NUM))
    {
        endwin();
        printf("Could not initialize windows.\n");
        exit(1);
    }

    //set new windows to panels
    initPanels(panel,win,WIN_NUM);
    update_panels();
    doupdate();

    //pause program
    c = getch();

    //show popup box

    displayWindowSet(panel,1,true);
    update_panels();
    doupdate();

    c = getch();
    c = getch();

    //clean up all curses items;
    freePanels(panel,win, WIN_NUM);

    refresh();
    endwin();

    return 0;
}
