#include <conio.h>
#include <stdlib.h>
#include <c64.h>
/*******************
** Prototypes
*******************/
void START(void);
void MUOVI_SPRITES_ESEMPIO(void);
/******************/
int conta;
int main()
{
    START();
    while(1) {
    conta=conta+1;
        if (conta==50) {
            MUOVI_SPRITES_ESEMPIO();
            conta = 0;
        };
    };
}
