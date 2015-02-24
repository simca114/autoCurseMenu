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

  typedef struct {
    WINDOW * window;
    PANEL * panel;
    int height;
    int width;
    int posY;
    int posX;
    bool visible;
  } DISPLAYBOX;

  typedef struct {
    DISPLAYBOX dropshadow;
    DISPLAYBOX mainbox;
    DISPLAYBOX titlebox;
    DISPLAYBOX * items;
    int num_items;
  } MENUBOX;

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

  //functions to calculate length of inputs and adjust window sizes if necessary
  int newPosX(int width);
  void compareAndResizeMENUBOXs(MENUBOX * mbox_main,MENUBOX * mbox_popup,int title_len, int longest_item_len);
  void growDISPLAYBOX(DISPLAYBOX * dbox,int new_length);
  void growDISPLAYBOXset(MENUBOX * mbox,int new_length,int num_items);
  //void growDISPLAYBOXset(DISPLAYBOX ** dbox,int new_length,int num_items);
  void growMENUBOXmainAndShadow(MENUBOX * mbox,int new_length);
  int longestString(char ** array_of_strings, int num_strings);

  //functions for displaying and removing windows/panels/DISPLAYBOX
  void attachMENUBOXpanels(MENUBOX * mbox);
  int attachPANEL(DISPLAYBOX * dbox);
  void createMENUBOX(MENUBOX * mbox_main, MENUBOX * mbox_popup);
  int displayDISPLAYBOX(DISPLAYBOX * dbox, bool show);
  void drawItemContent(MENUBOX * mbox_main,MENUBOX * mbox_popup,char * title,char ** menu_options);
  int freePANEL(PANEL * panel);
  int freeWINDOW(WINDOW * window);
  void refreshAllWindows(DISPLAYBOX ** dbox, int dbox_total);
  void setDISPLAYBOXparams(DISPLAYBOX * dbox,int height, int width, int posY, int posX);
  //void setColorScheme(WINDOW ** win, int first_pair, int second_pair);

  //functions for creating and clearing menus
  void displayMENUBOX(MENUBOX * mbox, bool display);
  void drawMENUBOXwindows(MENUBOX * mbox);
  int drawWINDOW(DISPLAYBOX * dbox,int color);
  void initMENUBOX(MENUBOX * mbox_main, MENUBOX * mbox_popup,int num_items);
  void freeMenuWins(DISPLAYBOX ** dbox, int num_options);

  //functions for menu interactions
  void highlight(WINDOW * win,bool high);
  int mainMenu(char * title,char ** menu_options,int num_options);
  int popupMenu(MENUBOX * mbox,char * option);
  void shiftItems(MENUBOX * mbox,int range[],bool up);

#endif
