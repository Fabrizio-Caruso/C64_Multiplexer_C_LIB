#include <stdlib.h>
#include <stdint.h>
#include <peekpoke.h>


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

extern uint8_t MUSIC_ON;

#define INITIAL_LEVEL 1

// #define TRAINER 1

#define INITIAL_ENERGY 100
#define MAX_ENERGY 200

#pragma zpsym ("NUMSPRITES")
#pragma zpsym ("SPRUPDATEFLAG")
#pragma zpsym ("MULTIPLEX_DONE")
#pragma zpsym ("MUSIC_ON")
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

#define ONE_SHIFTED2_SINUS(F) \
    0/F, 0/F, 0/F, 1/F, 2/F, 3/F, 5/F, 7/F, \
    9/F, 11/F, 14/F, 16/F, 19/F, 22/F, 25/F, 28/F, \
    32/F, 35/F, 38/F, 41/F, 44/F, 47/F, 49/F, 52/F, \
    54/F, 56/F, 58/F, 60/F, 61/F, 62/F, 63/F, 63/F, \
    64/F, 63/F, 63/F, 62/F, 61/F, 60/F, 58/F, 56/F, \
    54/F, 52/F, 49/F, 47/F, 44/F, 41/F, 38/F, 35/F, \
    32/F, 28/F, 25/F, 22/F, 19/F, 16/F, 14/F, 11/F, \
    9/F, 7/F, 5/F, 3/F, 2/F, 1/F, 0/F, 0/F

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

#define SHIFTED2_SINUS(F) \
{ \
	ONE_SHIFTED2_SINUS(F), \
	ONE_SHIFTED2_SINUS(F), \
	ONE_SHIFTED2_SINUS(F), \
	ONE_SHIFTED2_SINUS(F) \
}

#include<joystick.h>
#define STANDARD_JOY 2


#define NUMBER_OF_BALLOONS 9

#define NUMBER_OF_GIFTS 4


#define BEFANA_MIN_X 36
#define BEFANA_MAX_X 140

#define BEFANA_MIN_Y 55
#define BEFANA_MAX_Y 208


#define BALLOON_DAMAGE 15

#define GIFT_ENERGY 20

#define LEVEL_DISTANCE 250U

uint8_t forward_thrust;

uint8_t accelleration;

#define BOOST_DURATION 15
#define BOOST_THRESHOLD 8

#define MAX_LEVEL 99
static uint8_t level;

static uint8_t harmful_balloon[NUMBER_OF_BALLOONS];
static uint8_t active_balloon[NUMBER_OF_BALLOONS];

static uint8_t freeze;

static uint8_t y_balloon[NUMBER_OF_BALLOONS];

// Pre-calculated sinus values
const uint8_t sinValues1[] = SINUS(1);

const uint8_t sinValues2[] = SINUS(2);

const uint8_t sinValues3[] = SINUS(3);

const uint8_t sinValues4[] = SINUS(4);

const uint8_t sinValues8[] = SINUS(8);

const uint8_t shifted_sinValues1[] = SHIFTED_SINUS(1);

const uint8_t shifted_sinValues2[] = SHIFTED_SINUS(2);

const uint8_t shifted_sinValues3[] = SHIFTED_SINUS(3);

const uint8_t shifted_sinValues4[] = SHIFTED_SINUS(4);



const uint8_t shifted2_sinValues1[] = SHIFTED2_SINUS(1);

const uint8_t shifted2_sinValues2[] = SHIFTED2_SINUS(2);

const uint8_t shifted2_sinValues3[] = SHIFTED2_SINUS(3);

const uint8_t shifted2_sinValues4[] = SHIFTED2_SINUS(4);



static uint16_t distance;
static uint16_t level_threshold;
static uint16_t record;

static uint8_t volume;

#define VOLUME_REG 0xD418 

void music_switch(uint8_t toggle)
{
    if(toggle)
    {
        POKE(VOLUME_REG,volume);
        MUSIC_ON=1;
    }
    else
    {
        MUSIC_ON=0;
        volume=PEEK(VOLUME_REG);
        POKE(VOLUME_REG,0);
    }
}




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

