/*
Auteurs : Steven Ho et Massoud Ibrahim   
Travail : TP2 problème 2
Section # : 01
Équipe # : 03
Correcteur : Charles Jiang
    
Description du programme : Programme qui permet de faire allumer la DEL bicolore d'une manière précise,
                           en utilisant uniquement la carte mere. La DEL bicolore s'allumera tout dépendamment
                           si l'on appuie ou relache le bouton-poussoir en passant par la couleur rouge, ambrée, 
                           verte, rouge, éteinte et verte. On revient ensuite à la couleur initiale rouge.
                           Les différentes couleurs en sortie suivent le tableau des états ci-dessous.
  
Identifications matérielles (Broches I/O) : -La DEL bicolore (vert/rouge) est reliée aux broches A0 et A1 en sortie du 
                                             microcontrôleur ATMega324PA.
                                            -Le bouton-poussoir est connecté à la broche D2 en entrée.
 
Tableau des états :
+--------------+--------+--------------+--------+
| Etat present | Entree | Etat suivant | Sortie |
+--------------+--------+--------------+--------+
| INIT         |      0 | INIT         | ROUGE  |
| INIT         |      1 | E1           | ROUGE  |
| E1           |      0 | E2           | AMBREE |
| E1           |      1 | E1           | AMBREE |
| E2           |      0 | E2           | VERT   |
| E2           |      1 | E3           | VERT   |
| E3           |      0 | E4           | ROUGE  |
| E3           |      1 | E3           | ROUGE  |
| E4           |      0 | E5           | ETEINT |
| E4           |      1 | E4           | ETEINT |
| E5           |      0 | INIT         | VERT   |
| E5           |      1 | E5           | VERT   |
+--------------+--------+--------------+--------+
*/

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


enum class Etat { INIT, E1, E2, E3, E4, E5 };
// enum Sortie { SORTIE_ETEINT, SORTIE_ROUGE, SORTIE_VERT, SORTIE_AMBRE };

const uint8_t PAUSE_AMBRE = 10; // en millisecondes
volatile Etat gEtatSuivant{ Etat::INIT };

void estEteint() {
    PORTA &= ~(1 << PA0 | 1 << PA1); 
}

void estRouge() {
    PORTA &= ~(1 << PA0);
    PORTA |= 1 << PA1;
}

void estVert() {
    PORTA |= 1 << PA0; 
    PORTA &= ~(1 << PA1);
}

void estAmbre() {
    estRouge();
    _delay_ms(PAUSE_AMBRE);
    estVert();
    _delay_ms(PAUSE_AMBRE);
}



// placer le bon type de signal d'interruption
// à prendre en charge en argument
ISR(INT0_vect) {
    // laisser un délai avant de confirmer la réponse du
    // bouton-poussoir: environ 30 ms (anti-rebond)
    _delay_ms(30);

    // se souvenir ici si le bouton est pressé ou relâché
    //'modifier ici'
    // changements d'état tels que ceux de la
    // semaine précédente
    //'modifier ici'
    switch (gEtatSuivant) {
        case Etat::INIT:
            gEtatSuivant = Etat::E1; 
            break;
        case Etat::E1:
            gEtatSuivant = Etat::E2; 
            break;
        case Etat::E2:
            gEtatSuivant = Etat::E3; 
            break;
        case Etat::E3:
            gEtatSuivant = Etat::E4;
            break;
        case Etat::E4:
            gEtatSuivant = Etat::E5; 
            break;
        case Etat::E5:
            gEtatSuivant = Etat::INIT; 
            break;
    }

    // Voir la note plus bas pour comprendre cette instruction et son rôle
    EIFR |= (1 << INTF0);

}

void initialisation(void) {

    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'être interrompu alors que
    // le microcontrôleur n'est pas prêt...
    cli();

    // configurer et choisir les ports pour les entrées
    // et les sorties. DDRx... Initialisez bien vos variables
    DDRA |= 1 << PA0 | 1 << PA1; 
    DDRD &= ~(1 << PD2);

    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes
    EIMSK |= (1 << INT0);

    // il faut sensibiliser les interruptions externes aux
    // changements de niveau du bouton-poussoir
    // en ajustant le registre EICRA
    EICRA &= ~(1 << ISC01); 
    EICRA |= 1 << ISC00;

    // sei permet de recevoir à nouveau des interruptions.
    sei();

}

int main() {
    initialisation();

     while (true) {
         switch (gEtatSuivant) {
             case Etat::INIT :
                estRouge();
                break;
            case Etat::E1 :
                estAmbre();
                break;
            case Etat::E2 :
                estVert();
                break;
            case Etat::E3 :
                estRouge();                
                break;
            case Etat::E4 :
                estEteint();                
                break;
            case Etat::E5 :
                estVert();                
                break;
        }
    }
}