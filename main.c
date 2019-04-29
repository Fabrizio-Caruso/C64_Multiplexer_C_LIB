#include <conio.h>
#include <stdlib.h>
#include <c64.h>
/*******************
** Prototypes
*******************/
void START(void);
void MUOVI_SPRITES_ESEMPIO(void);
void WAIT_FOR_SORT(void);

extern unsigned char *SPRX;
extern unsigned char *SPRY;
extern unsigned char *SPRC;
extern unsigned char *SPRF;

unsigned char XX = 0;

/******************/
int conta;
int main()
{
    START();
    while(1) {
    //conta=conta+1;
    //    if (conta==50) {            
            WAIT_FOR_SORT();
            // SPRX[0] = XX;
            // SPRY[0] = 50;
            // SPRX[1] = XX*2;
            // SPRY[1] = 80;
            // XX++;
            MUOVI_SPRITES_ESEMPIO();
            conta = 0;
    //    };
    };
	return 0;
}
