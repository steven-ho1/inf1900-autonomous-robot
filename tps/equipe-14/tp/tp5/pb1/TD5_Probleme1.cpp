/*
  * Nom: TD5_Probleme_1
  *
  * auteurs : Nazim Bertouche & Lawali Mahamane Yacine
  * 
 
  */
  





#define F_CPU 12000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <memoire_24.h>

void sortie(){
   DDRA = 0xff;
}

/*void verificationCompatibilite(uint8_t *donnee){
  if (donnee==lectureEcriture.lecture(0x0000, ptr,taille)){
    _delay_ms(5);
    return true;
  }
    
  else 
    return false

}
*/

/*void lumiere(){
  if (verificationCompatibilite==true){
    PORTA=0x01;
  }
  else
    PORTA=0x02;


}
*/



int main(){
  sortie();
  const char* texte = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
  const int taille= 45;
  
  char donnees[45];
  
  
  Memoire24CXXX lectureEcriture= Memoire24CXXX();
  lectureEcriture.init();
  lectureEcriture.ecriture(0x0000,(uint8_t*)texte,taille);
  _delay_ms(5);
  lectureEcriture.lecture(0x0000,(uint8_t*)donnees,taille);
  _delay_ms(5);

  if (donnees==texte)  {
      PORTA=0x01;// vert
  }  
  else {
    PORTA=0x02;//rouge
  }
    
  

  return 0;
}
   
