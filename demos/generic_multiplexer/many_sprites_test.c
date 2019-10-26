#include <conio.h>
#include <stdlib.h>
#include <peekpoke.h>
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
extern unsigned char MULTIPLEX_DONE;
extern unsigned short SPRITE_GFX;
#pragma zpsym ("NUMSPRITES")
#pragma zpsym ("SPRUPDATEFLAG")
#pragma zpsym ("MULTIPLEX_DONE")
/******************/
// Pre-calculated sinus values
const char yValues[] = {
    32, 35, 38, 41, 44, 47, 49, 52, 
    54, 56, 58, 60, 61, 62, 63, 63, 
    64, 63, 63, 62, 61, 60, 58, 56, 
    54, 52, 49, 47, 44, 41, 38, 35, 
    32, 28, 25, 22, 19, 16, 14, 11, 
    9, 7, 5, 3, 2, 1, 0, 0, 
    0, 0, 0, 1, 2, 3, 5, 7, 
    9, 11, 14, 16, 19, 22, 25, 28,
    32, 35, 38, 41, 44, 47, 49, 52, 
    54, 56, 58, 60, 61, 62, 63, 63, 
    64, 63, 63, 62, 61, 60, 58, 56, 
    54, 52, 49, 47, 44, 41, 38, 35, 
    32, 28, 25, 22, 19, 16, 14, 11, 
    9, 7, 5, 3, 2, 1, 0, 0, 
    0, 0, 0, 1, 2, 3, 5, 7, 
    9, 11, 14, 16, 19, 22, 25, 28,
    32, 35, 38, 41, 44, 47, 49, 52, 
    54, 56, 58, 60, 61, 62, 63, 63, 
    64, 63, 63, 62, 61, 60, 58, 56, 
    54, 52, 49, 47, 44, 41, 38, 35, 
    32, 28, 25, 22, 19, 16, 14, 11, 
    9, 7, 5, 3, 2, 1, 0, 0, 
    0, 0, 0, 1, 2, 3, 5, 7, 
    9, 11, 14, 16, 19, 22, 25, 28,
    32, 35, 38, 41, 44, 47, 49, 52, 
    54, 56, 58, 60, 61, 62, 63, 63, 
    64, 63, 63, 62, 61, 60, 58, 56, 
    54, 52, 49, 47, 44, 41, 38, 35, 
    32, 28, 25, 22, 19, 16, 14, 11, 
    9, 7, 5, 3, 2, 1, 0, 0, 
    0, 0, 0, 1, 2, 3, 5, 7, 
    9, 11, 14, 16, 19, 22, 25, 28
};

#if defined(__C64__)
    #define GFX_START 0x2000
#else
    #define GFX_START 0x3000
#endif
#define GFX_START_INDEX (GFX_START/0x40)

/******************/
int main()
{    
    unsigned char XX = 0;
    unsigned char i;

    // Only use clrscr() before when the kernal is still active
    clrscr();  
    
    INITSPRITES();
    INITRASTER();
    
    POKE(0xd020, 0x06);
    POKE(0xd021, 0x06);    
    
    NUMSPRITES = _NUMBER_OF_SPRITES_;
    
    for(i=0;i<NUMSPRITES;++i)
    {
        SPRF[i] = GFX_START_INDEX+1+i+21*(i>=(26));
        SPRC[i] = i&0xfd;        
    }
    for(i=36;i<NUMSPRITES;++i)
    {
        SPRF[i]= GFX_START_INDEX;
    }
    
    SPRX[NUMSPRITES-2]=120;
    SPRX[NUMSPRITES-1]=40;
    while(1) 
    {
        if (MULTIPLEX_DONE) {
            SPRY[NUMSPRITES-2]=XX;
            SPRY[NUMSPRITES-1]=255-XX;
            for(i=0;i<NUMSPRITES-2;++i)
            {
                SPRX[i] = XX+(i/7)*(yValues[XX]);
                SPRY[i] = 40+_SPRITE_SEPARATION_*(i&7)+(yValues[XX]>>1);
            }
            ++XX;
            MULTIPLEX_DONE = 0;
            SPRUPDATEFLAG = 1;
        }
    }
    return 0;
}
