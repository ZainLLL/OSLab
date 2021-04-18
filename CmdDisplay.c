#include "MemManage.h"

extern Error ErrNo;

char *cmdUI = "************ Momery Management **********\n"
              "   Allocate: type 'malloc size'          \n"
              "     Free  : type 'free size address'    \n"
              "     Quit  : type 'exit'                 \n"
              "     Help  : type 'help'                 \n"
              "*****************************************\n";


void cmdParse(){
    char ch;
    char cmd[10];
    char cmdAlloc[10] = "malloc";
    char cmdFree[10] = "free";
    char cmdExit[10] = "exit";
    char cmdHelp[10] = "help";
    unsigned int size, addr;

    printf(cmdUI);

    while(true)
    {
        printf(">>> ");
        scanf("%9s",cmd);
        if(strcmp(cmdAlloc, cmd) == 0)
        {// allocate memory
            scanf("%u", &size);
            //printf("get : %s %u\n", cmd, size);
            while((ch = getchar()) != '\n' && ch != EOF);
            lmalloc(size);
            // printf("head: %d", head->m_addr);
        }
        else if(strcmp(cmdFree, cmd) == 0)
        {// free memory
            scanf("%u %u", &size, &addr);
            //printf("get : %s %u %u\n", cmd, size, addr);
            while((ch = getchar()) != '\n' && ch != EOF);
            lfree(size, addr);
        }
        else if(strcmp(cmdExit, cmd) == 0)
        {// exit the system
            printf("Exit. Have a good day! :)\n");
            while((ch = getchar()) != '\n' && ch != EOF);
            break;
        }
        else if(strcmp(cmdHelp, cmd) == 0)
        {// help information
            printf("\n");
            printf(cmdUI);
        }
        else
        {// undifined command
            ErrNo = ERR_BAD_CMD;
            while((ch = getchar()) != '\n' && ch != EOF);
            pError();
        }
    }
}