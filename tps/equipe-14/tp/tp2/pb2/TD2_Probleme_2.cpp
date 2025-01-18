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

int main()
{
  
  
    DDRA |= (1<< PA0); 
    DDRA |= (1<< PA1); 
    //DDRD &= ~(1<< PD1); 
 
       
  enum class Etat {Init , etat_1, etat_2};

  Etat etat = Etat::Init;
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
   return 0;
}