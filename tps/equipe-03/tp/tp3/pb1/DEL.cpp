#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

const uint8_t PAUSE_AMBRE = 2;

void estEteint()
{
    PORTA &= ~(1 << PA0 | 1 << PA1);
}

void estRouge()
{
    PORTA &= ~(1 << PA0); // 0x02
    PORTA |= 1 << PA1;
}

void creeDelaiUs(double delai)
{
    for (int i = 0; i < delai; i++)
    {
        _delay_us(1);
    }
}

int main()
{
    DDRA |= (1 << PA0 | 1 << PA1);

    double periode = 1000; // en us
    double total = 3000;
    while (true)
    {
        for (int i = 0; i < total; i++)
        {
            estRouge();
            creeDelaiUs(periode - i * periode/total);
            estEteint();
            creeDelaiUs(i * periode/total);
        }

    }
}