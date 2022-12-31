// #include <conio.h>
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
extern unsigned char SPREX[];

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

const char AUTHOR[15] = "FABRIZIO CARUSO";
const char TEXT1[15] =  "LOGIC CODE IN C";
const char TEXT2[28] =  "MODIFIED CADAVER MULTIPLEXER";
const char TEXT3[29] =  "SID TUNE AND FONTS RIPPED OFF";
const char TEXT4[17] =  "FAST MODE ENABLED";

const char WRITTEN[15] = "WRITTEN IN C BY";

const char YEAR[14] =   "HAPPY NEW YEAR";

const unsigned char STAR_0[4] = {0x00,0x10,0x10,0x00};
const unsigned char STAR_1[4] = {0x10,0x28,0x38,0x10};
// const star STAR_0[4] = {};

#define X_OFFSET 46
#define Y_OFFSET 50

#define SCREEN 0x0400
#define COLOR  0xD800
#define SHAPE  0x2000

#define SEPARATION 40   

#define HAPPYNEWYEAR_POS (SCREEN+6)
#define MOON_OFFSET (40*2+36)


static unsigned char j;
static unsigned char h;
static unsigned char restored_text_row[35];

// $D018 = 53272
// -----------------
void init_udg(void)
{
    POKE(56576u,3);
	POKE(0xD018,PEEK(0xD018)|8);
	POKE(0xD018,PEEK(0xD018)&(255-4-2));    
	POKE(648,192);
}

void color_change(void)
{
    for(h=0;h<10;++h)
    {
        
        POKE(COLOR+6+h,3+(j&7));
        POKE(COLOR+40+6+h,3+(j&7));
    }

    for(h=0;h<6;++h)
    {
        
        POKE(COLOR+18+h,3+(j&7));
        POKE(COLOR+40+18+h,3+(j&7));
    }
    
    for(h=0;h<8;++h)
    {
        
        POKE(COLOR+26+h,3+(j&7));
        POKE(COLOR+40+26+h,3+(j&7));
    }  
}

#define COMET_X 0
#define COMET_Y 22

void print(const char *str, unsigned char len, unsigned short offset, unsigned char col)
{
    unsigned char k;
    
    for(k=0;k<len;++k)
    {
        if(str[k]!=' ')
        {
            POKE(SCREEN+offset+k,str[k]-'A'+1);
            POKE(COLOR+offset+k,col);
        }
    }
}

void restore_text_row(void)
{
    unsigned char i;
    
    for(i=0;i<35;++i)
    {
        POKE(SCREEN+120+i,restored_text_row[i]);
        POKE(COLOR+120+i,1);
    }
}

