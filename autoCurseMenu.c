/* Alexander Simchuk, autoCurseMenu.c
 * Methods for functions defined in autoCurseMenu.h
 */
#include "autoCurseMenu.h"

#define TAB 9
#define ENTER 10
#define ESCAPE 27

#define WIN_NUM_MAIN 2
#define WIN_NUM_POPUP 3

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
        EXIT_IF_NONZERO( (HIDE_SHOW(panel[cntr],show)) ,
			  "ERROR:displayPanelSet(): hide/show panel:%d\n",cntr);
    }
}

void initPanels(PANEL ** panel,WINDOW ** win, int win_total)
{
    int cntr;
    
    //attach the windows to individual panels
    for(cntr = 0; cntr < win_total; cntr++)
    {
        EXIT_IF_NULL( (panel[cntr] = new_panel(win[cntr])) ,
		      "ERROR:initPanels(): initializeing panel:%d\n",cntr);
    }

}

void initWindows(WINDOW ** win1,WINDOW ** win2)
{
    //main menu drop-shadow
    EXIT_IF_NULL( (win1[0] = newwin(HEIGHT,WIDTH,startY() + 1,startX() + 2)) ,
		  "ERROR:initWindows(): creating main shadow failed\n");
    EXIT_IF_NONZERO( (wbkgd(win1[0],COLOR_PAIR(2))) ,
		     "ERROR:initWindows(): setting main shadow background failed\n");

    //main menu border
    EXIT_IF_NULL( (win1[1] = newwin(HEIGHT,WIDTH,startY(),startX())) ,
		  "ERROR:initWindows(): creating main menu box failed\n");
    EXIT_IF_NONZERO( (wbkgd(win1[1],COLOR_PAIR(3))),
		     "ERROR:initWindows(): setting main shadow background failed\n");

    //confirmation popup dropshadow
    EXIT_IF_NULL( (win2[0] = newwin(popupHeight(),popupWidth(),popupStartY() + 1,popupStartX() + 2) ),
                  "ERROR:initWindows(): creating popup box shadow failed\n");
    EXIT_IF_NONZERO( (wbkgd(win2[0],COLOR_PAIR(2))) ,
		     "ERROR:initWindows(): setting popup shadow background failed\n");

    //confirmation popup border
    EXIT_IF_NULL( (win2[1] = newwin(popupHeight(),popupWidth(),popupStartY(),popupStartX())) ,
		  "ERROR:initWindows(): creating popup box failed\n");
    EXIT_IF_NONZERO( (wbkgd(win2[1],COLOR_PAIR(3))) ,
		     "ERROR:initWindows(): setting popup box background failed\n");

    //confirmation messsage box
    EXIT_IF_NULL( (win2[2] = newwin(1,popupMesgWidth(),popupMesgStartY(),popupMesgStartX())) ,
		  "ERROR:initWindows(): creating popup header field failed\n");
    EXIT_IF_NONZERO( (wbkgd(win2[2],COLOR_PAIR(4))) ,
		     "ERROR:initWindows(): setting popup header background failed\n");
}

void freePanels(PANEL ** panel, int win_total)
{
    int cntr;

    for(cntr = 0; cntr < win_total; cntr++)
    {
	EXIT_IF_NONZERO( (delwin(panel_window(panel[cntr]))) ,
			 "ERROR:freePanels(): could not delete panel window %d.\n",cntr);
    }
    for(cntr = 0; cntr < win_total; cntr++)
    {
        EXIT_IF_NONZERO( (del_panel(panel[cntr])) ,
			 "ERROR:freePanels(): could not delete panel %d.\n",cntr);
    }
}

void refreshAllWindows(WINDOW ** win, int win_total)
{
    int cntr;

    for(cntr = 0; cntr < win_total; cntr++)
    {
        EXIT_IF_NONZERO( (wrefresh(win[cntr])) ,
			 "ERROR:refreshAllWindows(): wrefresh[%d] failed\n",cntr);
    }
}

