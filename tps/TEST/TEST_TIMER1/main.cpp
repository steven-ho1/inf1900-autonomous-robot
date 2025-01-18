#define F_CPU 8000000

#include <avr/io.h> 
#include "timer1.h"
#include "del.h"
#include <util/delay.h>
#include "capteurDistance.h"

volatile bool lecturePermise = true;

Timer1 timer1(7812);
Del del(Del::ETEINT, &PORTA, 2, 3);

ISR(TIMER1_COMPA_vect)
{
    timer1.stop();
    //CapteurDistance capteur;
    del.changerCouleur(Del::VERT);
    _delay_ms(1000);
    del.changerCouleur(Del::ETEINT);
    _delay_ms(1000);
    lecturePermise = true;
}

int main()
{
    //CapteurDistance capteur;
    
    while (true)
    {
        if (lecturePermise)
        {
            timer1.start();
            lecturePermise = false;
        }
    }
}
