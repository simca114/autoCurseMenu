/* Alexander Simchuk, autoCurseMenu.c
 * Methods for functions defined in autoCurseMenu.h
 */
#include "autoCurseMenu.h"

#define WIN_NUM_MAIN 4
#define WIN_NUM_POPUP 3

#define HEIGHT 20
#define WIDTH 40

#define HIDE_SHOW(PAN,h_s) (h_s ? show_panel(PAN) : hide_panel(PAN))

int startY()
{
    return ((LINES - HEIGHT) / 2);    
}

int startX()
{
    return ((COLS - WIDTH) / 2);
}

int menuBoxHeight()
{
    return (HEIGHT - 6);
}

int menuBoxWidth()
{
    return (WIDTH - 4);
}

int menuStartY()
{
    return ((LINES - menuBoxHeight()) / 2);
}

int menuStartX()
{
    return ((COLS - menuBoxWidth()) / 2);
}

int bottomMenu()
{
    return (menuStartY() + menuBoxHeight());
}

int bottomMenuWidth()
{
    return (menuBoxWidth() / 4);
}

int bottomMenuStartX()
{
    return ((COLS - bottomMenuWidth()) / 2);
}

int bottomMenuStartY()
{
    return (bottomMenu() + 1);
}

int popupHeight()
{
    return (HEIGHT / 4);
}

int popupWidth()
{
    return (WIDTH - 8);
}

int popupStartY()
{
    return ((LINES - popupHeight()) / 2);
}

int popupStartX()
{
    return ((COLS - popupWidth()) / 2);
}

int popupMesgWidth()
{
    return (popupWidth() - 4);
}

int popupMesgStartY()
{
    return (popupStartY() + 1);
}

int popupMesgStartX()
{
    return (popupStartX() + 2);
}

int popupMesgChoiceWidth()
{
    return (popupWidth() / 4);
}

int popupMesgChoiceStartY()
{
    return (popupStartY() + popupHeight() - 2);
}

int popupMesgChoice1StartX()
{
    return (popupStartX() + 6);
}

int popupMesgChoice2StartX()
{
    return (popupStartX() + popupMesgWidth() - popupMesgChoiceWidth() - 2);
}

