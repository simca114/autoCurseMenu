/* Alexander Simchuk, autoCurseMenu.h
 * Functions defenitions for generating a ncurses menu template are listed here
 */

#include <ncurses.h>
#include <panel.h>
#include <menu.h>
#include <stdio.h>

void initWindows(WINDOW ** win,int win_total);
void initPanels(PANEL ** panel,WINDOW ** win, int win_total);
void freeWindows(WINDOW ** win, win_total);
void freePanels(PANEL ** panel);
void setColorScheme(WINDOW ** win, int first_pair, int second_pair);
