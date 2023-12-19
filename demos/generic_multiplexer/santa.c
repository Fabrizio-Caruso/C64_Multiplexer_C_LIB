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

#include<joystick.h>
#define STANDARD_JOY 2


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

#define BEFANA_INDEX 0
#define HOT_AIR_BALLOON_INDEX 1

uint8_t counter;


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
#define BEFANA BEFANA_LEFT_TO_RIGHT

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
#define PINK       0x0A
#define LIGHT_BLUE 0x0E
#define LIGHT_GREY 0x0F


#define NUMBER_OF_COLS 40

#define SLOW_STAR_TILE ('9'+31)

#define FAST_STAR_TILE (SLOW_STAR_TILE+NUMBER_OF_COLS)




static uint8_t slow_loop = 0;
static uint8_t fast_loop = 20;
static uint8_t i;
static uint8_t j;
static uint8_t slow_star_mask = 1;
static uint8_t fast_star_mask = 1;

static uint16_t k;

static uint8_t input;

uint8_t prev_slow_loop;
uint8_t prev_fast_loop;



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


#define MIN_STAR  1
#define MAX_STAR 24

void fill_sky(uint8_t star_offset, uint8_t star_type)
{
	uint8_t offset;
	uint8_t previous;
	// uint8_t char_type;
	
	offset = 0;
	
	for(i=MIN_STAR+star_offset;i<MAX_STAR;i+=2)
	{
		previous = offset;
		do
		{
			offset = 2+(rand())%NUMBER_OF_COLS;
		} while((offset==previous)||(offset==previous-1)||(offset==previous+1));

		// if(i&1)
		// {
			// char_type = SLOW_STAR_TILE;
		// }
		// else
		// {
			// char_type = FAST_STAR_TILE;
		// }
		for(j=0;j<NUMBER_OF_COLS;++j)
		{
			POKE(SCREEN+i*NUMBER_OF_COLS+(j+offset)%NUMBER_OF_COLS,star_type+(j%NUMBER_OF_COLS));	
		}
	}	
}


void init_stars(void)
{
	// uint8_t offset;
	// uint8_t previous;
	// uint8_t char_type;
	
	// offset = 0;
	
	
	fill_sky(0,SLOW_STAR_TILE);
	fill_sky(1,FAST_STAR_TILE);
	
	// for(i=MIN_STAR;i<MAX_STAR;++i)
	// {
		// previous = offset;
		// do
		// {
			// offset = 2+(rand())%NUMBER_OF_COLS;
		// } while((offset==previous)||(offset==previous-1)||(offset==previous+1));

		// if(i&1)
		// {
			// char_type = SLOW_STAR_TILE;
		// }
		// else
		// {
			// char_type = FAST_STAR_TILE;
		// }
		// for(j=0;j<NUMBER_OF_COLS;++j)
		// {
			// POKE(SCREEN+i*NUMBER_OF_COLS+(j+offset)%NUMBER_OF_COLS,char_type+(j%NUMBER_OF_COLS));	
		// }
	// }	
	
}

#define GRASS_TILE 36

void init_grass(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_COLS;++i)
    {
        POKE(SCREEN+1000-40+i,GRASS_TILE);
        POKE(COLOR+1000-40+i,GREEN);
    }
}


void init_background(void)
{
	clear_screen();
	
    init_udg();
    
	set_background_colors();

	init_stars();
    
    init_grass();

	draw_the_moon();
	
}

#define SLOW_STAR_TILE_OFFSET SHAPE+8*SLOW_STAR_TILE
#define FAST_STAR_TILE_OFFSET  SHAPE+8*FAST_STAR_TILE

void draw_stars(void)
{
	POKE(SLOW_STAR_TILE_OFFSET+((slow_loop)<<3),slow_star_mask);

	// POKE(SLOW_STAR_TILE_OFFSET+(((slow_loop+(NUMBER_OF_COLS))%NUMBER_OF_COLS)<<3),slow_star_mask);

	
	POKE(FAST_STAR_TILE_OFFSET+((fast_loop)<<3),fast_star_mask);

}


void init_player(void)
{
	SPRX[BEFANA_INDEX] = 50;
	SPRY[BEFANA_INDEX] = 100;
	SPRM[BEFANA_INDEX] = 1;
    SPRC[BEFANA_INDEX] = PINK;
}

