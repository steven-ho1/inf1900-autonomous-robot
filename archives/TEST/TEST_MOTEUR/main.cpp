#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "moteur.h"

int main()
{
    Moteur moteur;

    moteur.accelerer(90);
    _delay_ms(3000);
    moteur.accelerer(90,10);
    _delay_ms(3000);
    moteur.accelerer(10,90);
    _delay_ms(3000);
    moteur.accelerer(90, 90);
    _delay_ms(3000);
    moteur.arreter();
    _delay_ms(3000);
    moteur.accelerer(90, Direction::ARRIERE);
    _delay_ms(3000);
    moteur.accelerer(90,10, Direction::ARRIERE);
    _delay_ms(3000);
    moteur.accelerer(10,90, Direction::ARRIERE);
    _delay_ms(3000);
    moteur.accelerer(90, 90, Direction::ARRIERE);
}
