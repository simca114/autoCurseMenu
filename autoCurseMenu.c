/* Alexander Simchuk, autoCurseMenu.c
 * Methods for functions defined in autoCurseMenu.h
 */
#include "autoCurseMenu.h"

#define TAB 9
#define ENTER 10
#define ESCAPE 27

#define DBOX_NUM_MAIN 3
#define DBOX_NUM_POPUP 3

#define HEIGHT 20
#define WIDTH 40

#define HIDE_SHOW(PAN,h_s) (h_s ? show_panel(PAN) : hide_panel(PAN))

#define EXIT_IF_NULL(var,...) do {  \
  if(!var)                          \
  {                                 \
    endwin();                       \
    fprintf(stderr,__VA_ARGS__);    \
    exit(-1);                       \
  }                                 \
}while(0)

#define EXIT_IF_NONZERO(var,...) do { \
  if(var)                             \
  {                                   \
    endwin();                         \
    fprintf(stderr,__VA_ARGS__);      \
    exit(-1);                         \
  }                                   \
}while(0)

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

int menuTitleStartY()
{
    return (startY() + 2);
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

void displayPanelSet(DISPLAYBOX ** dbox,int dbox_total, bool show)
{
    int cntr;

    for(cntr = 0; cntr < dbox_total; cntr++)
    {
        EXIT_IF_NONZERO( (HIDE_SHOW(dbox[cntr]->panel,show)) ,
                         "ERROR:displayPanelSet(): hide/show dbox[%d]->panel failed\n",cntr);
    }
}

void initDISPLAYBOX(DISPLAYBOX ** menu_set,DISPLAYBOX ** popup_set,DISPLAYBOX ** menu_items,DISPLAYBOX ** popup_items, int total_items)
{
    int cntr,offset;

    setDISPLAYBOXparams(menu_set[0],HEIGHT,WIDTH,startY()+1,startX()+2);
    setDISPLAYBOXparams(menu_set[1],HEIGHT,WIDTH,startY(),startX());
    setDISPLAYBOXparams(menu_set[2],1,menuBoxWidth(),startY()+2,menuStartX());

    setDISPLAYBOXparams(popup_set[0],popupHeight(),popupWidth(),popupStartY()+1,popupStartX()+2);
    setDISPLAYBOXparams(popup_set[1],popupHeight(),popupWidth(),popupStartY(),popupStartX());
    setDISPLAYBOXparams(popup_set[2],1,popupMesgWidth(),popupMesgStartY(),popupMesgStartX());

    for(cntr = 0;cntr < total_items;cntr++)
    {
        if(cntr < menuBoxHeight())
        {
            offset = cntr;
        }
        setDISPLAYBOXparams(menu_items[cntr],1,menuBoxWidth(),menuStartY()+offset,menuStartX());
    }

    setDISPLAYBOXparams(popup_items[0],1,7,popupMesgChoiceStartY(),popupMesgChoice1StartX());
    setDISPLAYBOXparams(popup_items[1],1,6,popupMesgChoiceStartY(),popupMesgChoice2StartX());
}

void initPanels(DISPLAYBOX ** dbox, int dbox_total)
{
    int cntr;

    //attach the windows to individual panels
    for(cntr = 0; cntr < dbox_total; cntr++)
    {
        EXIT_IF_NULL( (dbox[cntr]->panel = new_panel(dbox[cntr]->window)) ,
                      "ERROR:initPanels(): initializeing dbox[%d]->panel failed\n",cntr);
    }

}

void initWindows(DISPLAYBOX ** dbox1,DISPLAYBOX ** dbox2)
{
    //main menu drop-shadow
    EXIT_IF_NULL( (dbox1[0]->window = newwin(dbox1[0]->height,dbox1[0]->width,dbox1[0]->posY,dbox1[0]->posX)) ,
                  "ERROR:initWindows(): creating main shadow failed\n");
    EXIT_IF_NONZERO( (wbkgd(dbox1[0]->window,COLOR_PAIR(2))) ,
                     "ERROR:initWindows(): setting main shadow background failed\n");

    //main menu border
    EXIT_IF_NULL( (dbox1[1]->window = newwin(dbox1[1]->height,dbox1[1]->width,dbox1[1]->posY,dbox1[1]->posY)) ,
                  "ERROR:initWindows(): creating main menu box failed\n");
    EXIT_IF_NONZERO( (wbkgd(dbox1[1]->window,COLOR_PAIR(3))) ,
                     "ERROR:initWindows(): setting main shadow background failed\n");
    EXIT_IF_NULL( (dbox1[2]->window = newwin(dbox1[2]->height,dbox1[2]->width,dbox1[2]->posY,dbox1[2]->posY)) ,
                  "ERROR:initWindows(): creating main menu title box\n");
    EXIT_IF_NONZERO( (wbkgd(dbox1[2]->window,COLOR_PAIR(4))) ,
                  "ERROR:initWindows(): setting main menu title background failed\n");

    //confirmation popup dropshadow
    EXIT_IF_NULL( (dbox2[0]->window = newwin(dbox2[0]->height,dbox2[0]->width,dbox2[0]->posY,dbox2[0]->posX)) ,
                  "ERROR:initWindows(): creating popup box shadow failed\n");
    EXIT_IF_NONZERO( (wbkgd(dbox2[0]->window,COLOR_PAIR(2))) ,
                     "ERROR:initWindows(): setting popup shadow background failed\n");

    //confirmation popup border
    EXIT_IF_NULL( (dbox2[1]->window = newwin(dbox2[1]->height,dbox2[1]->width,dbox2[1]->posY,dbox2[1]->posX)) ,
                  "ERROR:initWindows(): creating popup box failed\n");
    EXIT_IF_NONZERO( (wbkgd(dbox2[1]->window,COLOR_PAIR(3))) ,
                     "ERROR:initWindows(): setting popup box background failed\n");

//confirmation messsage box
    EXIT_IF_NULL( (dbox2[2]->window = newwin(dbox2[2]->height,dbox2[2]->width,dbox2[2]->posY,dbox2[2]->posX)) ,
                  "ERROR:initWindows(): creating popup header field failed\n");
    EXIT_IF_NONZERO( (wbkgd(dbox2[2]->window,COLOR_PAIR(4))) ,
                     "ERROR:initWindows(): setting popup header background failed\n");
}

void freeDISPLAYBOX(DISPLAYBOX ** dbox, int dbox_total)
{
    freePanels(dbox,dbox_total);
}

void freePanels(DISPLAYBOX ** dbox, int dbox_total)
{
    int cntr;

    for(cntr = 0; cntr < dbox_total; cntr++)
    {
        EXIT_IF_NONZERO( (delwin(dbox[cntr]->window)) ,
                         "ERROR:freePanels(): could not delete panel window %d.\n",cntr);
        EXIT_IF_NONZERO( (dbox[cntr]->panel) ,
                         "ERROR:freePanels(): could not delete panel %d.\n",cntr);
    }
}

void refreshAllWindows(DISPLAYBOX ** dbox, int dbox_total)
{
    int cntr;

    for(cntr = 0; cntr < dbox_total; cntr++)
    {
        EXIT_IF_NONZERO( (wrefresh(dbox[cntr]->window)) ,
                         "ERROR:refreshAllWindows(): wrefresh[%d] failed\n",cntr);
    }
}

void setDISPLAYBOXparams(DISPLAYBOX * dbox,int height, int width, int posY, int posX)
{
    dbox->height = height;
    dbox->width  = width;
    dbox->posY   = posY;
    dbox->posX   = posX;
}

/*
void setColorScheme(WINDOW ** win, int first_pair, int second_pair);
*/

void initMenu(DISPLAYBOX ** dbox,char ** menu_options,int num_options)
{
    int cntr;

    //create windows and panels for each menu item
    for(cntr = 0; cntr < num_options;cntr++)
    {
        EXIT_IF_NULL( (dbox[cntr]->window = newwin(dbox[cntr]->height,dbox[cntr]->width,dbox[cntr]->posY+cntr,dbox[cntr]->posX)) ,
                      "ERROR:initMenu(): win_menu_item[%d] create failed\n",cntr);
        EXIT_IF_NONZERO( (wbkgd(dbox[cntr]->window,COLOR_PAIR(4))) ,
                         "ERROR:initMenu(): win_menu_item[%d] set bkgd failed\n",cntr);
        EXIT_IF_NONZERO( (wprintw(dbox[cntr]->window," %s",menu_options[cntr])) ,
                         "ERROR:initMenu(): win_menu_item[%d] wprintw() failed\n",cntr);
        EXIT_IF_NONZERO( (wrefresh(dbox[cntr]->window)) ,
                         "ERROR:initMenu(): win_menu_item[%d] wrefresh() failed\n",cntr);
    }
        EXIT_IF_NULL( (dbox[num_options]->window = newwin(dbox[cntr]->height,dbox[cntr]->width,dbox[cntr]->posY,dbox[cntr]->posX)) ,
                  "ERROR:initMenu(): win_menu_item_exit create failed\n");
        EXIT_IF_NONZERO( (wbkgd(dbox[num_options]->window,COLOR_PAIR(4))) ,
                         "ERROR:initMenu(): win_menu_item_exit set bkgd failed\n");
        EXIT_IF_NONZERO( (wprintw(dbox[num_options]->window," < Exit >")) ,
                         "ERROR:initMenu(): win_menu_item_exit wprintw() failed\n");

    for(cntr = 0; cntr < num_options;cntr++)
    {
        EXIT_IF_NULL( (dbox[cntr]->panel = new_panel(dbox[cntr]->window)) ,
                      "ERROR:initMenu(): panel_menu_item[%d]  failed\n",cntr);
    }
        EXIT_IF_NULL( (dbox[num_options]->panel = new_panel(dbox[num_options]->window)) ,
                      "ERROR:initMenu(): panel_menu_item_exit create failed\n");

    //hide the panels that exceed window range
    if(num_options > menuBoxHeight())
    {
        for(cntr = menuBoxHeight(); cntr < num_options; cntr++)
        {
            EXIT_IF_NONZERO( (hide_panel(dbox[cntr]->panel)) ,
                             "ERROR:initMenu(): hiding panel[%d] failed",cntr);
        }
    }

}

void initMenuPopup(DISPLAYBOX ** dbox,char ** menu_options)
{
    int cntr;

    //setup < yes > box
    EXIT_IF_NULL( (dbox[0]->window = newwin(dbox[0]->height,dbox[0]->width,dbox[0]->posY,dbox[0]->posX)) ,
                  "ERROR:initMenuPopup(): < Yes > box failed\n");
    EXIT_IF_NULL( (dbox[1]->window = newwin(dbox[1]->height,dbox[1]->width,dbox[1]->posY,dbox[1]->posX)) ,
                  "ERROR:initMenuPopup(): < No > box failed\n");

    for(cntr = 0; cntr < 2; cntr++)
    {
        EXIT_IF_NONZERO( (wbkgd(dbox[cntr]->window,COLOR_PAIR(4))) ,
                         "ERROR:initMenuPopup(): set yes/no %d/2 bkgd failed\n",cntr);
        EXIT_IF_NONZERO( (wprintw(dbox[cntr]->window," %s",menu_options[cntr])) ,
                         "ERROR:initMenuPopup(): set yes/no %d/2 printw() failed\n",cntr);
        EXIT_IF_NONZERO( (wrefresh(dbox[cntr]->window)) ,
                         "ERROR:initMenuPopup(): wrefresh() window[%d] failed\n",cntr);
    }
}

void highlight(WINDOW * win,bool high)
{
    EXIT_IF_NONZERO( (wbkgd(win,COLOR_PAIR( (high) ? 1 : 4 ))) ,
		     "ERROR:highlight(): highlight toggle failed\n");
}

int mainMenu(char * title,char ** menu_options,int num_options)
{
    if(!menu_options)
    {
        endwin();
        fprintf(stderr,"ERROR:mainMenu(): inputted char** is null\n");
        exit(-1);
    }


    int c, choice, selected, offset, range[2];
    DISPLAYBOX *dbox_main[DBOX_NUM_MAIN], *dbox_popup[DBOX_NUM_MAIN], *dbox_menu_items[num_options+1], *dbox_popup_items[2];
    char *popup_options[2];
    bool in_menu;

    popup_options[0] = "< Yes >";
    popup_options[1] = "< No >";

    //initialize curses
    EXIT_IF_NULL( (initscr()) ,
                  "ERROR:mainMenu(): initscr() failed\n");

    //make sure terminal is big enough to support the menu
    EXIT_IF_NONZERO( (COLS < (WIDTH + 2)) ,
                     "ERROR: Terminal window is not wide enough to display the menu\n");
    EXIT_IF_NONZERO( (LINES < (HEIGHT + 2)) ,
                     "ERROR: Terminal window is not tall enough to display the menu\n");

    //continue to initialize base settings
    EXIT_IF_NONZERO( (start_color()) ,
                     "ERROR:mainMenu(): start_color() failed\n");
    cbreak();
    noecho();
    if( (curs_set(0)) == ERR)
    {
        endwin();
        fprintf(stderr,"ERROR:mainMenu(): curs_set() failed\n");
        exit(-1);
    }
    keypad(stdscr, true);

    //initialize color pairs
    EXIT_IF_NONZERO( (init_pair(1, COLOR_WHITE, COLOR_BLUE)) ,
                     "ERROR:mainMenu(): init_pair(1) failed\n");
    EXIT_IF_NONZERO( (init_pair(2, COLOR_BLACK, COLOR_BLACK)) ,
                     "ERROR:mainMenu(): init_pair(2) failed\n");
    EXIT_IF_NONZERO( (init_pair(3, COLOR_WHITE, COLOR_WHITE)) ,
                     "ERROR:mainMenu(): init_pair(3) failed\n");
    EXIT_IF_NONZERO( (init_pair(4, COLOR_BLACK, COLOR_CYAN)) ,
                     "ERROR:mainMenu(): init_pair(4) failed\n");

    EXIT_IF_NONZERO( (wbkgd(stdscr, COLOR_PAIR(1))) ,
                     "ERROR:mainMenu(): setting main background failed\n");
    refresh();

    //create windows
    initWindows(dbox_main, dbox_popup);

    //set new windows to panels
    initPanels(dbox_main,DBOX_NUM_MAIN);

    EXIT_IF_NONZERO( (wprintw(dbox_main[2]->window," %s",title)) ,
                     "ERROR:mainMenu(): main menu title set text failed\n");

    initPanels(dbox_popup,DBOX_NUM_POPUP);

    //create main menu
    initMenu(dbox_menu_items,menu_options,num_options);
    //create popup menu
    initMenuPopup(dbox_popup_items,popup_options);

    choice = selected = offset = 0;
    range[0] = 0;
    range[1] = menuBoxHeight() - 1;
    highlight(dbox_menu_items[0]->window,true);
    in_menu = true;

    while(choice != 1)
    {
        c = 0;
        //hide confirmation box
        displayPanelSet(dbox_popup,DBOX_NUM_POPUP,false);
        displayPanelSet(dbox_popup_items,2,false);
        update_panels();
        doupdate();


        while(c != ENTER && c != ESCAPE)
        {
            c = getch();
            switch(c)
            {
                case KEY_UP:
                {
                    if(in_menu == true)
                    {
                        if(selected == 0)
                        {
                        }
                        else if(selected == range[0])
                        {
                            shiftItems(dbox_menu_items,true);
                            EXIT_IF_NONZERO( (hide_panel(dbox_menu_items[range[1]]->panel)) ,
                                             "ERROR:mainMenu(): hide_panel() on dbox_menu_items[%d] failed\n",range[1]);
                            highlight(dbox_menu_items[selected]->window,false);
                            selected--;
                            EXIT_IF_NONZERO( (dbox_menu_items[range[0]-1]->panel) ,
                                             "ERROR:mainMenu(): show_panel() on panel_menu[%d] failed\n",range[0]-1);
                            range[0]--;
                            range[1]--;
                            highlight(dbox_menu_items[selected]->window,true);
                        }
                        else
                        {
                            highlight(dbox_menu_items[selected]->window,false);
                            selected--;
                            highlight(dbox_menu_items[selected]->window,true);
                        }
                    }
                    break;
                }
                case KEY_DOWN:
                {
                    if(in_menu == true)
                    {
                        if(selected == (num_options - 1))
                        {
                        }
                        else if(selected == range[1])
                        {
                            shiftItems(dbox_menu_items,false);
                            EXIT_IF_NONZERO( (hide_panel(dbox_menu_items[range[0]]->panel)) ,
                                             "ERROR:mainMenu(): hide_panel() on panel_menu[%d] failed\n",range[1]);
                            highlight(dbox_menu_items[selected]->window,false);
                            selected++;
                            show_panel(dbox_menu_items[range[1] + 1]->panel);
                            EXIT_IF_NONZERO( (show_panel(dbox_menu_items[range[1]+1]->panel)) ,
                                             "ERROR:mainMenu(): show_panel() on panel_menu[%d] failed\n",range[1]+1);
                            range[0]++;
                            range[1]++;
                            highlight(dbox_menu_items[selected]->window,true);
                        }
                        else
                        {
                            highlight(dbox_menu_items[selected]->window,false);
                            selected++;
                            highlight(dbox_menu_items[selected]->window,true);
                        }
                    }
                    break;
                }
                case TAB:
                {
                    if(in_menu == true)
                    {
                        highlight(dbox_menu_items[selected]->window,false);
                        highlight(dbox_menu_items[num_options]->window,true);
                        in_menu = false;
                    }
                    else
                    {
                        highlight(dbox_menu_items[selected]->window,true);
                        highlight(dbox_menu_items[num_options]->window,false);
                        in_menu = true;
                    }
                    break;
                }
                case ENTER:
                {
                    if(in_menu == true)
                    {
                        //show popup box
                        displayPanelSet(dbox_popup,DBOX_NUM_POPUP,true);
                        displayPanelSet(dbox_popup_items,2,true);
                        update_panels();
                        doupdate();

                        choice = popupMenu(dbox_popup,dbox_popup_items,menu_options[selected]);
                        break;
                    }
                    else
                    {
                        choice = 1;
                    }
                }
                case ESCAPE:
                {
                    choice = 1;
                }
            }
            update_panels();
            doupdate();
        }

    }

    //clean up all curses items;
    freePanels(dbox_popup_items,2);
    freePanels(dbox_menu_items, num_options + 1);
    freePanels(dbox_popup, DBOX_NUM_POPUP);
    freePanels(dbox_main, DBOX_NUM_MAIN);
    refresh();
    EXIT_IF_NONZERO( (endwin()) ,
                     "ERROR:menuMain(): ncurses did not exit normally\n");

    if(in_menu == false || c == ESCAPE)
    {
        selected = -1;
    }

    return selected;
}

int popupMenu(DISPLAYBOX ** dbox_popup, DISPLAYBOX ** dbox_popup_items,char * option)
{
    int current = 1, c = 0;
    WINDOW * current_win = dbox_popup_items[0]->window;

    EXIT_IF_NONZERO( (mvwprintw(dbox_popup[2]->window,0,0," %s , are you sure?",option)) ,
                     "ERROR:popupMenu(): printing confirmation message failed\n");
    EXIT_IF_NONZERO( (wrefresh(dbox_popup[2]->window)) ,
                     "ERROR:popupMenu(): refreshing confirmation message failed\n");

    keypad(dbox_popup[1]->window,TRUE);
    highlight(current_win,true);

    EXIT_IF_NONZERO( (wrefresh(current_win)) ,
                     "ERROR:popupMenu(): initial current_win refresh failed\n");

    while(c != ENTER && c != ESCAPE) 
    {
        c = wgetch(dbox_popup[1]->window);
        switch(c)
        {
            case KEY_LEFT:
            {
                current = 1;
                highlight(current_win,false);
                EXIT_IF_NONZERO( (wrefresh(current_win)) ,
                                 "ERROR:popupMenu(): first key_left current_win refresh failed\n");
                current_win = dbox_popup_items[0]->window;
                highlight(current_win,true);
                EXIT_IF_NONZERO( (wrefresh(current_win)) ,
                                 "ERROR:popupMenu(): second key_left current_win refresh failed\n");
                break;
            }
            case KEY_RIGHT:
            {
                current = 0;
                highlight(current_win,false);
                EXIT_IF_NONZERO( (wrefresh(current_win)) ,
                                 "ERROR:popupMenu(): first key_right current_win refresh failed\n");
                current_win = dbox_popup_items[1]->window;
                highlight(current_win,true);
                EXIT_IF_NONZERO( (wrefresh(current_win)) ,
                                 "ERROR:popupMenu(): second key_right current_win refresh failed\n");
                break;
            }
            case ESCAPE:
            {
                current = 0;
                break;
            }
        }
    }
    highlight(current_win,false);

    return current;
}

void shiftItems(DISPLAYBOX ** items,bool up)
{
    int cntr;
    if(up)
    {
        for(cntr = 0; items[cntr]->posY != menuStartY();cntr++);

        for(cntr = 0; cntr < (menuStartY() + menuBoxHeight() +1);cntr++)
        {
            setDISPLAYBOXparams(items[cntr],items[cntr]->height,items[cntr]->width,(items[cntr]->posY-1),(items[cntr]->posX));
            EXIT_IF_NONZERO( (move_panel(items[cntr]->panel,items[cntr]->posY,items[cntr]->posX)) ,
                             "ERROR:shiftItems(): moving panel[%d] up failed",cntr);
        }
    }
    else
    {
        for(cntr = 0; items[cntr]->posY != menuStartY();cntr++);

        for(cntr = 0; cntr < (menuStartY() + menuBoxHeight());cntr++)
        {
            setDISPLAYBOXparams(items[cntr],items[cntr]->height,items[cntr]->width,(items[cntr]->posY+1),(items[cntr]->posX));
            EXIT_IF_NONZERO( (move_panel(items[cntr]->panel,items[cntr]->posY,items[cntr]->posX)) ,
                             "ERROR:shiftItems(): moving panel[%d] up failed",cntr);
        }
    }
}
