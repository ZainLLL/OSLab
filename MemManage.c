#include "MemManage.h"

char *ErrMsg[] = {
    /* 0 */ "No error",
    /* 1 */ "Fail to Free! Check Size and Address.",
    /* 2 */ "Fail to Allocate. Memory Space not Enough!",
    /* 3 */ "Memory Overflow!",
    /* 4 */ "Invalid Input. (type 'help' for manual)"
};

Map *cur;
Map *head;
char *mem;
Error ErrNo = ERR_NO_ERR;

void pError(){
    printf("Error: %s\n", ErrMsg[ErrNo]);
}

char* e_addr(Map *pos){
    return pos->m_addr + pos->m_size;
}

char* b_addr(Map *pos){
    return pos->m_addr;
}

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

bool memFree(unsigned size,char *addr){
    Map *prev_map;
    Map *next_map;
    Map *tail = head->prior;
    char *end_addr = addr + size;

    //check range 
    if(end_addr>mem+M_SIZE){
        ErrNo = ERR_MEM_OVERFLOW;
        return false;
    }
    
    //if all memory used
    if(tail==head&&!head->m_size){
        head->m_addr = addr;
        head->m_size = size;
        return true;
    }

    //if freeblock is before head
    if(head->m_addr>addr){
        if(end_addr<head->m_addr){
            memFreeCase1(size, addr, head);
            head = head->prior;
            return true;
        }
        else if(end_addr==head->m_addr){
            memFreeCase2(size, addr, head);
            return true;
        }
        else
            ErrNo = ERR_FREE_FAIL;
            return false;
    }
    
    //if freeblock is after tail
    if(e_addr(tail)<=addr){
        if(e_addr(tail)<addr){
            memFreeCase1(size, addr, head);
            return true;
        }
        else{
            memFreeCase3(size, tail);
            return true;
        }
    }
    
    //illeagal check
    bool flag = false;
    for (prev_map = head, next_map = head->next; prev_map->next != head;prev_map = prev_map->next,next_map=next_map->next)
    {
        if(e_addr(prev_map)<=addr&&next_map->m_addr>=end_addr){
            flag = true;
            break;
        }
    }
    if(!flag){
        ErrNo = ERR_FREE_FAIL;
        return false;
    }

    //freeblock is in the middle. consider 4 cases
    if(addr>e_addr(prev_map)&&end_addr<next_map->m_addr){
        memFreeCase1(size, addr, next_map);
        return true;
    }
    if(addr>e_addr(prev_map)&&end_addr==next_map->m_addr){
        memFreeCase2(size, addr, next_map);
        return true;
    }
    if(addr==e_addr(prev_map)&&end_addr<next_map->m_addr){
        memFreeCase3(size, prev_map);
        return true;
    }
    if(addr==e_addr(prev_map)&&end_addr==next_map->m_addr){
        memFreeCase4(size, prev_map);
        return true;
    }    
}

void lfree(unsigned size,unsigned r_addr){
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
    
    printf("%-15s|%10s|%10s|%10s|\n","Free Memory:","address","r_address","size");
    // iterate the items
    while(1){
        if(cur == pos)
            printf("%14s*|%10u|%10u|%10u|", " ", pos->m_addr, pos->m_addr - mem, pos->m_size);
        else
            printf("%14s |%10u|%10u|%10u|", " ", pos->m_addr, pos->m_addr - mem, pos->m_size);
        printf("\n");
        pos = pos->next;
        if(pos==head)
            break;
    }
    // make some space
    printf("\n");
}


void memFreeCase1(unsigned size, char *addr, Map *next_map){
    Map* newblock= (Map *)malloc(sizeof(Map));
    Map* prev_map = next_map->prior;
    newblock->m_addr = addr;
    newblock->m_size = size;
    newblock->prior = prev_map;
    newblock->next = next_map;
    prev_map->next = newblock;
    next_map->prior = newblock;
}

void memFreeCase2(unsigned size, char *addr, Map *next_map){
    next_map->m_size += size;
    next_map->m_addr = addr;
}

void memFreeCase3(unsigned size, Map *prev_map){
    prev_map->m_size += size;
}

void memFreeCase4(unsigned size, Map *prev_map){
    Map *next_map = prev_map->next;
    prev_map->m_size += (size + next_map->m_size);
    if(cur==next_map)
        cur = prev_map;
    prev_map->next = next_map->next;
    next_map->next->prior = prev_map;
    free(next_map);
}