/*
void setColorScheme(WINDOW ** win, int first_pair, int second_pair);
*/

PANEL ** initMenu(WINDOW ** menu_items,char ** menu_options,int num_options)
{
    int cntr;
    PANEL **items;
    EXIT_IF_NULL( (items = (PANEL**)malloc((num_options + 1) * sizeof(PANEL*))) ,
		  "ERROR:initMenu(): malloc failed\n");

    //create windows and panels for each menu item
    for(cntr = 0; cntr < num_options;cntr++)
    {
	EXIT_IF_NULL( (menu_items[cntr] = newwin(1,menuBoxWidth(),menuStartY() + cntr,menuStartX())) ,
		      "ERROR:initMenu(): win_menu_item[%d] create failed\n",cntr);
        EXIT_IF_NONZERO( (wbkgd(menu_items[cntr],COLOR_PAIR(4))) ,
			 "ERROR:initMenu(): win_menu_item[%d] set bkgd failed\n",cntr);
	EXIT_IF_NONZERO( (wprintw(menu_items[cntr]," %s",menu_options[cntr])) ,
			 "ERROR:initMenu(): win_menu_item[%d] wprintw() failed\n",cntr);
        EXIT_IF_NONZERO( (wrefresh(menu_items[cntr])) ,
			 "ERROR:initMenu(): win_menu_item[%d] wrefresh() failed\n",cntr);
    }
    EXIT_IF_NULL( (menu_items[num_options] = newwin(1,strlen("< Exit >") + 2,bottomMenuStartY(),bottomMenuStartX())) ,
		  "ERROR:initMenu(): win_menu_item_exit create failed\n");
    EXIT_IF_NONZERO( (wbkgd(menu_items[num_options],COLOR_PAIR(4))) ,
		     "ERROR:initMenu(): win_menu_item_exit set bkgd failed\n");
    EXIT_IF_NONZERO( (wprintw(menu_items[num_options]," < Exit >")) ,
		     "ERROR:initMenu(): win_menu_item_exit wprintw() failed\n");

    for(cntr = 0; cntr < num_options;cntr++)
    {
	EXIT_IF_NULL( (items[cntr] = new_panel(menu_items[cntr])) ,
		      "ERROR:initMenu(): panel_menu_item[%d]  failed\n",cntr);
    }
    EXIT_IF_NULL( (items[num_options] = new_panel(menu_items[num_options])) ,
		  "ERROR:initMenu(): panel_menu_item_exit create failed\n");

    //hide the panels that exceed window range
    if(num_options > menuBoxHeight())
    {
	for(cntr = menuBoxHeight(); cntr < num_options; cntr++)
	{
	    EXIT_IF_NONZERO( (hide_panel(items[cntr])) ,
			     "ERROR:initMenu(): hiding panel[%d] failed",cntr);
	} 
    }

    return items;
}

WINDOW ** initMenuPopup(WINDOW * win_menu, char ** menu_options)
{
    int cntr;
    WINDOW **items;

    EXIT_IF_NULL( (items = (WINDOW**)malloc(2 * sizeof(WINDOW *))) ,
		  "ERROR:initMenuPopup(): malloc failed");

    //setup < yes > box
    EXIT_IF_NULL( (items[0] = subwin(win_menu,1,strlen(menu_options[0])+2,popupMesgChoiceStartY(),popupMesgChoice1StartX())) ,
		  "ERROR:initMenuPopup(): < Yes > box failed\n");
    EXIT_IF_NULL( (items[1] = subwin(win_menu,1,strlen(menu_options[1])+2,popupMesgChoiceStartY(),popupMesgChoice2StartX())) ,
		  "ERROR:initMenuPopup(): < No > box failed\n");

    for(cntr = 0; cntr < 2; cntr++)
    {
	EXIT_IF_NONZERO( (wbkgd(items[cntr],COLOR_PAIR(4))) ,
			 "ERROR:initMenuPopup(): set yes/no %d/2 bkgd failed\n",cntr);
	EXIT_IF_NONZERO( (wprintw(items[cntr]," %s",menu_options[cntr])) ,
			 "ERROR:initMenuPopup(): set yes/no %d/2 printw() failed\n",cntr);
    }

    EXIT_IF_NONZERO( (wrefresh(win_menu)) ,
		     "ERROR:initMenuPopup(): wrefresh() failed\n");

    return items;
}

