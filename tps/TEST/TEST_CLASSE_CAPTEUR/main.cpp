#define F_CPU 8000000

#include <avr/io.h>
#include "suiveurLigne.h"
#include "moteur.h"
#include "capteurDistance.h"
#include "timer1.h"

CapteurDistance capteur;

ISR(TIMER1_COMPA_vect)
{
    capteur.activerLecture();
}
int main()
{
    SuiveurLigne suiveurLigne;

    while (true)
    {
        capteur.capter();
        suiveurLigne.deplacerRobot();
    }
}