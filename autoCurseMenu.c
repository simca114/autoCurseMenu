/* Alexander Simchuk, autoCurseMenu.c
 * Methods for functions defined in autoCurseMenu.h
 */
#include "autoCurseMenu.h"

#define ENTER 10
#define ESCAPE 27

#define WIN_NUM_MAIN 2
#define WIN_NUM_POPUP 3

#define HEIGHT 20
#define WIDTH 40

#define HIDE_SHOW(PAN,h_s) (h_s ? show_panel(PAN) : hide_panel(PAN))

int debug_pos = 0;

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

void freePanels(PANEL ** panel, int win_total)
{
    int cntr;

    for(cntr = 0; cntr < win_total; cntr++)
    {
	if(delwin(panel_window(panel[cntr])))
        {
	    printf("ERROR: could not delete panel window %d.\n",cntr);
        }
    }
    for(cntr = 0; cntr < win_total; cntr++)
    {
        if(del_panel(panel[cntr]))
        {
            printf("ERROR: could not delete panel %d.\n",cntr);
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

PANEL ** initMenu(WINDOW ** menu_items,char ** menu_options,int num_options)
{
    int cntr;
    PANEL **items;
    items = (PANEL**)malloc((num_options + 1) * sizeof(PANEL*));

    //create windows and panels for each menu item
    for(cntr = 0; cntr < num_options;cntr++)
    {
	menu_items[cntr] = newwin(1,menuBoxWidth(),menuStartY() + cntr,menuStartX());
        wbkgd(menu_items[cntr],COLOR_PAIR(4));
	wprintw(menu_items[cntr],"%s",menu_options[cntr]);
        wrefresh(menu_items[cntr]);
    }
    menu_items[num_options] = newwin(1,strlen("< Exit >"),bottomMenuStartY(),bottomMenuStartX());
    wbkgd(menu_items[num_options],COLOR_PAIR(4));
    wprintw(menu_items[num_options],"< Exit >");

    for(cntr = 0; cntr < num_options;cntr++)
    {
	items[cntr] = new_panel(menu_items[cntr]);
    }
    items[num_options] = new_panel(menu_items[num_options]);

    //hide the panels that exceed window range
    if(num_options > menuBoxHeight())
    {
	mvprintw(debug_pos,0,"Need to hide panel\n");
        debug_pos++;
        refresh();
	for(cntr = menuBoxHeight(); cntr < num_options; cntr++)
	{
            mvprintw(debug_pos,0,"Hiding panel %d\n",cntr);
            debug_pos++;
	    refresh();

	    if((hide_panel(items[cntr])) == ERR)
            {
            	mvprintw(debug_pos,0,"ERROR: hide_panel(%d) failed\n",cntr);
            	debug_pos++;
	    	refresh();
	    }
	} 
    }

    update_panels();
    doupdate();

    return items;
}

WINDOW ** initMenuPopup(WINDOW * win_menu, char ** menu_options)
{
    int cntr;
    WINDOW **items;
    items = (WINDOW**)malloc(2 * sizeof(WINDOW *));
    //setup < yes > box
    items[0] = subwin(win_menu,1,strlen(menu_options[0]),popupMesgChoiceStartY(),popupMesgChoice1StartX());
    items[1] = subwin(win_menu,1,strlen(menu_options[1]),popupMesgChoiceStartY(),popupMesgChoice2StartX());

    for(cntr = 0; cntr < 2; cntr++)
    {
	wbkgd(items[cntr],COLOR_PAIR(4));
	wprintw(items[cntr],"%s",menu_options[cntr]);
    }

    wrefresh(win_menu);

    return items;
}

void freeMenuWins(WINDOW ** items, int num_options)
{
    int cntr;

    for(cntr = num_options - 1; cntr >= 0; cntr--)
    {
	delwin(items[cntr]);
    }

    free(items);
}

int mainMenu(char ** menu_options,int num_options)
{
    if(!menu_options)
    {
        printf("mainmenu: inputted char** is null\n");
        return -1;
    }

    int c, choice;
    WINDOW *win_main[WIN_NUM_MAIN], *win_popup[WIN_NUM_POPUP], *panel_menu_wins[num_options + 1];
    PANEL *panel_main[WIN_NUM_MAIN], *panel_popup[WIN_NUM_POPUP], **panel_menu;
    WINDOW ** items_popup;
    char *popup_options[2];

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
        return -2;
    }

    //set new windows to panels
    initPanels(panel_main,win_main,WIN_NUM_MAIN);
    initPanels(panel_popup,win_popup,WIN_NUM_POPUP);

    //create main menu
    panel_menu = initMenu(panel_menu_wins,menu_options,num_options);
    //create popup menu
    items_popup = initMenuPopup(panel_window(panel_popup[1]),popup_options);

    choice = 0;
    while(choice == 0)
    {
        displayPanelSet(panel_popup,WIN_NUM_POPUP,false);
        update_panels();
        doupdate();

        c = getch();

        //show popup box
        displayPanelSet(panel_popup,WIN_NUM_POPUP,true);
        show_panel(panel_menu[num_options]);
        update_panels();
        doupdate();

        choice = popupMenu(panel_window(panel_popup[1]),panel_window(panel_popup[2]),items_popup,"TestPhrase");
    }
    
    //clean up all curses items;
    freeMenuWins(items_popup,2);

    freePanels(panel_menu, num_options + 1);
    free(panel_menu);
    freePanels(panel_popup, WIN_NUM_POPUP);
    freePanels(panel_main, WIN_NUM_MAIN);
    refresh();
    endwin();

    return 0;
}

int popupMenu(WINDOW * menu_win,WINDOW * mesg_win,WINDOW ** items,char * option)
{
    int current = 1, c = 0;
    WINDOW * current_win = items[0];

    wprintw(mesg_win," %s , are you sure?",option);
    wrefresh(mesg_win);

    keypad(menu_win,TRUE);
    wbkgd(current_win,COLOR_PAIR(1));
    wrefresh(current_win);

    while(c != ENTER && c != ESCAPE) 
    {
	c = wgetch(menu_win);
        switch(c)
        {
            case KEY_LEFT:
            {
		current = 1;
		wbkgd(current_win,COLOR_PAIR(4));
		wrefresh(current_win);
                current_win = items[0];
		wbkgd(current_win,COLOR_PAIR(1));
		wrefresh(current_win);
                break;
            }
            case KEY_RIGHT:
            {
		current = 0;
		wbkgd(current_win,COLOR_PAIR(4));
		wrefresh(current_win);
                current_win = items[1];
		wbkgd(current_win,COLOR_PAIR(1));
		wrefresh(current_win);
                break;
            }
	    case ESCAPE:
            {
		current = 0;
		break;
	    }
        }
    }
    wbkgd(current_win,COLOR_PAIR(4));

    return current;
}