void handle_player(void)
{

	input = joy_read(STANDARD_JOY);
	
	if(JOY_LEFT(input))
	{
		--SPRX[BEFANA_INDEX];
	}
	else if(JOY_RIGHT(input))
	{
		++SPRX[BEFANA_INDEX];
	}
	
	if(JOY_UP(input))
	{
		--SPRY[BEFANA_INDEX];
	}
	else if(JOY_DOWN(input))
	{
		++SPRY[BEFANA_INDEX];
	}
	
	SPRF[BEFANA_INDEX] = GFX_START_INDEX+BEFANA+((counter/4)&3);
}


void handle_stars(void)
{
    slow_star_mask<<=1;

    if(!slow_star_mask)
    {
        ++slow_star_mask;
        prev_slow_loop = slow_loop;
        if(!slow_loop)
        {
            slow_loop=NUMBER_OF_COLS-1;
        }
        else
        {
            --slow_loop;
        }
    }

    fast_star_mask<<=2;
    
    if(!fast_star_mask)
    {
        ++fast_star_mask;
        prev_fast_loop = fast_loop;
        if(!fast_loop)
        {
            fast_loop=NUMBER_OF_COLS-1;
        }
        else
        {
            --fast_loop;
        }
    }
    
    draw_stars();
    if(slow_star_mask==1)
    {
        POKE(SLOW_STAR_TILE_OFFSET+((prev_slow_loop)<<3),0);
    }
    if(fast_star_mask==1)
    {
        POKE(FAST_STAR_TILE_OFFSET+((prev_fast_loop)<<3),0);
    }	    
}


// TODO: could be done for each pixel shift
static const uint8_t GRASS_SHAPE[4][7]=
{
    {0x18,0x0C,0x66,0x26,0x36,0x96,0x94},
    {0x60,0x30,0x99,0x98,0xD8,0x5A,0x52},
    {0x81,0xC0,0x66,0x62,0x63,0x69,0x49},
    {0x06,0x03,0x99,0x89,0x8D,0xA5,0x25}  
};




void scroll_grass(void)
{
    uint8_t i;
    
    for(i=0;i<7;++i)
    {
        
        POKE(SHAPE+1+(GRASS_TILE*8)+i,GRASS_SHAPE[(counter/2)&3][i]);
    }
}


#define NUMBER_OF_BALLOONS 12
void init_balloons(void)
{
    uint8_t i;
    
    for(i=1;i<=NUMBER_OF_BALLOONS;++i)
    {
        SPRF[i]=GFX_START_INDEX + HOT_AIR_BALLOON;
        SPRC[i]=CYAN;
        SPRM[i]=0;
        SPRY[i]=30U+i*16U;//;255-i*12;
        
        if(i&1)
        {
            SPRX[i]=70;
        }
        else
        {
            SPRX[i]=110;
        }
    }
}


void handle_balloons(void)
{
    uint8_t i;
    
    for(i=1;i<=NUMBER_OF_BALLOONS;++i)
    {
        --SPRY[i];
        SPRX[i]=xValues[counter+i*8];
    }
}


/******************/
int main()
{        

    NUMSPRITES = _NUMBER_OF_SPRITES_;

	counter = 50;
	
	init_background();
    
    INITSPRITES();
    INITRASTER();	

	init_player();
    init_balloons();

	slow_loop=0;
	fast_loop=0;
	
	SPRX[BEFANA_INDEX] = 100;
	SPRY[BEFANA_INDEX] = 50;
	// SPRF[BEFANA_INDEX] = GFX_START+BEFANA;
	POKE(MULTICOLOR_1,BROWN);
	POKE(MULTICOLOR_2,LIGHT_GREY);
	SPRC[BEFANA_INDEX]=PINK;

	joy_install((void *)joy_static_stddrv);
    	
    while(1) 
    {
        if (MULTIPLEX_DONE) {	
            handle_stars();
		
			handle_player();
            
            handle_balloons();
			++counter; 
			
            if(counter&1)
            {
                scroll_grass();
            }
			MULTIPLEX_DONE = 0;
			SPRUPDATEFLAG = 1;	
        }

    }
    return 0;
}
