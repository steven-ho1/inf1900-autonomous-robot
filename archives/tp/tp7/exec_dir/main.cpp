#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "../lib_dir/del.h"
#include "../lib_dir/moteur.h"
#include "../lib_dir/debug.h"
#include "../lib_dir/bouton.h"
#include "../lib_dir/timer1.h"
    
volatile uint8_t gMinuterieExpiree;

ISR(TIMER1_COMPA_vect)
{
    gMinuterieExpiree = 1;
}

//*************************************************
// creation de la classe bouton pour l'ISR
// ************************************************
int main() {

    // ************************************************
	// Les tests pour notre classe Moteur

    Moteur moteur;

    // Le robot ne devrait pas bouger puisque le % est mis � 0.
    moteur.accelerer(Moteur::ARRIERE, 0);
    _delay_ms(1000);

    // Le robot devrait effectuer un virage sur place � droite
    // ensuite � gauche, avancer puis reculer et finir par s'arr�ter.
    moteur.virer90(Moteur::DROITE);
    moteur.virer90(Moteur::GAUCHE);
    
    for (int i = 0; i < 100; i++)
    {
        moteur.accelerer(Moteur::AVANT, 70);
        _delay_ms(15); //nombres aleatoires pour tester le fonctionnement 
    }

    for (int i = 0; i < 100; i++)
    {
        moteur.accelerer(Moteur::ARRIERE, 70);
        _delay_ms(15); //nombres aleatoires pour tester le fonctionnement 
    }
    // tester la fonction virer
    for (int i = 0; i < 100; i++)
    {
        moteur.accelerer(Moteur::AVANT, 70);
        moteur.virer(100,50);
        _delay_ms(15);
    }

    moteur.arreter();
    // ************************************************
    _delay_ms(2000); //nombres aleatoires pour tester le fonctionnement 

    // ************************************************
	// Les tests pour notre classe Del
    
    Del del(Del::VERT, &PORTC, 2, 3); 
    del.activer();
    _delay_ms(1000);
    del.fixerCouleur(Del::ROUGE);
    _delay_ms(1000);
    del.fixerCouleur(Del::ETEINT);
    
    INITIALISER_UART();
    while (true)
        DEBUG_TRANSMETTRE(("Testing"));

    Timer1 timer1(Timer1::CTC, 0, F_CPU/1024, 1024);

    timer1.start();

    do
    {
        // attendre qu'une des deux variables soit modifiée
        // par une ou l'autre des interruptions.
    } while (gMinuterieExpiree == 0);

    del.fixerCouleur(Del::VERT);
    _delay_ms(2000);
    del.desactiver();
    // Aucune couleur ne devrait être émise puisque Del inactive
    del.fixerCouleur(Del::VERT);

	return 0;
}