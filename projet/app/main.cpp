/*
Auteurs : Steven Ho, Massoud Ibrahim, Nazim Bertouche et Yacine Mahamadou 
Travail : Projet
Section # : 01
Équipe # : 0314
Correcteur : Charles Jiang

Description du programme : Programme qui effectue un parcours dépendamment du départ sélectionné (A, B ou S).

Identifications matérielles (Broches I/O) : Sur les ports du microcontrôleur ATmega324PA
                                            -La DEL bicolore (vert/rouge) est connectée aux broches C2 et C3 
                                             en sortie 
                                            -Le bouton-poussoir interrupt est connecté à la broche D2 en entrée.
                                            -Le bouton-poussoir blanc est connecté à la broche D3 en entrée.
                                            -Le piezo est connecté aux broches D6 et D7 en sortie.
                                            -Le moteur gauche est connecté aux broches B2 et B4 en sortie.
                                            -Le moteur droit est connecté aux broches B1 et B3 en sortie.
                                            -Le capteur de distance est connecté à la broche A6 en entrée.
                                            -Le suiveur de ligne est connecté aux broches A0, A1, A2, 
                                             A3, A4 et A5 en entrée.
*/

#define F_CPU 8000000
#define BOUTON_INTERRUPT_RELACHE !(PIND & 1 << PD2)
#define BOUTON_BLANC_RELACHE (PIND & 1 << PD3)

#include <avr/io.h>
#include "robot.h"

volatile bool gBoutonAppuye = false;

// Confirmer la sélection.
ISR(INT0_vect) 
{
    _delay_ms(DELAI_ANTI_REBOND);
    gBoutonAppuye = BOUTON_BLANC_RELACHE;
    Robot::controler().gererInterruptionBouton(INT0, gBoutonAppuye);
}

// Changer la sélection.
ISR(INT1_vect) 
{
    _delay_ms(DELAI_ANTI_REBOND);
    gBoutonAppuye = BOUTON_INTERRUPT_RELACHE;
    Robot::controler().gererInterruptionBouton(INT1, gBoutonAppuye);
}

ISR(TIMER1_COMPA_vect)
{
    Robot::controler().accederTimer1().stop();
    Robot::controler().gererInterruptionMinuterie();
}

int main()
{    
    // Méthode bloquante
    Robot::controler().initialiser();
    
    while (true)
    {
        Robot::controler().effectuerParcours();
    }
}
