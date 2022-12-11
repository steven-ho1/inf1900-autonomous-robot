#include "moteur.h"

Moteur::Moteur()
{
    // Sorties pour vitesse pwm
    DDRB |= (1 << PB4) | (1 << PB3); 
    
    // Sorties pour la direction
    DDRB |= (DIRECTION_MOTEUR_GAUCHE | DIRECTION_MOTEUR_DROIT); 

    // Immobile au départ
    arreter(); 
}

Moteur::~Moteur() 
{
    // Rien
}

void Moteur::ajusterPwm(uint8_t rapportCycliqueGauche, 
                        uint8_t rapportCycliqueDroit)
{
    // Valeur maximale du compteur
    const uint8_t TOP = 255;
    
    // mise � un des sorties OC1A et OC1B sur comparaison
    // r�ussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe � 0xFF (mode #1 de la table 16-5
    // page 130 de la description technique du ATmega324PA)

    // Référence: 8-bit Atmel Microcontroller DATASHEET
    OCR0A = TOP * rapportCycliqueDroit / 100; 
    OCR0B = TOP * rapportCycliqueGauche / 100;

    TCCR0A = (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
    TCCR0B = 1 << CS01;
}

void Moteur::accelerer(uint8_t rapportCycliqueGauche, 
                       uint8_t rapportCycliqueDroit, 
			           Direction direction)
{
    switch (direction)
    {
        case Direction::AVANT :
            PORTB &= ~(DIRECTION_MOTEUR_GAUCHE | DIRECTION_MOTEUR_DROIT); 
            break;
        case Direction::ARRIERE :
            PORTB |= (DIRECTION_MOTEUR_GAUCHE | DIRECTION_MOTEUR_DROIT);
            break;
        default :
            break;
    }
    
    ajusterPwm(rapportCycliqueGauche + AJUSTEMENT_VITESSE_GAUCHE, 
               rapportCycliqueDroit);
}

void Moteur::accelerer(uint8_t rapportCyclique, Direction direction)
{
    accelerer(rapportCyclique, rapportCyclique, direction);
}

void Moteur::arreter()
{
    TCCR0A &= ~(1 << COM0A1 | 1 << COM0B1);
    TCCR0A &= ~(1 << COM0A0 | 1 << COM0B0);
}
