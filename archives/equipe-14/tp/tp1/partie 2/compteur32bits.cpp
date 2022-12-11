/*
  * Nom: compteur 32 bits
  * Copyright (C) 2005 Matthew Khouzam
  * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
  * Description: Ceci est un exemple simple de programme
  * Version: 1.1
  */

#define F_CPU 12000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>

int main()
{
  int i;
   DDRA = 0xff; // PORT A est en mode sortie
   DDRD = 0x00; 
   while (true){
   while (PIND & 0x04) // tant qu on appuie sur le bouton poussoir 
   {
      PORTA = 0x01;
      
       }
       PORTA=0x02;
   }
   return 0;
}