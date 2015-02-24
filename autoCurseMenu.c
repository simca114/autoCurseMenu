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

int debug = 0;

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

int newPosX(int width)
{
    return ((COLS - width) / 2);
}

void compareAndResizeMENUBOXs(MENUBOX * mbox_main,MENUBOX * mbox_popup,int title_len, int longest_item_len)
{
    int new_length, title, option;

    mvprintw(debug,0,"inside comp func");
    debug++;
    refresh();
    if((longest_item_len+14) > mbox_popup->titlebox.width)
    {
    mvprintw(debug,0,"popup title box too small");
    debug++;
    refresh();
        new_length = longest_item_len+16;
        growMENUBOXmainAndShadow(mbox_popup,new_length);
        growDISPLAYBOX(&mbox_popup->titlebox,new_length);
    }

    title = option = 0;
    if(title_len > mbox_main->titlebox.width)
    {
    mvprintw(debug,0,"main title box too small");
    debug++;
    refresh();
        new_length = title_len;
        growDISPLAYBOX(&mbox_main->titlebox,new_length);
        title = 1;
    }
    if(longest_item_len > mbox_main->items[0].width)
    {
    mvprintw(debug,0,"main menu box too small");
    debug++;
    refresh();
        new_length = longest_item_len;
        growDISPLAYBOXset(&mbox_main->items,new_length,mbox_main->num_items);
        option = 1;
    }

    if(title || option)
    {
        if(title && option)
        {
            if(title_len > longest_item_len)
            {
                new_length = title_len;
            }
        }
        growMENUBOXmainAndShadow(mbox_main,new_length);
    }
}

void growDISPLAYBOX(DISPLAYBOX * dbox,int new_length)
{
    dbox->width = new_length+2;
    dbox->posX  = newPosX(dbox->width);
}

void growDISPLAYBOXset(DISPLAYBOX ** dbox,int new_length,int num_items)
{
    int cntr;

    for(cntr = 0; cntr < num_items; cntr++)
    {
        dbox[cntr]->width = new_length+2;
        dbox[cntr]->posX  = newPosX(dbox[cntr]->width);
    }
}

void growMENUBOXmainAndShadow(MENUBOX * mbox,int new_length)
{
    mbox->mainbox.width = new_length+6;
    mbox->mainbox.posX  = newPosX(mbox->mainbox.width);

    mbox->dropshadow.width = mbox->mainbox.width;
    mbox->dropshadow.posX = mbox->mainbox.posX+2;
}

int longestString(char ** array_of_strings, int num_strings)
{
    int cntr, longest = 0;

    for(cntr = 0; cntr < num_strings; cntr++)
    {
        if(strlen(array_of_strings[cntr]) > longest)
        {
            longest = strlen(array_of_strings[cntr]);
        }
    }

    return longest;
}

int displayDISPLAYBOX(DISPLAYBOX * dbox, bool show)
{
    return (HIDE_SHOW(dbox->panel,show));
}

void attachMENUBOXpanels(MENUBOX * mbox)
{
    int cntr;

    EXIT_IF_NONZERO( (attachPANEL(&mbox->dropshadow)) ,
                     "ERROR:attachMENUBOXpanels(): dropshadow failed\n");
    EXIT_IF_NONZERO( (attachPANEL(&mbox->mainbox)) ,
                     "ERROR:attachMENUBOXpanels(): mainbox failed\n");
    EXIT_IF_NONZERO( (attachPANEL(&mbox->titlebox)) ,
                     "ERROR:attachMENUBOXpanels(): titlebox failed\n");

    for(cntr = 0; cntr < mbox->num_items;cntr++)
    {
        EXIT_IF_NONZERO( (attachPANEL(&mbox->items[cntr])) ,
                         "ERROR:attachMENUBOXpanels(): items[%d] failed\n",cntr);
    }
}