#define BEFANA_INDEX (NUMBER_OF_BALLOONS)
#define BALLOON_INDEX 0
#define GIFT_INDEX (BEFANA_INDEX+1)
#define SMOKE_INDEX 14

#define SMOKE 60

static uint8_t counter;
static uint8_t energy;
static uint16_t points = 0;



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

#define ENERGY_ICON ('z'-'a'+1+1)
// #define BOOST_TILE ('z'-'a'+1+1)

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

#define BALLOON ('9' + 1)
#define COMMODORE_LOGO ('9' + 2)

#define BEFANA_LEFT_TO_RIGHT (- 'A' + 1 + 'Z' + 1)
#define BEFANA_RIGHT_TO_LEFT (- 'A' + 1 + 'Z' + 11)
#define BEFANA BEFANA_LEFT_TO_RIGHT

#define GIFT (- 'A' + 1 + 'Z' + 15)

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
#define BLACK       0x00
#define WHITE       0x01
#define RED         0x02
#define CYAN        0x03
#define PURPLE      0x04
#define GREEN       0x05
#define YELLOW      0x07
#define BROWN       0x09
#define PINK        0x0A
#define LIGHT_GREEN 0x0D
#define LIGHT_BLUE  0x0E
#define LIGHT_GREY  0x0F

// {CYAN, PURPLE, GREEN, LIGHT_BLUE, YELLOW, RED, LIGHT_GREY, CYAN};
const static uint8_t BALLOON_COLORS[] = {CYAN, PURPLE, GREEN, LIGHT_BLUE, YELLOW, RED, LIGHT_GREY, CYAN};
//{CYAN, PURPLE, GREEN, LIGHT_BLUE, YELLOW, BROWN, LIGHT_GREY, CYAN};

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

static uint8_t prev_slow_loop;
static uint8_t prev_fast_loop;


void _set_noise(void)
{
	SID.v3.ad    = 0x00; 
	SID.v3.sr    = 0xA8; 
	SID.flt_ctrl = 0x44; 
	SID.amp      = 0x1F; 
	SID.v3.ctrl  = 0x81; 	
}

void _short_sound(void)
{
	uint16_t i;
	
	SID.v3.freq  = 0x2000; 
	SID.v3.ad    = 0x00; 
	SID.v3.sr    = 0xC8; 
	SID.flt_ctrl = 0x44; 
	SID.amp      = 0x1F; 
	SID.v3.ctrl  = 0x21; 	

	for(i=0;i<200;++i) {} 
	SID.amp      = 0x00; 
	SID.v3.ctrl  = 0x08; 	
}

void _XL_SHOOT_SOUND(void) 
{ 
	uint16_t i; 
	
	SID.v3.freq  = 0x3000; 
	SID.flt_freq = 0x9000; 

	_set_noise();
	
	for(i=0;i<800;++i) {} 
	SID.amp      = 0x00; 
	SID.v3.ctrl  = 0x08; 
};	
	

void _XL_EXPLOSION_SOUND(void)
{ 
	uint16_t i; 
	uint16_t j; 
	
	SID.v3.freq  = 0x1200; 
	SID.flt_freq = 0x2000; 

	_set_noise();
	
	for(i=0;i<300;++i) 
		{ 
		} 
	for(j=0;j<15;++j) 
	{ 
		SID.amp      = 0x1F - j; 
		for(i=0;i<100;++i) 
		{ 
		} 
	} 
	SID.amp      = 0x00; 
	SID.v3.ctrl  = 0x08; 
};


void _XL_PING_SOUND(void)
{ 
	SID.flt_freq = 0x3500; 
	_short_sound();
};


void _XL_TICK_SOUND(void) 
{ 
	SID.flt_freq = 0x2000; 
	_short_sound();
};

void _XL_TOCK_SOUND(void) 
{
	SID.flt_freq = 0x1000; 
	_short_sound();
};


