/* Alexander Simchuk, autoCurseMenu.h
 * Functions defenitions for generating a ncurses menu template are listed here
 */

#include <ncurses.h>
#include <panel.h>
#include <menu.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//functions to calculate windows
int startY();
int startX();
int menuBoxHeight();
int menuBoxWidth();
int menuStartY();
int menuStartX();
int bottomMenu();
int bottomMenuWidth();
int bottomMenuStartX();
int bottomMenuStartY();
int popupHeight();
int popupWidth();
int popupStartY();
int popupStartX();
int popupMesgWidth();
int popupMesgStartY();
int popupMesgStartX();
int popupMesgChoiceWidth();
int popupMesgChoiceStartY();
int popupMesgChoice1StartX();
int popupMesgChoice2StartX();

//functions for displaying and removing windows/panels
void displayPanelSet(PANEL ** panel, int panel_total, bool show);
int initWindows(WINDOW ** win1,WINDOW ** win2);
void initPanels(PANEL ** panel,WINDOW ** win, int win_total);
void freePanels(PANEL ** panel, int win_total);
void refreshAllWindows(WINDOW ** win, int win_total);
void setColorScheme(WINDOW ** win, int first_pair, int second_pair);

//functions for creating and clearing menus
PANEL ** initMenu(WINDOW ** menu_items,char ** menu_options,int num_options);
WINDOW ** initMenuPopup(WINDOW * win_menu, char ** menu_options);
void freeMenuWins(WINDOW ** items, int num_options);

//functions for menu interactions
void highlight(WINDOW * win,bool high);
int mainMenu(char ** menu_options,int num_options);
int popupMenu(WINDOW * menu_win,WINDOW * mesg_win,WINDOW ** items,char * option);
void shiftItems(PANEL ** items,int num_items,bool up);
