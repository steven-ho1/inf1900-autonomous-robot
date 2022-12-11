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


volatile uint8_t gMinuterieExpiree;
volatile uint8_t gBoutonPoussoir;

void estEteint()
{
    PORTA &= ~(1 << PA0 | 1 << PA1);
}

void estRouge()
{
    PORTA &= ~(1 << PA0);
    PORTA |= 1 << PA1;
}

void estVert()
{
    PORTA |= 1 << PA0;
    PORTA &= ~(1 << PA1);
}

// 1<<ISC01|1<<ISC00

// placer le bon type de signal d'interruption

// à prendre en charge en argument

ISR(TIMER1_COMPA_vect)
{

    gMinuterieExpiree = 1;
}

ISR(INT0_vect)
{
    gBoutonPoussoir = 1;

    // anti-rebond

    _delay_ms(30);
}

void partirMinuterie(uint16_t duree)
{
    gMinuterieExpiree = 0;

    // mode CTC du timer 1 avec horloge divisée par 1024

    // interruption après la durée spécifiée

    TCNT1 = 0;

    OCR1A = duree;

    TCCR1A &= ~(1 << WGM11 | 1 << WGM10);

    TCCR1B &= ~(1 << WGM13);
    TCCR1B |= 1 << WGM12;

    TCCR1B |= 1 << CS12 | 1 << CS10;
    TCCR1B &= ~(1 << CS11);

    TCCR1C = 0;

    TIMSK1 |= 1 << OCIE1A;
}

void initialisation(void)
{

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

    EICRA |= (1 << ISC01 | 1 << ISC00);

    // permet de recevoir à nouveau des interruptions.
    
}


int main()
{
    initialisation();
    _delay_ms(10000);
    estRouge();
    _delay_ms(100);
    estEteint();
    EIFR |= (1 << INTF0);
    sei(); // demander pourquoi on peut déclencher une interruption même avant
    partirMinuterie(F_CPU/1024);
    do
    {
        // attendre qu'une des deux variables soit modifiée
        // par une ou l'autre des interruptions.
    } while (gMinuterieExpiree == 0 && gBoutonPoussoir == 0);

    // Une interruption s'est produite. Arrêter toute

    // forme d'interruption. Une seule réponse suffit.

    cli();

    // Verifier la réponse
    if (gBoutonPoussoir == 1)
        estVert();
    else
        estRouge();    
}