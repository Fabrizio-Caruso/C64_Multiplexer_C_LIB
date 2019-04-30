#include <conio.h>
#include <stdlib.h>
#include <c64.h>
/*******************
** Prototypes
*******************/
void START(void);
void MUOVI_SPRITES_ESEMPIO(void);
void WAIT_FOR_SORT(void);

extern unsigned char SPRX[];
extern unsigned char SPRY[];
extern unsigned char SPRC[];
extern unsigned char SPRF[];
extern unsigned char NUMSPRITES;
extern unsigned char SPRUPDATEFLAG;
#pragma zpsym ("NUMSPRITES")
#pragma zpsym ("SPRUPDATEFLAG")
extern unsigned char SPRIRQCOUNTER;
extern unsigned short SPRITE_GFX;


/******************/
int main()
{    
    unsigned char XX = 0;
    unsigned char i;
    START();
    for(i=0;i<32;++i)
    {
        SPRF[i] = (SPRITE_GFX&0x3fff)>>6;        
    }
    SPRX[30]=30;
    SPRX[31]=130;
    clrscr();
    while(1) 
    {
        gotoxy(1,1); cprintf("%u",SPRUPDATEFLAG);
    
        SPRY[30]=XX;
        SPRY[31]=255-XX;
        for(i=0;i<30;++i)
        {
            SPRC[i] = i;
            SPRX[i] = XX+4*i;
            SPRY[i] = 55+(i & 7)*24;
        }
        ++XX;
        SPRUPDATEFLAG = 1;
    }
    return 0;
}
