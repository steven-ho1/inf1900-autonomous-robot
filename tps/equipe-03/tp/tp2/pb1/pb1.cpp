/*
Auteurs : Massoud Ibrahim et Steven Ho
Travail : TP2 problème 1
Section # : 01
Équipe # : 03
Correcteur : Charles Jiang

Description du programme : Programme qui allume une DEL bicolore à partir d'un bouton-poussoir. La DEL, initialement éteinte, 
                           doit être appuyée et relâchée 3 fois pour être allumée au vert pendant 2 secondes avant de s'éteindre
                           à nouveau. On revient alors au point de départ.

Identifications matérielles : -La DEL bicolore (vert/rouge) est connectée aux broches A0 et A1 en sortie du microcontrôleur ATmega324PA.
                              -Le bouton-poussoir est connecté à la broche D2 en entrée

Tableau des états :
+------------ + ------ + ------------ + -------- +
| Etat present  | Entree | Etat suivant | Sortie |
+-------------- + ------ + ------------ + ------ +
| INIT			| 0		 | INIT			| Eteint |
| INIT			| 1		 | E1			| Eteint |
| E1			| 0		 | E1			| Eteint |
| E1			| 1		 | E2			| Eteint |
| E2			| 0		 | E2			| Eteint |
| E2			| 1		 | E3			| Eteint |
| E3			| X		 | INIT			| Vert   |
+-------------- + -----  + ------------ + ------ +
*/

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

enum class Etat { INIT, E1, E2, E3 };
enum Sortie { SORTIE_ETEINT, SORTIE_VERT };
const uint8_t PAUSE_VERT = 2; // en secondes

struct Bouton {
    bool relache = false;

    bool estAppuye() {
        uint8_t pauseAntiRebond = 10;

        if (PIND & 1 << PD2) {
            _delay_ms(pauseAntiRebond);
            return PIND & 1 << PD2;
        }
        else
            return false;
    }
};

void estEteint() {
    PORTA &= ~(1 << PA0 | 1 << PA1);
}

void estVert() {
    PORTA |= 1 << PA0; 
    PORTA &= ~(1 << PA1); 
    _delay_ms(PAUSE_VERT*1000);
}

int main() {
    Etat etatSuivant{ Etat::E1 };
    DDRA |= 1 << PA0 | 1 << PA1; 
    DDRD &= ~(1 << PD2);

    Sortie sortie = SORTIE_ETEINT;
    Bouton bouton = {};

    while (true) {
        switch (sortie) {
            case SORTIE_ETEINT:
                estEteint();
                break;
            case SORTIE_VERT:
                estVert();
                break;
        }
        
        // Boucle while pour éviter de changer d'état lorsque l'utilisateur maintient le bouton-poussoir
        while (bouton.estAppuye()) {
            bouton.relache = true;

            if (etatSuivant == Etat::INIT)
                break;
        }

        
        // La deuxieme condition permet de revenir à l'état INIT sans qu'on n'ait à appuyer le bouton
        bool changeEtat = bouton.relache || etatSuivant == Etat::INIT;
        
        if (changeEtat) {
            switch (etatSuivant) {
                case Etat::INIT:
                    etatSuivant = Etat::E1;
                    sortie = SORTIE_ETEINT;
                    break;
                case Etat::E1:
                    etatSuivant = Etat::E2;
                    break;
                case Etat::E2:
                    etatSuivant = Etat::E3;
                    break;
                case Etat::E3:
                    etatSuivant = Etat::INIT;
                    sortie = SORTIE_VERT;
                    break;
            }
            bouton.relache = false; 
        }
    }
}