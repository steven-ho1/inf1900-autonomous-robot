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

   for(;;)  // boucle sans fin
   {
      PORTA = 0x02;
      
      _delay_ms(2000);
      PORTA = 0x01;

      _delay_ms(2000);
      for (i=1; i <= 100; i+=1)
           {
            _delay_ms(10);
            PORTA = 0x02;

            _delay_ms(10);
            PORTA = 0x01;

           }
    }

           
   return 0;
}