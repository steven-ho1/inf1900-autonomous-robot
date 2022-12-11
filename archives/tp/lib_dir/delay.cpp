#include "delay.h"

void creerDelaiMs(uint16_t delai)
{
    for (uint16_t i = 0; i < delai; i++)
    {
        _delay_ms(1);
    }
}

void creerDelaiUs(uint16_t delai)
{
    for (uint16_t i = 0; i < delai; i++)
    {
        _delay_us(1);
    }
}