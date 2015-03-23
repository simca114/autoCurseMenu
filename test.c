#include "autoCurseMenu.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    int choice;
    char *sample[] = {
                        "Choice1",
                        "Choice2",
                        "Choice3",
                        "Choice4",
                        "Choice5",
                        "Choice6",
                        "Choice7",
                        "Choice8",
                        "Choice9",
                        "Choice10",
                        "Choice11",
                        "Choice12",
                        "Choice13",
                        "Choice14",
                        "Choice15",
                        "Choice16",
                     };

    choice = mainMenu("Please make a seleciton",sample,16);

    system("clear");

    if(choice == -1)
    {
        printf("Program terminated by exit\n");
    }
    else
    {
        printf("Choice %d was selected\n",choice + 1);
    }

    return 0;
}
