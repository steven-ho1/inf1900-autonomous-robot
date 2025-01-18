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

enum class Etat { INIT, E1, E2, E3, E4, E5 };
enum Sortie { SORTIE_ETEINT, SORTIE_ROUGE, SORTIE_VERT, SORTIE_AMBRE };


struct Bouton {
    bool estMaintenu = false;
    bool relache = false;

    bool estAppuye() {
        uint8_t pauseAntiRebond = 10; // en millisecondes
        
        if (PIND & 1 << PD2) {
            _delay_ms(pauseAntiRebond);
            return PIND & 1 << PD2;
        }
        else
            return false;
    }

    void reinitialiser() {
        estMaintenu = false;
        relache = false;
    }
};

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
    _delay_ms(5);
    estVert();
    _delay_ms(3);
}

//j'ajoute la fonction prochainEtat et initiliser registres

void initialiserRegistres() {
    DDRA |= 1 << PA0 | 1 << PA1;
    DDRD &= ~(1 << PD2);
}

void prochainEtat(Etat& etatSuivant, Sortie& sortie, Bouton& bouton) {

    switch (sortie) {
    case SORTIE_ETEINT:
        estEteint();
        break;
    case SORTIE_VERT:
        estVert();
        break;
    case SORTIE_ROUGE:
        estRouge();
        break;
    case SORTIE_AMBRE:
        estAmbre();
        break;
    }

    bool changeEtat = (bouton.estAppuye() && bouton.estMaintenu == false) || bouton.relache;
    bool boutonEstRelache = bouton.estMaintenu && bouton.estAppuye() == false;

    if (changeEtat) {
        bouton.estMaintenu = true;
        switch (etatSuivant) {
        case Etat::INIT:
            etatSuivant = Etat::E1; // relache
            sortie = SORTIE_ROUGE;
            break;
        case Etat::E1:
            etatSuivant = Etat::E2; // appuie
            sortie = SORTIE_AMBRE;
            break;
        case Etat::E2:
            etatSuivant = Etat::E3; // relache
            sortie = SORTIE_VERT;
            break;
        case Etat::E3:
            etatSuivant = Etat::E4; // appuie 
            sortie = SORTIE_ROUGE;
            break;
        case Etat::E4:
            etatSuivant = Etat::E5; // relache
            sortie = SORTIE_ETEINT;
            break;
        case Etat::E5:
            etatSuivant = Etat::INIT; // appuie
            sortie = SORTIE_VERT;
            break;
        }

        if (bouton.relache) {
            bouton.reinitialiser();
        }
    }
    else if (boutonEstRelache) {
        bouton.relache = true;
    }
}


int main() {
    Etat etatSuivant{ Etat::E1 };
    initialiserRegistres();
    Sortie sortie = SORTIE_ROUGE;
    Bouton bouton = {};

    while (true) {
        prochainEtat(etatSuivant,sortie, bouton);
    }
}