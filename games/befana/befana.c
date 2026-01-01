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


static uint8_t item_type;



#define INITIAL_LEVEL 1

// #define BALLOON_DAMAGE 10
#define BALLOON_DAMAGE 12


// #define TRAINER 1

#define BETA_VERSION 1

#define INITIAL_LIVES 3
#define MAX_LIVES 9

#define GIFT_POINTS  100
#define BALLOON_POINTS 25
#define ARMOR_POINTS 20
#define SANTA_POINTS 5

#define SANTA_THRESHOLD (20)
#define SANTA_CHARGE 50
#define SANTA_ENERGY 1

#define ARMOR_RECHARGE 1
#define BULLET_RECHARGE 99

#define MAX_ARMOR 3

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


#define ONE_SHIFTED3_SINUS(F) \
    64/F, 63/F, 63/F, 62/F, 61/F, 60/F, 58/F, 56/F, \
    54/F, 52/F, 49/F, 47/F, 44/F, 41/F, 38/F, 35/F, \
    32/F, 28/F, 25/F, 22/F, 19/F, 16/F, 14/F, 11/F, \
    9/F, 7/F, 5/F, 3/F, 2/F, 1/F, 0/F, 0/F, \
    0/F, 0/F, 0/F, 1/F, 2/F, 3/F, 5/F, 7/F, \
    9/F, 11/F, 14/F, 16/F, 19/F, 22/F, 25/F, 28/F, \
    32/F, 35/F, 38/F, 41/F, 44/F, 47/F, 49/F, 52/F, \
    54/F, 56/F, 58/F, 60/F, 61/F, 62/F, 63/F, 63/F


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

#define SHIFTED3_SINUS(F) \
{ \
	ONE_SHIFTED3_SINUS(F), \
	ONE_SHIFTED3_SINUS(F), \
	ONE_SHIFTED3_SINUS(F), \
	ONE_SHIFTED3_SINUS(F) \
}

#include<joystick.h>
#define STANDARD_JOY 2


#define NUMBER_OF_BALLOONS 10

#define NUMBER_OF_ITEMS 2


#define BEFANA_MIN_X 14
#define BEFANA_MAX_X 120

#define BEFANA_MIN_Y 60
#define BEFANA_MAX_Y 214


#define GIFT_ENERGY 20

#define LEVEL_DISTANCE 160U



#define FEWEST_GIFTS_THRESHOLD 3
#define FEWER_GIFTS_THRESHOLD 7


#define MAX_NO_ITEM_THRESHOLD 2
#define MAX_GIFT_THRESHOLD 4


#define GIFT_ITEM 0
#define SHIELD_ITEM 1
#define FIRE_ITEM 2




#define COLLISION_BOX_X 15
#define COLLISION_BOX_Y 12
#define SHIELD_COLLISION_BOX_X 8
#define SHIELD_COLLISION_BOX_Y 8


#define BULLET_X_SIZE 15


uint8_t forward_thrust;

uint8_t super_weapon_status;
uint8_t weapon_cool_down;

uint8_t exploded_balloons;
uint8_t santa;
uint8_t santa_y;

uint16_t extra_life;

uint8_t shock_level;


uint8_t befana_x;
uint8_t befana_y;


#define EXTRA_LIFE_THRESHOLD 5000

// uint8_t SPRX[BULLET_INDEX];
// uint8_t SPRY[BULLET_INDEX];

#define GAME_OVER_TIME 150
// #define TITLE_SCREEN_TIME 100


#define HIT_bullet_cool_down 30
#define MAX_bullet_cool_down 50

#define GIFT_COOL_DOWN_BONUS 60

#define SHOCK_DURATION 40
#define SHOCK_THRESHOLD 30

#define BULLET ('9'+1)

#define MAX_LEVEL 30
static uint8_t level;

#define MAX_ACTIVE_BULLETS 6


#define GRASS_TILE 36

#define SHOCK_TILE 40

#define BENCHMARK   

static uint8_t armor_level;

// static uint8_t remaining_bullets;
// static uint8_t active_bullets;

// static uint8_t bullet_x[MAX_ACTIVE_BULLETS];
// static uint8_t bullet_y[MAX_ACTIVE_BULLETS];
// static uint8_t bullet_background[MAX_ACTIVE_BULLETS];
// static uint8_t bullet_tile[MAX_ACTIVE_BULLETS];


#define bullet_active bullet_y
// 1 -> 22
// 2 -> 18
// 3 -> 14
// 4 -> 10
static uint8_t bullet_cool_down_value[] = {22, 18, 15}; 
uint8_t bullet_cool_down;

static uint8_t harmful_balloon[NUMBER_OF_BALLOONS];
static uint8_t active_balloon[NUMBER_OF_BALLOONS];
static uint8_t balloons_to_rest;

static uint8_t immortality;

#define DEAD_COOL_DOWN 9

static uint8_t y_balloon[NUMBER_OF_BALLOONS];
// static uint8_t falling_balloon[NUMBER_OF_BALLOONS];
static uint8_t dead_balloon[NUMBER_OF_BALLOONS];

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


const uint8_t shifted3_sinValues1[] = SHIFTED3_SINUS(1);

const uint8_t shifted3_sinValues2[] = SHIFTED3_SINUS(2);

const uint8_t shifted3_sinValues3[] = SHIFTED3_SINUS(3);

const uint8_t shifted3_sinValues4[] = SHIFTED3_SINUS(4);


static uint8_t no_item;
static uint8_t item;


static uint8_t distance;
// static uint16_t level_threshold;


static uint8_t bullet_y_size;

static uint8_t balloon_x;
static uint8_t balloon_y;
static uint8_t bullet_x;
static uint8_t bullet_y;




static uint16_t record;

static uint8_t volume;

#define VOLUME_REG 0xD418 

#define SPRITE_BACKGROUND_PRIORITY 0xD01B

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

// Balloons      0,..,9
// Befana       10
// Item         11
// Bullets      12,13,14,15
// Santa        16,17



#define BALLOON_INDEX 0
#define BEFANA_INDEX  (BALLOON_INDEX+(NUMBER_OF_BALLOONS))
#define ITEM_INDEX    ((BEFANA_INDEX)+1)
#define BULLET_INDEX  ((ITEM_INDEX)+1)
#define SANTA_INDEX   ((BULLET_INDEX)+NUMBER_OF_BULLETS)

#define SMOKE 60


#define TOTAL_SPRITE_COUNT ((NUMBER_OF_BALLOONS)+1+1+(NUMBER_OF_BULLETS)+2)

#define TOTAL_SPRITE_NUMBER 1

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
#define BLUE        0x06
#define YELLOW      0x07
#define ORANGE      0x08
#define BROWN       0x09
#define PINK        0x0A    
#define DARK_GREY   0x0B
#define GREY        0x0C
#define LIGHT_GREEN 0x0D
#define LIGHT_BLUE  0x0E
#define LIGHT_GREY  0x0F

