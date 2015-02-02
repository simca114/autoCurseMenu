/* Alexander Simchuk, autoCurseMenu.c
 * Methods for functions defined in autoCurseMenu.h
 */
#include "autoCurseMenu.h"

#define HEIGHT 20
#define WIDTH 40

#define START_Y ((LINES - HEIGHT) / 2)
#define START_X ((COLS - WIDTH) / 2)

#define MENU_BOX_HEIGHT (HEIGHT - 6)
#define MENU_BOX_WIDTH (WIDTH - 4)
#define MENU_START_Y ((LINES - MENU_BOX_HEIGHT) / 2)
#define MENU_START_X ((COLS - MENU_BOX_WIDTH) / 2)

#define BOTTOM_MENU ((MENU_START_Y + MENU_BOX_HEIGHT))

//BOTTOM_MENU HEIGHT is set to 1
#define BOTTOM_MENU_WIDTH (MENU_BOX_WIDTH / 4)
#define BOTTOM_MENU_START_X ((COLS - BOTTOM_MENU_WIDTH) / 2)
#define BOTTOM_MENU_START_Y (BOTTOM_MENU + 1)

#define POPUP_HEIGHT (HEIGHT / 4)
#define POPUP_WIDTH (WIDTH - 8)
//#define POPUP_WIDTH ((WIDTH / 4) * 3)
#define POPUP_START_Y ((LINES - POPUP_HEIGHT) / 2)
#define POPUP_START_X ((COLS - POPUP_WIDTH) / 2)

//POPUP_MESG_HEIGHT is set to 1
#define POPUP_MESG_WIDTH (POPUP_WIDTH - 4)
#define POPUP_MESG_START_Y (POPUP_START_Y + 1)
#define POPUP_MESG_START_X (POPUP_START_X + 2)

//POPUP_MESG_C_HEIGHT is set to 1
#define POPUP_MESG_C_WIDTH (POPUP_WIDTH / 4)
#define POPUP_MESG_C_START_Y (POPUP_START_Y + POPUP_HEIGHT - 2)

#define POPUP_MESG_C1_START_X (POPUP_START_X + 6)

#define POPUP_MESG_C2_START_X (POPUP_START_X + POPUP_MESG_WIDTH - POPUP_MESG_C_WIDTH - 2)

#define HIDE_SHOW(PAN,h_s) (h_s ? show_panel(PAN) : hide_panel(PAN))

void displayWindowSet(PANEL ** panel,int win_set, bool show)
{
    int cntr;

    switch(win_set)
    {
        case 0:
        {
            for(cntr = 0; cntr < 4; cntr++)
            {
                HIDE_SHOW(panel[cntr],show);
            }
            break;
        }
        case 1:
        {
            for(cntr = 4; cntr < 8; cntr++)
            {
                HIDE_SHOW(panel[cntr],show);
            }
            break;
        }
        default:
        {
            printf("displayWindowSet: invalid window set input\n");
        }
    }
}

void initPanels(PANEL ** panel,WINDOW ** win, int win_total)
{
    int cntr;
    
    //attach the windows to individual panels
    for(cntr = 0; cntr < win_total; cntr++)
    {
        panel[cntr] = new_panel(win[cntr]);
    }

    //auto-hide the popup box
    displayWindowSet(panel,1,FALSE);

    //set two main menu panels to point to each other, for tabbing feature
    set_panel_userptr(panel[2],panel[3]);
    set_panel_userptr(panel[3],panel[2]);

    //set popup options to be linked to each other
    set_panel_userptr(panel[6],panel[7]);
    set_panel_userptr(panel[7],panel[6]);
}

int initWindows(WINDOW ** win,int win_total)
{
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_WHITE);
    init_pair(4, COLOR_CYAN, COLOR_CYAN);

    switch(win_total)
    {
        case 8:
        {
            //confirmation popup border
            win[4] = newwin(POPUP_HEIGHT,POPUP_WIDTH,POPUP_START_Y,POPUP_START_X);
            wbkgd(win[4],COLOR_PAIR(3));
            //confirmation messsage box
            win[5] = newwin(1,POPUP_MESG_WIDTH,POPUP_MESG_START_Y,POPUP_MESG_START_X);
            wbkgd(win[5],COLOR_PAIR(4));
            //confirmation box choice 1
            win[6] = newwin(1,POPUP_MESG_C_WIDTH,POPUP_MESG_C_START_Y,POPUP_MESG_C1_START_X);
            wbkgd(win[6],COLOR_PAIR(4));
            //confirmation box choice 1
            win[7] = newwin(1,POPUP_MESG_C_WIDTH,POPUP_MESG_C_START_Y,POPUP_MESG_C2_START_X);
            wbkgd(win[7],COLOR_PAIR(4));
        }
        case 4: //basic single menu window
        {
            //main menu drop-shadow
            win[0] = newwin(HEIGHT,WIDTH,START_Y + 1,START_X + 2);
            wbkgd(win[0],COLOR_PAIR(2));
            //main menu border
            win[1] = newwin(HEIGHT,WIDTH,START_Y,START_X);
            wbkgd(win[1],COLOR_PAIR(3));
            //main menu
            win[2] = newwin(MENU_BOX_HEIGHT,MENU_BOX_WIDTH,MENU_START_Y,MENU_START_X);
            wbkgd(win[2],COLOR_PAIR(4));
            //cancel button
            win[3] = newwin(1,BOTTOM_MENU_WIDTH,BOTTOM_MENU_START_Y,BOTTOM_MENU_START_X);
            wbkgd(win[3],COLOR_PAIR(4));
            break;
        }
        default:
        {
            printf("initWindows: invalid option for win_total.\n");
            return 1;
        }
    }

    return 0;
}

void freePanels(PANEL ** panel,WINDOW ** win, int win_total)
{
    int cntr;

    freeWindows(win,win_total);

    for(cntr = 0; cntr < win_total; cntr++)
    {
        if(del_panel(panel[cntr]))
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
