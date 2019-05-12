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

/***************************************
* Pre-calculated
* sinus values
*******************/
const char yValues[] = {
    0x50, 0x58, 0x60, 0x67, 0x6F, 0x76, 0x7C, 0x83,
    0x89, 0x8E, 0x93, 0x97, 0x9A, 0x9D, 0x9E, 0xA0,
    0xA0, 0xA0, 0x9E, 0x9D, 0x9A, 0x97, 0x93, 0x8E,
    0x89, 0x83, 0x7C, 0x76, 0x6F, 0x67, 0x60, 0x58,
    0x50, 0x48, 0x40, 0x39, 0x31, 0x2A, 0x24, 0x1D,
    0x17, 0x12, 0x0D, 0x09, 0x06, 0x03, 0x02, 0x00,
    0x00, 0x00, 0x02, 0x03, 0x06, 0x09, 0x0D, 0x12,
    0x17, 0x1D, 0x24, 0x2A, 0x31, 0x39, 0x40, 0x48,
    0x50, 0x58, 0x60, 0x67, 0x6F, 0x76, 0x7C, 0x83,
    0x89, 0x8E, 0x93, 0x97, 0x9A, 0x9D, 0x9E, 0xA0,
    0xA0, 0xA0, 0x9E, 0x9D, 0x9A, 0x97, 0x93, 0x8E,
    0x89, 0x83, 0x7C, 0x76, 0x6F, 0x67, 0x60, 0x58,
    0x50, 0x48, 0x40, 0x39, 0x31, 0x2A, 0x24, 0x1D,
    0x17, 0x12, 0x0D, 0x09, 0x06, 0x03, 0x02, 0x00,
    0x00, 0x00, 0x02, 0x03, 0x06, 0x09, 0x0D, 0x12,
    0x17, 0x1D, 0x24, 0x2A, 0x31, 0x39, 0x40, 0x48,
    0x50, 0x58, 0x60, 0x67, 0x6F, 0x76, 0x7C, 0x83,
    0x89, 0x8E, 0x93, 0x97, 0x9A, 0x9D, 0x9E, 0xA0,
    0xA0, 0xA0, 0x9E, 0x9D, 0x9A, 0x97, 0x93, 0x8E,
    0x89, 0x83, 0x7C, 0x76, 0x6F, 0x67, 0x60, 0x58,
    0x50, 0x48, 0x40, 0x39, 0x31, 0x2A, 0x24, 0x1D,
    0x17, 0x12, 0x0D, 0x09, 0x06, 0x03, 0x02, 0x00,
    0x00, 0x00, 0x02, 0x03, 0x06, 0x09, 0x0D, 0x12,
    0x17, 0x1D, 0x24, 0x2A, 0x31, 0x39, 0x40, 0x48,
    0x50, 0x58, 0x60, 0x67, 0x6F, 0x76, 0x7C, 0x83,
    0x89, 0x8E, 0x93, 0x97, 0x9A, 0x9D, 0x9E, 0xA0,
    0xA0, 0xA0, 0x9E, 0x9D, 0x9A, 0x97, 0x93, 0x8E,
    0x89, 0x83, 0x7C, 0x76, 0x6F, 0x67, 0x60, 0x58,
    0x50, 0x48, 0x40, 0x39, 0x31, 0x2A, 0x24, 0x1D,
    0x17, 0x12, 0x0D, 0x09, 0x06, 0x03, 0x02, 0x00,
    0x00, 0x00, 0x02, 0x03, 0x06, 0x09, 0x0D, 0x12,
    0x17, 0x1D, 0x24, 0x2A, 0x31, 0x39, 0x40, 0x48
};
/***************************************
* Sprites color table
*******************/
const char Colors[] = {
    0x0B, 0x05, 0x0D, 0x01, 0x0D, 0x05, 0x0B, 0x0C,  // Green 2
    0x06, 0x0E, 0x03, 0x01, 0x03, 0x0E, 0x06, 0x0C,  // Blue
    0x0B, 0x0C, 0x0F, 0x01, 0x0F, 0x0C, 0x0B, 0x0C,  // Grey
    0x05, 0x03, 0x0D, 0x01, 0x0D, 0x03, 0x05, 0x0C,  // Green
    0x06, 0x0E, 0x03, 0x01, 0x03, 0x0E, 0x06, 0x0C  // Blue
};
/***************************************
* Scrolltext
******************
* C64 screen codes for scrolltext
*****************/
#include <cbm_screen_charmap.h>
/******************/
const char scrolltext[] =
    "ciao a tutti !"
    " questo e' un "    
    "esempio di sin"
    " scroller basa"
    "to su sprites."
    "              "
    "by flavioweb, "
    "fabrizio carus"
    "o e antonio po"
    "rcino per gli "
    "amici di faceb"
    "ook e non...  "
    "happy hacking!"
    "              ";