#define NUMBER_OF_BULLETS 3
// static uint8_t bullet_sprite_index[] = {BULLET_INDEX, BULLET_INDEX+1, BULLET_INDEX+2}; 
static uint8_t bullet_status[NUMBER_OF_BULLETS];
// static uint8_t bullet_status[NUMBER_OF_BULLETS];

static uint8_t ARMOR_COLOR[] = {RED, DARK_GREY, GREY, CYAN};

static uint8_t counter;
static uint8_t grass_counter;
static uint8_t lives;
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
// #define SHOCK_TILE ('z'-'a'+1+1)


#if !defined(YEAR_LOW)
	#define YEAR_LOW '4'
#endif

#if !defined(YEAR_HIGH)
	#define YEAR_HIGH '2'
#endif

#define BALLOON ('9' + 1)
#define COMMODORE_LOGO ('9' + 2)

#define BEFANA_ARMOR (- 'A' + 1 + 'Z' + 11)

#define BEFANA_LEFT_TO_RIGHT (- 'A' + 1 + 'Z' + 1)
// #define BEFANA_RIGHT_TO_LEFT (- 'A' + 1 + 'Z' + 11)
#define BEFANA BEFANA_LEFT_TO_RIGHT

#define GIFT (- 'A' + 1 + 'Z' + 15)

#define FIRE ('0' - 2)

#define SHIELD ('0' - 1)

#define SHIELD_ON 0

#define DEAD_BALLOON_1 (BALLOON + 1)
#define DEAD_BALLOON_2 (BALLOON + 10)

#define SMALL_SHIELD (BALLOON + 11)

// #define FIRE ('0' - 1 - 5)

#define SANTA (- 'A' + 1 + 'Z' + 5)

#define REINDEER (- 'A' + 1 + 'Z' + 8)

#define MULTICOLOR_1 0xD025
#define MULTICOLOR_2 0xD026

// Characters for flashing stars
// #define BIG_FLASHING_STAR 0
// #define FLASHING_STAR 30
// #define SMALL_FLASHING_STAR 27
// #define SMALL_FIXED_STAR 28
// #define BIG_FIXED_STAR 29
// #define COMET 33


// {CYAN, PURPLE, GREEN, LIGHT_BLUE, YELLOW, RED, LIGHT_GREY, CYAN};
const static uint8_t BALLOON_COLORS[] = {CYAN, PURPLE, GREEN, LIGHT_BLUE, LIGHT_GREEN, RED, LIGHT_GREY, CYAN};
//{CYAN, PURPLE, GREEN, LIGHT_BLUE, YELLOW, BROWN, LIGHT_GREY, CYAN};

#define NUMBER_OF_COLS 40

#define SLOW_STAR_TILE ('9'+31)

#define FAST_STAR_TILE (SLOW_STAR_TILE+NUMBER_OF_COLS)

#define DISTANCE_OFFSET 11
#define LEVEL_OFFSET (DISTANCE_OFFSET+2)
#define ARMOR_OFFSET ((LEVEL_OFFSET)+14-5)

#define SHOCK_OFFSET (LEVEL_OFFSET+5)
#define HI_OFFSET (NUMBER_OF_COLS-9)
#define INITIAL_HI_OFFSET (NUMBER_OF_COLS/2-2)


#define MIN_SHOCK 4

#define MAX_SHOCK_LEVEL NUMBER_OF_BULLETS
#define SUPER_SHOCK_CHARGE 12



static uint8_t slow_loop = 0;
static uint8_t fast_loop = 20;
static uint8_t i;
static uint8_t j;
static uint8_t slow_star_mask = 1;
static uint8_t fast_star_mask = 1;

// static uint16_t k;

static uint8_t input;

static uint8_t prev_slow_loop;
static uint8_t prev_fast_loop;


static uint8_t alive;

void _short_sound(void)
{
	uint16_t i;
	
	SID.v3.freq  = 0x2000; 
	SID.v3.ad    = 0x00; 
	SID.v3.sr    = 0xC8; 
	SID.flt_ctrl = 0x44; 
	SID.amp      = 0x1F; 
	SID.v3.ctrl  = 0x21;

	for(i=0;i<100;++i) {} 
	SID.amp      = 0x00; 
	SID.v3.ctrl  = 0x08; 
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


void display_score(void)
{
    printd(points,5,5,WHITE);
}


void _XL_PING_SOUND(void)
{ 
    if(!MUSIC_ON)
    {
        SID.flt_freq = 0x3500; 
        _short_sound();
    }
};


void _XL_TICK_SOUND(void) 
{ 
    if(!MUSIC_ON)
    {
        SID.flt_freq = 0x2000; 
        _short_sound();
    }
};

void _XL_TOCK_SOUND(void) 
{
    if(!MUSIC_ON)
    {
        SID.flt_freq = 0x1000; 
        _short_sound();
    }
};


void increase_lives(void)
{
    uint8_t i;
    uint8_t j;
    
    ++lives;
    if(lives>MAX_LIVES)
    {
        lives = MAX_LIVES;
    }
    _XL_PING_SOUND();

    for(i=0;i<50;++i)
    {
        _XL_TOCK_SOUND();

        POKE(COLOR+NUMBER_OF_COLS-2,YELLOW);     
        for(j=0;j<20;++j)
        {
        }
        _XL_TICK_SOUND();

        POKE(COLOR+NUMBER_OF_COLS-2,RED);     
        for(j=0;j<20;++j)
        {
        }
    }
    _XL_PING_SOUND();
}

void display_lives(void)
{
    printd(lives,1,NUMBER_OF_COLS-1,WHITE);
}


void increase_points(uint8_t amount)
{
    points+=amount;
    display_score();
    
    if(points>extra_life)
    {
        increase_lives();
        display_lives();
        extra_life += EXTRA_LIFE_THRESHOLD;
    }
}


// $D018 = 53272
// -----------------
void init_udg(void)
{
    POKE(56576u,3);
	POKE(0xD018,PEEK(0xD018)|8);
	POKE(0xD018,PEEK(0xD018)&(255-4-2));    
	POKE(648,192);
}


void _set_noise(void)
{
	SID.v3.ad    = 0x00; 
	SID.v3.sr    = 0xA8; 
	SID.flt_ctrl = 0x44; 
	SID.amp      = 0x1F; 
	SID.v3.ctrl  = 0x81; 
}


void noise(void)
{
    SID.v3.freq  = 0x1200; 
    SID.flt_freq = 0x2000; 
    _set_noise();
}

void _XL_SHOOT_SOUND(void) 
{ 
	
    if(!MUSIC_ON)
    {
        uint16_t i; 

        SID.v3.freq  = 0x2800; 
        SID.flt_freq = 0x9000; 

        _set_noise();
        
        for(i=0;i<5;++i) {} 
        SID.amp      = 0x00; 
        SID.v3.ctrl  = 0x08; 
    }
};	
	



void _silence(void)
{
    if(!MUSIC_ON)
    {
        // SID.amp      = 0x00; 
        // SID.v3.ctrl  = 0x08;
        POKE(VOLUME_REG,0);
    }
}

void _XL_EXPLOSION_SOUND(void)
{ 

    if(!MUSIC_ON)
    {
        uint16_t i; 
        uint16_t j; 
	
        SID.v3.freq  = 0x1200; 
        SID.flt_freq = 0x2000; 

        noise();

        for(j=0;j<15;++j) 
        { 
            SID.amp      = 0x1F - j; 
            if(!santa)
            {
                for(i=0;i<80;++i) 
                { 
                } 
            }
        } 
        SID.amp      = 0x00; 
        SID.v3.ctrl  = 0x08;
    }
};


void _XL_ZAP_SOUND(void) 
{ 
    if(!MUSIC_ON)
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
    }
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

        }
        else
        {
            POKE(SCREEN+offset+k,0);
        }
        POKE(COLOR+offset+k,col);        
    }
}


