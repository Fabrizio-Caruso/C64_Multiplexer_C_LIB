#include <conio.h>
#include <stdlib.h>
#include <c64.h>

#define PEEK(addr)         (*(unsigned char*) (addr))

/*******************
** Prototypes
*******************/
void startirq(void);
void stopirq(void);

/***************************************
** entry point
***************************************/
int main(void)
{
    startirq();
    while(1){};
}
