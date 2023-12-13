// #include <conio.h>
#include <stdlib.h>
#include <peekpoke.h>
/*******************
** Prototypes
*******************/

#define SLOW_COMETS 1

void INITSPRITES(void);
void INITRASTER(void);
/******************/
extern unsigned char SPRX[];
extern unsigned char SPRY[];
extern unsigned char SPRC[];
extern unsigned char SPRF[];
extern unsigned char SPREX[];

extern unsigned char SPRM[];

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

const char santa_map[] = {0,1,2,1};


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
#define MESSAGE_LENGTH 12

const char MESSAGE2[14] = "MERRYCHRISTMAS";
#define MESSAGE2_LENGTH 14

const unsigned char COLORS[14] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15};

const char AUTHOR[15] = "FABRIZIO CARUSO";
const char TEXT1[27] =  "ALL LOGIC CODE WRITTEN IN C";
const char TEXT2[28] =  "MODIFIED CADAVER MULTIPLEXER";
const char TEXT3[29] =  "SID TUNE AND FONTS RIPPED OFF";
const char TEXT4[17] =  "FAST MODE ENABLED";

const char WRITTEN[15] = "WRITTEN IN C BY";

const char YEAR[14] =   "HAPPY NEW YEAR";

const unsigned char STAR_0[4] = {0x00,0x10,0x10,0x00};
const unsigned char STAR_1[4] = {0x10,0x28,0x38,0x10};
// const star STAR_0[4] = {};

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


static unsigned char j;
static unsigned char h;
static unsigned char restored_text_row[35];

#define COMET_X 4
#define COMET_Y 23

unsigned char XX = 0;
unsigned char XX2 = 100;
unsigned char i;
unsigned short k;
unsigned short star_loc;
unsigned char flip = 1;

unsigned char comet_flash;

// SLOW COMET 1
unsigned char slow_comet_1_x = COMET_X;
unsigned char slow_comet_1_y = COMET_Y;
unsigned char slow_comet_1_x_start;
unsigned short slow_comet_1_pos;
unsigned slow_comet_1_counter = 0;
unsigned short old_slow_comet_1_pos;

// SLOW COMET 2
unsigned char slow_comet_2_x = COMET_X+16;
unsigned char slow_comet_2_y = COMET_Y;
unsigned char slow_comet_2_x_start;
unsigned short slow_comet_2_pos;
unsigned slow_comet_2_counter = 0;
unsigned short old_slow_comet_2_pos;

// FAST COMET 1
unsigned char fast_comet_1_x = COMET_X+12;
unsigned char fast_comet_1_y = COMET_Y;
unsigned char fast_comet_1_x_start;
unsigned short fast_comet_1_pos;
unsigned fast_comet_1_counter = 0;
unsigned short old_fast_comet_1_pos;

// FAST COMET 2
unsigned char fast_comet_2_x = COMET_X+18;
unsigned char fast_comet_2_y = COMET_Y;
unsigned char fast_comet_2_x_start;
unsigned short fast_comet_2_pos;
unsigned fast_comet_2_counter = 0;
unsigned short old_fast_comet_2_pos;

unsigned char below;
unsigned char text_counter = 0;

unsigned char XB;
unsigned cycle = 1;


#define TILE_BASE ('9'+31)

#define NUMBER_OF_COLUMNS 40
#define TREE_LEVEL(n) ((24-n)*(NUMBER_OF_COLUMNS))

#define MULTI_CHAR_COLOR_1 0xD022
#define MULTI_CHAR_COLOR_2 0xD023

#define MULTI_COLOR_BIT 8

#define XTREE_LIGHT_1 (COLOR+TREE_LEVEL(3)+1)
#define XTREE_LIGHT_2 (COLOR+TREE_LEVEL(2)+1)
#define XTREE_LIGHT_3 (COLOR+TREE_LEVEL(2)+2)
#define XTREE_LIGHT_4 (COLOR+TREE_LEVEL(1)+1)
#define XTREE_LIGHT_5 (COLOR+TREE_LEVEL(1)+2)

#define MULTICOLOR_1 0xD025
#define MULTICOLOR_2 0xD026

// $D018 = 53272
// -----------------
void init_udg(void)
{
    POKE(56576u,3);
	POKE(0xD018,PEEK(0xD018)|8);
	POKE(0xD018,PEEK(0xD018)&(255-4-2));    
	POKE(648,192);
}

