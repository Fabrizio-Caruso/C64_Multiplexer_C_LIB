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

#define BEFANA_MIN_X 30
#define BEFANA_MAX_X 140

#define BEFANA_MIN_Y 55
#define BEFANA_MAX_Y 208



uint8_t active_balloon[NUMBER_OF_BALLOONS];

uint8_t y_balloon[NUMBER_OF_BALLOONS];

// Pre-calculated sinus values
const uint8_t sinValues1[] = SINUS(1);

const uint8_t sinValues2[] = SINUS(2);

const uint8_t sinValues3[] = SINUS(3);

const uint8_t sinValues4[] = SINUS(4);

// const uint8_t sinValues8[] = SINUS(8);

const uint8_t shifted_sinValues1[] = SHIFTED_SINUS(1);

const uint8_t shifted_sinValues2[] = SHIFTED_SINUS(2);

const uint8_t shifted_sinValues3[] = SHIFTED_SINUS(3);

const uint8_t shifted_sinValues4[] = SHIFTED_SINUS(4);



const uint8_t shifted2_sinValues1[] = SHIFTED2_SINUS(1);

const uint8_t shifted2_sinValues2[] = SHIFTED2_SINUS(2);

const uint8_t shifted2_sinValues3[] = SHIFTED2_SINUS(3);

const uint8_t shifted2_sinValues4[] = SHIFTED2_SINUS(4);

// const uint8_t shifted_sinValues2[] = SHIFTED_SINUS(2);

// const uint8_t yValues4[] = SINUS(4);


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

static uint8_t counter;
static uint8_t energy;
static uint16_t points = 0;


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
const static uint8_t BALLOON_COLORS[] = {CYAN, PURPLE, GREEN, LIGHT_BLUE, YELLOW, BROWN, LIGHT_GREY, CYAN};

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


// TODO: Clear redefined star characters
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
	// uint8_t char_type;
	
	offset = 0;
	
	for(i=MIN_STAR+star_offset;i<last_position;i+=2)
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

	fill_sky(0,MAX_STAR-4, SLOW_STAR_TILE);
	fill_sky(1,MAX_STAR, FAST_STAR_TILE);
	
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
	SPRX[BEFANA_INDEX] = 40;
	SPRY[BEFANA_INDEX] = 20;
	SPRM[BEFANA_INDEX] = 1;
    SPRC[BEFANA_INDEX] = PINK;
}

void handle_player(void)
{

	input = joy_read(STANDARD_JOY);
	
	if(JOY_LEFT(input) && SPRX[BEFANA_INDEX]>BEFANA_MIN_X)
	{
		--SPRX[BEFANA_INDEX];
	}
	else if(JOY_RIGHT(input) && SPRX[BEFANA_INDEX]<BEFANA_MAX_X)
	{
		++SPRX[BEFANA_INDEX];
	}
	
	if(JOY_UP(input) && SPRY[BEFANA_INDEX]>BEFANA_MIN_Y)
	{
		--SPRY[BEFANA_INDEX];
	}
	else if(JOY_DOWN(input) && SPRY[BEFANA_INDEX]<BEFANA_MAX_Y)
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
    SPRC[NUMBER_OF_BALLOONS-1]=RED;
}


void init_balloons(void)
{
	// uint8_t i;
	
	init_sprite_balloons();
	
	active_balloon[0]=1;
    for(i=1;i<NUMBER_OF_BALLOONS;++i)
	{
		active_balloon[i]=1;
		y_balloon[i]= 40+i*22;

	}

}


// const uint8_t BALLOON_Y_INIT_POS[] = {80,120,140,160}

#define BALLON_THRESHOLD_X 9

void handle_balloons(void)
{
    uint8_t i;
    
    for(i=BALLOON_INDEX;i<=BALLOON_INDEX+NUMBER_OF_BALLOONS-1;++i)
    {
		if(active_balloon[i])
		{
			--SPRX[i];
			if(SPRX[i]<BALLON_THRESHOLD_X)
			{
				if(i<8)
				{
                    y_balloon[i]=62+(i&3)*32+(rand()&0x1F);
					// do
					// {
						// y_balloon[i]=rand()&0xFF;
						
					// } while((y_balloon[i]<60)||(y_balloon[i]>BALLOON_BOTTOM_Y));		
                    SPRC[i]=BALLOON_COLORS[rand()&7];

				}
				else
				{
                    SPRC[i]=WHITE;
                    if(SPRY[BEFANA_INDEX]>150)
                    {
                        y_balloon[i]=SPRY[BEFANA_INDEX]-16;
                    }
                    else if(SPRY[BEFANA_INDEX]<70)     
                    {
                        y_balloon[i]=SPRY[BEFANA_INDEX];                        
                    }
                    else if(SPRY[BEFANA_INDEX]>100)
                    {
                        y_balloon[i]=160;
                        SPRC[i]=RED;
                        // while(1){};
                    }
                    else
                    {
                        y_balloon[i]=80;
                        SPRC[i]=RED;
                        // while(1){};
                    }
                    
                   
                    // SPRX[i]=230;

				}
                SPRX[i]=184;
                
                // SPRC[8]=WHITE; // TODO: DEBUG
			}
            if(!(i&3))
            {
				SPRY[i]=y_balloon[i]+sinValues3[counter];
            }
            else if((i&3)==1)
            {
				SPRY[i]=y_balloon[i]+shifted_sinValues2[counter];
            }
            else if((i&3)==2)
            {
				SPRY[i]=y_balloon[i]+shifted2_sinValues2[counter];
            }   
            else
            {
				SPRY[i]=y_balloon[i]+shifted_sinValues3[counter];
            }              
		}
			
        // SPRY[i]= i*32-counter;
        // SPRX[i]= i*16+xValues[counter+i*8];
		// if(SPRY[i]<40)
		// {
			// SPRY[i]=255;
			// SPRX[i]=i*24;
		// }
    }
}

