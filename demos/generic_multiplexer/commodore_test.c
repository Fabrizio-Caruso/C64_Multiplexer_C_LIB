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


#define SINUS(F) \
{ \
    32/F, 35/F, 38/F, 41/F, 44/F, 47/F, 49/F, 52/F, \
    54/F, 56/F, 58/F, 60/F, 61/F, 62/F, 63/F, 63/F, \
    64/F, 63/F, 63/F, 62/F, 61/F, 60/F, 58/F, 56/F, \
    54/F, 52/F, 49/F, 47/F, 44/F, 41/F, 38/F, 35/F, \
    32/F, 28/F, 25/F, 22/F, 19/F, 16/F, 14/F, 11/F, \
    9/F, 7/F, 5/F, 3/F, 2/F, 1/F, 0/F, 0/F, \
    0/F, 0/F, 0/F, 1/F, 2/F, 3/F, 5/F, 7/F, \
    9/F, 11/F, 14/F, 16/F, 19/F, 22/F, 25/F, 28/F, \
    32/F, 35/F, 38/F, 41/F, 44/F, 47/F, 49/F, 52/F, \
    54/F, 56/F, 58/F, 60/F, 61/F, 62/F, 63/F, 63/F, \
    64/F, 63/F, 63/F, 62/F, 61/F, 60/F, 58/F, 56/F, \
    54/F, 52/F, 49/F, 47/F, 44/F, 41/F, 38/F, 35/F, \
    32/F, 28/F, 25/F, 22/F, 19/F, 16/F, 14/F, 11/F, \
    9/F, 7/F, 5/F, 3/F, 2/F, 1/F, 0/F, 0/F, \
    0/F, 0/F, 0/F, 1/F, 2/F, 3/F, 5/F, 7/F, \
    9/F, 11/F, 14/F, 16/F, 19/F, 22/F, 25/F, 28/F, \
    32/F, 35/F, 38/F, 41/F, 44/F, 47/F, 49/F, 52/F, \
    54/F, 56/F, 58/F, 60/F, 61/F, 62/F, 63/F, 63/F, \
    64/F, 63/F, 63/F, 62/F, 61/F, 60/F, 58/F, 56/F, \
    54/F, 52/F, 49/F, 47/F, 44/F, 41/F, 38/F, 35/F, \
    32/F, 28/F, 25/F, 22/F, 19/F, 16/F, 14/F, 11/F, \
    9/F, 7/F, 5/F, 3/F, 2/F, 1/F, 0/F, 0/F, \
    0/F, 0/F, 0/F, 1/F, 2/F, 3/F, 5/F, 7/F, \
    9/F, 11/F, 14/F, 16/F, 19/F, 22/F, 25/F, 28/F, \
    32/F, 35/F, 38/F, 41/F, 44/F, 47/F, 49/F, 52/F, \
    54/F, 56/F, 58/F, 60/F, 61/F, 62/F, 63/F, 63/F, \
    64/F, 63/F, 63/F, 62/F, 61/F, 60/F, 58/F, 56/F, \
    54/F, 52/F, 49/F, 47/F, 44/F, 41/F, 38/F, 35/F, \
    32/F, 28/F, 25/F, 22/F, 19/F, 16/F, 14/F, 11/F, \
    9/F, 7/F, 5/F, 3/F, 2/F, 1/F, 0/F, 0/F, \
    0/F, 0/F, 0/F, 1/F, 2/F, 3/F, 5/F, 7/F, \
    9/F, 11/F, 14/F, 16/F, 19/F, 22/F, 25/F, 28/F, \
}

/******************/
// Pre-calculated sinus values
const char yValues1[] = SINUS(1);

const char yValues2[] = SINUS(2);

const char yValues4[] = SINUS(4);

// const char yValues8[] = SINUS(8);

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
const char MULTIPLEX_MESSAGE[9]    = "MULTIPLEX";

#define NUMBER_OF_COLORS 14
const unsigned char COLORS[NUMBER_OF_COLORS] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15};

// #define NUMBER_OF_COLORS 13
// const unsigned char COLORS[NUMBER_OF_COLORS] = {2,3,4,5,7,8,9,10,11,12,13,14,15};


