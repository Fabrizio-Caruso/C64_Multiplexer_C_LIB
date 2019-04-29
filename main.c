#include <conio.h>
#include <stdlib.h>
#include <c64.h>
/*******************
** Prototypes
*******************/
void START(void);
void MUOVI_SPRITES_ESEMPIO(void);
void WAIT_FOR_SORT(void);

extern char SPRX[];
extern char SPRY[];
extern char SPRC[];
extern char SPRF[];
extern char NUMSPRITES;
extern char SPRUPDATEFLAG;
extern char SPRIRQCOUNTER;



/******************/
int main()
{	
	unsigned char XX = 0;
	unsigned char i;
    START();
	NUMSPRITES = 24;	
    
	while(1) 
	{
		{
			for(i=0;i<24;++i)
			{
				SPRX[i] = XX+5*i;
				SPRY[i] = 80 + (i & 7)*20;					
				XX = XX+1;
			}
		}
    }
	return 0;
}
