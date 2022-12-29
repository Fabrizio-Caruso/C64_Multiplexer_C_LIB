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
    #if defined(SPRITES_AT_2800)
        #define GFX_START 0x2800U
    #else
        #define GFX_START 0x2000U
    #endif
#else
    #define GFX_START 0x3000U
#endif
#define GFX_START_INDEX (GFX_START/0x40U)

const char MESSAGE[12] = "HAPPYNEWYEAR";

const unsigned char COLORS[14] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15};

#define X_OFFSET 46
#define Y_OFFSET 55

// $D018 = 53272
// -----------------
void init_udg(void)
{
    POKE(56576u,3);
	POKE(0xD018,PEEK(0xD018)|8);
	POKE(0xD018,PEEK(0xD018)&(255-4-2));    
    // POKE(0xD018,PEEK(0xD018)&0xF0);
	POKE(648,192);
}




/******************/
int main()
{    
    unsigned char XX = 0;
    unsigned char XX2 = 100;
    unsigned char i;
    unsigned char j;

    // Only use clrscr() before when the kernal is still active
    clrscr();  
    
    INITSPRITES();
    INITRASTER();
    
    init_udg();
    
    POKE(0xd020, 0x00);
    POKE(0xd021, 0x00);    
    
    for(i=0;i<200;++i)
    {
        POKE(0x0400+i,i);
    }
    
    for(i=0;i<200;++i)
    {
        POKE(0x2000+i,255);
    }    
    
    
    NUMSPRITES = _NUMBER_OF_SPRITES_;

    // SPRF[i] = GFX_START_INDEX + 'H' - 'A' + 1;
    

    for(i=0;i<12;++i)
    {
        // SPRF[i] = GFX_START_INDEX+1+i+21*(i>=(26));
        // SPRF[i]= GFX_START_INDEX+i;
        SPRF[i] = GFX_START_INDEX + MESSAGE[i] - 'A' + 1;

        SPRC[i] = 2+i;        
    }    
    
    // SPRF[0] = 112;
    // SPRF[1] = 113;
    // SPRF[2] = 114;
    // SPRF[3] = 115;
    // SPRF[4] = 116;
    
        // SPRF[0] = 128;
        // SPRF[1] = 129;
        // SPRF[2] = 130;
        // SPRF[3] = 131;
        // SPRF[4] = 132;
    
    
    for(i=12;i<NUMSPRITES;++i)
    {
        // SPRF[i] = GFX_START_INDEX+1+i+21*(i>=(26));
        SPRF[i]= GFX_START_INDEX+62+(i&1);

        SPRC[i] = 1;        
    }
    
    // Position snow flakes
    SPRX[NUMSPRITES-1]=120;
    SPRX[NUMSPRITES-3]=40;
    SPRX[NUMSPRITES-2]=100;
    SPRX[NUMSPRITES-4]=60;   

    SPRY[NUMSPRITES-5]=90;
    SPRY[NUMSPRITES-6]=60;  
    SPRY[NUMSPRITES-7]=140;
    SPRY[NUMSPRITES-8]=170;  
    
    while(1) 
    {
        if (MULTIPLEX_DONE) {
            SPRY[NUMSPRITES-1]=XX;
            SPRY[NUMSPRITES-2]=255-XX;

            SPRY[NUMSPRITES-3]=XX2;
            SPRY[NUMSPRITES-4]=255-XX2;
            
            SPRX[NUMSPRITES-5]=XX2;
            SPRX[NUMSPRITES-6]=255-XX2;

            SPRX[NUMSPRITES-7]=XX;
            SPRX[NUMSPRITES-8]=255-XX;

            for(i=0;i<5;++i)
            {
                SPRX[i]=X_OFFSET+i*22;
                SPRY[i]=i*8+Y_OFFSET+yValues[XX];
            }
            for(i=0;i<3;++i)
            {
                SPRX[i+5]=X_OFFSET+i*22;
                SPRY[i+5]=i*8+Y_OFFSET+40+yValues[XX];;
            }    
            for(i=0;i<4;++i)
            {
                SPRX[i+8]=X_OFFSET+i*22;
                SPRY[i+8]=i*8+Y_OFFSET+80+yValues[XX];;
            }            

            if(!(XX&31  ))
            {
                ++j;
                for(i=0;i<12;++i)
                {
                    SPRC[i] = 2+((XX+i+j)&7);        
                }
            }


            ++XX;
            ++XX2;
            MULTIPLEX_DONE = 0;
            SPRUPDATEFLAG = 1;
        }
    }
    return 0;
}
