#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

const uint8_t TOP = 0xFF;
const uint16_t PAUSE = 2000; // en ms

void ajustementPwm(uint16_t rapportCyclique)
{
    // mise à un des sorties OC1A et OC1B sur comparaison
    // réussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5
    // page 130 de la description technique du ATmega324PA)
    OCR1A = TOP * rapportCyclique / 100; // donc, valeur max de compteur est 0xFF
    OCR1B = TOP * rapportCyclique / 100;
    // division d'horloge par 8 - implique une fréquence de PWM fixe
    TCCR1A &= ~(1 << WGM11);
    TCCR1A |= (1 << WGM10);
    TCCR1A |= (1 << COM1A1 | 1 << COM1B1);
    TCCR1A &= ~(1 << COM1A0 | 1 << COM1B0);

    TCCR1B &= ~(1 << WGM13 | 1 << WGM12 | 1 << CS12 | 1 << CS10);
    TCCR1B |= (1 << CS11);

    TCCR1C = 0;
}

int main()
{
    DDRD |= (1 << PD4) | (1 << PD5 | 1 << PD6 | 1 << PD7); // PD4 - OC1B, PD5 - OC1A

    PORTD &= ~(1 << PD6 | 1 << PD7);


    while (true)
    {
        ajustementPwm(0);
        _delay_ms(PAUSE);

        ajustementPwm(25);
        _delay_ms(PAUSE);

        ajustementPwm(50);
        _delay_ms(PAUSE);

        ajustementPwm(75);
        _delay_ms(PAUSE);

        ajustementPwm(100);
        _delay_ms(PAUSE);
        
    }
}