// #include <conio.h>
#include <stdlib.h>
#include <peekpoke.h>
#include <cbm_screen_charmap.h>


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

const char COMMODORE_MESSAGE[9] = "COMMODORE";
const char SPRITE_MESSAGE[9]    = "28SPRITES";

#define NUMBER_OF_COLORS 14
const unsigned char COLORS[NUMBER_OF_COLORS] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15};

// #define NUMBER_OF_COLORS 13
// const unsigned char COLORS[NUMBER_OF_COLORS] = {2,3,4,5,7,8,9,10,11,12,13,14,15};


const char AUTHOR[15] = "FABRIZIO CARUSO";
const char TEXT1[15] =  "LOGIC CODE IN C";
const char TEXT2[28] =  "MODIFIED CADAVER MULTIPLEXER";
const char TEXT3[29] =  "SID TUNE AND FONTS RIPPED OFF";
const char TEXT4[17] =  "FAST MODE ENABLED";

const char WRITTEN[15] = "WRITTEN IN C BY";

// const char YEAR[14] =   "HAPPY NEW YEAR";

const unsigned char STAR_0[4] = {0x00,0x10,0x10,0x00};
const unsigned char STAR_1[4] = {0x10,0x28,0x38,0x10};
// const star STAR_0[4] = {};

#define X_OFFSET 22
#define Y_OFFSET 38

#define SCREEN 0x0400
#define COLOR  0xD800
#define SHAPE  0x2000

#define SEPARATION 50   

#define HAPPYNEWYEAR_POS (SCREEN+6)
#define MOON_OFFSET (40*2+36)


static unsigned char j;
static unsigned char h;
// static unsigned char restored_text_row[35];

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


void print(const char *str, unsigned char len, unsigned short offset, unsigned char col)
{
    unsigned char k;
    
    for(k=0;k<len;++k)
    {
        // if(str[k]!=' ')
        // {
            POKE(SCREEN+offset+k,str[k]-'A'+1);
            POKE(COLOR+offset+k,col);
        // }
    }
}

#define TEXT_POSITION 840

void restore_text_row(void)
{
    unsigned char i;
    
    for(i=0;i<35;++i)
    {
        POKE(SCREEN+TEXT_POSITION+i,32);
        POKE(COLOR+TEXT_POSITION+i,1);
    }
}


void commodore_sprites(void)
{
	unsigned char i;
	
    for(i=0;i<9;++i)
    {

		SPRF[i+9] = GFX_START_INDEX + COMMODORE_MESSAGE[i] - 'A' + 1;

        SPRC[i+9] = 12;        
    } 
}



void message_sprites(void)
{
	unsigned char i;
	

    for(i=0;i<9;++i)
    {

		SPRF[i+9] = GFX_START_INDEX + SPRITE_MESSAGE[i] - 'A' + 1;

        SPRC[i+9] =  COLORS[i%NUMBER_OF_COLORS];  ;        
    }
	
	SPRF[9] = GFX_START_INDEX +'2';
	SPRF[10] = GFX_START_INDEX +'8';
	
}


void init_sprites(void)
{
	unsigned char i;
	
    for(i=0;i<27;++i)
    {
        SPRC[i] = COLORS[i%NUMBER_OF_COLORS];        
    }

	for(i=0;i<9;++i)
	{
		SPRF[i] = GFX_START_INDEX + 63;
		SPRF[i+18] = GFX_START_INDEX + 63;
	}
	SPRF[27] = GFX_START_INDEX + 63;
	SPRC[27] = 6;
	
	message_sprites();
}