void _XL_ZAP_SOUND(void) 
{ 
	uint8_t i;
	uint8_t j;
	
	SID.v3.freq  = 0x6800; 
	SID.v3.ad    = 0x88; 
	SID.v3.sr    = 0xC8; 
	SID.flt_freq = 0x5000; 
	SID.flt_ctrl = 0x44; 
	SID.amp      = 0x1F; 
	SID.v3.ctrl  = 0x21; 
	
	for(i=0;i<253;++i) 
	{ 
		SID.v3.freq+=8;
		for(j=0;j<25;++j){};
	}; 

	SID.amp      = 0x00; 
	SID.v3.ctrl  = 0x08; 
};


// Print a string on screen memory
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


// Print a 16-bit number on screen memory
void printd(uint16_t val, uint8_t length, unsigned short offset, uint8_t color)
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

void fill_sky(uint8_t star_offset, uint8_t last_position, uint8_t star_type)
{
	uint8_t offset;
	uint8_t previous;
	
	offset = 0;
	
	for(i=MIN_STAR+star_offset;i<last_position;i+=2)
	{
		previous = offset;
		do
		{
			offset = 2+(rand())%NUMBER_OF_COLS;
		} while((offset==previous)||(offset==previous-1)||(offset==previous+1));

		for(j=0;j<NUMBER_OF_COLS;++j)
		{
			POKE(SCREEN+i*NUMBER_OF_COLS+(j+offset)%NUMBER_OF_COLS,star_type+(j%NUMBER_OF_COLS));	
		}
	}	
}


void init_stars(void)
{

	fill_sky(1,MAX_STAR-4, SLOW_STAR_TILE);
	fill_sky(2,MAX_STAR, FAST_STAR_TILE);
	
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

	// draw_the_moon();
	
}

#define SLOW_STAR_TILE_OFFSET SHAPE+8*SLOW_STAR_TILE
#define FAST_STAR_TILE_OFFSET  SHAPE+8*FAST_STAR_TILE

void draw_stars(void)
{
	POKE(SLOW_STAR_TILE_OFFSET+((slow_loop)<<3),slow_star_mask);
	POKE(FAST_STAR_TILE_OFFSET+((fast_loop)<<3),fast_star_mask);
}


void clear_stars(void)
{
	uint16_t i;
	for(i=0;i<NUMBER_OF_COLS*8;++i)
	{
		POKE(SLOW_STAR_TILE_OFFSET+i,0);
		POKE(FAST_STAR_TILE_OFFSET+i,0);
	}
}


void init_player(uint8_t sprite_index)
{
	// SPRX[BEFANA_INDEX] = 40;
	// SPRY[BEFANA_INDEX] = 20;
	// SPRM[BEFANA_INDEX] = 1;
    // SPRC[BEFANA_INDEX] = PINK;
    
    SPRX[sprite_index] = 30;
    SPRY[sprite_index] = 50;
    SPRC[sprite_index]=PINK;   
    SPRM[sprite_index] = 1;    
}


void _handle_stars(void)
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
	
	// Delete previous star character
    if(slow_star_mask==1)
    {
        POKE(SLOW_STAR_TILE_OFFSET+((prev_slow_loop)<<3),0);
    }
    if(fast_star_mask==1)
    {
        POKE(FAST_STAR_TILE_OFFSET+((prev_fast_loop)<<3),0);
    }	    
}


void handle_stars(void)
{
	_handle_stars();
    if(accelleration>BOOST_THRESHOLD)
    {
        _handle_stars();
    }
	else if(accelleration)
	{
		_handle_stars();
	}
	if(forward_thrust)
	{
		_handle_stars();
	}
}


