#include <conio.h>
#include <stdlib.h>
#include <c64.h>
#include <peekpoke.h>

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
	
	return 0;
}
