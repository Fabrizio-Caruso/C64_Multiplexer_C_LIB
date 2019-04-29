#include <conio.h>
#include <stdlib.h>
#include <c64.h>
/*******************
** Prototypes
*******************/
void start(void);
void muovi_sprites_esempio(void);
/******************/
int conta;
int main()
{
    start();
    while(1) {
    conta=conta+1;
        if (conta==50) {
            muovi_sprites_esempio();
            conta = 0;
        };
    };
}
