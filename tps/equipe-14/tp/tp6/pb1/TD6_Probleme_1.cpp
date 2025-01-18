/*
  * Nom: TD6_Probleme_1
  * auteurs : Nazim Bertouche & Lawali Mahamane Yacine
  * 17 octobre 2022
  * description du code: apres avoir appuyer sur le bouton blanc placer sur le breadboard un compteur qui augmente de 10 chaque minutes 
  se declenche, si on relache le bouton blanc ou si le compteur atteint 120 , on aura une lumiere vert qui va clignoter pendant 1/2 secondes Ensuite, la carte mère ne fait rien.
   Puis, deux secondes plus tard, la lumière rouge s'allume. Elle va clignoter (compteur / 2) fois au rythme de 2 fois par seconde. 
  Ensuite, la lumière tourne au vert pendant une seconde. Finalement, elle s'éteint et le robot revient à son état original.

  *description du materiel: configuration du breadboard en le connectant a notre carte mere via un fil de donner vert-gris
   et un fil rouge-noir relier au VCC et au GRN de ces broches. On a egalement  place une resistance de 10K sur le breadboard et deux fils noir et bleus qui relient des parties 
   du breadbord;
   on admet que si PB0 = 0 et PB1 = 1 la led s<allume verte 
*/

#define F_CPU 12000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>



volatile uint8_t etat = 0;
volatile uint8_t compteur = 0;
const int rebond= 10 ;
const int delay1s = 1000; 

ISR ( INT0_vect ) {

_delay_ms ( rebond );

// changements d'état
if (etat==3)

  etat==0;

else

  etat++;


// Voir la note plus bas pour comprendre cette instruction et son rôle

EIFR |= (1 << INTF0) ;

}




void initialisation ( void ) {

// cli est une routine qui bloque toutes les interruptions.

// Il serait bien mauvais d'être interrompu alors que

// le microcontrôleur n'est pas prêt...

cli ();


// configurer et choisir les ports pour les entrées

// et les sorties. DDRx... Initialisez bien vos variables


    DDRB |= (1<< PB0); 

    DDRB |= (1<< PB1); 

    DDRD &= ~(1<< PD1); 
 

// cette procédure ajuste le registre EIMSK

// de l’ATmega324PA pour permettre les interruptions externes

EIMSK |= (1<< INT0) ;


// il faut sensibiliser les interruptions externes aux

// changements de niveau du bouton-poussoir

// en ajustant le registre EICRA

EICRA |= (1<< ISC00);

sei();

}


void lumiere ()
{

  switch(etat)
      {

        case 0:

          PORTB &=  ~(1 << PB0);
          PORTB &=  ~(1 << PB1);

          break;
        
        case 1:

            if (compteur < 120)
            {
                compteur+=10; // un ajout de 10 au compteur chaque seconde 
                _delay_ms(delay1s);

            }

            else 

                etat=2;

            break;
          
        case 2:

            for ( int i=0; i<5; i++)

            {
                PORTB &=  ~(1 << PB0);
                PORTB |=  (1 << PB1);

                _delay_ms(50);

                PORTB &=  ~(1 << PB0);
                PORTB &=  ~(1 << PB1);
                
                _delay_ms(50);

            }

            etat = 3;
            break;

        case 3 : 
            _delay_ms (2000);

            for (int i = 0; i < (compteur/2) ; i++){

              PORTB &=  ~(1 << PB0);
              PORTB |=  (1 << PB1);

              _delay_ms(250);

              PORTB &=  ~(1 << PB0);
              PORTB &=  ~(1 << PB1);

              _delay_ms(250);

          }

          PORTB |=  (1 << PB0);
          PORTB &=  ~(1 << PB1);

          _delay_ms(1000);

          PORTB &=  ~(1 << PB0);
          PORTB &=  ~(1 << PB1);
          
          etat = 0;
          break;

      }


}

int main(){

  compteur = 0;

  initialisation();

  lumiere();
}