void handle_befana(void)
{

	input = joy_read(STANDARD_JOY);
	
	if(accelleration && SPRX[BEFANA_INDEX]<BEFANA_MAX_X)
	{
        SPRX[SMOKE_INDEX]=SPRX[BEFANA_INDEX]-14;
        SPRY[SMOKE_INDEX]=SPRY[BEFANA_INDEX]+2; // TODO: this could be done in the sprite shape     
        SPRF[SMOKE_INDEX]=GFX_START_INDEX+SMOKE+(counter&1);
		--accelleration;
		++SPRX[BEFANA_INDEX];
        if(accelleration>BOOST_THRESHOLD)
        {
            ++SPRX[BEFANA_INDEX];
            
        }
        else
        {
            SPRY[SMOKE_INDEX]=255;
        }
		// handle_stars();
	}
    else
    {
        SPRY[SMOKE_INDEX]=255;  
    }
	
	forward_thrust = 0;
	if(!freeze)
	{
		if(JOY_LEFT(input) && SPRX[BEFANA_INDEX]>BEFANA_MIN_X)
		{
			--SPRX[BEFANA_INDEX];
			
		}
		else if(JOY_RIGHT(input) && SPRX[BEFANA_INDEX]<BEFANA_MAX_X)
		{
			++SPRX[BEFANA_INDEX];
			forward_thrust = 1;
		}
		
		if(JOY_UP(input) && SPRY[BEFANA_INDEX]>BEFANA_MIN_Y)
		{
			--SPRY[BEFANA_INDEX];
		}
		else if(JOY_DOWN(input) && SPRY[BEFANA_INDEX]<BEFANA_MAX_Y)
		{
			++SPRY[BEFANA_INDEX];
		}
		
		if(JOY_FIRE(input))
		{
			if(!accelleration)
			{
				accelleration=BOOST_DURATION;
			}
		}
		
		SPRF[BEFANA_INDEX] = GFX_START_INDEX+BEFANA+((counter/4)&3);
	}
	
		
}


static const uint8_t GRASS_SHAPE[8][7]=
{
	{0x18,0x0C,0x66,0x26,0x36,0x96,0x94},
	{0x30,0x18,0xCC,0x4C,0x6C,0x2D,0x29},
	{0x60,0x30,0x99,0x98,0xD8,0x5A,0x52},
	{0xC0,0x60,0x33,0x31,0xB1,0xB4,0xA4},
	{0x81,0xC0,0x66,0x62,0x63,0x69,0x49},
	{0x03,0x81,0xCC,0xC4,0xC6,0xD2,0x92},
	{0x06,0x03,0x99,0x89,0x8D,0xA5,0x25},
	{0x0C,0x06,0x33,0x13,0x1B,0x4B,0x4A},
};

void scroll_grass(void)
{
    uint8_t i;
    
    for(i=0;i<7;++i)
    {
        
        POKE(SHAPE+1+(GRASS_TILE*8)+i,GRASS_SHAPE[(counter)&7][i]);
    }
}


void handle_grass(void)
{
	scroll_grass();
	if(accelleration)
	{
		scroll_grass();
	}
}

#define BALLOON_TOP_Y 45
#define BALLOON_BOTTOM_Y 180

void init_sprite_balloons(void)
{
    uint8_t i;
    
    for(i=BALLOON_INDEX;i<=BALLOON_INDEX+NUMBER_OF_BALLOONS-1;++i)
    {
		
        SPRF[i]=GFX_START_INDEX + BALLOON;
        SPRC[i]=CYAN;
        SPRM[i]=0;
		
		SPRX[i]=255-i*28;
		
		do
		{
			y_balloon[i]=rand()&0xFF;
		} while((y_balloon[i]<BALLOON_TOP_Y)||(y_balloon[i]>BALLOON_BOTTOM_Y));		
		SPRY[i]=y_balloon[i];

    }
    // SPRC[NUMBER_OF_BALLOONS-1]=RED;
}


uint8_t compute_y_ballon(uint8_t i)
{
	return 40+i*22;
}