//#define TEXT_COLOR_OFFSET 2
#define TEXT_COLOR_OFFSET 0
// #define COLOR_MASK 3
#define COLOR_MASK 7

void color_change(void)
{
	unsigned char color;

	if(!flip)
	{
		color = 0;
	}
	else
	{
		color = +(j&COLOR_MASK);
	}
	
    for(h=0;h<10;++h)
    {
        
        POKE(COLOR+6+h,TEXT_COLOR_OFFSET+color);
        POKE(COLOR+40+6+h,TEXT_COLOR_OFFSET+color);
    }

    for(h=0;h<6;++h)
    {
        
        POKE(COLOR+18+h,TEXT_COLOR_OFFSET+color);
        POKE(COLOR+40+18+h,TEXT_COLOR_OFFSET+color);
    }
    
    for(h=0;h<8;++h)
    {
        
        POKE(COLOR+26+h,TEXT_COLOR_OFFSET+color);
        POKE(COLOR+40+26+h,TEXT_COLOR_OFFSET+color);
    }  
	
	
	POKE(XTREE_LIGHT_1,MULTI_COLOR_BIT+RED+(j&3));
	POKE(XTREE_LIGHT_2,MULTI_COLOR_BIT+RED+((j+1)&3));
	POKE(XTREE_LIGHT_3,MULTI_COLOR_BIT+RED+((j+2)&3));
	POKE(XTREE_LIGHT_4,MULTI_COLOR_BIT+RED+((j+3)&3));
	POKE(XTREE_LIGHT_5,MULTI_COLOR_BIT+RED+((j)&3));
	POKE(MULTI_CHAR_COLOR_1,MULTI_COLOR_BIT+6+(j&1));
}


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


// Characters for flashing stars
#define BIG_FLASHING_STAR 0
#define FLASHING_STAR 30
#define SMALL_FLASHING_STAR 27

// Characters for fixed stars
#define SMALL_FIXED_STAR 28
#define BIG_FIXED_STAR 29

// Characters for comets
#define COMET 33

#define SANTA_INDEX 5
#define REINDEER_INDEX 6
#define BEFANA_INDEX 7

#define TOP_SNOW_INDEX (NUMSPRITES-6)
#define BOTTOM_SNOW_INDEX (NUMSPRITES-8)

#define TOP_PRESENT_INDEX (NUMSPRITES-5)
#define BOTTOM_PRESENT_INDEX (NUMSPRITES-7)

#define RIGHT_HOT_AIR_BALLOON_INDEX (NUMSPRITES-2)
#define LEFT_HOT_AIR_BALLOON_INDEX (NUMSPRITES-4)

#define RIGHT_BIG_SNOW_INDEX (NUMSPRITES-1)
#define LEFT_BIG_SNOW_INDEX (NUMSPRITES-3)



void draw_top_text(void)
{
	// Set color for top fixed "happy new year" text
    color_change();
    
	// Draw top fixed "happy new year" text
	
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
}


void draw_the_moon(void)
{
    for(i=1;i<4;++i)
    {
        for(j=0;j<4;++j)
        {
            POKE(SCREEN+MOON_OFFSET+i+40*j,(255-16)+i+j*4);
            POKE(COLOR+MOON_OFFSET+i+40*j,1);
        }
    }	
}