const char AUTHOR[15] = "FABRIZIO CARUSO";
const char TEXT1[15] =  "LOGIC CODE IN C";
const char TEXT2[28] =  "MODIFIED CADAVER MULTIPLEXER ";
const char TEXT3[29] =  "SID TUNE AND FONTS RIPPED OFF";
const char TEXT4[17] =  "FAST MODE ENABLED";

const char WRITTEN[15] = "WRITTEN IN C BY";

// const char YEAR[14] =   "HAPPY NEW YEAR";

const unsigned char STAR_3[8] = {0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00};
const unsigned char STAR_2[8] = {0x00,0x00,0x00,0x10,0x38,0x10,0x00,0x00};
const unsigned char STAR_1[8] = {0x00,0x10,0x10,0x10,0x7C,0x10,0x10,0x10};
const unsigned char STAR_0[8] = {0x10,0x28,0x38,0x7C,0xFE,0x7C,0x38,0x28};
// const star STAR_0[4] = {};

#define X_OFFSET 22
#define Y_OFFSET 47

#define SPRITE_Y_OFFSET ((Y_OFFSET)+148)

#define SCREEN 0x0400
#define COLOR  0xD800
#define SHAPE  0x2000

#define SEPARATION 50   

#define MOON_OFFSET (40*2+36)

#define TEXT_POSITION 840


static unsigned char counter;
// static unsigned char j;
static unsigned char sprite_text;
static unsigned char restored_text_row[40];


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

void color_text(unsigned char len, unsigned short offset, unsigned char col)
{
    unsigned short k;
    
    for(k=COLOR+offset;k<COLOR+offset+len;++k)
    {
		POKE(k,col);
    }
}


void restore_text_row(void)
{
    unsigned char i;
    
    for(i=0;i<40;++i)
    {
        POKE(SCREEN+TEXT_POSITION+i,restored_text_row[i]);
        POKE(COLOR+TEXT_POSITION+i,11);
    }
}


void color_commodore_sprites(void)
{
	unsigned char i;
	
    for(i=0;i<9;++i)
    {
        SPRC[i+9] = 11+(i&1);    
	}
}

void yellow_commodore_sprites(void)
{
	unsigned char i;
	
    for(i=0;i<9;++i)
    {
        SPRC[i+9] = 7;    
	}
}


void commodore_sprites(void)
{
	unsigned char i;
	
    for(i=0;i<9;++i)
    {

		SPRF[i+9] = GFX_START_INDEX + COMMODORE_MESSAGE[i] - 'A' + 1;
		sprite_text = 0;

        // SPRC[i+9] = 11+(i&1);        
    } 
	
	color_commodore_sprites();
	
	for(i=0;i<9;++i)
	{
		SPRF[i] = GFX_START_INDEX + 62;
		SPRF[i+18] = GFX_START_INDEX + 62;
	}	
}



void message_sprites(void)
{
	unsigned char i;
	
    for(i=0;i<9;++i)
    {

		if(counter&1)
		{
			SPRF[i+9] = GFX_START_INDEX + SPRITE_MESSAGE[i] - 'A' + 1;
			sprite_text = 1;
		}
		else
		{
			SPRF[i+9] = GFX_START_INDEX + MULTIPLEX_MESSAGE[i] - 'A' + 1;
			sprite_text = 0;
		}
		
        SPRC[i+9] =  COLORS[(i+9)%NUMBER_OF_COLORS];  ;        
    }

	for(i=0;i<9;++i)
	{
		SPRF[i] = GFX_START_INDEX + 63;
		SPRF[i+18] = GFX_START_INDEX + 63;
	}

	if(counter&1)
	{
		SPRF[9] = GFX_START_INDEX +'2';
		SPRF[10] = GFX_START_INDEX +'8';
	}
	++counter;

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
	SPRC[27] = 3;
	
	message_sprites();
}

void color_screen(unsigned char color)
{
    unsigned char i;
	unsigned char j;

    for(i=0;i<40;++i)
    {
		for(j=0;j<25;++j)
		{
			if((i+j)&1)
			{
				POKE(COLOR+i+j*40,color+(i&1));
			}
		}
    }
}

