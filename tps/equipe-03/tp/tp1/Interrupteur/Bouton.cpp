#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

struct Bouton
{
    bool appuye;

    bool lireEtat()
    {
        uint8_t lecture1 = PIND & 0x04;
        _delay_ms(10);
        uint8_t lecture2 = PIND & 0x04;

        return lecture1 && lecture2;
    }
};

int main()
{
    DDRA = 0x03;
    DDRD = 0x00;

    uint8_t choixLED = 1;
    Bouton bouton = {false};

    while (true)
    {
        if (bouton.lireEtat())
        {
            bouton.appuye = true;
            
            switch (choixLED)
            {
            case 1:
                PORTA = 1 << 0;
                break;
            case 2:
                PORTA = 1 << 1;
                break;
            case 3:
                PORTA = 1 << 0;
                _delay_ms(5);
                PORTA = ~PORTA;
                _delay_ms(5);
                break;
            }
        }
        else
        {
            PORTA = 0x00;
            if (bouton.appuye)
            {
                choixLED++;
                if (choixLED > 3)
                    choixLED = 1;
                
                bouton.appuye = false;
            }
        }
    }
}