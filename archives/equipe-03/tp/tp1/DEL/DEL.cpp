#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

int main()
{
    DDRA = 0x03;
    int pause = 2000; // en s

    for (;;)
    {
        PORTA = 1 << 0; // 0x01 
        _delay_ms(pause); 
        PORTA = 1 << 1; // 0x02
        _delay_ms(pause);

    }
}