void draw_stars(void)
{
	// Set stars color to white
	for(k=COLOR+160;k<COLOR+960;++k)
	{
		POKE(k,1);
	}	
	
    for(k=0;k<55;++k)
    {
        star_loc = rand()%1000;
        POKE(SCREEN+star_loc,BIG_FLASHING_STAR); // big flashing star
        POKE(COLOR+star_loc,1);
    }
    for(k=0;k<30;++k)
    {
        star_loc = rand()%1000;
        POKE(SCREEN+star_loc,BIG_FIXED_STAR); // big fixed star
        POKE(COLOR+star_loc,1);
    }
    for(k=0;k<60;++k)
    {
        star_loc = rand()%1000;
        POKE(SCREEN+star_loc,SMALL_FLASHING_STAR); // small flashing star
        POKE(COLOR+star_loc,1);
    }
    for(k=0;k<30;++k)
    {
        star_loc = rand()%1000;
        POKE(SCREEN+star_loc,SMALL_FIXED_STAR); // small fixed star
        POKE(COLOR+star_loc,1);
    }
    for(k=0;k<75;++k)
    {
        star_loc = rand()%1000;
        POKE(SCREEN+star_loc,FLASHING_STAR); // small/big flashing star
        POKE(COLOR+star_loc,1);
    }


    for(k=0;k<40;++k)
    {
        star_loc = rand()%80;
        POKE(SCREEN+star_loc,SMALL_FLASHING_STAR); // small flashing star (top 2 rows)
        POKE(COLOR+star_loc,1);
    }

    for(k=0;k<40;++k)
    {
        star_loc = rand()%80;
        POKE(SCREEN+star_loc,FLASHING_STAR); // small/big flashing star (top 2 rows)
        POKE(COLOR+star_loc,1);
    }	
}


void set_background_colors(void)
{
    POKE(0xd020, 0x00);
    POKE(0xd021, 0x00);  	
}


void init_text(void)
{
    for(i=0;i<35;++i)
    {
        restored_text_row[i]=PEEK(SCREEN+120+i);
    }
	    
    print(AUTHOR, 15, 1000-15,3);	
}

void clear_screen(void)
{
    for(k=0;k<1000;++k)
    {
        POKE(SCREEN+k,32);
        POKE(COLOR+k,0);
    }	
}


void draw_xmas_tree(void)
{
	
	POKE(MULTI_CHAR_COLOR_1,YELLOW);
	POKE(MULTI_CHAR_COLOR_2,GREEN);

	
	POKE(SCREEN+TREE_LEVEL(3)+1,TILE_BASE);
	POKE(SCREEN+TREE_LEVEL(3)+2,TILE_BASE+1);

	POKE(SCREEN+TREE_LEVEL(2),  TILE_BASE+2);
	POKE(SCREEN+TREE_LEVEL(2)+1,TILE_BASE+3);	
	POKE(SCREEN+TREE_LEVEL(2)+2,TILE_BASE+4);
	POKE(SCREEN+TREE_LEVEL(2)+3,TILE_BASE+5);	

	POKE(SCREEN+TREE_LEVEL(1),  TILE_BASE+6);
	POKE(SCREEN+TREE_LEVEL(1)+1,TILE_BASE+7);	
	POKE(SCREEN+TREE_LEVEL(1)+2,TILE_BASE+8);
	POKE(SCREEN+TREE_LEVEL(1)+3,TILE_BASE+9);	

	POKE(SCREEN+TREE_LEVEL(0),  TILE_BASE+10);
	POKE(SCREEN+TREE_LEVEL(0)+1,TILE_BASE+11);	
	POKE(SCREEN+TREE_LEVEL(0)+2,TILE_BASE+12);
	POKE(SCREEN+TREE_LEVEL(0)+3,TILE_BASE+13);	
	
	
	POKE(COLOR+TREE_LEVEL(3)+1,RED+MULTI_COLOR_BIT);
	POKE(COLOR+TREE_LEVEL(3)+2,YELLOW+MULTI_COLOR_BIT);

	POKE(COLOR+TREE_LEVEL(2),  3+MULTI_COLOR_BIT);
	POKE(COLOR+TREE_LEVEL(2)+1,YELLOW+MULTI_COLOR_BIT);	
	POKE(COLOR+TREE_LEVEL(2)+2,RED+MULTI_COLOR_BIT);
	POKE(COLOR+TREE_LEVEL(2)+3,3+MULTI_COLOR_BIT);	

	POKE(COLOR+TREE_LEVEL(1),  4+MULTI_COLOR_BIT);
	POKE(COLOR+TREE_LEVEL(1)+1,RED+MULTI_COLOR_BIT);	
	POKE(COLOR+TREE_LEVEL(1)+2,YELLOW+MULTI_COLOR_BIT);
	POKE(COLOR+TREE_LEVEL(1)+3,4+MULTI_COLOR_BIT);	

	POKE(COLOR+TREE_LEVEL(0),  GREEN+MULTI_COLOR_BIT);
	POKE(COLOR+TREE_LEVEL(0)+1,RED);	
	POKE(COLOR+TREE_LEVEL(0)+2,RED);
	POKE(COLOR+TREE_LEVEL(0)+3,GREEN+MULTI_COLOR_BIT);		
	
	// while(1)
	// {}
}


