/*
  * Nom: TD2_Probleme_2
  *
  * auteurs : Nazim Bertouche & Lawali Mahamane Yacine
  * 08 septembre 2022
  * description du code: implementation de la table des etats ci dessous 
  */
  // Table des etats
  
  // Etats          Action          Etat suivant            DEL

  // INIT        poussoir pese            /                ambree
  // INIT    poussoir pese-relache       etat_1            verte
  // etat_1       poussoir pese           /                rouge
  // etat_1  poussoir pese-relache       etat_2            eteinte
  // etat_2       poussoir pese           /                verte
  // etat_1  poussoir pese-relache       INIT            rouge






#define F_CPU 12000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>



volatile bool etatBouton = false;


volatile uint8_t gMinuterieExpiree;
volatile uint8_t gBoutonPoussoir; 
volatile bool minuterieBegin;


void checkBoutonPoussoir(){
  _delay_ms(10);
  if (PIND & 0x04){
    etatBouton= true;
  }
    
  else {
    etatBouton= false;
  }
    
  
}

ISR ( TIMER1_COMPA_vect ) {

gMinuterieExpiree = 1;

}


ISR ( INT0_vect) {

gBoutonPoussoir = 1;

// anti-rebond

checkBoutonPoussoir();

}

void partirMinuterie ( uint16_t duree ) {
minuterieBegin= true;
gMinuterieExpiree = 0;

// mode CTC du timer 1 avec horloge divisée par 1024

// interruption après la durée spécifiée

TCNT1 = 0;

OCR1A = duree;

TCCR1A |= ;

TCCR1B |=  ;

TCCR1C = 0;

TIMSK1 |= ;

}









void initialisation ( void ) {

// cli est une routine qui bloque toutes les interruptions.

// Il serait bien mauvais d'être interrompu alors que

// le microcontrôleur n'est pas prêt...

cli ();


// configurer et choisir les ports pour les entrées

// et les sorties. DDRx... Initialisez bien vos variables


    DDRA |= (1<< PA0); 
    DDRA |= (1<< PA1); 
    DDRD &= ~(1<< PD2); 
 

// cette procédure ajuste le registre EIMSK

// de l’ATmega324PA pour permettre les interruptions externes

EIMSK |= (1<< INT0) ;


// il faut sensibiliser les interruptions externes aux

// changements de niveau du bouton-poussoir

// en ajustant le registre EICRA

EICRA |= (1<< ISC00);


// sei permet de recevoir à nouveau des interruptions.

sei ();

}

void routine(){
  do {

// attendre qu'une des deux variables soit modifiée

// par une ou l'autre des interruptions.

} while ( gMinuterieExpiree == 0 && gBoutonPoussoir == 0 );


// Une interruption s'est produite. Arrêter toute

// forme d'interruption. Une seule réponse suffit.

cli ();

// Verifier la réponse

if (gBoutonPoussoir){
  PORTA= 0x01;
}
else {
  PORTA = 0x02;
}
}



int main()
{
  initialisation();
  _delay_ms(10000);
  PORTA=0x02;
  _delay_ms(100);
  PORTA =0x00;
  partirMinuterie(0x1E84);
  routine();
  
  
  return 0;
}
     
   
   