void fill_screen_with_even_stars(void)
{
    unsigned char i;
	unsigned char j;

    for(i=0;i<40;++i)
    {
		for(j=0;j<25;++j)
		{
			if((i+j)&1)
			{
				POKE(SCREEN+i+j*40,30); // small/big flashing star (top 2 rows)
				POKE(COLOR+i+j*40,11+(i&1));
			}
		}
    }
}

void fill_screen_with_odd_stars(void)
{
    unsigned char i;
	unsigned char j;

    for(i=0;i<40;++i)
    {
		for(j=0;j<25;++j)
		{
			if(!((i+j)&1))
			{
				POKE(SCREEN+i+j*40,31); // small/big flashing star (top 2 rows)
				POKE(COLOR+i+j*40,11+(i&1));
			}
		}
    }
}


void print_author(void)
{
    print(WRITTEN, 15, 1000-40-15,1);
    print(AUTHOR, 15, 1000-15,3);
}

/******************/
int main()
{    
    unsigned char XX = 10;
    unsigned char i;
	unsigned char j;
    unsigned short k;
	// unsigned char aux;
    // unsigned short star_loc;
    unsigned char text_counter = 0;
	// unsigned char yellow;
	
    // unsigned char below_1;

	unsigned char variation = 0;
	

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
	
	fill_screen_with_even_stars();
	fill_screen_with_odd_stars();
    
	print_author();
    
    // color_change();
    
    for(i=0;i<40;++i)
    {
        restored_text_row[i]=PEEK(SCREEN+TEXT_POSITION+i);
    }
	
	init_sprites();
    
    NUMSPRITES = _NUMBER_OF_SPRITES_;

    SPRY[NUMSPRITES-1]=220;

    while(1) 
    {
        if (MULTIPLEX_DONE) 
		{
            SPRX[NUMSPRITES-1]=XX+100;
			SPRY[NUMSPRITES-1]=SPRITE_Y_OFFSET+yValues2[XX];
            
			// Animate small/big stars
            if(!(XX&15))
            {
                ++j;
				i=j&7;
				k= SHAPE+30*8;
				POKE(k,STAR_3[i]);

                POKE(++k,STAR_2[i]);
          
                POKE(++k,STAR_1[i]);
                POKE(++k,STAR_0[i]);
                POKE(++k,STAR_1[i]);

                POKE(++k,STAR_2[i]);
				POKE(++k,STAR_3[i]);
				
				if((variation&7)>=5)
				{
				k= SHAPE+31*8;
				i=(j+4)&7;
				POKE(k,STAR_3[i]);

                POKE(++k,STAR_2[i]);
          
                POKE(++k,STAR_1[i]);
                POKE(++k,STAR_0[i]);
                POKE(++k,STAR_1[i]);

                POKE(++k,STAR_2[i]);
				POKE(++k,STAR_3[i]);
				}
            }
			
			if(!(variation&1)) // 28 sprites/multiplex message
			{
                switch(variation&3)
                {
                    case 0:
                        for(i=0;i<9;++i)
                        {
                            SPRX[i]=X_OFFSET+i*16;
                            SPRY[i]=i*4+Y_OFFSET+yValues2[XX]+4*(i&1);
                        }

                        for(i=0;i<9;++i)
                        {
                            if((sprite_text)&&i>1)
                            {
                                SPRX[i+9]=X_OFFSET+i*16+8;
                            }
                            else
                            {
                                SPRX[i+9]=X_OFFSET+i*16;
                            }
                            SPRY[i+9]=i*4+Y_OFFSET+SEPARATION+yValues2[XX];
                        }            

                        for(i=0;i<9;++i)
                        {
                            SPRX[i+18]=X_OFFSET+i*16;
                            SPRY[i+18]=i*4+Y_OFFSET+2*SEPARATION+yValues2[XX]+4*(i&1);
                        }   
                        break;
                    default:
                        for(i=0;i<9;++i)
                        {
                            SPRX[i]=i*16+yValues1[XX];;
                            SPRY[i]=i*4+Y_OFFSET;
                        }

                        for(i=0;i<9;++i)
                        {
                            if((sprite_text)&&i>1)
                            {
                                SPRX[i+9]=i*16+8+yValues1[XX];
                            }
                            else
                            {
                                SPRX[i+9]=i*16+yValues1[XX];
                            }
                            SPRY[i+9]=i*4+Y_OFFSET+SEPARATION;
                        }            

                        for(i=0;i<9;++i)
                        {
                            SPRX[i+18]=i*16+yValues1[XX];;
                            SPRY[i+18]=i*4+Y_OFFSET+2*SEPARATION;
                        }   
                        break;
				}
				if(XX==255)
				{
					commodore_sprites();
				}
			}
			else // Commodore message
			{
				switch(variation&3)
				{
					case 1:
						for(i=0;i<9;++i)
						{
							SPRX[i]=X_OFFSET+i*16+(XX&15);
							SPRY[i]=i*4+Y_OFFSET+yValues4[XX];
						}

						for(i=0;i<9;++i)
						{
							
							SPRX[i+9]=X_OFFSET+i*16;

							SPRY[i+9]=i*4+Y_OFFSET+SEPARATION+yValues4[XX];
							
						}            

						for(i=0;i<9;++i)
						{
							SPRX[i+18]=X_OFFSET+i*16-(XX&15);
							SPRY[i+18]=i*4+Y_OFFSET+2*SEPARATION+yValues4[XX];
						}  
						break;
					
					default:
						for(i=0;i<9;++i)
						{
							SPRX[i]=X_OFFSET+i*16+(XX&15);
							SPRY[i]=i*4+Y_OFFSET;
						}

						for(i=0;i<9;++i)
						{
							
							SPRX[i+9]=i*16+yValues1[XX];

							SPRY[i+9]=i*4+Y_OFFSET+SEPARATION;
							
						}            

						for(i=0;i<9;++i)
						{
							SPRX[i+18]=X_OFFSET+i*16-(XX&15);
							SPRY[i+18]=i*4+Y_OFFSET+2*SEPARATION;
						}  
				}
				
				if(!(XX&15))
				{
					yellow_commodore_sprites();
				}
				else if((XX&15)==1)
				{
					commodore_sprites();
				}
				
				if(XX==255)
				{
					message_sprites();
				}
			}

            if(!(XX&63))
            {
				switch (text_counter&3)
				{
					case 0:
					
						restore_text_row();
						print(TEXT1, 15,TEXT_POSITION+12,5);
						break;
					
					case 1:
					
					    restore_text_row();
					    print(TEXT2, 29,TEXT_POSITION+6,4);
					    break;
					case 2:
					{
						restore_text_row();
						print(TEXT3, 29,TEXT_POSITION+6,10);
						break;
					}
					default:
					{
						restore_text_row();
					}
				}
                ++text_counter;
			}
			else
			{
				switch (text_counter&3)
				{
					case 1:
					
						if(!(XX&3))
						{
							color_text(15,TEXT_POSITION+12,7);
						}
						else if((XX&3)==1)
						{
							color_text(15,TEXT_POSITION+12,5);
						}
						break;
					case 2:
						if(!(XX&7))
						{
							color_text(29,TEXT_POSITION+6,7);
						}
						else if((XX&7)==1)
						{
							color_text(29,TEXT_POSITION+6,4);
						}
						break;
					case 3:
						if(!(XX&7))
						{
							color_text(29,TEXT_POSITION+6,7);
						}
						else if((XX&7)==1)
						{
							color_text(29,TEXT_POSITION+6,10);
						}
						
				}
			}

			
            MULTIPLEX_DONE = 0;
            SPRUPDATEFLAG = 1;
			
			++XX;
			if(XX==0)
			{
				++variation;
				if((variation&7)==3)
				{
					color_screen(14);
					print_author();
				}
				else if(!(variation&7))
				{
					color_screen(11);
					print_author();
					k= SHAPE+31*8;
					i=(j+4)&7;
					POKE(k,0);

					POKE(++k,0);
			  
					POKE(++k,0);
					POKE(++k,0);
					POKE(++k,0);

					POKE(++k,0);
					POKE(++k,0);
				}
			}
        }
    }
    return 0;
}
