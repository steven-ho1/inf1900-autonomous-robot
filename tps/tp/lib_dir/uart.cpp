#include "uart.h"

void Uart::initialiserUart()
{
    // 2400 bauds. Nous vous donnons la valeur des deux.
    UBRR0H = 0;
    UBRR0L = 0xCF;
    // Permettre la réception et la transmission par le UART0.
    UCSR0A = 0;
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0);

    // Format des trames: 8 bits, 1 stop bits, sans parité.

    // Asynchronous USART par défaut.

    // 8 bits.
    UCSR0C &= ~(1 << UCSZ02);
    UCSR0C |= (1 << UCSZ01 | 1 << UCSZ00);

    // 1 stop bit par défaut.

    // Sans parité par défaut.
}

// Transmettre un caractère.
void Uart::transmettreViaUart(char caractere)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)));
    /* Put data into buffer, sends the data */
    UDR0 = caractere;
}

// Transmettre une chaîne de caractères.
void Uart::transmettreViaUart(const char *chaine)
{
    for (uint8_t i = 0; i < strlen(chaine); i++)
    {
        transmettreViaUart(chaine[i]);
    }
}

// Recevoir un caractère.
uint8_t Uart::recevoirViaUart(void) 
{
    while (!((UCSR0A) & (1 << RXC0))); //RXC0 = USART COMPLETRE FLAG
    return UDR0; //UDR0 = RECEIVE BUFFER
}