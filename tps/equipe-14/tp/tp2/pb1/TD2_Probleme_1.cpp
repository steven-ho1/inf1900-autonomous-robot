/*
  * Nom: TD2_Probleme_1
  * auteurs : Nazim Bertouche & Lawali Mahamane Yacine
  * 08 septembre 2022
  * description du code: impl/mentation de la table des etats ci dessous 
  */
  //Table des etats

  // Etats          Action          Etat suivant            DEL

  // INIT           poussoir            etat_1               /
  // etat_1         poussoir            etat_2               /
  // etat_2         poussoir            etat_3              verte
  // etat_3       attendre 2 s           INIT              eteinte


  // description du materiel : nos sorties A1, A2 sont reliees a notre DEL libre 
  //on admet que la DEL s allume vert si A1==1 et A0==0

#define F_CPU 12000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>


int main()
{
  
    DDRA |= (1<< PA0); 
    DDRA |= (1<< PA1); 
    DDRD &= ~(1<< PD1); 
 
       
  enum class Etat {Init , etat_1, etat_2 , etat_3};

  Etat etat = Etat::Init;
  while (true)
  {

    switch (etat)
    {
      case (Etat::Init):
        if (PIND & 0x04)
        {
          while (PIND & 0x04)
          {
            etat = Etat::Init;
          }
          etat = Etat::etat_1;
        }
        PORTA &= ~(1<< PA0);
        PORTA &= ~(1<< PA1);
        break;

      case (Etat::etat_1):
        if (PIND & 0x04)
        {
          while (PIND & 0x04)
          {
            etat = Etat::etat_1;
          }
          etat = Etat::etat_2;
        }  
        break;

      case (Etat::etat_2):
        if (PIND & 0x04)
        {
          while (PIND & 0x04)
          {
            etat = Etat::etat_2;
          }
          etat = Etat::etat_3;
        }
        break;

      case(Etat::etat_3):
        PORTA &= ~(1<< PA0);
        PORTA |= (1<< PA1);   
        _delay_ms(2000);
        etat = Etat::Init;
        break;


    }
  }
   return 0;
}