void init_balloons(void)
{
	// uint8_t i;
	
	init_sprite_balloons();
	
    for(i=0;i<NUMBER_OF_BALLOONS;++i)
	{
		harmful_balloon[i]=1;
		if(i&1)
		{
			active_balloon[i]=1;
			y_balloon[i]= compute_y_ballon(i);
		}
		else
		{
			// active_balloon[i]=1;
			// y_balloon[i]= compute_y_ballon(i);			
			active_balloon[i]=0;
			y_balloon[i]= 255; //-i*16;
		}
		SPRY[i]=255; //-i*16;
	}	
}


// uint16_t distances[]={DISTANCE_LEV_1,DISTANCE_LEV_2,DISTANCE_LEV_3,DISTANCE_LEV_4, DISTANCE_LEV_5, DISTANCE_LEV_6,DISTANCE_LEV_7,DISTANCE_LEV_8};

// uint8_t distance_counter;

void activate_balloon(uint8_t i)
{
	if(level>=10)
	{
		if(i==4)
		{
			active_balloon[4]=1;
			y_balloon[4]=compute_y_ballon(4);		
		}
	}	
	else if(level>=8)
	{
		if(i==2)
		{
			active_balloon[2]=1;
			y_balloon[2]=compute_y_ballon(2);
		}
	}
	else if(level>=6)
	{
		if(i==6)
		{
			active_balloon[6]=1;
			y_balloon[6]=compute_y_ballon(6);
		}			
	}
	else if(level>=4)
	{
		if(!i)
		{
			active_balloon[0]=1;
			y_balloon[0]=compute_y_ballon(0);
		}
	}
	else if(level>=2)
	{
		if(i==8)
		{
			active_balloon[8]=1;
			y_balloon[8]=compute_y_ballon(8);
		}
	}
}


#define DISTANCE_OFFSET 12

void display_distance(void)
{
    printd(distance,4,DISTANCE_OFFSET,WHITE);
}



#define LEVEL_OFFSET (DISTANCE_OFFSET+4+5)

void display_level(void)
{
    printd(level,2,LEVEL_OFFSET,WHITE);
}

#define HI_OFFSET (LEVEL_OFFSET+9)

void display_hi(uint8_t position)
{
    print("HI",2,position-2,GREEN);
	printd(record,5,position,WHITE);
}


void check_level_trigger()
{
    // printd(level_threshold,5,160,WHITE);
    // printd(((uint16_t) level),5,200,WHITE);

    
    if((distance>=level_threshold)&&(level<MAX_LEVEL))
    {
        ++level;
        display_level();
        level_threshold = ((uint16_t) level)*(uint16_t) LEVEL_DISTANCE;
    }
    // else if(level==10)
    // {
        // print("DEBUG",5,280,WHITE);
        
    // }

	// activate_balloon(i);
	
}



#define BALLON_THRESHOLD_X 8

void _handle_balloons(void)
{
    uint8_t i;
    
    for(i=BALLOON_INDEX;i<=BALLOON_INDEX+NUMBER_OF_BALLOONS-1;++i)
    {
		// TODO: This should be optimized because we move all balloons when we may just more one offset
		--SPRX[i];
		
		if(active_balloon[i])
		{
			if(SPRX[i]<BALLON_THRESHOLD_X)
			{
				harmful_balloon[i]=1;
				if(i<8)
				{
					y_balloon[i]=62+(i&3)*32+(rand()&0x1F);

					SPRC[i]=BALLOON_COLORS[rand()&7];

				}
				else
				{
					if(SPRY[BEFANA_INDEX]>150)
					{
						y_balloon[8]=SPRY[BEFANA_INDEX]-16;
					}
					else if(SPRY[BEFANA_INDEX]<70)     
					{
						y_balloon[8]=SPRY[BEFANA_INDEX];                        
					}
					else if(SPRY[BEFANA_INDEX]>100)
					{
						y_balloon[8]=160;
					}
					else
					{
						y_balloon[8]=80;
					}

				}
				SPRX[i]=184;
			}
			if(!(i&3))
			{
				SPRY[i]=y_balloon[i]+sinValues3[counter];
			}
			else if((i&3)==1)
			{
				if((level&1))
				{
					SPRY[i]=y_balloon[i]+shifted_sinValues2[counter];
				}
				else
				{
					SPRY[i]=y_balloon[i]+shifted_sinValues3[counter];
				}
			}
			else if((i&3)==2)
			{
				
				if((level&1))
				{
					SPRY[i]=y_balloon[i]+shifted2_sinValues2[counter];
				}
				else
				{
					SPRY[i]=y_balloon[i]+shifted2_sinValues3[counter];
				}
			}   
			else
			{
				SPRY[i]=y_balloon[i]+shifted_sinValues3[counter];
			}
		}
        else if(SPRX[i]<BALLON_THRESHOLD_X)
		{
			SPRX[i]=184;
            activate_balloon(i);

		}
    }
}

