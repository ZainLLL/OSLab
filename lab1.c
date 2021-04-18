#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define M_SIZE 1000
typedef struct _Map{
    unsigned m_size;
    char *m_addr;
    struct _Map *next, *prior;
} Map;

Map *cur;
Map *head;
char *mem;

/*_____________________________Err Info____________________________*/
typedef enum _Error
{
    ERR_NO_ERR = 0,
    ERR_BAD_SIZE = 1,
    ERR_BAD_ADDR = 2,
    ERR_ALLOCATE_FALI = 3,
    ERR_MEM_NONE = 4,
    ERR_BAD_CMD = 5
} Error;

Error ErrNo = ERR_NO_ERR;

char *ErrMsg[] = {
    /* 0 */ "No error",
    /* 1 */ "Fail to Free. Too Large Size!",
    /* 2 */ "Fail to Free. Wrong Address!",
    /* 3 */ "Fail to Allocate. Memory Space not Enough!",
    /* 4 */ "Memeroy is not enough",
    /* 5 */ "Invalid Input. (use 'help')"
};

void pError(){
    printf("Error: %s\n", ErrMsg[ErrNo]);
}


char* e_addr(Map *pos){
    return pos->m_addr + pos->m_size;
}

void memDisplay();

void memInit(){
    mem = (char *)malloc(M_SIZE);
    head = (Map *)malloc(sizeof(Map));
    head->m_addr = mem;
    head->m_size = M_SIZE;
    head->next = head;
    head->prior = head;
    cur = head;
} 

char* memAlloc(unsigned size){
    Map *pos = cur;
    char *l_addr;
    while(pos->m_size<size){
        pos = pos->next;
        if(pos==cur){
            ErrNo=ERR_ALLOCATE_FALI;
            return NULL;
        }
    }
    l_addr = pos->m_addr;
    pos->m_size -= size;
    pos->m_addr += size;
    cur = pos->next;
    if(pos->m_size==0){
        if(pos->next!=pos){
            pos->next->prior = pos->prior;
            pos->prior->next = pos->next;
            free(pos);
        }
    }
    return l_addr;
}

void lmalloc(unsigned size){
    char *addr=memAlloc(size);
    if(!addr){
        pError();
    }
    else{
        printf("Successfully Allocated %u Bytes at %u!\n", size, addr);
        memDisplay();
    }
}

//TODO:error info
//TODO:if in the last 
//info: use relative address

bool memFree(unsigned size,char *addr){
    Map *pos = head;
    Map *prev=pos->prior;
    //case1: addr is before head
    if(addr<head->m_addr){
        if(addr+size<pos->m_addr){
            Map* newblock= (Map *)malloc(sizeof(Map));
            newblock->m_addr = addr;
            newblock->m_size = size;
            newblock->prior = prev;
            newblock->next = pos;
            prev->next = newblock;
            pos->prior = newblock;
            head = newblock;
            return true;
        }
        else if(addr+size==pos->m_addr){
            pos->m_size += size;
            pos->m_addr = addr;
            return true;
        }
        else
            ErrNo = ERR_BAD_SIZE;
            return false;
    }
    while(pos->m_addr<addr){
        pos = pos->next;
    }
    prev = pos->prior;

    //TODO:error--if addr is before the prev block
    if(addr<e_addr(prev)){
        ErrNo = ERR_BAD_ADDR;
        return false;
    }
    if(addr==e_addr(prev)){
        if(addr+size<pos->m_addr){
            prev->m_size += size;
            return true;
        }
        if(addr+size==pos->m_addr){
            prev->m_size += (size + pos->m_size);
            if(cur==pos)
                cur = prev;
            prev->next = pos->next;
            pos->next->prior = prev;
            free(pos);
            return true;
        }
        if(addr+size>pos->m_addr){
            ErrNo = ERR_BAD_SIZE;
            return false;
        }
    }
    if(addr>e_addr(prev)){
        if(addr+size<pos->m_addr){
            Map* newblock= (Map *)malloc(sizeof(Map));
            newblock->m_addr = addr;
            newblock->m_size = size;
            newblock->prior = prev;
            newblock->next = pos;
            prev->next = newblock;
            pos->prior = newblock;
            return true;
        }
        else if(addr+size==pos->m_addr){
            pos->m_size += size;
            pos->m_addr = addr;
            return true;
        }
        else{
            ErrNo = ERR_BAD_SIZE;
            return false;
        }
    }
}

int lfree(unsigned size,unsigned r_addr){
    bool flag = memFree(size, mem + r_addr);
    if(!flag){
        pError();
    }
    else {
        printf("Successfully Free %u Bytes at %u!\n", size, r_addr);
        memDisplay();
    }
}

void memDisplay(){
    Map *pos = head;

    if(pos->m_size == 0)
    {
        printf("All Memory Used Out!\n\n");
        return ;
    }
    printf("Free Memory: \n");
    // iterate the items
    while(1){
        if(cur == pos)
            printf("    *|%d|%d|",pos->m_addr, pos->m_size);
        else
            printf("     |%d|%d|",pos->m_addr, pos->m_size);
        printf("\n");
        pos = pos->next;
        if(pos==head)
            break;
    }
    // make some space
    printf("\n");
}


void cmdParse(){
    char ch;
    char cmd[10];
    char cmdAlloc[10] = "lalloc";
    char cmdFree[10] = "lfree";
    char cmdExit[10] = "exit";
    char cmdHelp[10] = "help";
    unsigned int size, addr;
    
    // welcome information
    printf("************ Momery Management **********\n");
    printf("   Allocate: type 'lalloc size'          \n");
    printf("     Free  : type 'lfree size address'   \n");
    printf("     Quit  : type 'exit'                 \n");
    printf("     Help  : type 'help'                 \n");
    printf("*****************************************\n");
    
    while(1)
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
            printf("Have a good day!\n");
            while((ch = getchar()) != '\n' && ch != EOF);
            break;
        }
        else if(strcmp(cmdHelp, cmd) == 0)
        {// help information
            printf("************ Momery Management **********\n");
            printf("   Allocate: type 'lalloc size'          \n");
            printf("     Free  : type 'lfree size address'   \n");
            printf("     Quit  : type 'exit'                 \n");
            printf("     Help  : type 'help'                 \n");
            printf("*****************************************\n");
        }
        else
        {// undifined command
            ErrNo = ERR_BAD_CMD;
            pError();
            while((ch = getchar()) != '\n' && ch != EOF);
        }
    }
}


int main(){
    memInit();
    cmdParse();
    return 0;
}