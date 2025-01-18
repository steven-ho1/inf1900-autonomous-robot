/*
Auteurs : Steven Ho et Massoud Ibrahim
Travail : TP6 problème 1
Section # : 01
Équipe # : 03
Correcteur : Dorine Dantrun 

Description du programme : Programme qui fait clignoter une DEL bicolore entre la couleur rouge et éteinte pendant une durée 
                           déterminée par le temps d'appuie du bouton-poussoir. La DEL est initialement éteinte et appuyer sur 
                           le bouton-poussoir active un compteur. Une fois le bouton relâché ou lorsque le compteur 
                           atteint son maximum, la DEL devient verte et s'éteint avant de débuter une série de clignotements 
                           Enfin, elle finit ce cycle en devenant verte avant de revenir à l'état initial éteint.
                           Plus le bouton-poussoir est maintenu, plus le nombre de clignotements augmente. 

Identifications matérielles (Broches I/O) : -La DEL bicolore (vert/rouge) est reliée aux broches B0 et B1 en sortie du
                                             microcontrôleur ATMega324PA.
                                            -Le bouton-poussoir est connecté à la broche D2 en entrée.

Note: Programme inspiré du document offert « Comment programmer une machine à état »
*/

#define F_CPU 8000000
#define DELAI_ANTI_REBOND 30
#define COMPTE_MAXIMAL 120
#define DELAI_COMPTEUR 100
#define DELAI_E2 500
#define DELAI_E3 2000
#define DELAI_CLIGNOTEMENT 250
#define DELAI_E5 1000
#define MASQUE_BOUTON 1 << PD2
#define COMPTE_INITIAL 0

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

enum class Etat
{
    INIT,
    E1,
    E2,
    E3,
    E4,
    E5
};

volatile bool gBoutonAppuye = false;

void initialiserRegistres()
{
    cli();
    DDRB |= (1 << PB0) | (1 << PB1);
    DDRD &= ~(1 << PD2);

    EIMSK |= (1 << INT0);

    EICRA &= ~(1 << ISC01);
    EICRA |= 1 << ISC00;

    sei();
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

void clignoterRouge(uint8_t compteur)
{
    uint8_t nClignotements = compteur/2;
    for (int i = 0; i < nClignotements; ++i)
    {
        estRouge(); // 0.5 s par clignotement.
        _delay_ms(DELAI_CLIGNOTEMENT);
        estEteint();
        _delay_ms(DELAI_CLIGNOTEMENT);
    }
}

void estVert()
{
    PORTB |= 1 << PB0;
    PORTB &= ~(1 << PB1);
}

uint8_t partirCompteur(Etat& etat, uint8_t& compteur)
{
    compteur = 0;
    while (compteur != COMPTE_MAXIMAL && gBoutonAppuye == true)
    {
        compteur += 1; // 10 incrémentations par seconde = 1 par 0.1s.
        _delay_ms(DELAI_COMPTEUR);
    }
    
    etat = Etat::E2;

    return compteur;
}

void lireBouton()
{
    gBoutonAppuye = !(PIND & MASQUE_BOUTON); 
}

void changerCouleur(Etat& etat, uint8_t& compteur)
{
    switch (etat)
    {
        case Etat::INIT :
            estEteint();
        
            if (gBoutonAppuye == true) 
                etat = Etat::E1;
            break;
        case Etat::E1 :
            compteur = partirCompteur(etat, compteur);
            break;
        case Etat::E2 :
            estVert();
            _delay_ms(DELAI_E2);
            // Fallthrough
        case Etat::E3 :
            estEteint();
            _delay_ms(DELAI_E3);
            // Fallthrough
        case Etat::E4 :
            clignoterRouge(compteur);
            // Fallthrough
        case Etat::E5 :
            estVert();
            _delay_ms(DELAI_E5);
            etat = Etat::INIT;
            // s'assure qu'un appuie et non un relâche change d'état dans INIT.
            gBoutonAppuye = false;
            break;
    }
}

ISR(INT0_vect)
{
    _delay_ms(DELAI_ANTI_REBOND);

    lireBouton();
}

int main()
{
    initialiserRegistres();

    Etat etat = Etat::INIT;
    uint8_t compteur = COMPTE_INITIAL;
    while (true)
    { 
        changerCouleur(etat, compteur);
    }
}