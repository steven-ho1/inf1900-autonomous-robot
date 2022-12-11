#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "memoire_24.h"

void initialiserUART(void)
{
    // 2400 bauds. Nous vous donnons la valeur des deux
    // premiers registres pour vous éviter des complications.
    UBRR0H = 0;
    UBRR0L = 0xCF;
    // permettre la réception et la transmission par le UART0
    UCSR0A = 0;
    UCSR0B = 1 << TXEN0;
    // Format des trames: 8 bits, 1 stop bits, sans parité

    // Asynchronous USART par défaut

    // 8 bits
    UCSR0C &= ~(1 << UCSZ02);
    UCSR0C |= (1 << UCSZ01 | 1 << UCSZ00);

    // 1 stop bit par défaut

    // Sans parité par défaut
}

// Du USART vers le PC
void transmettreViaUART(uint8_t donnee)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    /* Put data into buffer, sends the data */
    UDR0 = donnee;
}

int main()
{
    initialiserUART();

    Memoire24CXXX memoire;

    uint8_t donnees[] = "Cette chaîne de caractères se trouvait dans la mémoire externe de la carte-mère!\n";
    uint8_t donneeLue = 0; 
    uint16_t adresse = 0x0000;
    uint8_t longueur = sizeof(donnees);

    memoire.ecriture(adresse, donnees, longueur);

    while (true)
    {
        memoire.lecture(adresse++, &donneeLue);

        if (donneeLue == 0xFF)
            break;
        
        transmettreViaUART(donneeLue);

        // test
        if (adresse == sizeof(donnees))
        {
            adresse = 0x0000;
        }
    }
}