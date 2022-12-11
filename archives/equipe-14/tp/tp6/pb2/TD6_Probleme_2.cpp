/*
  * Nom: TD6_Probleme_1
  * auteurs : Nazim Bertouche & Lawali Mahamane Yacine
  * 17 octobre 2022
  * description du code : en fonction de l intensite de la lumiere que va capter notre photoresistance on observera un changement de couleur dont rouge si elle 
    est trop forte , ambre si elle est normal et vert si elle est faible;

  *description materielle: configuration du breadboard en le connectant a notre carte mere via un fil de donner vert-gris
   et un fil rouge-noir relier au VCC et au GRN de ces broches. On a egalement  place une resistance de 100K sur le breadboard et deux fils noir et bleus qui relient des 
   parties du breadbord et enfin une photoresitance qui reagit a l intensite de la lumiere .
*/

#define F_CPU 12000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <can.h>

const int Sortie=0xff;
const int Entree= 0x00;

//const int SORTIE_0_1 = 0x03;

//const int ENTREE_0_1 = 0x00;

void changementDeLumiere(){
  while (true)
  {
    can sensor = can();
    int dureeAmbree = 50;
    uint16_t intensite;
    uint8_t intensite1;
    intensite = sensor.lecture(1); 

    intensite1 = intensite >> 2; 

                              

    if(intensite1 < 150){ //150 determine par essaie-erreur au labo 

          PORTB &= ~(1 << PB0);
          PORTB |=  (1 << PB1);

      }

      else if(intensite1> 250){ //250 determine par essaie-erreur au labo

          PORTB |=  (1 << PB0);
          PORTB &= ~(1 << PB1);
      }

      else {

        PORTB &= ~(1 << PB0);
        PORTB |=  (1 << PB1);

        _delay_ms(dureeAmbree);

        PORTB |=  (1 << PB0);
        PORTB &= ~(1 << PB1);

        _delay_ms(dureeAmbree);
      }
  }

  void ajustementEntreSortie(){
      DDRB |=(1<< PB0);
      DDRB |=(1<< PB1);

      DDRA &=~(1<< PA1);

  }


}
int main(){

    ajustementEntreSortie();

    changementDeLumiere();

  return 0;
}