void init_background(void)
{
	clear_screen();
	
    init_udg();
    
	set_background_colors();
	
	// Set multi-color character mode
	POKE(0xD016,PEEK(0xD016)|0x10);
    
	draw_stars();

	init_text();
    
	draw_top_text();

	draw_the_moon();
	
	draw_xmas_tree();
	
}

void handle_slow_comets(void)
{
	if(!(XX&3))
	{
		if(slow_comet_1_counter)
		{
			--slow_comet_1_counter;
		}
		if(!slow_comet_1_counter)
		{

			old_slow_comet_1_pos = slow_comet_1_x+40U*slow_comet_1_y;

			--slow_comet_1_y;
			++slow_comet_1_x;
			if((slow_comet_1_x>36)||(slow_comet_1_y<4))
			{
				do
				{
					slow_comet_1_x_start = COMET_X+(rand()&15);
				}
				#if SLOW_COMETS==2
				while((slow_comet_1_x_start==slow_comet_2_x_start)||(slow_comet_1_x_start==fast_comet_1_x_start)||(slow_comet_1_x_start==fast_comet_2_x_start));
				#else
				while((slow_comet_1_x_start==fast_comet_1_x_start)||(slow_comet_1_x_start==fast_comet_2_x_start));
				#endif
				slow_comet_1_x = slow_comet_1_x_start;
				slow_comet_1_y = COMET_Y;
				slow_comet_1_counter = rand()&31;
			}
			slow_comet_1_pos = slow_comet_1_x+40U*slow_comet_1_y;

			POKE(SCREEN+old_slow_comet_1_pos,below);

			below = PEEK(SCREEN+slow_comet_1_pos);
			if(!slow_comet_1_counter)
			{
				POKE(SCREEN+slow_comet_1_pos,COMET);
			} 
		}
		
		#if SLOW_COMETS==2
		if(slow_comet_2_counter)
		{
			--slow_comet_2_counter;
		}
		if(!slow_comet_2_counter)
		{

			old_slow_comet_2_pos = slow_comet_2_x+40U*slow_comet_2_y;

			--slow_comet_2_y;
			++slow_comet_2_x;
			if((slow_comet_2_x>36)||(slow_comet_2_y<4))
			{
				do
				{
					slow_comet_2_x_start = COMET_X+(rand()&31);
				}
				while((slow_comet_2_x_start==slow_comet_1_x_start)||(slow_comet_2_x_start==fast_comet_1_x_start)||(slow_comet_2_x_start==fast_comet_2_x_start));
				slow_comet_2_x = slow_comet_2_x_start;
				slow_comet_2_y = COMET_Y;
				slow_comet_2_counter = rand()&31;
			}
			slow_comet_2_pos = slow_comet_2_x+40U*slow_comet_2_y;

			POKE(SCREEN+old_slow_comet_2_pos,below);

			below = PEEK(SCREEN+slow_comet_2_pos);
			if(!slow_comet_2_counter)
			{
				POKE(SCREEN+slow_comet_2_pos,COMET);
			} 
		}	
		#endif
	}
}