void freeMenuWins(WINDOW ** items, int num_options)
{
    int cntr;

    for(cntr = num_options - 1; cntr >= 0; cntr--)
    {
	EXIT_IF_NONZERO( (delwin(items[cntr])) ,
			 "ERROR:freeMenuWins(): delwin[%d] failed\n",cntr);
    }

    free(items);
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
    WINDOW *win_main[WIN_NUM_MAIN], *win_popup[WIN_NUM_POPUP], *panel_menu_wins[num_options + 1], ** items_popup;
    PANEL *panel_main[WIN_NUM_MAIN], *panel_popup[WIN_NUM_POPUP], **panel_menu, *panel_title;
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
    initWindows(win_main, win_popup);

    //set new windows to panels
    initPanels(panel_main,win_main,WIN_NUM_MAIN);

    EXIT_IF_NULL( (panel_title = new_panel(newwin(1,strlen(title) + 2,menuStartY()-2,((COLS - (strlen(title) + 2)) / 2)))) ,
		  "ERROR:mainMenu(): main menu title initialization failed\n");
    EXIT_IF_NONZERO( (wbkgd(panel_window(panel_title),COLOR_PAIR(4))) ,
		     "ERROR:mainMenu(): main menu title set bkgd failed\n");
    EXIT_IF_NONZERO( (wprintw(panel_window(panel_title)," %s",title)) ,
		     "ERROR:mainMenu(): main menu title set text failed\n");

    initPanels(panel_popup,win_popup,WIN_NUM_POPUP);

    //create main menu
    panel_menu = initMenu(panel_menu_wins,menu_options,num_options);
    //create popup menu
    items_popup = initMenuPopup(panel_window(panel_popup[1]),popup_options);

    choice = selected = offset = 0;
    range[0] = 0;
    range[1] = menuBoxHeight() - 1;
    highlight(panel_window(panel_menu[0]),true);
    in_menu = true;    

    while(choice != 1)
    {
	c = 0;
        //hide confirmation box
        displayPanelSet(panel_popup,WIN_NUM_POPUP,false);
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
			    shiftItems(panel_menu,num_options,true,&offset);
			    EXIT_IF_NONZERO( (hide_panel(panel_menu[range[1]])) ,
					     "ERROR:mainMenu(): hide_panel() on panel_menu[%d] failed\n",range[1]);
			    highlight(panel_window(panel_menu[selected]),false);
			    selected--;
			    EXIT_IF_NONZERO( (show_panel(panel_menu[range[0]-1])) ,
					     "ERROR:mainMenu(): show_panel() on panel_menu[%d] failed\n",range[0]-1);
			    range[0]--;
			    range[1]--;
			    highlight(panel_window(panel_menu[selected]),true);
		        }
		    	else
		    	{
		       	    highlight(panel_window(panel_menu[selected]),false);
			    selected--;
			    highlight(panel_window(panel_menu[selected]),true);
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
			    shiftItems(panel_menu,num_options,false,&offset);
			    EXIT_IF_NONZERO( (hide_panel(panel_menu[range[0]])) ,
					     "ERROR:mainMenu(): hide_panel() on panel_menu[%d] failed\n",range[1]);
			    highlight(panel_window(panel_menu[selected]),false);
			    selected++;
			    show_panel(panel_menu[range[1] + 1]);
			    EXIT_IF_NONZERO( (show_panel(panel_menu[range[1]+1])) ,
					     "ERROR:mainMenu(): show_panel() on panel_menu[%d] failed\n",range[1]+1);
			    range[0]++;
			    range[1]++;
			    highlight(panel_window(panel_menu[selected]),true);
		   	}
		    	else
		   	{
			    highlight(panel_window(panel_menu[selected]),false);
			    selected++;
			    highlight(panel_window(panel_menu[selected]),true);
		    	}
		    }
		    break;
		}
		case TAB:
		{
		    if(in_menu == true)
		    {
			highlight(panel_window(panel_menu[selected]),false);
			highlight(panel_window(panel_menu[num_options]),true);
			in_menu = false;
		    }
		    else
		    {
			highlight(panel_window(panel_menu[selected]),true);
			highlight(panel_window(panel_menu[num_options]),false);
			in_menu = true;
		    }
		    break;
		}
		case ENTER:
		{
		    if(in_menu == true)
		    {
        	    	//show popup box
        	    	displayPanelSet(panel_popup,WIN_NUM_POPUP,true);
        	    	update_panels();
        	    	doupdate();

        	    	choice = popupMenu(panel_window(panel_popup[1]),panel_window(panel_popup[2]),items_popup,menu_options[selected]);
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
    freeMenuWins(items_popup,2);

    freePanels(panel_menu, num_options + 1);
    free(panel_menu);
    freePanels(panel_popup, WIN_NUM_POPUP);
    freePanels(&panel_title,1);
    freePanels(panel_main, WIN_NUM_MAIN);
    refresh();
    EXIT_IF_NONZERO( (endwin()) ,
		     "ERROR:menuMain(): ncurses did not exit normally\n");

    if(in_menu == false || c == ESCAPE)
    {
	selected = -1;
    }

    return selected;
}

int popupMenu(WINDOW * menu_win,WINDOW * mesg_win,WINDOW ** items,char * option)
{
    int current = 1, c = 0;
    WINDOW * current_win = items[0];

    EXIT_IF_NONZERO( (mvwprintw(mesg_win,0,0," %s , are you sure?",option)) ,
		     "ERROR:popupMenu(): printing confirmation message failed\n");
    EXIT_IF_NONZERO( (wrefresh(mesg_win)) ,
		     "ERROR:popupMenu(): refreshing confirmation message failed\n");

    keypad(menu_win,TRUE);
    highlight(current_win,true);

    EXIT_IF_NONZERO( (wrefresh(current_win)) ,
		     "ERROR:popupMenu(): initial current_win refresh failed\n");

    while(c != ENTER && c != ESCAPE) 
    {
	c = wgetch(menu_win);
        switch(c)
        {
            case KEY_LEFT:
            {
		current = 1;
		highlight(current_win,false);
		EXIT_IF_NONZERO( (wrefresh(current_win)) ,
				 "ERROR:popupMenu(): first key_left current_win refresh failed\n");
                current_win = items[0];
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
                current_win = items[1];
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

void shiftItems(PANEL ** items,int num_items,bool up,int * offset)
{
    int cntr;
    if(up == 1)
    {

	for(cntr = 0; cntr < num_items;cntr++)
	{
	    EXIT_IF_NONZERO( (move_panel(items[cntr],((menuStartY()+*offset) + (cntr+1)),menuStartX())) ,
			     "ERROR:shiftItems(): moving panel[%d] up failed",cntr);
	}
	(*offset)++;
    }
    else
    {
	for(cntr = 0; cntr < num_items;cntr++)
	{
	    EXIT_IF_NONZERO( (move_panel(items[cntr],((menuStartY()+*offset) + (cntr-1)),menuStartX())) ,
			     "ERROR:shiftItems(): moving panel[%d] up failed",cntr);
	}
	(*offset)--;
    }    
}
