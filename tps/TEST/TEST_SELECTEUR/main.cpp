/*
Auteurs : Steven Ho, Massoud Ibrahim et Nazim Bertouche
Travail : Projet
Section # : 01
Équipe # : 0314

Description du programme :


Identifications matérielles (Broches I/O) :

*/

#define F_CPU 8000000

#define BOUTON_INTERRUPT_RELACHE !(PIND & 1 << PD2)
#define BOUTON_BLANC_RELACHE (PIND & 1 << PD3)

#include <avr/io.h>
#include <util/delay.h>
#include "del.h"
#include "bouton.h"
#include "capteurDistance.h"
#include "suiveurLigne.h"
#include "point.h"
#include "selecteur.h"

volatile uint8_t gInterruption = INT0;
Parcours gParcours = Parcours::A;
void gererAppuie();

// Confirmer la sélection.
ISR(INT0_vect) 
{
    gInterruption = INT0;
    gererAppuie();
}

// Changer la sélection.
ISR(INT1_vect) 
{
    gInterruption = INT1;
    gererAppuie();
}

void gererAppuie()
{
    // délai défini dans bouton.h
    _delay_ms(DELAI_ANTI_REBOND);

    Selecteur selecteur;

    switch (gInterruption) 
    {
        case INT0 :
            if (BOUTON_INTERRUPT_RELACHE)
                selecteur.confirmerParcours();
            break;
        case INT1 :   
            if (BOUTON_BLANC_RELACHE) 
                selecteur.changerParcours(gParcours);
            break;
    }
    // Éviter des interruptions consécutives
    EIFR |= (1 << INTF0);
}

int main()
{
    Selecteur selecteur;
    SuiveurLigne suiveurLigne;

    selecteur.initialiser();
}