/******************/
int main()
{    
    unsigned char XX = 1;
    unsigned char XX2 = 100;
    unsigned char i;
    unsigned short k;
    unsigned short star_loc;
    unsigned char flip = 1;
    unsigned char comet_flash;
    unsigned char comet_x = COMET_X;
    unsigned char comet_y = COMET_Y;
    unsigned short comet_pos;
    unsigned comet_counter = 0;
    unsigned short old_comet_pos;
    
    // unsigned char comet2_flash;
    unsigned char comet2_x = COMET_X+3;
    unsigned char comet2_y = COMET_Y-2;
    unsigned short comet2_pos;
    unsigned comet2_counter = 0;
    unsigned short old_comet2_pos;
    
    unsigned char below;
    unsigned char text_counter = 0;
    
    // unsigned char below_1;

    // Only use clrscr() before when the kernal is still active
    // clrscr();  
    for(k=0;k<1000;++k)
    {
        POKE(SCREEN+k,32);
        POKE(COLOR+k,0);
    }
    
    INITSPRITES();
    INITRASTER();
    
    init_udg();
    
    POKE(0xd020, 0x00);
    POKE(0xd021, 0x00);    
    
    for(k=0;k<55;++k)
    {
        star_loc = rand()%1000;
        POKE(SCREEN+star_loc,0); // big flashing star
        POKE(COLOR+star_loc,1);
    }
    for(k=0;k<30;++k)
    {
        star_loc = rand()%1000;
        POKE(SCREEN+star_loc,28); // big fixed star
        POKE(COLOR+star_loc,1);
    }
    for(k=0;k<60;++k)
    {
        star_loc = rand()%1000;
        POKE(SCREEN+star_loc,27); // small flashing star
        POKE(COLOR+star_loc,1);
    }
    for(k=0;k<30;++k)
    {
        star_loc = rand()%1000;
        POKE(SCREEN+star_loc,29); // small fixed star
        POKE(COLOR+star_loc,1);
    }
    for(k=0;k<75;++k)
    {
        star_loc = rand()%1000;
        POKE(SCREEN+star_loc,30); // small/big flashing star
        POKE(COLOR+star_loc,1);
    }


    for(k=0;k<40;++k)
    {
        star_loc = rand()%80;
        POKE(SCREEN+star_loc,27); // small flashing star (top 2 rows)
        POKE(COLOR+star_loc,1);
    }

    for(k=0;k<40;++k)
    {
        star_loc = rand()%80;
        POKE(SCREEN+star_loc,30); // small/big flashing star (top 2 rows)
        POKE(COLOR+star_loc,1);
    }

    for(i=0;i<35;++i)
    {
        restored_text_row[i]=PEEK(SCREEN+120+i);
    }
    
    print(WRITTEN, 15, 1000-40-15-1,1);
    
    print(AUTHOR, 15, 1000-15-1,3);
    
    color_change();
    
    // H
    POKE(HAPPYNEWYEAR_POS      ,'9'+10); 
    POKE(HAPPYNEWYEAR_POS+1    ,'9'+11);
    POKE(HAPPYNEWYEAR_POS+40   ,'9'+ 9);
    POKE(HAPPYNEWYEAR_POS+40+1 ,'9'+12);

    // A
    POKE(HAPPYNEWYEAR_POS+2    ,'9'+ 1);
    POKE(HAPPYNEWYEAR_POS+3    ,'9'+ 2);
    POKE(HAPPYNEWYEAR_POS+40+2 ,'9'+ 3);
    POKE(HAPPYNEWYEAR_POS+40+3 ,'9'+ 4);

    // P
    POKE(HAPPYNEWYEAR_POS+4    ,'9'+ 5);
    POKE(HAPPYNEWYEAR_POS+5    ,'9'+17);
    POKE(HAPPYNEWYEAR_POS+40+4 ,'9'+ 9);
    POKE(HAPPYNEWYEAR_POS+40+5 ,'9'+18);

    // P
    POKE(HAPPYNEWYEAR_POS+6    ,'9'+ 5);
    POKE(HAPPYNEWYEAR_POS+7    ,'9'+17);
    POKE(HAPPYNEWYEAR_POS+40+6 ,'9'+ 9);
    POKE(HAPPYNEWYEAR_POS+40+7 ,'9'+18);
    
    // Y
    POKE(HAPPYNEWYEAR_POS+8    ,'9'+30);
    POKE(HAPPYNEWYEAR_POS+9    ,'9'+28);
    POKE(HAPPYNEWYEAR_POS+40+8 ,'9'+23);
    POKE(HAPPYNEWYEAR_POS+40+9 ,'9'+24);    
    
    // N
    POKE(HAPPYNEWYEAR_POS+12   ,'9'+13);
    POKE(HAPPYNEWYEAR_POS+13   ,'9'+14);
    POKE(HAPPYNEWYEAR_POS+40+12,'9'+15);
    POKE(HAPPYNEWYEAR_POS+40+13,'9'+16); 

    // E
    POKE(HAPPYNEWYEAR_POS+14   ,'9'+ 5);
    POKE(HAPPYNEWYEAR_POS+15   ,'9'+ 6);
    POKE(HAPPYNEWYEAR_POS+40+14,'9'+ 7);
    POKE(HAPPYNEWYEAR_POS+40+15,'9'+ 8); 

    // W
    POKE(HAPPYNEWYEAR_POS+16   ,'9'+10);
    POKE(HAPPYNEWYEAR_POS+17   ,'9'+25);
    POKE(HAPPYNEWYEAR_POS+40+16,'9'+26);
    POKE(HAPPYNEWYEAR_POS+40+17,'9'+27); 

    // Y
    POKE(HAPPYNEWYEAR_POS+20   ,'9'+30);
    POKE(HAPPYNEWYEAR_POS+21   ,'9'+28);
    POKE(HAPPYNEWYEAR_POS+40+20,'9'+23);
    POKE(HAPPYNEWYEAR_POS+40+21,'9'+24);  

    // E
    POKE(HAPPYNEWYEAR_POS+22   ,'9'+ 5);
    POKE(HAPPYNEWYEAR_POS+23   ,'9'+ 6);
    POKE(HAPPYNEWYEAR_POS+40+22,'9'+ 7);
    POKE(HAPPYNEWYEAR_POS+40+23,'9'+ 8); 
    
    // A
    POKE(HAPPYNEWYEAR_POS+24   ,'9'+ 1);
    POKE(HAPPYNEWYEAR_POS+25   ,'9'+ 2);
    POKE(HAPPYNEWYEAR_POS+40+24,'9'+ 3);
    POKE(HAPPYNEWYEAR_POS+40+25,'9'+ 4);

    // R
    POKE(HAPPYNEWYEAR_POS+26   ,'9'+ 19);
    POKE(HAPPYNEWYEAR_POS+27   ,'9'+ 20);
    POKE(HAPPYNEWYEAR_POS+40+26,'9'+ 21);
    POKE(HAPPYNEWYEAR_POS+40+27,'9'+ 22);


    for(i=1;i<4;++i)
    {
        for(j=0;j<4;++j)
        {
            POKE(SCREEN+MOON_OFFSET+i+40*j,(255-16)+i+j*4);
            POKE(COLOR+MOON_OFFSET+i+40*j,1);
        }
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
            if(flip)
            {
                for(i=0;i<3;++i)
                {
                    SPRX[i+5]=X_OFFSET+i*22;
                    SPRY[i+5]=i*8+Y_OFFSET+SEPARATION+yValues[XX];;
                }    
            }
            else
            {
                for(i=0;i<3;++i)
                {
                    SPRY[i+5]=255;
                }
            }
            for(i=0;i<4;++i)
            {
                SPRX[i+8]=X_OFFSET+i*(22+4-4*flip);
                SPRY[i+8]=i*8+Y_OFFSET+(3*SEPARATION/2)+flip*(SEPARATION/2)+yValues[XX];;
            }            

            if(!(XX&3))
            {
                if(comet_counter)
                {
                    --comet_counter;
                }
                if(!comet_counter)
                {

                    old_comet_pos = comet_x+40U*comet_y;

                    --comet_y;
                    ++comet_x;
                    if((comet_x>36)||(comet_y<4))
                    {
                        comet_x = COMET_X+(rand()&31);
                        comet_y = COMET_Y;
                        comet_counter = rand()&31;
                    }
                    comet_pos = comet_x+40U*comet_y;

                    POKE(SCREEN+old_comet_pos,below);
                    POKE(COLOR+old_comet_pos,1);

                    below = PEEK(SCREEN+comet_pos);
                    if(!comet_counter)
                    {
                        POKE(SCREEN+comet_pos,33);
                        POKE(COLOR+comet_pos,1);
                    } 
                }
            }

            if(!(XX&1))
            {
                if(comet2_counter)
                {
                    --comet2_counter;
                }
                if(!comet2_counter)
                {

                    old_comet2_pos = comet2_x+40U*comet2_y;

                    --comet2_y;
                    ++comet2_x;
                    if((comet2_x>36)||(comet2_y<4))
                    {
                        comet2_x = COMET_X+(rand()&31);
                        comet2_y = COMET_Y;
                        comet2_counter = rand()&63;
                    }
                    comet2_pos = comet2_x+40U*comet2_y;

                    POKE(SCREEN+old_comet2_pos,below);
                    POKE(COLOR+old_comet2_pos,1);

                    below = PEEK(SCREEN+comet2_pos);
                    if(!comet2_counter)
                    {
                        POKE(SCREEN+comet2_pos,33);
                        POKE(COLOR+comet2_pos,1);
                    } 
                }
            }


            // Flash comet
            if(!(XX&1))
            {
                if(comet_flash)
                {
                    POKE(SHAPE+33*8+1,0);
                    POKE(SHAPE+33*8,0);
                    comet_flash = 0;
                }
                else
                {
                    POKE(SHAPE+33*8+1,3);
                    POKE(SHAPE+33*8,3);
                    comet_flash = 1;
                }
            }
            
            // Animate small/big stars
            if(!(XX&31))
            {
                ++j;
                for(i=0;i<12;++i)
                {
                    SPRC[i] = 2+((XX+i+j)&7);        
                }
                POKE(SHAPE+3,0x28+16*(j&1));
                POKE(SHAPE+3+27*8,8*(j&1));
                
                color_change();
                
                POKE(SHAPE+2+30*8,STAR_0[j&3]);
                POKE(SHAPE+3+30*8,STAR_1[j&3]);
                POKE(SHAPE+4+30*8,STAR_0[j&3]);
                
                
            }
            if(!(XX))
            {
                if(text_counter==0)
                {
                    restore_text_row();
                    print(TEXT1, 15,120+6,5);
                }
                else if(text_counter==1)
                {
                   restore_text_row();
                   print(TEXT2, 28,120+6,11);
                }
                else if(text_counter==2)
                {
                    restore_text_row();
                    print(TEXT3, 29,120+6,12);
                }
                else
                {
                    restore_text_row();
                    // print(TEXT4, 17,120,8);
                }
                ++text_counter;
                text_counter%=4;
                    
                if(flip)
                {
                    SPRF[ 5]=GFX_START_INDEX + 32;
                    SPRF[ 6]=GFX_START_INDEX + 32;
                    SPRF[ 7]=GFX_START_INDEX + 32;
                    
                    SPRF[ 8]=GFX_START_INDEX + '2';//'Z' - 'A' + 25; // 2
                    SPRF[ 9]=GFX_START_INDEX + '0'; // 0
                    SPRF[10]=GFX_START_INDEX + '2'; // 2
                    SPRF[11]=GFX_START_INDEX + '3'; // 3
                    SPREX[ 8]=1;
                    SPREX[ 9]=1;
                    SPREX[10]=1;
                    SPREX[11]=1;                    
                    flip = 0;
                }
                else
                {
                    SPRF[ 5]=GFX_START_INDEX - 'A' + 1 + 'N';
                    SPRF[ 6]=GFX_START_INDEX - 'A' + 1 + 'E';
                    SPRF[ 7]=GFX_START_INDEX - 'A' + 1 + 'W';
                    
                    SPRF[ 8]=GFX_START_INDEX - 'A' + 1 + 'Y';
                    SPRF[ 9]=GFX_START_INDEX - 'A' + 1 + 'E';
                    SPRF[10]=GFX_START_INDEX - 'A' + 1 + 'A';
                    SPRF[11]=GFX_START_INDEX - 'A' + 1 + 'R';
                    SPREX[ 8]=0;
                    SPREX[ 9]=0;
                    SPREX[10]=0;
                    SPREX[11]=0; 
                    flip = 1;
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