void displayPanelSet(PANEL ** panel,int panel_total, bool show)
{
    int cntr;

    for(cntr = 0; cntr < panel_total; cntr++)
    {
        HIDE_SHOW(panel[cntr],show);
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

}

int initWindows(WINDOW ** win1,WINDOW ** win2)
{
    //main menu drop-shadow
    win1[0] = newwin(HEIGHT,WIDTH,startY() + 1,startX() + 2);
    wbkgd(win1[0],COLOR_PAIR(2));
    //main menu border
    win1[1] = newwin(HEIGHT,WIDTH,startY(),startX());
    wbkgd(win1[1],COLOR_PAIR(3));
    //main menu
    win1[2] = newwin(menuBoxHeight(),menuBoxWidth(),menuStartY(),menuStartX());
    wbkgd(win1[2],COLOR_PAIR(4));
    //cancel button
    win1[3] = newwin(1,bottomMenuWidth(),bottomMenuStartY(),bottomMenuStartX());
    wbkgd(win1[3],COLOR_PAIR(4));

    //confirmation popup dropshadow
    win2[0] = newwin(popupHeight(),popupWidth(),popupStartY() + 1,popupStartX() + 2);
    wbkgd(win2[0],COLOR_PAIR(2));
    //confirmation popup border
    win2[1] = newwin(popupHeight(),popupWidth(),popupStartY(),popupStartX());
    wbkgd(win2[1],COLOR_PAIR(3));
    //confirmation messsage box
    win2[2] = newwin(1,popupMesgWidth(),popupMesgStartY(),popupMesgStartX());
    wbkgd(win2[2],COLOR_PAIR(4));

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
    for(cntr = win_total - 1; cntr >= 0; cntr--)
    {
        if((delwin(win[cntr])) == ERR)
        {
            printf("freeWindows: delwin() returned error for window %d\n",cntr);
        }
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

ITEM * initItems(char * menu_option)
{
    return new_item(menu_option,"");
}

//void initMenu(MENU ** menu,PANEL ** panel,ITEM ** items,char ** menu_options,int num_options);
void initMenuPopup(MENU ** menu, WINDOW * win, ITEM ** item1, ITEM ** item2, char ** menu_options)
{
    item1[0] = initItems(menu_options[0]);
    item1[1] = (ITEM *)NULL;
    item2[0] = initItems(menu_options[1]);
    item2[1] = (ITEM *)NULL;

    menu[0] = new_menu((ITEM**)item1);
    menu[1] = new_menu((ITEM**)item2);

    //setup < yes > box
    set_menu_win(menu[0], win);
    set_menu_sub(menu[0],subwin(win,1,popupMesgChoiceWidth(),popupMesgChoiceStartY(),popupMesgChoice1StartX()));

    //setup < no > box
    set_menu_win(menu[1], win);
    set_menu_sub(menu[1],subwin(win,1,popupMesgChoiceWidth(),popupMesgChoiceStartY(),popupMesgChoice2StartX()));

    set_menu_format(menu[0], 1, 1);
    set_menu_format(menu[1], 1, 1);

    set_menu_mark(menu[0],"");
    set_menu_mark(menu[1],"");

    set_menu_fore(menu[0],COLOR_PAIR(4));
    set_menu_fore(menu[1],COLOR_PAIR(4));

    set_menu_back(menu[0],COLOR_PAIR(1));
    set_menu_back(menu[1],COLOR_PAIR(1));

    post_menu(menu[0]);
    post_menu(menu[1]);
    wrefresh(win);
}

void freeItems(ITEM ** items,int num_options)
{
    int cntr;

    for(cntr = 0;cntr < num_options;cntr++)
    {
        free_item(items[cntr]);
    }
}

void freeMenu(MENU ** menu,ITEM ** item1,ITEM ** item2,int num_options)
{
    unpost_menu(menu[0]);
    unpost_menu(menu[1]);

    freeItems(item1,num_options);
    freeItems(item2,num_options);

    delwin(menu_sub(menu[1]));
    delwin(menu_sub(menu[0]));

    free_menu(menu[1]);
    free_menu(menu[0]);
}

int mainMenu(char ** menu_options)
{
    if(!menu_options)
    {
        printf("mainmenu: inputted char** is null\n");
        return 1;
    }

    int c;
    WINDOW *win_main[WIN_NUM_MAIN], *win_popup[WIN_NUM_POPUP];
    PANEL *panel_main[WIN_NUM_MAIN], *panel_popup[WIN_NUM_POPUP];
    ITEM ** items_main, *items_popup1[1], *items_popup2[1];
    MENU *menu_main[2], *menu_popup[2];
    char *popup_options[2], choice;

    popup_options[0] = "< Yes >";
    popup_options[1] = "< No >";

    //initialize curses
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, true);

    //initialize color pairs
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_WHITE);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);

    wbkgd(stdscr, COLOR_PAIR(1));
    refresh();

    //create windows
    if(initWindows(win_main, win_popup))
    {
        endwin();
        printf("mainMenu: Could not initialize main windows.\n");
        return 2;
    }

    //set new windows to panels
    initPanels(panel_main,win_main,WIN_NUM_MAIN);
    initPanels(panel_popup,win_popup,WIN_NUM_POPUP);

    //create main menu
    //
    //create popup menu
    initMenuPopup(menu_popup, panel_window(panel_popup[1]), items_popup1, items_popup2, popup_options);

    choice = 'n';
    while(choice == 'n')
    {
        displayPanelSet(panel_popup,WIN_NUM_POPUP,false);
        update_panels();
        doupdate();

        c = getch();

        //show popup box
        displayPanelSet(panel_popup,WIN_NUM_POPUP,true);
        update_panels();
        doupdate();

        choice = popupMenu(menu_popup,panel_window(panel_popup[1]),panel_window(panel_popup[2]),"TestPhrase");
    }
    

    //clean up all curses items;
    freeMenu(menu_popup,items_popup1,items_popup2,1);

    freePanels(panel_popup,win_popup, WIN_NUM_POPUP);
    freePanels(panel_main,win_main, WIN_NUM_MAIN);
    refresh();
    endwin();

    return 0;
}

char popupMenu(MENU ** menu,WINDOW * menu_win,WINDOW * mesg_win,char * choice)
{
    int c = 0;
    MENU * current_menu = menu[0];

    wprintw(mesg_win," %s , are you sure?",choice);
    wrefresh(mesg_win);

    keypad(menu_sub(current_menu),TRUE);
    wbkgd(menu_sub(current_menu),COLOR_PAIR(1));
    wrefresh(menu_win);

    while(c = wgetch(menu_sub(current_menu))) 
    {
        switch(c)
        {
            case KEY_LEFT:
            {
                keypad(menu_sub(current_menu),FALSE);
		wbkgd(menu_sub(current_menu),COLOR_PAIR(4));
		wrefresh(menu_sub(current_menu));
                current_menu = menu[0];
                keypad(menu_sub(current_menu),TRUE);
		wbkgd(menu_sub(current_menu),COLOR_PAIR(1));
		wrefresh(menu_sub(current_menu));
                break;
            }
            case KEY_RIGHT:
            {
                keypad(menu_sub(current_menu),FALSE);
		wbkgd(menu_sub(current_menu),COLOR_PAIR(4));
		wrefresh(menu_sub(current_menu));
                current_menu = menu[1];
                keypad(menu_sub(current_menu),TRUE);
		wbkgd(menu_sub(current_menu),COLOR_PAIR(1));
		wrefresh(menu_sub(current_menu));
                break;
            }
            case 27:
            {
                return 'n';
            }
            case 10:
            {
		wbkgd(menu_sub(current_menu),COLOR_PAIR(4));
                if((strcmp(item_name(current_item(current_menu)),"< Yes >")) == 0)
                {
                    return 'y';
                }
                else
                {
                    return 'n';
                }
            }
        }
        wrefresh(menu_win);
    }

    return 'n';
}
