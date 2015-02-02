/* Alexander Simchuk, autoCurseMenu.h
 * Functions defenitions for generating a ncurses menu template are listed here
 */

#include <ncurses.h>
#include <panel.h>
#include <menu.h>
#include <stdio.h>

//functions for displaying and removing windows/panels
void displayWindowSet(PANEL ** panel,int win_set, bool show);
int initWindows(WINDOW ** win,int win_total);
void initPanels(PANEL ** panel,WINDOW ** win, int win_total);
void freeWindows(WINDOW ** win, int win_total);
void freePanels(PANEL ** panel, WINDOW ** win, int win_total);
void refreshAllWindows(WINDOW ** win, int win_total);
void setColorScheme(WINDOW ** win, int first_pair, int second_pair);

//functions for creating and clearing menus
void initMenu(MENU *menu,PANEL ** panel,ITEM ** items,char ** menu_options,int num_options);
void initItems(ITEM ** items,char ** menu_options,int num_options);
void freeItems(ITEM ** items,int num_options);
void freeMenu(MENU *menu,ITEM ** items,int num_options);

//functions for menu interactions
int popupMenu(PANEL ** panel);