int attachPANEL(DISPLAYBOX * dbox)
{
    dbox->panel = new_panel(dbox->window);

    if(!dbox->panel)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void createMENUBOX(MENUBOX * mbox_main, MENUBOX * mbox_popup)
{
    drawMENUBOXwindows(mbox_main);
    attachMENUBOXpanels(mbox_main);

    drawMENUBOXwindows(mbox_popup);
    attachMENUBOXpanels(mbox_popup);
}

void displayMENUBOX(MENUBOX * mbox, bool display)
{
    int cntr;

    EXIT_IF_NONZERO( (displayDISPLAYBOX(&mbox->dropshadow,display)) ,
                     "ERROR:displayMENUBOX: toggling dropshadow failed\n");
    EXIT_IF_NONZERO( (displayDISPLAYBOX(&mbox->mainbox,display)) ,
                     "ERROR:displayMENUBOX: toggling mainbox failed\n");
    EXIT_IF_NONZERO( (displayDISPLAYBOX(&mbox->titlebox,display)) ,
                     "ERROR:displayMENUBOX: toggling titlebox failed\n");

    for(cntr = 0; cntr < mbox->num_items;cntr++)
    {
        EXIT_IF_NONZERO( (displayDISPLAYBOX(&mbox->items[cntr],display)) ,
                     "ERROR:displayMENUBOX: toggling items[%d] failed\n",cntr);
    }
}

void drawMENUBOXwindows(MENUBOX * mbox)
{
    int cntr;

    EXIT_IF_NONZERO( (drawWINDOW(&mbox->dropshadow,2)) ,
                     "ERROR:drawMENUBOXwindows(): dropshadow failed\n");
    EXIT_IF_NONZERO( (drawWINDOW(&mbox->mainbox,3)) ,
                     "ERROR:drawMENUBOXwindows(): mainbox failed\n");
    EXIT_IF_NONZERO( (drawWINDOW(&mbox->titlebox,4)) ,
                     "ERROR:drawMENUBOXwindows(): titlebox failed\n");

    for(cntr = 0; cntr < mbox->num_items;cntr++)
    {
        EXIT_IF_NONZERO( (drawWINDOW(&mbox->items[cntr],4)) ,
                         "ERROR:drawMENUBOXwindows(): items[%d] failed\n",cntr);
    }
}

int drawWINDOW(DISPLAYBOX * dbox,int color)
{
    int bkgd_fail;

    dbox->window = newwin(dbox->height,dbox->width,dbox->posY,dbox->posX);

    if(dbox->window)
    {
        bkgd_fail = wbkgd(dbox->window,COLOR_PAIR(color));
    }

    wrefresh(dbox->window);

    if(!dbox->window || bkgd_fail)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void initMENUBOX(MENUBOX * mbox_main, MENUBOX * mbox_popup,int num_items)
{
    int cntr,offset;

    setDISPLAYBOXparams(&mbox_main->dropshadow,HEIGHT,WIDTH,startY()+1,startX()+2);
    setDISPLAYBOXparams(&mbox_main->mainbox,HEIGHT,WIDTH,startY(),startX());
    setDISPLAYBOXparams(&mbox_main->titlebox,1,menuBoxWidth(),startY()+1,menuStartX());
    mbox_main->items = malloc((num_items+1)*sizeof(DISPLAYBOX));
    for(cntr = 0; cntr < num_items;cntr++)
    {
        if(cntr < menuBoxHeight())
        {
            offset = cntr;
        }
        setDISPLAYBOXparams(&mbox_main->items[cntr],1,menuBoxWidth(),menuStartY()+offset,menuStartX());
    }
    setDISPLAYBOXparams(&mbox_main->items[cntr],1,bottomMenuWidth()+1,bottomMenuStartY(),bottomMenuStartX());

    mbox_main->num_items = num_items+1;

    setDISPLAYBOXparams(&mbox_popup->dropshadow,popupHeight(),popupWidth(),popupStartY()+1,popupStartX()+2);
    setDISPLAYBOXparams(&mbox_popup->mainbox,popupHeight(),popupWidth(),popupStartY(),popupStartX());
    setDISPLAYBOXparams(&mbox_popup->titlebox,1,popupMesgWidth(),popupMesgStartY(),popupMesgStartX());
    mbox_popup->items = malloc(2*sizeof(DISPLAYBOX));
    setDISPLAYBOXparams(&mbox_popup->items[0],1,9,popupMesgChoiceStartY(),popupMesgChoice1StartX());
    setDISPLAYBOXparams(&mbox_popup->items[1],1,8,popupMesgChoiceStartY(),popupMesgChoice2StartX());
    mbox_popup->num_items = 2;
}

void freeMENUBOX(MENUBOX * mbox)
{
    int cntr;

    for(cntr = (mbox->num_items-1); cntr >= 0; cntr--)
    {
        EXIT_IF_NONZERO( (freeWINDOW(mbox->items[cntr].window)) ,
                         "couldnt delete win\n");
    }
    EXIT_IF_NONZERO( (freeWINDOW(mbox->titlebox.window)) ,
                     "ERROR:freeMENUBOX(): titlebox failed\n");
    EXIT_IF_NONZERO( (freeWINDOW(mbox->mainbox.window)) ,
                     "ERROR:freeMENUBOX(): mainbox failed\n");
    EXIT_IF_NONZERO( (freeWINDOW(mbox->dropshadow.window)) ,
                     "ERROR:freeMENUBOX(): dropshadow failed\n");

    for(cntr = (mbox->num_items-1); cntr >= 0; cntr--)
    {
        EXIT_IF_NONZERO( (freePANEL(mbox->items[cntr].panel)) ,
                         "couldnt delete win\n");
    }
    EXIT_IF_NONZERO( (freePANEL(mbox->titlebox.panel)) ,
                     "ERROR:freeMENUBOX(): titlebox panel failed\n");
    EXIT_IF_NONZERO( (freePANEL(mbox->mainbox.panel)) ,
                     "ERROR:freeMENUBOX(): mainbox panel failed\n");
    EXIT_IF_NONZERO( (freePANEL(mbox->dropshadow.panel)) ,
                     "ERROR:freeMENUBOX(): dropshadow panel failed\n");
}

int freePANEL(PANEL * panel)
{
    return (del_panel(panel));
}

int freeWINDOW(WINDOW * window)
{
    return (delwin(window));
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

void drawItemContent(MENUBOX * mbox_main,MENUBOX * mbox_popup,char * title,char ** menu_options)
{
    int cntr;

    EXIT_IF_NONZERO( (wprintw(mbox_main->titlebox.window," %s",title)) ,
                     "ERROR:drawItemContent(): mbox_main title wprintw() failed\n");
    EXIT_IF_NONZERO( (wrefresh(mbox_main->titlebox.window)) ,
                     "ERROR:drawItemContent(): mbox_main title wrefresh() failed\n");
    //print content for each main menu item
    for(cntr = 0; cntr < (mbox_main->num_items-1);cntr++)
    {
        EXIT_IF_NONZERO( (wprintw(mbox_main->items[cntr].window," %s",menu_options[cntr])) ,
                         "ERROR:drawItemContent(): mbox_main items[%d] wprintw() failed\n",cntr);
        EXIT_IF_NONZERO( (wrefresh(mbox_main->items[cntr].window)) ,
                         "ERROR:drawItemContent(): mbox_main items[%d] wrefresh() failed\n",cntr);
    }
    EXIT_IF_NONZERO( (wprintw(mbox_main->items[cntr].window," < Exit >")) ,
                     "ERROR:drawItemContent(): mbox_main exit items[%d] wprintw() failed\n",(mbox_main->num_items-1));
    EXIT_IF_NONZERO( (wrefresh(mbox_main->items[cntr].window)) ,
                     "ERROR:drawItemContent(): mbox_main exit items[%d] wrefresh() failed\n",cntr);

    //hide the panels that exceed window range (NOTE: this part should be moved elsewhere)
    if(mbox_main->num_items > menuBoxHeight())
    {
        for(cntr = menuBoxHeight(); cntr < (mbox_main->num_items-1); cntr++)
        {
            EXIT_IF_NONZERO( (hide_panel(mbox_main->items[cntr].panel)) ,
                             "ERROR:drawItemContent(): hiding mbox_main dbox[%d] panel failed",cntr);
        }
    }


    EXIT_IF_NONZERO( (wprintw(mbox_popup->items[0].window," < Yes >")) ,
                     "ERROR:drawItemContent(): mbox_popup Yes wprintw() failed\n");
    EXIT_IF_NONZERO( (wrefresh(mbox_popup->items[0].window)) ,
                     "ERROR:drawItemContent(): mbox_popup Yes wrefresh() failed\n");
    EXIT_IF_NONZERO( (wprintw(mbox_popup->items[1].window," < No >")) ,
                     "ERROR:drawItemContent(): mbox_popup < No > wprintw() failed\n");
    EXIT_IF_NONZERO( (wrefresh(mbox_popup->items[1].window)) ,
                     "ERROR:drawItemContent(): mbox_popup < No > wrefresh() failed\n");
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

    MENUBOX mbox_main, mbox_popup;
    int c, choice, selected, range[2], longest_item_length;
    bool in_menu;

    longest_item_length = longestString(menu_options,num_options);

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

    initMENUBOX(&mbox_main,&mbox_popup,num_options);
    //TODO this function implementation needs to be redesigned
    compareAndResizeMENUBOXs(&mbox_main,&mbox_popup,strlen(title),longest_item_length);


    createMENUBOX(&mbox_main,&mbox_popup);
    mvprintw(debug,0,"post drawMENUBOXwindows()");
    debug++;
    refresh();
    drawItemContent(&mbox_main,&mbox_popup,title,menu_options);

    choice = selected = 0;
    range[0] = 0;
    range[1] = menuBoxHeight() - 1;
    highlight(mbox_main.items[0].window,true);
    in_menu = true;

    while(choice != 1)
    {
        c = 0;
        //hide confirmation box
        displayMENUBOX(&mbox_popup,false);
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
                            shiftItems(&mbox_main.items,true);
                            EXIT_IF_NONZERO( (hide_panel(mbox_main.items[range[1]].panel)) ,
                                             "ERROR:mainMenu(): hide_panel() on dbox_menu_items[%d] failed\n",range[1]);
                            highlight(mbox_main.items[selected].window,false);
                            selected--;
                            EXIT_IF_NONZERO( (show_panel(mbox_main.items[range[0]-1].panel)) ,
                                             "ERROR:mainMenu(): show_panel() on panel_menu[%d] failed\n",range[0]-1);
                            range[0]--;
                            range[1]--;
                            highlight(mbox_main.items[selected].window,true);
                        }
                        else
                        {
                            highlight(mbox_main.items[selected].window,false);
                            selected--;
                            highlight(mbox_main.items[selected].window,true);
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
                            shiftItems(&mbox_main.items,false);
                            EXIT_IF_NONZERO( (hide_panel(mbox_main.items[range[0]].panel)) ,
                                             "ERROR:mainMenu(): hide_panel() on panel_menu[%d] failed\n",range[1]);
                            highlight(mbox_main.items[selected].window,false);
                            selected++;
                            show_panel(mbox_main.items[range[1] + 1].panel);
                            EXIT_IF_NONZERO( (show_panel(mbox_main.items[range[1]+1].panel)) ,
                                             "ERROR:mainMenu(): show_panel() on panel_menu[%d] failed\n",range[1]+1);
                            range[0]++;
                            range[1]++;
                            highlight(mbox_main.items[selected].window,true);
                        }
                        else
                        {
                            highlight(mbox_main.items[selected].window,false);
                            selected++;
                            highlight(mbox_main.items[selected].window,true);
                        }
                    }
                    break;
                }
                case TAB:
                {
                    if(in_menu == true)
                    {
                        highlight(mbox_main.items[selected].window,false);
                        highlight(mbox_main.items[num_options].window,true);
                        in_menu = false;
                    }
                    else
                    {
                        highlight(mbox_main.items[selected].window,true);
                        highlight(mbox_main.items[num_options].window,false);
                        in_menu = true;
                    }
                    break;
                }
                case ENTER:
                {
                    if(in_menu == true)
                    {
                        //show popup box
                        displayMENUBOX(&mbox_popup,true);
                        update_panels();
                        doupdate();

                        choice = popupMenu(&mbox_popup,menu_options[selected]);
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

    freeMENUBOX(&mbox_popup);
    freeMENUBOX(&mbox_main);

    refresh();
    EXIT_IF_NONZERO( (endwin()) ,
                     "ERROR:menuMain(): ncurses did not exit normally\n");

    if(in_menu == false || c == ESCAPE)
    {
        selected = -1;
    }

    return selected;
}

int popupMenu(MENUBOX * mbox,char * option)
{
    int current = 1, c = 0;
    WINDOW * current_win = mbox->items[0].window;

    EXIT_IF_NONZERO( (mvwprintw(mbox->titlebox.window,0,0," %s , are you sure?",option)) ,
                     "ERROR:popupMenu(): printing confirmation message failed\n");
    EXIT_IF_NONZERO( (wrefresh(mbox->titlebox.window)) ,
                     "ERROR:popupMenu(): refreshing confirmation message failed\n");

    //keypad(mwindow,TRUE);
    highlight(current_win,true);

    EXIT_IF_NONZERO( (wrefresh(current_win)) ,
                     "ERROR:popupMenu(): initial current_win refresh failed\n");

    while(c != ENTER && c != ESCAPE) 
    {
        c = getch();
        switch(c)
        {
            case KEY_LEFT:
            {
                current = 1;
                highlight(current_win,false);
                EXIT_IF_NONZERO( (wrefresh(current_win)) ,
                                 "ERROR:popupMenu(): first key_left current_win refresh failed\n");
                current_win = mbox->items[0].window;
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
                current_win = mbox->items[1].window;
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
                             "ERROR:shiftItems(): moving panel[%d] up failed\n",cntr);
        }
    }
    else
    {
        for(cntr = 0; items[cntr]->posY != menuStartY();cntr++);

        for(cntr = 0; cntr < (menuStartY() + menuBoxHeight());cntr++)
        {
            setDISPLAYBOXparams(items[cntr],items[cntr]->height,items[cntr]->width,(items[cntr]->posY+1),(items[cntr]->posX));
            EXIT_IF_NONZERO( (move_panel(items[cntr]->panel,items[cntr]->posY,items[cntr]->posX)) ,
                             "ERROR:shiftItems(): moving panel[%d] up failed\n",cntr);
        }
    }
}