void handle_balloons(void)
{
	_handle_balloons();
	if((level==2)||(level==4)||(level==6)||(level==8))
	{
		if(counter&1)
		{
			_handle_balloons();
		}
	}
	else if((level>=10) && !(level&1))
	{
		_handle_balloons();
	}
}

void init_gifts(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_GIFTS;++i)
    {
        SPRF[GIFT_INDEX+i]=GFX_START_INDEX+GIFT;
        SPRC[GIFT_INDEX+i]=BALLOON_COLORS[i];
        SPRY[GIFT_INDEX+i]=80+40*i;
        SPRM[GIFT_INDEX+i]=1;
        SPRX[GIFT_INDEX+i]=i*64;
    }
}

void clear_gifts(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_GIFTS;++i)
    {
        // SPRF[GIFT_INDEX+i]=GFX_START_INDEX+GIFT;
        // SPRC[GIFT_INDEX+i]=BALLOON_COLORS[i];
        SPRY[GIFT_INDEX+i]=255;
        // SPRM[GIFT_INDEX+i]=1;
        // SPRX[GIFT_INDEX+i]=i*64;
    }
}


void handle_gifts(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_GIFTS;++i)
    {
        --SPRX[GIFT_INDEX+i];
        if(!(counter&3))
        {
            SPRF[GIFT_INDEX+i]=GFX_START_INDEX+GIFT+((counter/4)%3);
        }
        
        // Re-position gift
        if(!SPRX[GIFT_INDEX+i])
        {
			if(rand()&3)
			{
				SPRY[GIFT_INDEX+i]=100+i*40-(rand()&0x1F);
			}
			else
			{
				SPRY[GIFT_INDEX+i]=255;
			}
        }
    }
}

#define COLLISION_BOX_X 8
#define COLLISION_BOX_Y 12


uint8_t collision(uint8_t i)
{
    uint8_t x;
	uint8_t y;
	uint8_t befana_x;
	uint8_t befana_y;
	
	befana_x = SPRX[BEFANA_INDEX];
	befana_y = SPRY[BEFANA_INDEX];
    
    x = SPRX[i];
    if(x>=befana_x)
    {
        if((x-befana_x)>COLLISION_BOX_X)
        {
            return 0;
        }
    }
    else // x < befana_x
    {
        if((befana_x-x)>COLLISION_BOX_X)
        {
            return 0;
        }
    }
    
    y = SPRY[i];
    if(y>=befana_y)
    {
        if((y-befana_y)>COLLISION_BOX_Y)
        {
            return 0;
        }
    }
    else // y < befana_y
    {
        if((befana_y-y)>COLLISION_BOX_Y)
        {
            return 0;
        }
    }
    return 1;    
}


uint8_t balloon_collision(void)
{
    for(i=BALLOON_INDEX;i<=BALLOON_INDEX+NUMBER_OF_BALLOONS-1;++i)
    {
		if(collision(i))
		{
			if(harmful_balloon[i])
			{
				_XL_EXPLOSION_SOUND();
				harmful_balloon[i]=0;
				return 1;
			}
		}
		
	}
	return 0;
}


void handle_befana_color(void)
{
	if(freeze)
	{
		--freeze;
		SPRC[BEFANA_INDEX]=RED;
	}
	else if(energy<(MAX_ENERGY/4))
    {
        SPRC[BEFANA_INDEX]=LIGHT_GREEN;
    }
    else
    {
        SPRC[BEFANA_INDEX]=PINK;
    }
}