void handle_fast_comets(void)
{
	if(!(XX&1))
	{
		if(fast_comet_1_counter)
		{
			--fast_comet_1_counter;
		}
		if(!fast_comet_1_counter)
		{

			old_fast_comet_1_pos = fast_comet_1_x+40U*fast_comet_1_y;

			--fast_comet_1_y;
			++fast_comet_1_x;
			if((fast_comet_1_x>36)||(fast_comet_1_y<4))
			{
				do
				{
					fast_comet_1_x_start = COMET_X+(rand()&31);
				}
				#if SLOW_COMETS==2
				while((fast_comet_1_x_start==slow_comet_1_x_start)||(fast_comet_1_x_start==slow_comet_2_x_start)||(fast_comet_1_x_start==fast_comet_2_x_start));
				#else
				while((fast_comet_1_x_start==slow_comet_1_x_start)||(fast_comet_1_x_start==fast_comet_2_x_start));
				#endif
				fast_comet_1_x = fast_comet_1_x_start;
				fast_comet_1_y = COMET_Y;
				fast_comet_1_counter = rand()&63;
			}
			fast_comet_1_pos = fast_comet_1_x+40U*fast_comet_1_y;

			POKE(SCREEN+old_fast_comet_1_pos,below);

			below = PEEK(SCREEN+fast_comet_1_pos);
			if(!fast_comet_1_counter)
			{
				POKE(SCREEN+fast_comet_1_pos,COMET);
			} 
		}
		
		if(fast_comet_2_counter)
		{
			--fast_comet_2_counter;
		}
		if(!fast_comet_2_counter)
		{

			old_fast_comet_2_pos = fast_comet_2_x+40U*fast_comet_2_y;

			--fast_comet_2_y;
			++fast_comet_2_x;
			if((fast_comet_2_x>36)||(fast_comet_2_y<4))
			{
				do
				{
					fast_comet_2_x_start = COMET_X+(rand()&31);
				}
				#if SLOW_COMETS==2				
				while((fast_comet_2_x_start==fast_comet_1_x_start)||(fast_comet_2_x_start==slow_comet_1_x_start)||(fast_comet_2_x_start==slow_comet_2_x_start));
				#else
				while((fast_comet_2_x_start==fast_comet_1_x_start)||(fast_comet_2_x_start==slow_comet_1_x_start));
				#endif
				fast_comet_2_x = fast_comet_2_x_start;
				fast_comet_2_y = COMET_Y;
				fast_comet_2_counter = rand()&63;
			}
			fast_comet_2_pos = fast_comet_2_x+40U*fast_comet_2_y;

			POKE(SCREEN+old_fast_comet_2_pos,below);

			below = PEEK(SCREEN+fast_comet_2_pos);
			if(!fast_comet_2_counter)
			{
				POKE(SCREEN+fast_comet_2_pos,33);
			} 
		}
		
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
	
}


void init_happy_new_year_sprites(void)
{
    NUMSPRITES = _NUMBER_OF_SPRITES_;

    for(i=0;i<14;++i)
    {
        SPRF[i] = GFX_START_INDEX + MESSAGE2[i] - 'A' + 1;
        SPRC[i] = 2+i;        
    }    
    
    for(i=14;i<NUMSPRITES;++i)
    {
        SPRF[i]= GFX_START_INDEX+62+(i&1);

        SPRC[i] = 1;        
    }
    
	// hot air balloon
	SPRF[RIGHT_HOT_AIR_BALLOON_INDEX] = GFX_START_INDEX + HOT_AIR_BALLOON;
	SPRF[LEFT_HOT_AIR_BALLOON_INDEX] = GFX_START_INDEX + HOT_AIR_BALLOON;
	
	// commodore logo or present
	SPRF[BOTTOM_PRESENT_INDEX] = GFX_START_INDEX + PRESENT;
	SPRF[TOP_PRESENT_INDEX] = GFX_START_INDEX + PRESENT;

	// SPRF[BOTTOM_PRESENT_INDEX] = GFX_START_INDEX + PRESENT;
	// SPRF[TOP_PRESENT_INDEX] = GFX_START_INDEX + PRESENT;
	// SPRM[BOTTOM_PRESENT_INDEX] = 1;
	// SPRM[TOP_PRESENT_INDEX] = 1;	

	
	
	SPRC[RIGHT_HOT_AIR_BALLOON_INDEX] = 3;
	SPRC[LEFT_HOT_AIR_BALLOON_INDEX] = 2;
	SPRC[BOTTOM_PRESENT_INDEX] = 4;
	SPRC[TOP_PRESENT_INDEX] = 5;
	
    // Position snow flakes, commodore logos and hot air balloons
    SPRX[RIGHT_BIG_SNOW_INDEX]=120;
    SPRX[LEFT_BIG_SNOW_INDEX]=40;
    SPRX[RIGHT_HOT_AIR_BALLOON_INDEX]=100;
    SPRX[LEFT_HOT_AIR_BALLOON_INDEX]=60;   

    SPRY[TOP_PRESENT_INDEX]=90;
	
    SPRY[TOP_SNOW_INDEX]=60;  
    
	SPRY[BOTTOM_PRESENT_INDEX]=200;
	
    SPRY[BOTTOM_SNOW_INDEX]=210;  
    
	for(i=0;i<BOTTOM_SNOW_INDEX;++i)
	{
		SPRM[i]=1;
	}
	for(i=BOTTOM_SNOW_INDEX;i<NUMSPRITES;++i)
	{
		SPRM[i]=0;
	}
	
	SPRM[BOTTOM_PRESENT_INDEX] = 1;
	SPRM[TOP_PRESENT_INDEX] = 1;	
	
	POKE(MULTICOLOR_1,2);
	POKE(MULTICOLOR_2,3);
	
	for(i=0;i<5;++i)
	{
		SPRX[i]=XMAS_X_OFFSET+i*XMAS_FONT_SPACING;
	}

	for(i=0;i<9;++i)
	{
		SPRX[i+5]=25+i*(XMAS_FONT_SPACING/2+4); // TODO: do we need this on all cycles?
	} 

	// for(i=0;i<6;++i)
	// {
		// SPRX[i+8]=10+i*XMAS_FONT_SPACING;
	// }  

	// draw_xmas_tree();
}


void handle_stars_and_colors(void)
{
	if(!(XX&31))
	{
		++j;
		for(i=0;i<5;++i)
		{
			SPRC[i] = 5+((XX+i+j)&7);        
		}
		if(flip)
		{
			for(i=5;i<8;++i)
			{
				SPRC[i] = 5+((XX+i+j)&7);        
			}
		}
		for(i=8;i<12;++i)
		{
			SPRC[i] = 5+((XX+i+j)&7);        
		}
		POKE(SHAPE+3,0x28+16*(j&1));
		POKE(SHAPE+3+SMALL_FLASHING_STAR*8,8*(j&1));
		
		color_change();
		
		POKE(SHAPE+2+FLASHING_STAR*8,STAR_0[j&3]);
		POKE(SHAPE+3+FLASHING_STAR*8,STAR_1[j&3]);
		POKE(SHAPE+4+FLASHING_STAR*8,STAR_0[j&3]);
	}	
}


void handle_text(void)
{
	if(text_counter==0)
	{
		restore_text_row();
		print(TEXT1, 27,120+6,YELLOW);
	}
	else if(text_counter==1)
	{
	   restore_text_row();
	   print(TEXT2, 28,120+6,GREEN);
	}
	else if(text_counter==2)
	{
		restore_text_row();
		print(TEXT3, 29,120+6,CYAN);
	}
	else
	{
		restore_text_row();
	}
	++text_counter;
	text_counter%=4;
}


void handle_sprite_movement(void)
{
	
	unsigned char xb_mod3;
	
	
	if(XX&1)
	{
		++XB;
		xb_mod3 = (XB%3);
	}
	
	SPRY[RIGHT_BIG_SNOW_INDEX]=XX;
	
	// Hot air balloon 1	
	SPRY[RIGHT_HOT_AIR_BALLOON_INDEX]=255-XX;
	SPRX[RIGHT_HOT_AIR_BALLOON_INDEX] = 100-10+xValues[XX2];

	SPRY[LEFT_BIG_SNOW_INDEX]=XX2;
	
	// Hot air balloon 2
	SPRY[LEFT_HOT_AIR_BALLOON_INDEX]=255-XX2;
	SPRX[LEFT_HOT_AIR_BALLOON_INDEX] = 60-10+shifted_xValues[XX2];
	
	// Present 1
	SPRX[TOP_PRESENT_INDEX]=XX2;
	
	// Small snow flake 1
	SPRX[TOP_SNOW_INDEX]=255-XX2;

	// Present 2
	SPRX[BOTTOM_PRESENT_INDEX]=XX;
	
	// Small snow flake 2
	SPRX[BOTTOM_SNOW_INDEX]= 255-XX;

	// HAPPY/MERRY
	for(i=0;i<5;++i)
	{
		SPRY[i]=i*8+Y_OFFSET+yValues[XX];
	}

	
	if(flip) // HAPPY NEW YEAR or MERRY CHRISTMAS
	{
		
		if(cycle) // NEW YEAR
		{
			for(i=0;i<3;++i)
			{
				SPRY[i+5]=i*8+Y_OFFSET+SEPARATION+yValues[XX];;
			}
			
			for(i=0;i<4;++i)
			{
				SPRY[i+8]=i*8+Y_OFFSET+(3*SEPARATION/2)+flip*(SEPARATION/2)+yValues[XX];;
			}			
		}
		else // CHRISTMAS
		{
			for(i=0;i<9;++i)
			{
				SPRY[i+5]=i*8+i+Y_OFFSET+35+yValues[XX];;
			}
		}
	}
	else // HAPPY <YEAR>
	{

		if(!(XX&1))
		{
			// ++XB;
			
			SPRX[SANTA_INDEX]=XB;
			SPRF[SANTA_INDEX]=GFX_START_INDEX + SANTA+santa_map[(XB&3)];
			
			SPRX[REINDEER_INDEX]=XB+12;
			SPRF[REINDEER_INDEX]=GFX_START_INDEX + REINDEER+santa_map[(XB&3)];

			SPRX[BEFANA_INDEX]=162-XB;
			SPRF[BEFANA_INDEX]=GFX_START_INDEX + BEFANA+(XB&3);

		}
		for(i=0;i<4;++i)
		{
			SPRY[i+8]=i*8+Y_OFFSET+(3*SEPARATION/2)+yValues[XX];;
		}			
	}

	if(!(XX&7))
	{
		SPRF[TOP_PRESENT_INDEX]=GFX_START_INDEX + PRESENT+xb_mod3;
		SPRF[BOTTOM_PRESENT_INDEX]=GFX_START_INDEX + PRESENT+xb_mod3;
	}


	

}


void handle_sprite_change(void)
{
	if(!(XX))
	{
		handle_text();
			
		if(flip)
		{
			// SPRF[ 7]=GFX_START_INDEX + 32;
			SPRY[SANTA_INDEX]=53;
			SPRY[REINDEER_INDEX]=53;
			SPRX[BEFANA_INDEX]=0;
			SPRY[BEFANA_INDEX]=230;
			// SPRC[SANTA_INDEX]=12;
			
			SPRF[SANTA_INDEX]=GFX_START_INDEX + SANTA;
			SPRF[REINDEER_INDEX]=GFX_START_INDEX + REINDEER;
			SPRF[BEFANA_INDEX]=GFX_START_INDEX + BEFANA;	
			XB = 15;
			
			SPRF[ 0] = GFX_START_INDEX - 'A' + 1 + 'H';
			SPRF[ 1] = GFX_START_INDEX - 'A' + 1 + 'A';
			SPRF[ 2] = GFX_START_INDEX - 'A' + 1 + 'P';
			SPRF[ 3] = GFX_START_INDEX - 'A' + 1 + 'P';	
				
			SPRF[ 8]=GFX_START_INDEX + '2';//'Z' - 'A' + 25; // 2
			SPRF[ 9]=GFX_START_INDEX + '0'; // 0
			SPRF[10]=GFX_START_INDEX + YEAR_HIGH; // 2
			SPRF[11]=GFX_START_INDEX + YEAR_LOW; // 3
			SPREX[ 8]=1;
			SPREX[ 9]=1;
			SPREX[10]=1;
			SPREX[11]=1;

			SPRY[12]=255;
			SPRY[13]=255;			
			flip = 0;
			
			for(i=0;i<4;++i)
			{
				SPRX[i+8]=X_OFFSET+i*26;
			} 				
			
			// SPRF[BOTTOM_PRESENT_INDEX] = GFX_START_INDEX + PRESENT;
			// SPRF[TOP_PRESENT_INDEX] = GFX_START_INDEX + PRESENT;
			// SPRM[BOTTOM_PRESENT_INDEX] = 1;
			// SPRM[TOP_PRESENT_INDEX] = 1;	
			SPRC[BOTTOM_PRESENT_INDEX] = RED;
			SPRC[TOP_PRESENT_INDEX] = CYAN;
			
			SPRC[SANTA_INDEX]=RED;
			SPRC[REINDEER_INDEX]=YELLOW;
			POKE(MULTICOLOR_1,BROWN);
			POKE(MULTICOLOR_2,LIGHT_GREY);
            SPRC[BEFANA_INDEX]=PINK;
		}
		else
		{
			++cycle;
			cycle&=1;
			if(cycle&1)
			{
				
				SPRF[ 0] = GFX_START_INDEX - 'A' + 1 + 'H';
				SPRF[ 1] = GFX_START_INDEX - 'A' + 1 + 'A';
				SPRF[ 2] = GFX_START_INDEX - 'A' + 1 + 'P';
				SPRF[ 3] = GFX_START_INDEX - 'A' + 1 + 'P';				
				// SPRY[ 4] = GFX_START_INDEX - 'A' + 1 + 'N';

				SPRF[SANTA_INDEX]=GFX_START_INDEX - 'A' + 1 + 'N';					
				SPRF[REINDEER_INDEX]=GFX_START_INDEX - 'A' + 1 + 'E';
				SPRF[BEFANA_INDEX]=GFX_START_INDEX - 'A' + 1 + 'W';
				
				SPRF[ 8]=GFX_START_INDEX - 'A' + 1 + 'Y';
				SPRF[ 9]=GFX_START_INDEX - 'A' + 1 + 'E';
				SPRF[10]=GFX_START_INDEX - 'A' + 1 + 'A';
				SPRF[11]=GFX_START_INDEX - 'A' + 1 + 'R';
				
				SPRY[12]=255;
				SPRY[13]=255;
				
				SPREX[ 8]=0;
				SPREX[ 9]=0;
				SPREX[10]=0;
				SPREX[11]=0; 
				POKE(MULTICOLOR_1,RED);
				POKE(MULTICOLOR_2,CYAN);
				
				SPRC[BOTTOM_PRESENT_INDEX] = PURPLE;
				SPRC[TOP_PRESENT_INDEX] = GREEN;
				flip = 1;
				for(i=0;i<3;++i)
				{
					SPRX[i+5]=X_OFFSET+i*22;
				} 	
			}
			else
			{
				SPRY[SANTA_INDEX] = 255;
				SPRY[REINDEER_INDEX] = 255;
				SPRY[BEFANA_INDEX] = 255;
				
				SPRF[ 0] = GFX_START_INDEX - 'A' + 1 + 'M';
				SPRF[ 1] = GFX_START_INDEX - 'A' + 1 + 'E';
				SPRF[ 2] = GFX_START_INDEX - 'A' + 1 + 'R';
				SPRF[ 3] = GFX_START_INDEX - 'A' + 1 + 'R';	
			
				SPRF[SANTA_INDEX]=GFX_START_INDEX - 'A' + 1 + 'C';					
				SPRF[REINDEER_INDEX]=GFX_START_INDEX - 'A' + 1 + 'H';
				SPRF[BEFANA_INDEX]=GFX_START_INDEX - 'A' + 1 + 'R';
				SPRF[ 8]=GFX_START_INDEX - 'A' + 1 + 'I';
				SPRF[ 9]=GFX_START_INDEX - 'A' + 1 + 'S';
				SPRF[10]=GFX_START_INDEX - 'A' + 1 + 'T';
				SPRF[11]=GFX_START_INDEX - 'A' + 1 + 'M';
				SPRF[12]=GFX_START_INDEX - 'A' + 1 + 'A';
				SPRF[13]=GFX_START_INDEX - 'A' + 1 + 'S';				
				SPREX[ 8]=0;
				SPREX[ 9]=0;
				SPREX[10]=0;
				SPREX[11]=0; 
				
				POKE(MULTICOLOR_1,RED);
				POKE(MULTICOLOR_2,GREEN);
				
			
				SPRC[BOTTOM_PRESENT_INDEX] = PURPLE;
				SPRC[TOP_PRESENT_INDEX] = CYAN;
				flip = 1;
				for(i=0;i<9;++i)
				{
					SPRX[i+5]=20+i*3*XMAS_FONT_SPACING/4;
				} 					
			}				
		}
	}	
}

void happy_new_year(void)
{
	
	handle_sprite_change();
	
	handle_sprite_movement();

	handle_slow_comets();

	handle_fast_comets();
	
	handle_stars_and_colors();
	
	++XX;
	++XX2;

}

/******************/
int main()
{        
	init_background();
    
    INITSPRITES();
    INITRASTER();	
	init_happy_new_year_sprites();
	
    while(1) 
    {
        if (MULTIPLEX_DONE) {	
			happy_new_year();
			
			MULTIPLEX_DONE = 0;
			SPRUPDATEFLAG = 1;	
        }

    }
    return 0;
}