/******************/
int main()
{    
    unsigned char XX = 1;
    unsigned char i;
    unsigned short k;
    // unsigned short star_loc;
    unsigned char flip = 1;
    
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
    
    // for(k=0;k<55;++k)
    // {
        // star_loc = rand()%1000;
        // POKE(SCREEN+star_loc,0); // big flashing star
        // POKE(COLOR+star_loc,1);
    // }
    // for(k=0;k<30;++k)
    // {
        // star_loc = rand()%1000;
        // POKE(SCREEN+star_loc,28); // big fixed star
        // POKE(COLOR+star_loc,1);
    // }
    // for(k=0;k<60;++k)
    // {
        // star_loc = rand()%1000;
        // POKE(SCREEN+star_loc,27); // small flashing star
        // POKE(COLOR+star_loc,1);
    // }
    // for(k=0;k<30;++k)
    // {
        // star_loc = rand()%1000;
        // POKE(SCREEN+star_loc,29); // small fixed star
        // POKE(COLOR+star_loc,1);
    // }
    // for(k=0;k<75;++k)
    // {
        // star_loc = rand()%1000;
        // POKE(SCREEN+star_loc,30); // small/big flashing star
        // POKE(COLOR+star_loc,1);
    // }


    // for(k=0;k<40;++k)
    // {
        // star_loc = rand()%80;
        // POKE(SCREEN+star_loc,27); // small flashing star (top 2 rows)
        // POKE(COLOR+star_loc,1);
    // }

    // for(k=0;k<40;++k)
    // {
        // star_loc = rand()%80;
        // POKE(SCREEN+star_loc,30); // small/big flashing star (top 2 rows)
        // POKE(COLOR+star_loc,1);
    // }

    // for(i=0;i<35;++i)
    // {
        // restored_text_row[i]=PEEK(SCREEN+120+i);
    // }
    
    print(WRITTEN, 15, 1000-40-15-1,1);
    
    print(AUTHOR, 15, 1000-15-1,3);
    
    color_change();
    

	init_sprites();
    
    NUMSPRITES = _NUMBER_OF_SPRITES_;

    // SPRF[i] = GFX_START_INDEX + 'H' - 'A' + 1;
    

   
    

    
    
    // for(i=19;i<NUMSPRITES;++i)
    // {
        // SPRF[i]= GFX_START_INDEX+63;

        // SPRC[i] = 1;        
    // }
    
    // Position snow flakes
    SPRY[NUMSPRITES-1]=220;
    // SPRX[NUMSPRITES-3]=40;
    // SPRY[NUMSPRITES-2]=220;
    // SPRX[NUMSPRITES-4]=60;   

    // SPRY[NUMSPRITES-5]=90;
    // SPRY[NUMSPRITES-6]=60;  
    // SPRY[NUMSPRITES-7]=140;
    // SPRY[NUMSPRITES-8]=170;  

                
	// SPRF[ 5]=GFX_START_INDEX - 'A' + 1 + 'N';
	// SPRF[ 6]=GFX_START_INDEX - 'A' + 1 + 'E';
	// SPRF[ 7]=GFX_START_INDEX - 'A' + 1 + 'W';
	
	// SPRF[ 8]=GFX_START_INDEX - 'A' + 1 + 'Y';
	// SPRF[ 9]=GFX_START_INDEX - 'A' + 1 + 'E';
	// SPRF[10]=GFX_START_INDEX - 'A' + 1 + 'A';
	// SPRF[11]=GFX_START_INDEX - 'A' + 1 + 'R';

                
    
    while(1) 
    {
        if (MULTIPLEX_DONE) 
		{
            SPRX[NUMSPRITES-1]=XX;

			if(XX<128)
			{
				for(i=0;i<9;++i)
				{
					SPRX[i]=X_OFFSET+i*16;
					SPRY[i]=i*4+Y_OFFSET+yValues[XX]/2;
				}

				for(i=0;i<9;++i)
				{
					if((flip)&&i>1)
					{
						SPRX[i+9]=X_OFFSET+i*16+8;
					}
					else
					{
						SPRX[i+9]=X_OFFSET+i*16;
					}
					SPRY[i+9]=i*4+Y_OFFSET+SEPARATION+yValues[XX]/2;
				}            

				for(i=0;i<9;++i)
				{
					SPRX[i+18]=X_OFFSET+i*16;
					SPRY[i+18]=i*4+Y_OFFSET+2*SEPARATION+yValues[XX]/2;
				}   
				
				if(XX==127)
				{
					commodore_sprites();
				}
			}
			else
			{


				for(i=0;i<9;++i)
				{
					SPRX[i]=X_OFFSET+i*16+(XX&15);
					SPRY[i]=i*4+Y_OFFSET+yValues[XX]/2;
				}

				for(i=0;i<9;++i)
				{
					
					// if((flip)&&i>1)
					// {
						// SPRX[i+9]=X_OFFSET+i*16+8;
					// }
					// else
					// {
						SPRX[i+9]=X_OFFSET+i*16;
					// }					
					
					// SPRX[i+9]=X_OFFSET+i*16;
					SPRY[i+9]=i*4+Y_OFFSET+SEPARATION+yValues[XX]/2;
				}            

				for(i=0;i<9;++i)
				{
					SPRX[i+18]=X_OFFSET+i*16-(XX&15);
					SPRY[i+18]=i*4+Y_OFFSET+2*SEPARATION+yValues[XX]/2;
				}  
				
				if(XX==255)
				{
					message_sprites();
				}
			}

            if(!(XX&63))
            {
				

				
                if(text_counter==0)
                {
                    restore_text_row();
                    print(TEXT1, 15,TEXT_POSITION+6,5);
                }
                else if(text_counter==1)
                {
                   restore_text_row();
                   print(TEXT2, 28,TEXT_POSITION+6,11);
                }
                else if(text_counter==2)
                {
                    restore_text_row();
                    print(TEXT3, 29,TEXT_POSITION+6,12);
                }
                else
                {
                    restore_text_row();
                    // print(TEXT4, 17,120,8);
                }
                ++text_counter;
                text_counter%=4;
			}


            MULTIPLEX_DONE = 0;
            SPRUPDATEFLAG = 1;
			
			++XX;
        }
    }
    return 0;
}
