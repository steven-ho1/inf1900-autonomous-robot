#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

const int16_t pause = 2000;

void creeDelaiMs(double delai)
{
    for (int i = 0; i < delai; i++)
    {
        _delay_ms(1);
    }
}

void arreter()
{
    PORTB &= ~(1 << PB1);
}

void activer()
{
    PORTB |= 1 << PB1;
}

void changerDutyCycle(double dutyCycle, double periode)
{
    for (int i = 0; i < pause / periode; i++)
    {
        activer();
        creeDelaiMs((dutyCycle / 100) * periode);
        arreter();
        creeDelaiMs(((100 - dutyCycle) / 100) * periode);
    }
}

int main()
{
    DDRB |= 1 << PB0 | 1 << PB1;

    PORTB |= 1 << PB0; // xxxx xxx1 -- Créer fonction plus tard

    double frequence = 60;
    int dutyCycle = 0; // en us

    while (true)
    {
            double periode = 1 / frequence * 1000;
            switch (dutyCycle)
            {
            case 0:
                changerDutyCycle(dutyCycle, periode);
                dutyCycle = 25;
                break;
            case 25:
                changerDutyCycle(dutyCycle, periode);
                dutyCycle = 50;
                break;
            case 50:
                changerDutyCycle(dutyCycle, periode);
                dutyCycle = 75;
                break;
            case 75:
                changerDutyCycle(dutyCycle, periode);
                dutyCycle = 100;
                break;
            case 100:
                changerDutyCycle(dutyCycle, periode);
                if (frequence == 400)
                    frequence = 60; 
                else
                    frequence = 400;
                dutyCycle = 0;
                break;
        }
    }
}