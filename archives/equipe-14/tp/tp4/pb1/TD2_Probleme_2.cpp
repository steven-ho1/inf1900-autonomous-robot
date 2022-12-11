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
enum Etat {Init , etat_1, etat_2};
volatile Etat etat= Init;


void checkBoutonPoussoir(){
  _delay_ms(10);
  if (PIND & 0x04){
    etatBouton= true;
  }
    
  else {
    etatBouton= false;
  }
    
  
}

void changementLumiere(){
    

  
  while (true)
  {

    switch (etat)
    {
      case (Etat::Init):
        PORTA &= (0<< PA1);
        PORTA |=  (1<< PA0);
        if (PIND & 0x04)
        {
          while (PIND & 0x04)
          {
            PORTA &= (1<< PA1);
            PORTA |= ~(1<< PA0);
            _delay_ms(10);
            PORTA &= ~(1<< PA1);
            PORTA |= (1<< PA0);
            _delay_ms(10);
          }
          etat = Etat::etat_1;
        }
        
        break;

      case (Etat::etat_1):
        PORTA |= (1<< PA1);
        PORTA &= ~(1<< PA0);
        if (PIND & 0x04)
        {
          while (PIND & 0x04)
          {
            PORTA &= ~(1<< PA1);
            PORTA |= (1<< PA0);
          }
          etat = Etat::etat_2;
        }  
        break;

      case (Etat::etat_2):
        
        PORTA &= ~(1<< PA1);
        PORTA &= ~(1<< PA0);
        if (PIND & 0x04)
        {
          while (PIND & 0x04)
          {
            PORTA |= (1<< PA1);
            PORTA &= ~(1<< PA0);
          }
          etat = Etat::Init;
        }
        break;



}

}

}

ISR ( INT0_vect ) {

  
// laisser un délai avant de confirmer la réponse du

// bouton-poussoir: environ 30 ms (anti-rebond)

_delay_ms ( 30 );

// se souvenir ici si le bouton est pressé ou relâché
checkBoutonPoussoir();

// changements d'état tels que ceux de la

// semaine précédente
changementLumiere();

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













int main()
{
  initialisation();
  
  while(true){
    changementLumiere();
  }
  
  return 0;
    }
     
   
   
