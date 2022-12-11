#define F_CPU 8000000
#define N_ECHANTILLONS 20
#define VALEUR_MAX 85
#define ZONE_GRISE 35
#define VALEUR_MIN 5

#include <avr/io.h>
#include <util/delay.h>
#include "memoire24.h"

int main()
{
    Memoire24 memoire; 
   
    memoire.ecrire(0); // N
    _delay_ms(5);
    memoire.ecrire(1); // L
    _delay_ms(5);
    memoire.ecrire(1); // IG
    _delay_ms(5);
    memoire.ecrire(3); // 3 poteaux
    _delay_ms(5);
}