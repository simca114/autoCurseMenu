#include "autoCurseMenu.h"
#include <stdlib.h>


int main()
{
    int error_code;
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

    if((error_code = mainMenu(sample,16)))
    {
        switch(error_code)
        {
            case -1:
            {
                printf("Error %d: input char** is not initialized\n",error_code);
                exit(-1);
                break;
            }
            case -2:
            {
                printf("Error %d: Failed to initialize window set\n",error_code);
                exit(-1);
                break;
            }
        }
    }

    return 0;
}
