/***************************************
* Irq Test-case
*******************/
#include <conio.h>
#include <stdlib.h>
#include <string.h>
/******************/
#include <peekpoke.h>
/*******************
* Prototypes
*******************/
void INITRASTER(void);
/******************/
#include <cbm_petscii_charmap.h>
/******************/
int main()
{
/******************/
    POKE(0xd020, 0x00);
    POKE(0xd021, 0x00);
    clrscr();
    INITRASTER();
/***************************************
* Main loop
*******************/
    while(1) 
    {
    
    }
    // Never fall here...
    return 0;
}
