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

#define X_SPACING 0x10
#define X_OFFSET 2

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
    #if defined(NO_INPUT)
        unsigned short k;
    #else
        unsigned char ch;
    #endif
/******************/
    // POKE(0xd020, 0x00);
    // POKE(0xd021, 0x00);
    clrscr();
    
    #if !defined(NO_INPUT)
        gotoxy(1,1);cprintf("press a key to start");
        cgetc();
        clrscr();
    #endif
        
    INITSPRITES();
    INITRASTER();
    NUMSPRITES = 16;
    
    #if defined(__C128__)
        POKE(0x0D8,0xFF);
        POKE(0x2564,PEEK(0x2564)&254);
    #endif

/***************************************
* Main loop
*******************/

    for(j=0;j<8;++j)
    {
        SPRX[j]= X_SPACING*((j&7)+X_OFFSET);
        gotoxy(1,j); cprintf("SPRX: %02u",SPRX[j]);
        
        SPRY[j]= 0x60-j*6;
        gotoxy(16,j); cprintf("SPRY: %02u",SPRY[j]);
        
        SPRF[j]= GFX_START_INDEX+j;
        gotoxy(30,j); cprintf("SPRF: %02u",SPRF[j]);  

        SPRC[j]= j&0xFD;        
    }

    for(j=0;j<8;++j)
    {
        SPRX[j+8]= X_SPACING*((j&7)+X_OFFSET);
        gotoxy(1,j+8); cprintf("SPRX: %02u",SPRX[j+8]);
        
        SPRY[j+8]= 0xA0+j*6;
        gotoxy(16,j+8); cprintf("SPRY: %02u",SPRY[j+8]);
        
        SPRF[j+8]= GFX_START_INDEX+0x08+j;
        gotoxy(30,j+8); cprintf("SPRF: %02u",SPRF[j+8]);        
    }    
    
    while(1) 
    {
        for(j=0;j<16;++j)
        {
            SPRC[j]= SPRF[(j+1)&15];
            SPRF[j]= SPRF[(j+1)&15];
        }  
        ++i;
        #if !defined(NO_INPUT)
            ch=cgetc();
            --SPRX[0];
            ++SPRX[8];
            --SPRX[1];
            ++SPRX[9];           
        #else
            for(k=0;k<7400;++k){};
        #endif
    }
    // Never fall here...
    return 0;
}
