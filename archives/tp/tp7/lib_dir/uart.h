#pragma once

#include <avr/io.h>
#include <string.h>

class Uart
{
public:
    Uart() {};
    ~Uart() {};    

    void initialiserUart();

    // Du USART vers le PC
    void transmettreViaUart(char caractere);
    void transmettreViaUart(const char* chaine);
};