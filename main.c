#include <conio.h>
#include <stdlib.h>
#include <c64.h>
/*******************
** Prototypes
*******************/
void INITSPRITES(void);
void INITRASTER(void);
/******************/
extern unsigned char SPRX[];
extern unsigned char SPRY[];
extern unsigned char SPRC[];
extern unsigned char SPRF[];
extern unsigned char NUMSPRITES;
extern unsigned char SPRUPDATEFLAG;
extern unsigned char SPRIRQCOUNTER;
extern unsigned short SPRITE_GFX;
#pragma zpsym ("NUMSPRITES")
#pragma zpsym ("SPRUPDATEFLAG")
/******************/
int main()
{    
    unsigned char XX = 0;
    unsigned char i;
    
    INITSPRITES();
    INITRASTER();
    NUMSPRITES = 32;
    
    for(i=0;i<32;++i)
    {
        SPRF[i] = (SPRITE_GFX&0x3fff)>>6; 
        SPRC[i] = i&0xfd;        
    }
    SPRX[30]=120;
    SPRX[31]=40;
    clrscr();
    while(1) 
    {
        gotoxy(1,1); cprintf("%u",SPRUPDATEFLAG);
    
        SPRY[30]=XX;
        SPRY[31]=255-XX;
        for(i=0;i<30;++i)
        {
            SPRX[i] = XX+(i/7)*42;
            SPRY[i] = 50+(i&7)*24;
        }
        XX = XX + 2;
        SPRUPDATEFLAG = 1;
    }
    return 0;
}
