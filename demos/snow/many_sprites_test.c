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

const char MESSAGE[12] = "HAPPYNEWYEAR";


#define Y_OFFSET 60

/******************/
int main()
{    
    unsigned char XX = 0;
    unsigned char XX2 = 100;
    unsigned char i;

    // Only use clrscr() before when the kernal is still active
    clrscr();  
    
    INITSPRITES();
    INITRASTER();
    
    POKE(0xd020, 0x00);
    POKE(0xd021, 0x00);    
    
    NUMSPRITES = _NUMBER_OF_SPRITES_;

    // SPRF[i] = GFX_START_INDEX + 'H' - 'A' + 1;
    

    for(i=0;i<12;++i)
    {
        // SPRF[i] = GFX_START_INDEX+1+i+21*(i>=(26));
        // SPRF[i]= GFX_START_INDEX+i;
        SPRF[i] = GFX_START_INDEX + MESSAGE[i] - 'A' + 1;

        SPRC[i] = 2+i;        
    }    
    for(i=12;i<NUMSPRITES;++i)
    {
        // SPRF[i] = GFX_START_INDEX+1+i+21*(i>=(26));
        SPRF[i]= GFX_START_INDEX+62+(i&1);

        SPRC[i] = 1;        
    }
    // for(i=36;i<NUMSPRITES;++i)
    // {
        // SPRF[i]= GFX_START_INDEX+i+1-36;

    // }
    

    
    for(i=0;i<5;++i)
    {
        SPRX[i]=40+i*22;
        SPRY[i]=90;
    }
    for(i=0;i<3;++i)
    {
        SPRX[i+5]=40+i*22;
        SPRY[i+5]=120;
    }    
    for(i=0;i<4;++i)
    {
        SPRX[i+8]=40+i*22;
        SPRY[i+8]=150;
    }
    
    SPRX[NUMSPRITES-1]=120;
    SPRX[NUMSPRITES-3]=40;
    SPRX[NUMSPRITES-2]=100;
    SPRX[NUMSPRITES-4]=60;   
    
    while(1) 
    {
        if (MULTIPLEX_DONE) {
            SPRY[NUMSPRITES-1]=XX;
            SPRY[NUMSPRITES-2]=255-XX;

            SPRY[NUMSPRITES-3]=XX2;
            SPRY[NUMSPRITES-4]=255-XX2;
            

            for(i=0;i<5;++i)
            {
                SPRX[i]=40+i*22;
                SPRY[i]=Y_OFFSET+yValues[XX];
            }
            for(i=0;i<3;++i)
            {
                SPRX[i+5]=40+i*22;
                SPRY[i+5]=Y_OFFSET+30+yValues[XX];;
            }    
            for(i=0;i<4;++i)
            {
                SPRX[i+8]=40+i*22;
                SPRY[i+8]=Y_OFFSET+60+yValues[XX];;
            }            


            ++XX;
            ++XX2;
            MULTIPLEX_DONE = 0;
            SPRUPDATEFLAG = 1;
        }
    }
    return 0;
}