#define NUMBER_OF_GIFTS 4


void init_gifts(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_GIFTS;++i)
    {
        SPRF[GIFT_INDEX+i]=GFX_START_INDEX+GIFT;
        SPRC[GIFT_INDEX+i]=BALLOON_COLORS[i];
        SPRY[GIFT_INDEX+i]=80+40*i;
        SPRM[GIFT_INDEX+i]=1;
    }
}


void handle_gifts(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_GIFTS;++i)
    {
        --SPRX[GIFT_INDEX+i];
        if(!SPRX[GIFT_INDEX+i])
        {
            SPRY[GIFT_INDEX+i]=80+i*40-(rand()&0xF);
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
    // uint8_t i;
    // uint8_t x;
	// uint8_t y;
	// uint8_t befana_x;
	// uint8_t befana_y;
	
	// befana_x = SPRX[BEFANA_INDEX];
	// befana_y = SPRY[BEFANA_INDEX];

    for(i=BALLOON_INDEX;i<=BALLOON_INDEX+NUMBER_OF_BALLOONS-1;++i)
    {
        if(collision(i))
        {
            return 1;
        }
		// x = SPRX[i];
		// if(x>=befana_x)
		// {
			// if((x-befana_x)>COLLISION_BOX_X)
			// {
				// continue;
			// }
		// }
		// else // x < befana_x
		// {
			// if((befana_x-x)>COLLISION_BOX_X)
			// {
				// continue;
			// }
		// }
		
		// y = SPRY[i];
		// if(y>=befana_y)
		// {
			// if((y-befana_y)>COLLISION_BOX_Y)
			// {
				// continue;
			// }
		// }
		// else // y < befana_y
		// {
			// if((befana_y-y)>COLLISION_BOX_Y)
			// {
				// continue;
			// }
		// }
		// return 1;
	}
	return 0;
}


void handle_befana_color(void)
{
    if(energy<50)
    {
        SPRC[BEFANA_INDEX]=RED;
    }
    else if(energy<100)
    {
        SPRC[BEFANA_INDEX]=PURPLE;
    }
    else if(energy<150)
    {
        SPRC[BEFANA_INDEX]=LIGHT_GREEN;
    }
    else
    {
        SPRC[BEFANA_INDEX]=PINK;
    }
}

void handle_collision(void)
{
	if(balloon_collision())
	{
		++SPRC[BEFANA_INDEX];
		--energy;
		printd(energy,3,NUMBER_OF_COLS-10,WHITE);
	}
    handle_befana_color();
}

/******************/
int main()
{       

	
    
    INITSPRITES();
    INITRASTER();	

	joy_install((void *)joy_static_stddrv);
	
	NUMSPRITES = _NUMBER_OF_SPRITES_;


	

    while(1)
    {
        init_player();
        init_balloons();
        init_gifts();

        slow_loop=0;
        fast_loop=0;
        
        points = 0;
     
        energy = 200;
        counter = 48;
        
        SPRX[BEFANA_INDEX] = 100;
        SPRY[BEFANA_INDEX] = 50;
        POKE(MULTICOLOR_1,BROWN);
        POKE(MULTICOLOR_2,LIGHT_GREY);
        SPRC[BEFANA_INDEX]=PINK;

        clear_screen();


        init_background();
        printd(energy,3,NUMBER_OF_COLS-10,WHITE);
        printd(0,4,6,WHITE);
        
        // print("PRESS FIRE TO START",18,490,WHITE);
        do
        {
        } while(!JOY_FIRE(joy_read(STANDARD_JOY))); 
            

        while(energy) 
        {
            if (MULTIPLEX_DONE) {	
                handle_stars();
            
                handle_player();
                
                handle_balloons();
                // NUMSPRITES=22;
                
                scroll_grass();

                // printd(PEEK(SPRITE_COLLISION_REGISTER),3,0,WHITE);

                handle_gifts();

                handle_collision();

                ++counter;
                
                if(!counter)
                {
                    points+=10U;
                    printd(points,4,6,WHITE);
                }
                
                MULTIPLEX_DONE = 0;
                SPRUPDATEFLAG = 1;	
            }

        }
        print("GAME OVER",9,492,RED);
        do
        { 
        } while(!JOY_FIRE(joy_read(STANDARD_JOY)));
    }
    return 0;
}

