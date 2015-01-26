/* Alexander Simchuk, autoCurseMenu.c
 * Methods for functions defined in autoCurseMenu.h
 */
#include "autoCurseMenu.h"

#define HEIGHT 20
#define WIDTH 40

#define CENTER_Y ((LINES - HEIGHT) / 2)
#define CENTER_X ((COLS - WIDTH) / 2)

void initWindows(WINDOW ** win,int win_total)
{
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_WHITE);
    init_pair(4, COLOR_CYAN, COLOR_CYAN);

    switch(win_total)
    {
        case 2: //basic single menu window
        {
            //main menu drop-shadow
            win[0] = newwin(HEIGHT,WIDTH,CENTER_Y + 1,CENTER_X + 2);
            wbkgd(win[1],COLOR_PAIR(2));
            //main menu border
            win[1] = newwin(HEIGHT,WIDTH,CENTER_Y,CENTER_X);
            wbkgd(win[1],COLOR_PAIR(3));
            //main menu
            //win[2] = newwin(HEIGHT - 2,WIDTH - 2,CENTER_Y,CENTER_X);
            //wbkgd(win[2],COLOR_PAIR(4));
            break;
        }
        default:
            printf("initWindows: invalid option for win_total.\n");
    }
}

void freeWindows(WINDOW ** win, int win_total)
{
    int cntr;

    for(cntr = 0; cntr < win_total; cntr++)
    {
        delwin(win[cntr]);
    }
}

void refreshAllWindows(WINDOW ** win, int win_total)
{
    int cntr;

    for(cntr = 0; cntr < win_total; cntr++)
    {
        wrefresh(win[cntr]);
    }
}
/*
void initPanels(PANEL ** panel,WINDOW ** win, int win_total);
void freePanels(PANEL ** panel);
void setColorScheme(WINDOW ** win, int first_pair, int second_pair);
*/