/**************************************
* C64 petscii char encoding
*******************/ 
#include <cbm_petscii_charmap.h>
/******************/
int main()
{    
    unsigned char XX = 0;
    unsigned char SX = 0;
    unsigned short SP = 0;
    unsigned char DL = 0;
    unsigned char CP = 0;
    unsigned char i;
    #if defined(MULTI_COLOR) 
    unsigned char bufm;
    #endif
    #if defined(EXPAND_X) 
    unsigned char bufex;
    #endif
    #if defined(EXPAND_Y) 
    unsigned char bufey;
    #endif    
/******************/
    POKE(0xd020, 0x00);
    POKE(0xd021, 0x00);
    clrscr();
    
    INITSPRITES();
    INITRASTER();
    NUMSPRITES = 16;
/***************************************
* Initial sprites frames and colors
* setup.
*******************/
    for(i=0;i<NUMSPRITES;++i)
    {
        SPRF[i] = GFX_START_INDEX+' '; 
        SPRC[i] = Colors[CP+i];
        #if defined(MULTI_COLOR)
        SPRM[i] = i&1;
        #endif
        #if defined(EXPAND_X)
        SPREX[i] = i&1;
        #endif
        #if defined(EXPAND_Y)
        SPREY[i] = i&1;
        #endif        
    }

    #if defined(MULTI_COLOR)
    POKE (0xd025,2);
    POKE (0xd026,4);
    #endif
/***************************************
* Main loop
*******************/
    while(1) 
    {
        //gotoxy(1,1); cprintf("%02u",DL);      // Value dump for debug
        // Check if we are in sync with Multiplexer
        if (MULTIPLEX_DONE) {
            // Check if we moved one sprite/char out of screen.
            if (DL == 12) {
                #if defined(MULTI_COLOR)
                bufm = SPRM[0];
                #endif
                #if defined(EXPAND_X)
                bufex = SPREX[0];
                #endif   
                #if defined(EXPAND_Y)
                bufey = SPREY[0];
                #endif                 
                // Move all sprites/linetext 1 chars forward
                for(i=1;i<NUMSPRITES;++i)
                {
                    SPRF[i-1]=SPRF[i];
                    #if defined(MULTI_COLOR)
                    SPRM[i-1]=SPRM[i];
                    #endif
                    #if defined(EXPAND_X)
                    SPREX[i-1]=SPREX[i];
                    #endif
                    #if defined(EXPAND_Y)
                    SPREY[i-1]=SPREY[i];
                    #endif                        
                }
                // Insert new char from scrolltext
                SPRF[NUMSPRITES-1]=GFX_START_INDEX+(scrolltext[SP++]);
                #if defined(MULTI_COLOR)
                SPRM[NUMSPRITES-1]=bufm;
                #endif
                #if defined(EXPAND_X)
                SPREX[NUMSPRITES-1]=bufex;
                #endif     
                #if defined(EXPAND_Y)
                SPREY[NUMSPRITES-1]=bufey;
                #endif                   
                // End of Scrolltext?
                if (SP>=strlen(scrolltext)-1) { SP=0; }
                // Reset Sinus and sprites scroll pointer
                SX = SX-59;
                DL = 0;
                CP++;
            }
            
            // Do sprites positioning and color change
            for(i=0;i<NUMSPRITES;++i)
            {
                XX = SX+(i*5);
                SPRX[i] = 12+(i*12)-DL;
                SPRY[i] = 60+yValues[XX];
                SPRC[i] = Colors[CP+i];
            }
        // Color cycles with some delay
        if ((DL&0x0F)==0) {
            ++CP;
            if (CP>=(sizeof(Colors)-NUMSPRITES)) { CP = 0; }
        }
        ++DL;
        ++SX;
        // Set flags to make multiplexer know that
        // new sprites can be displayed.
        MULTIPLEX_DONE = 0;
        SPRUPDATEFLAG = 1;
        }
    }
    // Never fall here...
    return 0;
}
