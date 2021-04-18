#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define M_SIZE 1000


typedef enum _Error
{
    ERR_NO_ERR = 0,
    ERR_FREE_FAIL = 1,
    ERR_ALLOCATE_FALI = 2,
    ERR_MEM_OVERFLOW = 3,
    ERR_BAD_CMD = 4
} Error;

void pError();


/*map struct and functions*/
typedef struct _Map{
    unsigned m_size;
    char *m_addr;
    struct _Map *next, *prior;
} Map;

char *e_addr(Map *pos);
char *b_addr(Map *pos);

void memInit();
void memDisplay();
char* memAlloc(unsigned size);
void lmalloc(unsigned size);

bool memFree(unsigned size, char *addr);
void memFreeCase1(unsigned size, char *addr, Map *next_map);
void memFreeCase2(unsigned size, char *addr, Map *next_map);
void memFreeCase3(unsigned size, Map *prev_map);
void memFreeCase4(unsigned size, Map *prev_map);
void lfree(unsigned size, unsigned r_addr);

void cmdParse();