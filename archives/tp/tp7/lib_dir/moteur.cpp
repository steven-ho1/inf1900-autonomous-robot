#include "moteur.h"

Moteur::Moteur()
{
    DDRB |= (1 << PB3) | (1 << PB4); // Entrée pour vitesse pwm
    DDRB |= (DIRECTION_MOTEUR_GAUCHE | DIRECTION_MOTEUR_DROIT); // Entrée pour la direction
    ajusterPwm(RAPPORT_CYCLIQUE_0, RAPPORT_CYCLIQUE_0); // par defaut cest en arret
}

void Moteur::ajusterPwm(uint8_t rapportCycliqueGauche, uint8_t rapportCycliqueDroit)
{
    const uint8_t TOP = 0xFF;
    
    // mise � un des sorties OC1A et OC1B sur comparaison
    // r�ussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe � 0xFF (mode #1 de la table 16-5
    // page 130 de la description technique du ATmega324PA)

    // Référence: 8-bit Atmel Microcontroller DATASHEET
    OCR0A = TOP * rapportCycliqueGauche / 100; // Donc, la valeur max de compteur est 0xFF
    OCR0B = TOP * rapportCycliqueDroit / 100;

    TCCR0A = (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
    TCCR0B = 1 << CS11;
}

void Moteur::accelerer(DirectionVerticale direction, uint8_t rapportCyclique)
{
    switch (direction)
    {
        case AVANT :
            PORTB &= ~(DIRECTION_MOTEUR_GAUCHE | DIRECTION_MOTEUR_DROIT); 
            break;
        case ARRIERE :
            PORTB |= (DIRECTION_MOTEUR_GAUCHE | DIRECTION_MOTEUR_DROIT);
            break;
    }
    
    ajusterPwm(rapportCyclique, rapportCyclique);
}

void Moteur::virer(uint8_t rapportCycliqueGauche, uint8_t rapportCycliqueDroit)
{
    ajusterPwm(rapportCycliqueGauche, rapportCycliqueDroit);
}

void Moteur::virer90(DirectionHorizontale direction)
{
    switch (direction)
    {
        case GAUCHE :
            PORTB &= ~(DIRECTION_MOTEUR_GAUCHE);
            PORTB |= (DIRECTION_MOTEUR_DROIT);
            break;
        case DROITE :
            PORTB |= (DIRECTION_MOTEUR_GAUCHE);
            PORTB &= ~(DIRECTION_MOTEUR_DROIT);
            break;
    }

    ajusterPwm(RAPPORT_CYCLIQUE_75, RAPPORT_CYCLIQUE_75); 
    _delay_ms(DELAI_VIRAGE_90);
}

void Moteur::arreter()
{
    PORTB &= ~(DIRECTION_MOTEUR_GAUCHE | DIRECTION_MOTEUR_DROIT);
    ajusterPwm(RAPPORT_CYCLIQUE_0, RAPPORT_CYCLIQUE_0);
}
