#include <conio.h>
#include <stdlib.h>
#include <c64.h>
/*******************
** Prototypes
*******************/
void START(void);
void MUOVI_SPRITES_ESEMPIO(void);
void WAIT_FOR_SORT(void);

extern char SPRX[];
extern char SPRY[];
extern char SPRC[];
extern char SPRF[];
extern char NUMSPRITES;
extern char SPRUPDATEFLAG;
extern char SPRIRQCOUNTER;

unsigned char XX = 0;
int conta;
/******************/
int main()
{
    START();
    while(1) {
            if (conta == 100) {
                SPRX[0] = XX;
                SPRY[0] = 50;
                SPRX[1] = XX*2;
                SPRY[1] = 70;
                SPRX[2] = XX*3;
                SPRY[2] = 90;
                SPRX[3] = XX*4;
                SPRY[3] = 110;
                SPRX[4] = XX*5;
                SPRY[4] = 130;
                SPRX[5] = XX*6;
                SPRY[5] = 150;
                SPRX[6] = XX*7;
                SPRY[6] = 170;
                XX = XX+1;
                NUMSPRITES = 7;
                conta = 0;
            }
        conta = conta +1;
    }
}
