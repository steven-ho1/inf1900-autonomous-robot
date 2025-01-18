/*
Auteurs : Steven Ho et Massoud Ibrahim
Travail : TP6 problème 2
Section # : 01
Équipe # : 03
Correcteur : Dorine Dantrun 

Description du programme : Le programme permet d'allumer une DEL bicolore à une couleur déterminée
                           par l'intensité de la lumière captée par la photorésistance: 
                           -verte pour une lumière faible
                           -ambrée pour une lumière ambiante
                           -rouge pour une lumière intense. 

Identifications matérielles (Broches I/O) : -La DEL bicolore (vert/rouge) est reliée aux broches B0 et B1 en sortie du
                                             microcontrôleur ATMega324PA.
                                            -La photorésistance est reliée à la broche A0 en entrée.
*/

#define F_CPU 8000000
#define DELAI_AMBRE 5
#define N_BITS_INSIGNIFICATIFS 2
#define BORNE_INFERIEURE_AMBRE 50
#define BORNE_SUPERIEURE_AMBRE 240

#include <avr/io.h>
#include <util/delay.h>
#include "can.h"

void initialiserRegistres()
{
    DDRB |= (1 << PB0) | (1 << PB1);
    DDRA &= ~(1 << PA0);
}

void estEteint()
{
    PORTB &= ~(1 << PB0 | 1 << PB1);
}

void estRouge()
{
    PORTB &= ~(1 << PB0);
    PORTB |= 1 << PB1;
}

void estVert()
{
    PORTB |= 1 << PB0;
    PORTB &= ~(1 << PB1);
}

void estAmbre()
{
    estRouge();
    _delay_ms(DELAI_AMBRE);
    estVert();
    _delay_ms(DELAI_AMBRE);
}

void changerCouleur(uint8_t tension)
{
    if (tension < BORNE_INFERIEURE_AMBRE)
        estVert();
    else if (tension < BORNE_SUPERIEURE_AMBRE)
        estAmbre();
    else
        estRouge();
}

uint8_t lireTension(can& convertisseur)
{
    uint16_t tension = convertisseur.lecture(PA0);
    
    return static_cast<uint8_t>(tension >> N_BITS_INSIGNIFICATIFS);
}

int main()
{
    initialiserRegistres();

    can convertisseur = can();
    while (true)
    { 
        uint8_t tension = lireTension(convertisseur);
        changerCouleur(tension);
    }
}