/* Alexander Simchuk, autoCurseMenu.h
 * Functions defenitions for generating a ncurses menu template are listed here
 */

#ifndef _AUTOCURSEMENU_H
  #define _AUTOCURSEMENU_H

  #include <ncurses.h>
  #include <panel.h>
  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include <stdarg.h>

  typedef struct _DISPLAYBOX {
    WINDOW * window;
    PANEL * panel;
    int height;
    int width;
    int posY;
    int posX;
  } DISPLAYBOX;

  //functions to calculate windows
  int startY();
  int startX();
  int menuBoxHeight();
  int menuBoxWidth();
  int menuStartY();
  int menuStartX();
  int menuTitleStartY();
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

  //functions for displaying and removing windows/panels/DISPLAYBOX
  void displayPanelSet(DISPLAYBOX ** dbox, int dbox_total, bool show);
  void initDISPLAYBOX(DISPLAYBOX ** menu_set,DISPLAYBOX ** popup_set,DISPLAYBOX ** menu_items,DISPLAYBOX ** popup_items, int total_items);
  void initWindows(DISPLAYBOX ** dbox1,DISPLAYBOX ** dbox2);
  void initPanels(DISPLAYBOX ** dbox, int dbox_total);
  void freePanels(DISPLAYBOX ** dbox, int dbox_total);
  void freeDISPLAYBOX(DISPLAYBOX ** dbox, int dbox_total);
  int longestString(char ** array_of_strings, int num_strings);
  void refreshAllWindows(DISPLAYBOX ** dbox, int dbox_total);
  void setDISPLAYBOXparams(DISPLAYBOX * dbox,int height, int width, int posY, int posX);
  //void setColorScheme(WINDOW ** win, int first_pair, int second_pair);

  //functions for creating and clearing menus
  void initMenu(DISPLAYBOX ** dbox,char ** menu_options,int num_options);
  void initMenuPopup(DISPLAYBOX ** dbox,char ** menu_options);
  void freeMenuWins(DISPLAYBOX ** dbox, int num_options);

  //functions for menu interactions
  void highlight(WINDOW * win,bool high);
  int mainMenu(char * title,char ** menu_options,int num_options);
  int popupMenu(DISPLAYBOX ** dbox_popup, DISPLAYBOX ** dbox_popup_items,char * option);
  void shiftItems(DISPLAYBOX ** items,bool up);

#endif
