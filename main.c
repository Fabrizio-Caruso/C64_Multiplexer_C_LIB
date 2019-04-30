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
// extern char NUMSPRITES;
extern char SPRUPDATEFLAG;
extern char SPRIRQCOUNTER;



/******************/
int main()
{	
	unsigned char XX = 0;
	unsigned char i;
    START();
    
	SPRX[30]=30;
	SPRX[31]=130;
	clrscr();
	while(1) 
	{
		gotoxy(1,1); cprintf("%u",SPRUPDATEFLAG);
	
		SPRY[30]=XX;
		SPRY[31]=255-XX;
		for(i=0;i<30;++i)
		{
			SPRX[i] = XX+4*i;
			SPRY[i] = 60 + (i & 7)*22;					
		}
		++XX;
    }
	return 0;
}
