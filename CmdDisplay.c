#include "MemManage.h"

extern Error ErrNo;

const char *cmdUI = "************ Momery Management **********\n"
                    "   Allocate: type 'malloc size'          \n"
                    "     Free  : type 'free size address'    \n"
                    "     Quit  : type 'exit'                 \n"
                    "     Help  : type 'help'                 \n"
                    "*****************************************\n";

const char *cmds[] = {"malloc", "free", "exit", "help"};

void cmdParse(){
    char ch;
    char cmd_in[10];
    unsigned int size, addr;

    printf(cmdUI);

    while(true)
    {
        printf(">>> ");
        scanf("%9s",cmd_in);
        if(strcmp(cmds[0], cmd_in) == 0)
        {// allocate memory
            scanf("%u", &size);
            //printf("get : %s %u\n", cmd, size);
            while((ch = getchar()) != '\n' && ch != EOF);
            lmalloc(size);
            // printf("head: %d", head->m_addr);
        }
        else if(strcmp(cmds[1], cmd_in) == 0)
        {// free memory
            scanf("%u %u", &size, &addr);
            //printf("get : %s %u %u\n", cmd, size, addr);
            while((ch = getchar()) != '\n' && ch != EOF);
            lfree(size, addr);
        }
        else if(strcmp(cmds[2], cmd_in) == 0)
        {// exit the system
            printf("Exit. Have a good day! :)\n");
            while((ch = getchar()) != '\n' && ch != EOF);
            break;
        }
        else if(strcmp(cmds[3], cmd_in) == 0)
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