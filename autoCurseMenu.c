/* Alexander Simchuk, autoCurseMenu.c
 * Methods for functions defined in autoCurseMenu.h
 */
#include "autoCurseMenu.h"

#define HEIGHT 20
#define WIDTH 40

#define CENTER_Y ((LINES - HEIGHT) / 2)
#define CENTER_X ((COLS - WIDTH) / 2)

#define MENU_BOX_HEIGHT (HEIGHT - 6)
#define MENU_BOX_WIDTH (WIDTH - 4)
#define MENU_CENTER_Y ((LINES - MENU_BOX_HEIGHT) / 2)
#define MENU_CENTER_X ((COLS - MENU_BOX_WIDTH) / 2)

#define BOTTOM_MENU ((MENU_CENTER_Y + MENU_BOX_HEIGHT))

#define BOTTOM_MENU_WIDTH (MENU_BOX_WIDTH / 4)
#define BOTTOM_MENU_CENTER_X ((COLS - BOTTOM_MENU_WIDTH) / 2)

void initPanels(PANEL ** panel,WINDOW ** win, int win_total)
{
    int cntr;
    
    //attach the windows to individual panels
    for(cntr = 0; cntr < win_total; cntr++)
    {
        panel[cntr] = new_panel(win[cntr]);
    }

    //set last two panels to point to each other, for tabbing feature
    set_panel_userptr(panel[2],panel[3]);
    set_panel_userptr(panel[3],panel[2]);
}

void initWindows(WINDOW ** win,int win_total)
{
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_WHITE);
    init_pair(4, COLOR_CYAN, COLOR_CYAN);

    switch(win_total)
    {
        case 4: //basic single menu window
        {
            //main menu drop-shadow
            win[0] = newwin(HEIGHT,WIDTH,CENTER_Y + 1,CENTER_X + 2);
            wbkgd(win[0],COLOR_PAIR(2));
            //main menu border
            win[1] = newwin(HEIGHT,WIDTH,CENTER_Y,CENTER_X);
            wbkgd(win[1],COLOR_PAIR(3));
            //main menu
            win[2] = newwin(MENU_BOX_HEIGHT,MENU_BOX_WIDTH,MENU_CENTER_Y,MENU_CENTER_X);
            wbkgd(win[2],COLOR_PAIR(4));
            //cancel button
            win[3] = newwin(1,BOTTOM_MENU_WIDTH,BOTTOM_MENU + 1,BOTTOM_MENU_CENTER_X);
            wbkgd(win[3],COLOR_PAIR(4));
            break;
        }
        default:
            printf("initWindows: invalid option for win_total.\n");
    }
}

void freePanels(PANEL ** panel,WINDOW ** win, int win_total)
{
    int cntr;

    freeWindows(win,win_total);

    for(cntr = 0; cntr < win_total; cntr++)
    {
        if(!del_panel(panel[cntr]))
        {
            printf("ERROR: could not delete panel %d.\n",cntr);
        }
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
void setColorScheme(WINDOW ** win, int first_pair, int second_pair);
*/
