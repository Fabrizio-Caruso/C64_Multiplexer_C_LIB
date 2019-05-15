/***************************************
* Sinus Sprite Scroller
* By Flavioweb
* Fabrizio Caruso
* Antonio Porcino
* 2019
*******************/
#include <conio.h>
#include <stdlib.h>
#include <string.h>
/******************/
#include <peekpoke.h>
/*******************
* Prototypes
*******************/
void INITSPRITES(void);
void INITRASTER(void);
/******************/
extern unsigned char SPRX[];
extern unsigned char SPRY[];
extern unsigned char SPRC[];
extern unsigned char SPRF[];
#if defined(MULTI_COLOR)
extern unsigned char SPRM[];
#endif
#if defined(EXPAND_X)
extern unsigned char SPREX[];
#endif
#if defined(EXPAND_Y)
extern unsigned char SPREY[];
#endif
extern unsigned char NUMSPRITES;
extern unsigned char SPRUPDATEFLAG;
extern unsigned char SPRIRQCOUNTER;
extern unsigned char MULTIPLEX_DONE;
#pragma zpsym ("NUMSPRITES")
#pragma zpsym ("SPRUPDATEFLAG")
#pragma zpsym ("MULTIPLEX_DONE")

#if defined(__C64__)
    #define GFX_START 0x2000
#else
    #define GFX_START 0x3000
#endif
#define GFX_START_INDEX (GFX_START/0x40)

int main()
{    

    unsigned char i = 0;
    unsigned char j;
    unsigned short k;
/******************/
    // POKE(0xd020, 0x00);
    // POKE(0xd021, 0x00);
    clrscr();
    // cgetc();
        
    INITSPRITES();
    INITRASTER();
    NUMSPRITES = 16;

/***************************************
* Main loop
*******************/

    for(j=0;j<8;++j)
    {
        SPRX[j]= 0x17*((j&7)+3);
        gotoxy(1,j); cprintf("SPRX: %02u",SPRX[j]);
        
        SPRY[j]= 0x60;
        gotoxy(16,j); cprintf("SPRY: %02u",SPRY[j]);
        
        SPRF[j]= 0x80+j;
        gotoxy(30,j); cprintf("SPRF: %02u",SPRF[j]);        
    }

    for(j=0;j<8;++j)
    {
        SPRX[j+8]= 0x17*((j&7)+3);
        gotoxy(1,j+8); cprintf("SPRX: %02u",SPRX[j+8]);
        
        SPRY[j+8]= 0xA0;
        gotoxy(16,j+8); cprintf("SPRY: %02u",SPRY[j+8]);
        
        SPRF[j+8]= 0x88+j;
        gotoxy(30,j+8); cprintf("SPRF: %02u",SPRF[j+8]);        
    }    
    
    while(1) 
    {
        for(j=0;j<16;++j)
        {
            SPRC[j]= (i+j)&0xFD;
            SPRF[j]= SPRF[(j+1)&15];
        }  
        ++i;
        i=i&0x1F;
        for(k=0;k<8000;++k){};
    }
    // Never fall here...
    return 0;
}