void display_energy(void)
{
    printd(energy,3,NUMBER_OF_COLS-1-2,WHITE);
}


void display_score(void)
{
    printd(points,5,5,WHITE);
}


void decrease_energy(uint8_t amount)
{
    #if !defined(TRAINER)
    if(energy>=amount)
    {
        energy-=amount;
    }
    else
    {
        energy=0;
    }
    #endif
    
	handle_befana_color();
}

#define FREEZE_DAMAGE 1

void handle_balloon_collision(void)
{
	if(balloon_collision())
	{
		// ++SPRC[BEFANA_INDEX];
        SPRC[BEFANA_INDEX]=RED;
		decrease_energy(BALLOON_DAMAGE);
		freeze=FREEZE_DAMAGE;
        display_energy();
	}
}


void increase_energy(uint8_t amount)
{
    energy+=amount;
    if(energy>MAX_ENERGY)
    {
        energy = MAX_ENERGY;
    }
}


void handle_gift_collision(void)
{
    uint8_t i;
    

    for(i=GIFT_INDEX;i<=GIFT_INDEX+NUMBER_OF_GIFTS-1;++i)
    {
        if(collision(i))
        {
			_XL_ZAP_SOUND();
            increase_energy(GIFT_ENERGY);
            display_energy();
            points+=100;
            display_score();
            SPRY[i]=255;
            return;
        }
	}
}


// static char MESSAGE[] = "BEFANAVSCOMMODORE";
static char MESSAGE[] = "BEFANAVSBALLOONS";
//                       1234567890123456

void init_letters()
{
    POKE(MULTICOLOR_1,BROWN);
    POKE(MULTICOLOR_2,LIGHT_GREY);
    SPRC[BEFANA_INDEX]=PINK;
        
    for(i=1;i<=16;++i)
    {
        // SPRX[i]=20; //+18*i;
        // SPRY[i]=80+2*i;
        SPRF[i]=GFX_START_INDEX + MESSAGE[i-1] - 'A' + 1;
        SPRM[i]=1;
        SPRC[i]=GREEN;
    }
    // SPRF[9]=GFX_START_INDEX + BEFANA;

    // SPRX[9]=50;
    // SPRY[9]=180;

    for(i=1;i<=6;++i)
    {
        SPRX[i]=45+18*(i-1); //+18*i;
        SPRY[i]=70+i+sinValues8[counter];//+2*i;
    }

    for(i=7;i<=8;++i)
    {
        SPRX[i]=75+18*(i-7); //+18*i;
        SPRY[i]=115+i+sinValues8[counter];;//+2*i;
    }
    
    for(i=1;i<=8;++i)
    {
        SPRX[i+8]=10+18*i; //+18*i;
        SPRY[i+8]=170+i+sinValues8[counter];;//+2*i;
    } 
}


void hide_sprites(void)
{
    uint8_t i;
    
    for(i=0;i<_NUMBER_OF_SPRITES_;++i)
    {
        SPRY[i]=255;
    }    
    MULTIPLEX_DONE=1;
    while(MULTIPLEX_DONE)
    {        

        MULTIPLEX_DONE = 0;
        SPRUPDATEFLAG = 1;        
    }
}