void color(uint8_t len, unsigned short offset, uint8_t col)
{
    uint8_t k;
    
    for(k=0;k<len;++k)
    {
        POKE(COLOR+offset+k,col);        
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




void set_fire_speed(uint8_t shock_level)
{
    bullet_cool_down = bullet_cool_down_value[shock_level-1];
}

void display_weapon_status(void)
{

    // else if(weapon_cool_down)
    // {
        // color(4,SHOCK_OFFSET,BLACK);
    // }
    // else
    // {
        color(shock_level,SHOCK_OFFSET,YELLOW);
        color(NUMBER_OF_BULLETS-shock_level, SHOCK_OFFSET+shock_level,BLACK);
        // if((counter&15)&1)
        // {
            // color(shock_level,SHOCK_OFFSET,GREEN);
        // }
        // else
        // {
            // color(shock_level,SHOCK_OFFSET,YELLOW);
        // }
    // }
}


void display_shock(void)
{
        // print("SHOCK",5,SHOCK_OFFSET,RED);
        uint8_t i;
        
        for(i=0;i<NUMBER_OF_BULLETS;++i)
        {
            POKE(SCREEN+SHOCK_OFFSET+i,SHOCK_TILE);
        }
        // POKE(SCREEN+SHOCK_OFFSET+1,SHOCK_TILE);
        // POKE(SCREEN+SHOCK_OFFSET+2,SHOCK_TILE);
        // POKE(SCREEN+SHOCK_OFFSET+3,SHOCK_TILE);

}

void set_background_colors(void)
{
    POKE(0xd020, BLACK);
    POKE(0xd021, BLACK); 
}


void clear_screen(void)
{
    
    uint16_t k;
    
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

    POKE(SPRITE_BACKGROUND_PRIORITY,0x00);    
    
	set_background_colors();

	init_stars();
    
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
    uint8_t i;
    
    SPRX[sprite_index] = 30;
    SPRY[sprite_index] = 100;
    SPRC[sprite_index] = RED;   
    SPRM[sprite_index] = 1;    
    
    for(i=0;i<NUMBER_OF_BULLETS;++i)
    {
        SPRM[BULLET_INDEX+i] = 0;
        SPRC[BULLET_INDEX+i] = YELLOW;
    }

    shock_level = 1;
    set_fire_speed(1);
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


void handle_bullet(uint8_t b)
{

    uint8_t index = BULLET_INDEX+b;
    SPRX[index]+=5;
    if(SPRX[index]>240)
    {
        bullet_status[b]=0;
        SPRY[index]=255;
        SPRF[index]=GFX_START_INDEX+SMOKE;
    }

}


void display_smoke(void)
{
    SPRX[BULLET_INDEX+NUMBER_OF_BULLETS-1]=SPRX[BEFANA_INDEX]-14-8*(counter&1);
    SPRY[BULLET_INDEX+NUMBER_OF_BULLETS-1]=SPRY[BEFANA_INDEX]; // TODO: this could be done in the sprite shape     
    SPRF[BULLET_INDEX+NUMBER_OF_BULLETS-1]=GFX_START_INDEX+SMOKE+(counter&1);
}


void hide_smoke(void)
{
    SPRY[BULLET_INDEX+NUMBER_OF_BULLETS-1]=255;  
}


uint8_t get_available_bullet(void)
{
    uint8_t b;
    for(b=0;b<NUMBER_OF_BULLETS;++b)
    {
        if(!(bullet_status[b]))
        {
            return b;
        }
    }
    return NUMBER_OF_BULLETS;
}


void handle_bullets(void)
{
    uint8_t b;
    
	if(bullet_status[NUMBER_OF_BULLETS-1])
    {
        handle_bullet(NUMBER_OF_BULLETS-1);
    }
    else if(!forward_thrust)
    {
        hide_smoke();
    }
    for(b=0;b<NUMBER_OF_BULLETS-1;++b)
    {
        if(bullet_status[b])
        {
            handle_bullet(b);
        }
    }
}

void handle_befana(void)
{
    uint8_t b;
    uint8_t a_bullet_index;
    
	input = joy_read(STANDARD_JOY);
	
    
	forward_thrust = 0;

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
    if(weapon_cool_down)
    {
        --weapon_cool_down;
    }
    // if(!weapon_cool_down)
    // {
        // display_weapon_status();
    // }
    if(JOY_FIRE(input) && !weapon_cool_down)
    {
            b = get_available_bullet();
            
            
            if(b<NUMBER_OF_BULLETS)
            {
                bullet_status[b] = 1;
                a_bullet_index = BULLET_INDEX+b;
                _XL_SHOOT_SOUND();
                SPRX[a_bullet_index] = SPRX[BEFANA_INDEX]+8;
                SPRY[a_bullet_index] = SPRY[BEFANA_INDEX];
                if(super_weapon_status)
                {
                    SPRF[a_bullet_index]=GFX_START_INDEX+SHIELD_ON;
                    --super_weapon_status;
                    if(!super_weapon_status)
                    {
                        display_weapon_status();
                    }

                }
                else
                {
                    SPRF[a_bullet_index]=GFX_START_INDEX+SMALL_SHIELD;
                }
            }
            weapon_cool_down=bullet_cool_down;
    }

}

void handle_befana_shape(void)
{
    if(armor_level)
    {
        SPRF[BEFANA_INDEX] = GFX_START_INDEX+BEFANA_ARMOR+((counter/4)&3);
    }
    else
    {
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
        
        POKE(SHAPE+1+(GRASS_TILE*8)+i,GRASS_SHAPE[(grass_counter)&7][i]);
    }
    ++grass_counter;
}


void handle_grass(void)
{
	scroll_grass();

}

#define BALLOON_TOP_Y 45
#define BALLOON_BOTTOM_Y 180

void init_sprite_balloons(void)
{
    uint8_t i;
    
    balloons_to_rest = 1;

    for(i=0;i<=0+NUMBER_OF_BALLOONS-1;++i)
    {
		
        SPRF[i]=GFX_START_INDEX + BALLOON;
        SPRC[i]=BALLOON_COLORS[i&7];
        SPRM[i]=0;
		
		SPRX[i]=255-i*28;
    }
    // SPRC[NUMBER_OF_BALLOONS-1]=RED;
}


#define BASE_Y_BALLOON 36
#define DISTANCE_Y_BALLOON 18   
static uint8_t PRECOMPUTED_Y_BALLOON[] = { \
    BASE_Y_BALLOON,      \
    BASE_Y_BALLOON+DISTANCE_Y_BALLOON,   \
    BASE_Y_BALLOON+DISTANCE_Y_BALLOON*2, \
    BASE_Y_BALLOON+DISTANCE_Y_BALLOON*3, \
    BASE_Y_BALLOON+DISTANCE_Y_BALLOON*4, \
    BASE_Y_BALLOON+DISTANCE_Y_BALLOON*5, \
    BASE_Y_BALLOON+DISTANCE_Y_BALLOON*6, \
    BASE_Y_BALLOON+DISTANCE_Y_BALLOON*7, \
    BASE_Y_BALLOON+DISTANCE_Y_BALLOON*8, \
    110 \
    };

// uint8_t compute_y_balloon(uint8_t i)
// {
    // if(i<=8)
    // {
        // return 40+i*22;
    // }
    // else
    // {
        // return 110;
    // }
// }


void init_balloons(void)
{
	// uint8_t i;
	
	init_sprite_balloons();
	
    for(i=0;i<NUMBER_OF_BALLOONS;++i)
	{
		harmful_balloon[i]=1;
        
        #if INITIAL_LEVEL>1
			active_balloon[i]=0;
			y_balloon[i]= 255; // compute_y_balloon(i);        
        #else
        active_balloon[i]=0;
        y_balloon[i]=255;
        // if(i==9)
        // {
            // active_balloon[i]=0;
			// y_balloon[i]= 255; //-i*16;
        // }
		// else if((i&1))
		// {
			// active_balloon[i]=0;
			// y_balloon[i]= 255; // compute_y_balloon(i);
		// }
		// else
		// {		
			// active_balloon[i]=0;
			// y_balloon[i]= 255; //-i*16;
		// }
        #endif
		SPRY[i]=255; //-i*16;
	}	
}


// uint16_t distances[]={DISTANCE_LEV_1,DISTANCE_LEV_2,DISTANCE_LEV_3,DISTANCE_LEV_4, DISTANCE_LEV_5, DISTANCE_LEV_6,DISTANCE_LEV_7,DISTANCE_LEV_8};

// uint8_t distance_counter;

void activate_balloon(uint8_t i)
{
    if(level>13)
    {
        return;
    }
	if(level==13)
	{
		if(i==4)
		{
			active_balloon[4]=1;
			y_balloon[4]=PRECOMPUTED_Y_BALLOON[4];		
		}
	}	
	else if(level==11)
	{
		if(i==2)
		{
			active_balloon[2]=1;
			y_balloon[2]=PRECOMPUTED_Y_BALLOON[2];
		}
	}
	else if(level==9)
	{
		if(i==6)
		{
			active_balloon[6]=1;
			y_balloon[6]=PRECOMPUTED_Y_BALLOON[6];
		}			
	}
	else if(level==7)
	{
		if(!i)
		{
			active_balloon[0]=1;
			y_balloon[0]=PRECOMPUTED_Y_BALLOON[0];
		}
	}
    else if(level==5)
        {
        if(i==9)
        {
            active_balloon[9]=1;
            y_balloon[9]=PRECOMPUTED_Y_BALLOON[9];
        }
    }
	else if(level==3)
	{
		if(i==8)
		{
			active_balloon[8]=1;
			y_balloon[8]=PRECOMPUTED_Y_BALLOON[8];
		}
	}
    else
    {   
        if((i&1)&&(i<8))
        {
            active_balloon[i]=1;
            y_balloon[i] =PRECOMPUTED_Y_BALLOON[i];
        }
    }   
}


void display_level(void)
{
    printd(level,2,LEVEL_OFFSET,WHITE);
}


void display_armor(void)
{
    // uint8_t armor_color;
    // printd(armor_level,1,ARMOR_OFFSET,WHITE);
    if(!armor_level)
    {
        color(5,ARMOR_OFFSET,BLACK);
    }
    // else if(armor_level==1)
    // {
        // armor_color = DARK_GREY;
    // }
    // else if(armor_level==2)
    // {
        // armor_color = GREY;
    // }
    // else
    // {
        // armor_color = LIGHT_GREY;
    // }
    else
    {
        color(5,ARMOR_OFFSET,ARMOR_COLOR[armor_level]);
    }
}



void display_hi(uint8_t position)
{
    print("HI",2,position-2,GREEN);
	printd(record,5,position,WHITE);
}

#define NEW_LEVEL_OFFSET (440+15)


void display_new_level(void)
{
    print("LEVEL",5,NEW_LEVEL_OFFSET,WHITE);
    printd(level,2,NEW_LEVEL_OFFSET+6,WHITE);
}

void erase_new_level(void)
{
    print("         ",8,NEW_LEVEL_OFFSET,WHITE);
}




#define LOST_LIFE_IMMORTALITY 20
#define EXTRA_ARMOR_IMMORTALITY 30
#define BALLOON_ARMOR_DAMAGE 1


void decrease_armor(void)
{
	if(armor_level)
	{
		--armor_level;
	}
    SPRC[BEFANA_INDEX]=ARMOR_COLOR[armor_level];

}



uint8_t one_sprite_collision(uint8_t i)
{
    uint8_t x;
	uint8_t y;
    
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

void check_level_trigger()
{
    // printd(level_threshold,5,160,WHITE);
    // printd(((uint16_t) level),5,200,WHITE);

    
    if((distance>=LEVEL_DISTANCE))
    {

        ++level;
        exploded_balloons = 0;
        // shock_level = 1;
        // set_fire_speed(1);
        // display_weapon_status();
        // armor_level = 0;
        // display_armor();
        // SPRC[BEFANA_INDEX]=RED;
        SPRC[BEFANA_INDEX]=ARMOR_COLOR[armor_level];


        
        if(level<=MAX_LEVEL)
        {
            display_level();
            // level_threshold = ((uint16_t) level)*(uint16_t) LEVEL_DISTANCE;
            distance = 0;
            display_new_level();
        }
    }
    else if(distance==30)
    {    
        erase_new_level();
        balloons_to_rest=0;
       
    }
    
    if(distance==LEVEL_DISTANCE-24)
    {

        balloons_to_rest = 1;
    
    }
    // else if(level==10)
    // {
        // print("DEBUG",5,280,WHITE);
        
    // }

	// activate_balloon(i);
	
}

#define handle_dead_balloon_shape(i) \
    do \
    { \
        if(counter&1) \
        { \
            SPRF[i] = GFX_START_INDEX + DEAD_BALLOON_1; \
        } \
        else \
        { \
            SPRF[i] = GFX_START_INDEX + DEAD_BALLOON_2; \
        } \
    } while(0)


#define compute_y_balloon(i) \
    do \
        if(i<8) \
        { \
            y_balloon[i]=48+i*16+(rand()&0xF); \
        } \
        else \
        { \
            if(i==8) \
            { \
                y_balloon[8]=SPRY[BEFANA_INDEX]; \
            } \
            else \
            { \
                y_balloon[9]=80+(counter&7)*8; \
            } \
        } \
    while(0)





void decrease_weapon_cool_down(void)
{
    if(weapon_cool_down>GIFT_COOL_DOWN_BONUS+1)
    {
        weapon_cool_down-=GIFT_COOL_DOWN_BONUS;
    }
}



void increase_armor(void)
{
    if(armor_level<MAX_ARMOR)
    {

        ++armor_level;
        SPRC[BEFANA_INDEX]=ARMOR_COLOR[armor_level];

    }
    else
    {
        immortality = EXTRA_ARMOR_IMMORTALITY;
    }
}




uint8_t handle_item_collision(void)
{
        if(one_sprite_collision(ITEM_INDEX))
        {
			if(item_type==GIFT_ITEM)
			{
				display_lives();
				increase_points(GIFT_POINTS);
                decrease_weapon_cool_down();

                if(shock_level==MAX_SHOCK_LEVEL)
                {
                    super_weapon_status = SUPER_SHOCK_CHARGE;
                }
                else
                {
                    ++shock_level;
                    set_fire_speed(shock_level);
                }
                display_weapon_status();

			}
			else if(item_type==SHIELD_ITEM)
			{
				increase_points(ARMOR_POINTS);
				increase_armor();
                display_armor();
			}
            display_score();

			_XL_ZAP_SOUND();
			SPRY[ITEM_INDEX]=255;
			return 1;
        }
        return 0;
}


#define BALLON_THRESHOLD_X 8

void _handle_balloons(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_BALLOONS;++i)
    {
		// TODO: This should be optimized because
		--SPRX[i];
		if(active_balloon[i])
		{
            if(dead_balloon[i])
            {
                handle_dead_balloon_shape(i);
            }
			if(SPRX[i]<BALLON_THRESHOLD_X)
			{
                SPRX[i]=184;

                if(balloons_to_rest)
                {
                    y_balloon[i] = 255;
                    SPRY[i] = 255;
                    return;
                }
                else
                {
                    SPRC[i]=BALLOON_COLORS[rand()&7];

                    compute_y_balloon(i);
                    
                }
              

			}
            if(y_balloon[i]<255)
            {
                if(level<6)
                {
                    if((i==1))
                    {
                        SPRY[i]=y_balloon[i]+sinValues2[counter]; // OK
                    }
                    else if(i==3)
                    {
                        SPRY[i]=y_balloon[i]+shifted_sinValues2[counter]; // OK
                    }
                    else if(i==5)
                    {
                        SPRY[i]=y_balloon[i]+shifted2_sinValues2[counter]; // OK
                    }
                    else
                    {
                        SPRY[i]=y_balloon[i]+shifted3_sinValues2[counter]; // OK
                    }
                }
                else
                {
                    
                    if(i==9)
                    {
                        SPRY[i]=y_balloon[i]+sinValues1[counter]; // OK
                    }
                    else if(i==8)
                    {
                        SPRY[i]=y_balloon[i]+sinValues4[counter]; // OK
                    }
                    else if(!(i&7))
                    {
                        SPRY[i]=y_balloon[i]+sinValues3[counter]; // OK
                    }
                    else if((i&7)==1)
                    {
                        if((level&1))
                        {
                            SPRY[i]=y_balloon[i]+shifted_sinValues3[counter]; // OK
                        }
                        else
                        {
                            SPRY[i]=y_balloon[i]+shifted_sinValues4[counter]; // OK
                        }
                    }
                    else if((i&7)==2)
                    {
                        
                        if((level&1))
                        {
                            SPRY[i]=y_balloon[i]+shifted2_sinValues2[counter]; // OK
                        }
                        else
                        {
                            SPRY[i]=y_balloon[i]+shifted2_sinValues3[counter]; // OK
                        }
                    }   
                    else if((i&7)==3)
                    {
                        SPRY[i]=y_balloon[i]+shifted3_sinValues4[counter]; // OK
                    }
                    else if((i&7)==4)
                    {
                        SPRY[i]=y_balloon[i]+shifted2_sinValues2[counter]; // OK
                    }
                    else if((i&7)==5)
                    {
                        SPRY[i]=y_balloon[i] +shifted3_sinValues3[counter]; // OK
                    }
                    else if((i&7)==6)
                    {
                        SPRY[i]=y_balloon[i] +shifted3_sinValues2[counter]; // OK
                    }
                    else
                    {
                        SPRY[i]=y_balloon[i] +shifted2_sinValues4[counter]; // OK
                    }
                }
            }
            else
            {
                SPRY[i] = 255;
            }
		}
        else if(SPRX[i]<BALLON_THRESHOLD_X)
		{
			SPRX[i]=184;
            harmful_balloon[i] = 1;
            activate_balloon(i);
		}
    }
}

void move_balloons(void)
{
	uint8_t i;
	
    for(i=0;i<=0+NUMBER_OF_BALLOONS-1;++i)
    {
        --SPRX[i];
    }
}


void handle_balloons(void)
{
	_handle_balloons();
	if((level==2)||(level==4)||(level==6)||(level==8)||(level==10))
	{
		if(counter&1)
		{
			_handle_balloons();
		}
	}
	// if((level>=12) && (!(level&1)) && (counter&3))
    if(level>=12)
	{
		_handle_balloons();
	}
    if(level>=18)
    {
		_handle_balloons();        
    }
}


void handle_stars(void)
{
	_handle_stars();

	if(forward_thrust)
	{
        if(!(counter&1))
        {
            move_balloons();
            handle_item_collision();
        }
        if(!bullet_status[NUMBER_OF_BULLETS-1])
        {
            display_smoke();
        }
        _handle_stars();
        scroll_grass();
	}
}


// const uint8_t item_colors[NUMBER_OF_ITEMS] = {PURPLE, BLUE, GREEN, CYAN};
const uint8_t GIFT_COLORS[] = {PURPLE, GREEN, CYAN, BLUE};

void init_items(void)
{
    // uint8_t i;
    
    // for(i=0;i<NUMBER_OF_ITEMS;++i)
    // {
        // SPRF[ITEM_INDEX]=GFX_START_INDEX+GIFT;
        // SPRC[ITEM_INDEX]=CYAN; // TODO: should be random
        SPRY[ITEM_INDEX]=255;
        SPRM[ITEM_INDEX]=1;
        // SPRX[ITEM_INDEX]=80+i*64;
        SPRX[ITEM_INDEX]=255;

    // }
}

void clear_items(void)
{
    // uint8_t i;
    
    // for(i=0;i<NUMBER_OF_ITEMS;++i)
    // {
        // SPRF[ITEM_INDEX+i]=GFX_START_INDEX+GIFT;
        // SPRC[ITEM_INDEX+i]=BALLOON_COLORS[i];
        SPRY[ITEM_INDEX]=255;
        // SPRM[ITEM_INDEX+i]=1;
        // SPRX[ITEM_INDEX+i]=i*64;
    // }
}


void spawn_item(void)
{
	uint8_t rnd; 
	
	rnd = rand()&3;
	
	SPRY[ITEM_INDEX]=100+(rand()&0x3F);
    SPRC[ITEM_INDEX]=GIFT_COLORS[rnd];
	
	if(!rnd)
	{
		item_type=SHIELD_ITEM;
		SPRF[ITEM_INDEX]=GFX_START_INDEX+SHIELD;
		// TODO: update counters?
	}
	else 
	{
		item_type=GIFT_ITEM;
		SPRF[ITEM_INDEX]=GFX_START_INDEX+GIFT;
		no_item=0;
		++item;
	}		
}


void unspawn_item(void)
{
	SPRY[ITEM_INDEX]=255;
	++no_item;
	item=0;	
}



void handle_items(void)
{
    // uint8_t i;
    
    // printd(distance,3,40,WHITE);//(uint16_t val, uint8_t length, unsigned short offset, uint8_t color)
    // for(i=0;i<NUMBER_OF_ITEMS;++i)
    // {
        --SPRX[ITEM_INDEX];
		
		
        if(!(counter&3))
        {
			if(item_type==GIFT_ITEM)
			{
				// TODO: Fix non-rotating items
				SPRF[ITEM_INDEX]=GFX_START_INDEX+GIFT+((counter/4)%3);
			}
			else if(item_type==SHIELD_ITEM)
			{// TODO: To optimize
				SPRF[ITEM_INDEX]=GFX_START_INDEX+SHIELD-((counter/4)&3);
			}
        }
        
        // Re-position item
        if(!SPRX[ITEM_INDEX])
        {

            if(level<=FEWEST_GIFTS_THRESHOLD)
            {
                // TODO: Simplify this
                if(distance>LEVEL_DISTANCE/2 && (item<=MAX_GIFT_THRESHOLD) && (!(rand()&3) || (no_item>MAX_NO_ITEM_THRESHOLD)))
                {
					spawn_item();
                }
                else
                {
					unspawn_item();
                }
            }
            else if(level<=FEWER_GIFTS_THRESHOLD)
            {
                // TODO: Simplify this
                if(distance>LEVEL_DISTANCE/3 && (item<=MAX_GIFT_THRESHOLD) && ((rand()&1) || (no_item>MAX_NO_ITEM_THRESHOLD)))
                {
					spawn_item();
                }
                else
                {
					unspawn_item();
                }
                
            }
            else // Very high and difficult levels 8-20
            {
                if(distance>LEVEL_DISTANCE/4 && (item<=2*MAX_GIFT_THRESHOLD) && ((rand()&3)|| (no_item>MAX_NO_ITEM_THRESHOLD)))
                {
					spawn_item();
                }
                else
                {
					unspawn_item();
                }
            }
            
        }
    
}


uint8_t bullet_collision(void)
{
    
    if(balloon_x>=bullet_x)
    {
        if((balloon_x-bullet_x)>BULLET_X_SIZE)
        {
            return 0;
        }
    }
    else // x < SPRX[BULLET_INDEX]
    {
        if((bullet_x-balloon_x)>BULLET_X_SIZE)
        {
            return 0;
        }
    }
    
    // y = SPRY[balloon_index];
    if(balloon_y>=bullet_y)
    {
        if((balloon_y-bullet_y)>bullet_y_size) // TODO: avoid +2
        {
            return 0;
        }
    }
    else // y < SPRY[BULLET_INDEX]
    {
        if((bullet_y-balloon_y)>bullet_y_size)
        {
            return 0;
        }
    }
    return 1;   
}

void ballon_hit(uint8_t i)
{
    uint8_t color ;
    // uint8_t j;
    
    color = SPRC[i];
    harmful_balloon[i]=0;
    
    noise();

    // for(j=0;j<120;++j)
    // {
    // }
    SPRC[i]=YELLOW;
    
    while(MULTIPLEX_DONE)
    {
        MULTIPLEX_DONE=0;
        SPRUPDATEFLAG=1;
    }


    // for(j=0;j<100;++j)
    // {
    // }
    
    SPRC[i]=color;
    
    MULTIPLEX_DONE=0;
    while(MULTIPLEX_DONE)
    {
        MULTIPLEX_DONE=0;
        SPRUPDATEFLAG=1;
    }
}


uint8_t befana_sprite_collision(void)
{
	befana_x = SPRX[BEFANA_INDEX];
	befana_y = SPRY[BEFANA_INDEX];
    
    for(i=0;i<=NUMBER_OF_BALLOONS-1;++i)
    {
		if(harmful_balloon[i])
		{
			if(one_sprite_collision(i))
			{				
				return i;
			}
		}
		
	}
	return 255;
}


#define BIG_SHOCK_SIZE 20
#define SMALL_SHOCK_SIZE 9



uint8_t bullet_balloon_collision(uint8_t b)
{
    // uint8_t size;
    uint8_t balloon_index;

    if(super_weapon_status)
    {
        bullet_y_size = BIG_SHOCK_SIZE;
    }
    else
    {
        bullet_y_size = SMALL_SHOCK_SIZE;
    }

    for(balloon_index=0;balloon_index<=NUMBER_OF_BALLOONS-1;++balloon_index)
    {
        balloon_x = SPRX[BALLOON_INDEX+balloon_index];
        balloon_y = SPRY[BALLOON_INDEX+balloon_index];

        if(harmful_balloon[balloon_index])
        {
            bullet_x = SPRX[BULLET_INDEX+b];
            bullet_y = SPRY[BULLET_INDEX+b];

            if(bullet_collision())
            {
                // ballon_hit(balloon_index);
                return balloon_index;
            }
        }
        
    }
    return 255;
}


void handle_immortality(void)
{
	if(immortality)
	{
		--immortality;
	}
}


void handle_dead_balloons(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_BALLOONS; ++i)
    {
        if(dead_balloon[i]>2)
        {
            --dead_balloon[i];
        }
        else if(dead_balloon[i]==2) // TODO: Really needed ?
        {
            y_balloon[i] = 255;
            --dead_balloon[i];
            _silence();
            
        }
        else if(dead_balloon[i])
        {
            dead_balloon[i] = 0;
            harmful_balloon[i] = 1;
            SPRF[i]=GFX_START_INDEX + BALLOON;
        }
    }
}


void handle_balloon_collision(void)
{
    uint8_t balloon_hit_by_befana;
    uint8_t b;
    
    for(b=0;b<NUMBER_OF_BULLETS;++b)
    {
        if(bullet_status[b])
        {
            uint8_t ballon_hit_by_bullet = bullet_balloon_collision(b);
            
            if(ballon_hit_by_bullet<255)
            {
                bullet_status[b] = 0;
                SPRY[BULLET_INDEX+b]=255;
                
                ballon_hit(ballon_hit_by_bullet);
                // if(!super_weapon_status)
                // {
                    // SPRY[BULLET_INDEX]=255;
                // }
                increase_points(BALLOON_POINTS);
                display_score();
                if(!santa)
                {
                    ++exploded_balloons;
                }
                
                dead_balloon[ballon_hit_by_bullet] = DEAD_COOL_DOWN;
            }
        }
    }
    if(!immortality)
    {
        balloon_hit_by_befana = befana_sprite_collision();
        if(balloon_hit_by_befana<255)
        {
            ballon_hit(balloon_hit_by_befana);
            // immortality=LOST_LIFE_IMMORTALITY;

            // falling_balloon[balloon_hit_by_befana] = 1;
            dead_balloon[balloon_hit_by_befana] = DEAD_COOL_DOWN;

            if(armor_level)
            {
                decrease_armor();
                display_armor();
            }
            else
            {
                alive = 0;
                SPRC[BEFANA_INDEX]=PURPLE;

                _XL_EXPLOSION_SOUND();
                // weapon_cool_down=HIT_bullet_cool_down;

            }
            super_weapon_status = 0;
            shock_level = 1;
            set_fire_speed(1);
            display_weapon_status();
            _silence();
        }
    }
}


static char MESSAGE[] = "BEFANAVSBALLOONS";

void init_letters()
{
    POKE(MULTICOLOR_1,PINK);
    POKE(MULTICOLOR_2,LIGHT_GREY);
        
    for(i=1;i<=16;++i)
    {
        SPRF[i]=GFX_START_INDEX + MESSAGE[i-1] - 'A' + 1;
        SPRM[i]=1;
    }

    for(i=1;i<=6;++i)
    {
        SPRX[i]=45+18*(i-1); //+18*i;
        SPRY[i]=70+i+sinValues8[counter];//+2*i;
        SPRC[i]=GREEN;
    }

    for(i=7;i<=8;++i)
    {
        SPRX[i]=75+18*(i-7); //+18*i;
        SPRY[i]=105+i+sinValues8[counter];;//+2*i;
        SPRC[i]=RED;
    }
    
    for(i=1;i<=8;++i)
    {
        SPRX[i+8]=10+18*i; //+18*i;
        if(i&1)
        {
            SPRY[i+8]=153+sinValues8[counter];;//+2*i;
        }
        else
        {
            SPRY[i+8]=153+8+sinValues8[counter];;//+2*i;
        }
        SPRC[i+8]=CYAN;
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


void print_press_fire(void)
{
    print("  PRESS FIRE TO START  ",23,1000-40+8,WHITE);
}


void title_screen(void)
{
    distance = 0;
    
    
    do
    {
        SPRY[0]=128;
        
        SPRY[17]=44+sinValues4[(counter/4)];
        SPRM[17]=0;
        SPRC[17]=CYAN;
        SPRF[17]=GFX_START_INDEX + BALLOON;
        
        SPRY[18]=216;
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
            if(distance<=128)
            {
                ++distance;
            }
            
            if(counter<64)
            {
                print("   AVOID THE BALLOONS  ",23,1000-40+8,CYAN);
            }
            else if(counter<128)
            {
                print_press_fire();
            }
            else if(counter<192)
            {
                print("  FETCH THE GIFT BOXES ",23,1000-40+8,GREEN);
            }
            else
            {
                print("PRESS FIRE TO USE SHOCK",23,1000-40+8,YELLOW);
            }
            
            #if defined(BETA_VERSION)
                
             print("BETA VERSION", 12, 1000-12-40,WHITE);
                // print("BETA     0.2", 12, 1000-12-40,WHITE);

            #endif
            
            MULTIPLEX_DONE = 0;
            SPRUPDATEFLAG = 1;
        }                
    } while(!JOY_FIRE(joy_read(STANDARD_JOY)) || (distance<64)); 
}



uint8_t santa_x;
uint8_t santa_bonus;


void handle_santa_trigger(void)
{
    if(exploded_balloons>=SANTA_THRESHOLD)
    {
        exploded_balloons = 0;
        santa = 1;
        santa_x = 10;
        santa_bonus = SANTA_CHARGE;
        santa_y = 24+40+(160*(rand()&1));
        music_switch(1);
        SPRY[SANTA_INDEX] = santa_y; 
        SPRY[SANTA_INDEX+1] = santa_y;
    }
}

#define SANTA_RANGE 13

void handle_santa(void)
{
    if(santa && (!(counter&3)))
    {
        uint8_t befana_x;
        uint8_t befana_y; 
        
        SPRF[SANTA_INDEX] = GFX_START_INDEX+BEFANA+4+((counter/4)%3);
        // SPRY[SANTA_INDEX] = santa_y; // TODO
        SPRX[SANTA_INDEX] = santa_x;

        
        SPRF[SANTA_INDEX+1] = GFX_START_INDEX+BEFANA+7+((counter/4)%3);
        // SPRY[SANTA_INDEX+1] = santa_y;
        SPRX[SANTA_INDEX+1] = santa_x+12;
        

        
        ++santa_x;
        if(santa_x>=80 )
        {
            ++santa_x;
        }
        
        
        if(santa_x>=184-10)
        {
            santa = 0;
            music_switch(0);
            SPRC[BEFANA_INDEX]=ARMOR_COLOR[armor_level];
        }
        
        befana_x = SPRX[BEFANA_INDEX];
        befana_y = SPRY[BEFANA_INDEX];
        
        if(santa_bonus && befana_y<=santa_y+SANTA_RANGE && befana_y>=santa_y-SANTA_RANGE && befana_x>=santa_x-SANTA_RANGE && befana_x<=santa_x+SANTA_RANGE)
        {
            _XL_PING_SOUND();
            if(counter&7)
            {
                SPRC[BEFANA_INDEX] = PINK;
            }
            else
            {
                SPRC[BEFANA_INDEX] = ORANGE;
            }
            --santa_bonus;
            increase_points(SANTA_POINTS);
            display_score();
        }
    }
    

}


void init_santa(void)
{
    SPRM[SANTA_INDEX] = 1; // TODO
    SPRC[SANTA_INDEX] = RED;
    SPRM[SANTA_INDEX+1] = 1; // TODO
    SPRC[SANTA_INDEX+1] = WHITE;
}

/******************/
int main()
{       

    INITSPRITES();
    INITRASTER();	

	joy_install((void *)joy_static_stddrv);
	
	NUMSPRITES = _NUMBER_OF_SPRITES_;

	record = 0;
    
    while(1)
    {
        uint8_t i;
        
        hide_sprites();
        
        clear_items();
		clear_stars();

        init_background();

        NUMSPRITES = _NUMBER_OF_SPRITES_;

        MULTIPLEX_DONE=1;
        
        init_letters();
        init_player(0);
        
        SPRY[17]=120;
        SPRF[17]=GFX_START_INDEX+BALLOON;
        SPRF[0]=GFX_START_INDEX+BEFANA;
		while(MULTIPLEX_DONE)
		{

			MULTIPLEX_DONE = 0;
			SPRUPDATEFLAG = 1;
            
		}
		clear_stars();

		distance=0;
        // level_threshold = (INITIAL_LEVEL)*LEVEL_DISTANCE;
        slow_loop=0;
        fast_loop=0;
        
		immortality=0;
		
        points = 0;
		level = INITIAL_LEVEL;
		
        lives = INITIAL_LIVES;
        counter = 0;
        grass_counter = 0;
		no_item = 0;
		item = 0;
		
        music_switch(1);
        
        display_hi(INITIAL_HI_OFFSET);
		SPRX[17]=150;
        SPRX[18]=166;
		
		armor_level = 0;
        super_weapon_status = 0;


        title_screen();
        
        distance = 0;
        
        init_grass();

        hide_sprites();

        draw_the_moon();
        
        print("     ",5,INITIAL_HI_OFFSET,WHITE);


        SPRF[BULLET_INDEX+NUMBER_OF_BULLETS-1]=GFX_START_INDEX+SMOKE;
        SPRM[BULLET_INDEX+NUMBER_OF_BULLETS-1]=0;
        SPRC[BULLET_INDEX+NUMBER_OF_BULLETS-1]=YELLOW;
        
		POKE(SCREEN+NUMBER_OF_COLS-2,ENERGY_ICON);
		POKE(COLOR+NUMBER_OF_COLS-2,RED);        
        display_lives();
		print("SCORE",5,0,CYAN);
		

        POKE(SCREEN+DISTANCE_OFFSET+5,0);
        POKE(SCREEN+DISTANCE_OFFSET+6,0);
		
		print("LV",2,LEVEL_OFFSET-2,CYAN);
		display_level();

        display_shock();
        
        print("ARMOR",5,ARMOR_OFFSET,GREY);
        display_armor();
        

        display_score();
		
		display_hi(HI_OFFSET);        
        
        init_balloons();
		init_items();
        init_player(BEFANA_INDEX);

        santa = 0;
        santa_bonus = 0;
        SPRY[17] = 255;
        SPRY[18] = 255;
		
        music_switch(0);
        display_new_level();
        exploded_balloons = 0;
        #if INITIAL_LEVEL>1
        exploded_balloons = 0 + level/4;
        #endif
        
        extra_life = EXTRA_LIFE_THRESHOLD;
        shock_level = 0;

        for(i=0;i<NUMBER_OF_BULLETS;++i)
        {
            bullet_status[i]=0;
        }
        
        shock_level = 1;
        set_fire_speed(1);
        display_weapon_status();
        
        // 10 balloons
        //  1 Befana
        //  2 Santa
        //  1 gift
        //  3 bullets (including smoke)
        NUMSPRITES = TOTAL_SPRITE_COUNT;

        alive = 1;
        
        init_santa();
        
        while(lives && (level<=MAX_LEVEL)) 
        {

                
            // printd(exploded_balloons,3,40,WHITE);
            if (MULTIPLEX_DONE) {
                #if defined(BENCHMARK)
                POKE(0xd020, WHITE);
                #endif
                
                // printd(weapon_cool_down,3,40,WHITE);
                handle_stars();
            
                if(alive)
                {
                    handle_befana();
                }
                handle_befana_shape();
                handle_bullets();
                
                handle_balloons();
                handle_dead_balloons();
                                
                handle_santa_trigger();
                handle_santa();
                
                handle_grass();

                handle_items();

                
                if(!(counter&3))
                {
                    handle_balloon_collision();
                    handle_item_collision();

                    if(immortality)
                    {
                        if(!(counter&7))    
                        {
                            SPRC[BEFANA_INDEX] = YELLOW;
                        }
                        else
                        {
                            SPRC[BEFANA_INDEX] = RED;
                        }
                    }
                }
                ++counter;
                
				if(!(counter&7))
				{
                    handle_item_collision();
                    if(super_weapon_status && !(counter&15))
                    {
                        color(NUMBER_OF_BULLETS,SHOCK_OFFSET,PINK);
                    }
                    else
                    {
                        display_weapon_status();
                    }
                    // display_weapon_status();

					handle_immortality();
                    ++distance;
                    
                    
                    check_level_trigger();
                    
				}

                if(!alive)
                {
                    ++SPRY[BEFANA_INDEX];
                    if(SPRY[BEFANA_INDEX]>110)
                    {
                        ++SPRY[BEFANA_INDEX];
                    }
                    if(SPRY[BEFANA_INDEX]>180)
                    {
                        ++SPRY[BEFANA_INDEX];
                    }
                    if(SPRY[BEFANA_INDEX]>230)
                    {
                        --lives;
                        display_lives();
                        _silence();
                        immortality = LOST_LIFE_IMMORTALITY;
                        alive = 1;
                        // display_weapon_status();
                        SPRX[BEFANA_INDEX] = 30;
                        SPRY[BEFANA_INDEX] = 130;
                    }
                }
		
                MULTIPLEX_DONE = 0;
                SPRUPDATEFLAG = 1;
                
                #if defined(BENCHMARK)
                POKE(0xD020, BLACK);
                #endif
            }

        }
        
        POKE(SPRITE_BACKGROUND_PRIORITY,0xFF);    
        
		if(points>record)
		{
			record=points;
		}
        distance=0;

        level=12;

        music_switch(1);

        if(!lives)
        {
            clear_items();
        }
        SPRC[BEFANA_INDEX] = RED;
        erase_new_level();
        
        do
        { 
            while(MULTIPLEX_DONE)
            {
                if(distance<=GAME_OVER_TIME)
                {
                    ++distance;
                }
                ++counter;

                if((counter&31)<16)
                {
                    
                    print("GAME OVER",9,495,RED);

                    if(lives)
                    {
                        print("JOURNEY COMPLETED",17,494-80-3,YELLOW);
                    }
                }
                else
                {
                    print("GAME OVER",9,495,YELLOW);

                    if(lives)
                    {
                        print("JOURNEY COMPLETED",17,494-80-3,CYAN);
                    }
                }

                    handle_balloons();
                    if(lives)
                    {
                        uint8_t i; 
                        
                        handle_balloons();
                        handle_balloons();
                        SPRF[BEFANA_INDEX] = GFX_START_INDEX+BEFANA+((counter/4)&3);
                        for(i=0;i<NUMBER_OF_ITEMS;++i)
                        {
                            SPRF[ITEM_INDEX]=GFX_START_INDEX+GIFT+((counter/4)%3);
                            --SPRX[ITEM_INDEX];
                        }
                    }
                    SPRUPDATEFLAG=1;
                    MULTIPLEX_DONE=0;
            }
            if(distance==GAME_OVER_TIME)
            {
                print_press_fire();
            }
        } while(!JOY_FIRE(joy_read(STANDARD_JOY)) || (distance < GAME_OVER_TIME));
        

		hide_sprites();
        forward_thrust = 0;
    }
    return 0;
}

