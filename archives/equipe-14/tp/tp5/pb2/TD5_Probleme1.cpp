/*
  * Nom: TD5_Probleme_2
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
   DDRD = 0xff;
}
void initialisationUART ( void ) {

  // 2400 bauds. Nous vous donnons la valeur des deux

  // premiers registres pour vous éviter des complications.

  UBRR0H = 0;

  UBRR0L = 0xCF;

  // permettre la réception et la transmission par le UART0

  UCSR0A = 0  ;

  UCSR0B = (1<<RXEN0)|(1<<TXEN0) ;

  // Format des trames: 8 bits, 1 stop bits, sans parité

  UCSR0C = (1<<UCSZ11)|(1<<UCSZ10); ;

}
// Du USART vers le PC

void transmissionUART ( uint8_t donnee ) {
while ( !( UCSR0A & (1<<UDRE0)) )
UDR0= donnee;

}





int main(){
    sortie();
  initialisationUART();

    char mots[21] = "Le robot en INF1900\n";

    uint8_t i, j;

    for ( i = 0; i < 100; i++ ) {

    for ( j=0; j < 20; j++ ) {

    transmissionUART ( mots[j] );
    }
    }
      
  return 0;
}
   
