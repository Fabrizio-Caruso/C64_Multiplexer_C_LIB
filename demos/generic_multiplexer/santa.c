#include <conio.h>
#include <stdlib.h>
#include <peekpoke.h>

#define uint8_t unsigned char
#define uint16_t unsigned short

/*******************
** Prototypes
*******************/
void INITSPRITES(void);
void INITRASTER(void);
/******************/
extern uint8_t SPRX[];
extern uint8_t SPRY[];
extern uint8_t SPRC[];
extern uint8_t SPRF[];
extern uint8_t SPREX[];

extern uint8_t SPRM[];

extern uint8_t NUMSPRITES;
extern uint8_t SPRUPDATEFLAG;
extern uint8_t SPRIRQCOUNTER;
extern uint8_t MULTIPLEX_DONE;
extern unsigned short SPRITE_GFX;
#pragma zpsym ("NUMSPRITES")
#pragma zpsym ("SPRUPDATEFLAG")
#pragma zpsym ("MULTIPLEX_DONE")
/******************/
// Pre-calculated sinus values
#define ONE_SINUS(F) \
    32/F, 35/F, 38/F, 41/F, 44/F, 47/F, 49/F, 52/F, \
    54/F, 56/F, 58/F, 60/F, 61/F, 62/F, 63/F, 63/F, \
    64/F, 63/F, 63/F, 62/F, 61/F, 60/F, 58/F, 56/F, \
    54/F, 52/F, 49/F, 47/F, 44/F, 41/F, 38/F, 35/F, \
    32/F, 28/F, 25/F, 22/F, 19/F, 16/F, 14/F, 11/F, \
    9/F, 7/F, 5/F, 3/F, 2/F, 1/F, 0/F, 0/F, \
    0/F, 0/F, 0/F, 1/F, 2/F, 3/F, 5/F, 7/F, \
    9/F, 11/F, 14/F, 16/F, 19/F, 22/F, 25/F, 28/F

#define ONE_SHIFTED_SINUS(F) \
    32/F, 28/F, 25/F, 22/F, 19/F, 16/F, 14/F, 11/F, \
    9/F, 7/F, 5/F, 3/F, 2/F, 1/F, 0/F, 0/F, \
    0/F, 0/F, 0/F, 1/F, 2/F, 3/F, 5/F, 7/F, \
    9/F, 11/F, 14/F, 16/F, 19/F, 22/F, 25/F, 28/F, \
    32/F, 35/F, 38/F, 41/F, 44/F, 47/F, 49/F, 52/F, \
    54/F, 56/F, 58/F, 60/F, 61/F, 62/F, 63/F, 63/F, \
    64/F, 63/F, 63/F, 62/F, 61/F, 60/F, 58/F, 56/F, \
    54/F, 52/F, 49/F, 47/F, 44/F, 41/F, 38/F, 35/F

#define SINUS(F) \
{ \
	ONE_SINUS(F), \
	ONE_SINUS(F), \
	ONE_SINUS(F), \
	ONE_SINUS(F) \
}


#define SHIFTED_SINUS(F) \
{ \
	ONE_SHIFTED_SINUS(F), \
	ONE_SHIFTED_SINUS(F), \
	ONE_SHIFTED_SINUS(F), \
	ONE_SHIFTED_SINUS(F) \
}

// Pre-calculated sinus values
const char yValues[] = SINUS(1);

const char xValues[] = SINUS(2);

const char shifted_xValues[] = SHIFTED_SINUS(2);

// const char yValues4[] = SINUS(4);


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

// const char MESSAGE[12] = "HAPPYNEWYEAR";
// #define MESSAGE_LENGTH 12

// const char MESSAGE2[14] = "MERRYCHRISTMAS";
// #define MESSAGE2_LENGTH 14

// const uint8_t COLORS[14] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15};

// const char AUTHOR[15] = "FABRIZIO CARUSO";
// const char TEXT1[27] =  "ALL LOGIC CODE WRITTEN IN C";
// const char TEXT2[28] =  "MODIFIED CADAVER MULTIPLEXER";
// const char TEXT3[29] =  "SID TUNE AND FONTS RIPPED OFF";
// const char TEXT4[17] =  "FAST MODE ENABLED";

// const char WRITTEN[15] = "WRITTEN IN C BY";

// const char YEAR[14] =   "HAPPY NEW YEAR";

// const uint8_t STAR_0[4] = {0x00,0x10,0x10,0x00};
// const uint8_t STAR_1[4] = {0x10,0x28,0x38,0x10};
// const star STAR_0[4] = {};


#define X_OFFSET 46
#define Y_OFFSET 58

#define XMAS_X_OFFSET X_OFFSET
#define FONT_SPACING 22
#define XMAS_FONT_SPACING 22

#define SCREEN 0x0400
#define COLOR  0xD800
#define SHAPE  0x2000

#define SEPARATION 40   

#define HAPPYNEWYEAR_POS (SCREEN+6)
#define MOON_OFFSET (40*2+36)



// $D018 = 53272
// -----------------
void init_udg(void)
{
    POKE(56576u,3);
	POKE(0xD018,PEEK(0xD018)|8);
	POKE(0xD018,PEEK(0xD018)&(255-4-2));    
	POKE(648,192);
}


#if !defined(YEAR_LOW)
	#define YEAR_LOW '4'
