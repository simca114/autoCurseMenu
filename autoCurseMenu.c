/* Alexander Simchuk, autoCurseMenu.c
 * Methods for functions defined in autoCurseMenu.h
 */


void initWindows(WINDOW ** win,int win_total)
{

    switch(win_total):
        case 4: //basic single menu window
        {
            //main background
            win[0] = newwin(COLS,LINES,0,0);
            wbkgd(win[0],COLOR_PAIR(1));
            //main menu drop-shadow
            //main menu border
            //main menu
            

            break;
        }
        default:
            printf("initWindows: invalid option for win_total.\n");
}

/*
void initPanels(PANEL ** panel,WINDOW ** win, int win_total);
void freeWindows(WINDOW ** win, win_total);
void freePanels(PANEL ** panel);
void setColorScheme(WINDOW ** win, int first_pair, int second_pair);
*/