/******************/
int main()
{       

    INITSPRITES();
    INITRASTER();	

	joy_install((void *)joy_static_stddrv);
	
	NUMSPRITES = _NUMBER_OF_SPRITES_;

	record = 0;
    
    POKE(MULTICOLOR_1,BROWN);
    POKE(MULTICOLOR_2,LIGHT_GREY);
    
    while(1)
    {
        hide_sprites();
        
        clear_gifts();
		clear_stars();

        init_background();
        
        MULTIPLEX_DONE=1;
        
        init_letters();
        init_player(0);
        
        SPRY[17]=120;
        SPRF[17]=GFX_START_INDEX+BALLOON;
        // SPRY[0]=255;    
        SPRF[0]=GFX_START_INDEX+BEFANA;
		while(MULTIPLEX_DONE)
		{

			MULTIPLEX_DONE = 0;
			SPRUPDATEFLAG = 1;
            
		}
		clear_stars();


		distance=(INITIAL_LEVEL-1)*LEVEL_DISTANCE;
        level_threshold = (INITIAL_LEVEL)*LEVEL_DISTANCE;
        slow_loop=0;
        fast_loop=0;
        
		freeze=0;
		
        points = 0;
		level = INITIAL_LEVEL;
		
        energy = INITIAL_ENERGY;
        counter = 0;
		        
        music_switch(1);
        
        display_hi(NUMBER_OF_COLS/2-3);
        SPRX[18]=128;

        // print("PRESS FIRE TO START",18,490,WHITE);
        do
        {
            SPRY[0]=106;
            
            SPRY[17]=44+sinValues4[(counter/4)];
            SPRM[17]=0;
            SPRC[17]=CYAN;
            SPRF[17]=GFX_START_INDEX + BALLOON;
            
            SPRY[18]=216;
            // SPRF[18]=GFX_START_INDEX + GIFT;
            SPRC[18]=RED;
            SPRM[18]=1;

            while(MULTIPLEX_DONE)
            {
                handle_stars();
                if(!(counter&3))
                {
                    ++SPRX[0];
                    --SPRX[17];
                    SPRX[18]-=2;
                    SPRF[18]=GFX_START_INDEX + GIFT + (SPRX[0]%3);
                    
                    init_letters();
                    // SPRY[17]=215+sinValues2[(counter/4)];
                    
                    SPRF[0] = GFX_START_INDEX+BEFANA+((counter/4)&3);                
                }
                ++counter;     
                MULTIPLEX_DONE = 0;
                SPRUPDATEFLAG = 1;	
            }                

        } while(!JOY_FIRE(joy_read(STANDARD_JOY))); 
        hide_sprites();

        draw_the_moon();
        
        // SPRY[14]=255;
        // SPRY[15]=255;
        // SPRY[16]=255;
        
        SPRF[SMOKE_INDEX]=GFX_START_INDEX+SMOKE;
        SPRM[SMOKE_INDEX]=1;
        SPRC[SMOKE_INDEX]=YELLOW;
        
		POKE(SCREEN+NUMBER_OF_COLS-4,ENERGY_ICON);
		POKE(COLOR+NUMBER_OF_COLS-4,RED);        
        display_energy();
		print("SCORE",5,0,CYAN);
		
		display_distance();
		print("M",1,DISTANCE_OFFSET+4,YELLOW);
        POKE(SCREEN+DISTANCE_OFFSET+5,0);
        POKE(SCREEN+DISTANCE_OFFSET+6,0);
		
		print("LV",2,LEVEL_OFFSET-2,CYAN);
		display_level();

        display_score();
		
		display_hi(HI_OFFSET);        
        
        init_balloons();
		init_gifts();
        init_player(BEFANA_INDEX);

		
        music_switch(0);
        while(energy) 
        {
            if (MULTIPLEX_DONE) {	
                handle_stars();
            
                handle_befana();
                
                handle_balloons();
                
                check_level_trigger();
                
                handle_grass();
	
                handle_gifts();

                handle_balloon_collision();
                
                handle_gift_collision();

                ++counter;
                
                if(!(counter&31))
                {
                    decrease_energy(1);
                    display_energy();
                    points+=10U;
                    display_score();
					distance+=5U;
					if(accelleration)
					{
						distance+=5U;
					}
					display_distance();
                }
                				
                MULTIPLEX_DONE = 0;
                SPRUPDATEFLAG = 1;	
            }

        }
        print("GAME OVER",9,494,RED);
		if(points>record)
		{
			record=points;
		}
		
        do
        { 
        } while(!JOY_FIRE(joy_read(STANDARD_JOY)));
		hide_sprites();
		
    }
    return 0;
}