#endif

#if !defined(YEAR_HIGH)
	#define YEAR_HIGH '2'
#endif

#define HOT_AIR_BALLOON ('9' + 1)
#define COMMODORE_LOGO ('9' + 2)

#define BEFANA_LEFT_TO_RIGHT (- 'A' + 1 + 'Z' + 1)
#define BEFANA_RIGHT_TO_LEFT (- 'A' + 1 + 'Z' + 11)
#define BEFANA BEFANA_RIGHT_TO_LEFT

#define PRESENT (- 'A' + 1 + 'Z' + 15)

#define SANTA (- 'A' + 1 + 'Z' + 5)

#define REINDEER (- 'A' + 1 + 'Z' + 8)

#define MULTICOLOR_1 0xD025
#define MULTICOLOR_2 0xD026

// Characters for flashing stars
#define BIG_FLASHING_STAR 0
#define FLASHING_STAR 30
#define SMALL_FLASHING_STAR 27

// Characters for fixed stars
#define SMALL_FIXED_STAR 28
#define BIG_FIXED_STAR 29

// Characters for comets
#define COMET 33

/*
0	$00	black
1	$01	white
2	$02	red
3	$03	cyan
4	$04	purple
5	$05	green
6	$06	blue
7	$07	yellow
8	$08	orange
9	$09	brown
10	$0A	pink
11	$0B	dark grey
12	$0C	grey
13	$0D	light green
14	$0E	light blue
15	$0F	light grey 

*/
#define BLACK      0x00
#define WHITE      0x01
#define RED        0x02
#define CYAN       0x03
#define PURPLE     0x04
#define GREEN      0x05
#define YELLOW     0x07
#define BROWN      0x09
#define LIGHT_BLUE 0x0E
#define LIGHT_GREY 0x0F


#define STAR_TILE ('9'+31)
#define NUMBER_OF_COLS 40



static uint8_t loop = 0;
static uint8_t i;
static uint8_t j;
static uint8_t star_mask = 1;

static uint16_t k;



void print(const char *str, uint8_t len, unsigned short offset, uint8_t col)
{
    uint8_t k;
    
    for(k=0;k<len;++k)
    {
        if(str[k]!=' ')
        {
            POKE(SCREEN+offset+k,str[k]-'A'+1);
            POKE(COLOR+offset+k,col);
        }
    }
}


void printd(char val, uint8_t length, unsigned short offset, uint8_t color)
{
	uint8_t i;
	uint8_t digit;
	
	for(i=0;i<length;++i)
	{
		digit = (uint8_t) ((val)%10);
		val-= digit;
		val/=10;
		
		POKE(SCREEN+offset+length-1-i, (uint8_t) (digit+(uint8_t) 48u));
		POKE(COLOR+offset+length-1-i, color);
	}
}


 

void draw_the_moon(void)
{
    for(i=1;i<4;++i)
    {
        for(j=0;j<4;++j)
        {
            POKE(SCREEN+MOON_OFFSET+i+NUMBER_OF_COLS*j,(255-16)+i+j*4);
            POKE(COLOR+MOON_OFFSET+i+NUMBER_OF_COLS*j,1);
        }
    }	
}


void set_background_colors(void)
{
    POKE(0xd020, BLACK);
    POKE(0xd021, BLACK);  	
}


void clear_screen(void)
{
    for(k=0;k<1000;++k)
    {
        POKE(SCREEN+k,32);
        POKE(COLOR+k,1);
    }	
}


void init_stars(void)
{
	uint8_t offset;
	uint8_t previous;
	
	offset = 0;
	
	for(i=2;i<22;++i)
	{
		previous = offset;
		do
		{
			offset = 2+(rand())%NUMBER_OF_COLS;
		} while((offset==previous)||(offset==previous-1)||(offset==previous+1));
		// offset = 0;
		// {
			// uint16_t t;
			
			// printd(offset,3,0,WHITE);
			// for(t=0;t<9000;++t)
			// {
			// }
		// }
		
		for(j=0;j<NUMBER_OF_COLS;++j)
		{
			POKE(SCREEN+i*NUMBER_OF_COLS+(j+offset)%NUMBER_OF_COLS,STAR_TILE+(j%NUMBER_OF_COLS));	
		}
	}
}

void init_background(void)
{
	clear_screen();
	
    init_udg();
    
	set_background_colors();

	init_stars();

	draw_the_moon();
	
}

#define  STAR_TILE_OFFSET SHAPE+8*STAR_TILE
void handle_stars(void)
{
POKE(STAR_TILE_OFFSET+((loop)<<3),star_mask);
}

#include<joystick.h>
/******************/
int main()
{        
	uint8_t input;
	
	init_background();
    
    INITSPRITES();
    INITRASTER();	

	loop=NUMBER_OF_COLS;

	joy_install((void *)joy_static_stddrv);
    	
    while(1) 
    {
        if (MULTIPLEX_DONE) {	
			star_mask<<=1;
			handle_stars();

			if(!star_mask)
			{
				++star_mask;
				--loop;
				if(!loop)
				{
					loop=NUMBER_OF_COLS;
				}
			}
		
			MULTIPLEX_DONE = 0;
			SPRUPDATEFLAG = 1;	
        }

    }
    return 0;
}
