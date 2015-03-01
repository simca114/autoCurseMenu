libAutoCurseMenu
================

This is a C library that uses ncurses to generate a intarctive menu. The inputted array of strings will appear as a scrollable menu. When a selection is made, a confirmation window will appear allowing the user to double check their choice.

Main Menu    |  Confirmation Window
:-----------:|:---------------------:
![][mainimg] | ![][confimg]

[mainimg]: https://github.com/simca114/autoCurseMenu/blob/master/libautoCMmain.png "Main Menu"
[confimg]: https://github.com/simca114/autoCurseMenu/blob/master/libAutoCMconf.png "Confirmation Window"

Requirements
------------
 * [GCC], the GNU Compiler Collection
 * [ncurses]

[GCC]: https://gcc.gnu.org/
[ncurses]: https://www.gnu.org/software/ncurses/

Compiling with autoCurseMenu
----------------------------

* Copy 'libAutoCurseMenu.a' and 'autoCurseMenu.h' into the directory with the .c file you want to compile with
* add '#include "autoCurseMenu.h"' to the top of your .c file
* add '-L. -lautoCurseMenu -lpanel -lncurses' to the end of your compile line

Usage
-----

To use the library, simply call the function mainMenu() with these three arguements
```
* A title for the menu (char array)
* The options you want to be displayed (array of char arrays)
* The number of options being passed in
```

The function returns one of the following integers:
```
* -1 if the menu was exited by use of the Exit button or Escape key
* index of the array whose char array was selected
```
