#include "timer1.h"

Timer1::Timer1(uint16_t ocrValue, uint16_t prescaler, 
               Mode mode, uint16_t initialCount):
    mode_(mode),
    initialCount_(initialCount),
    ocrValue_(ocrValue),
    prescaler_(prescaler)
{
    
}

void Timer1::start()
{
    cli();
    TCNT1 = initialCount_;
    configurateMode();
    OCR1A = ocrValue_;
    configuratePrescaler();
    
    TCCR1C = 0;
    TIMSK1 |= (1 << OCIE1A); // ISR nécessaire   
    sei(); 
}

void Timer1::stop()
{
    // Dissocier l'horloge du Timer1
    TCCR1B &= ~(1 << CS12 | 1 << CS11 | 1 << CS10); 

    // Désactiver les interruptions
    TIMSK1 &= ~(1 << OCIE1A);
}


void Timer1::configurateMode()
{
    switch(mode_) 
    {
        case Normal :  
            TCCR1A &= ~(1 << WGM11 | 1 << WGM10);
            TCCR1B &= ~(1 << WGM13 | 1 << WGM12);
            break;
        case CTC :  
            // CTC mode, OCnA/OCnB disconnected
            TCCR1A &= ~(1 << WGM11 | 1 << WGM10);
            TCCR1B |= 1 << WGM12;
            TCCR1B &= ~(1 << WGM13);
            break;
    }
}    

void Timer1::setMode(Mode mode)
{
    mode_ = mode;
}

void Timer1::setOcrValue(uint16_t ocrValue)
{   
    ocrValue_ = ocrValue;  
}

void Timer1::setPrescaler(uint16_t prescaler)
{
    prescaler_ = prescaler;
}

void Timer1::configuratePrescaler() 
{
     switch(prescaler_) 
     {
        case 1 :
            TCCR1B |= (1 << CS10);
            TCCR1B &= ~(1 << CS12 | 1 << CS11);
            break;
        case 8 :
            TCCR1B |= (1 << CS11);
            TCCR1B &= ~(1 << CS12 | 1 << CS10);
            break;
        case 64 :
            TCCR1B |= (1 << CS11) | (1 << CS10);
            TCCR1B &= ~(1 << CS12);
            break;
        case 256 :
            TCCR1B |= (1 << CS12);
            TCCR1B &= ~(1 << CS11 | 1 << CS10);
            break;
        case 1024 :
            TCCR1B |= (1 << CS12) | (1 << CS10);
            TCCR1B &= ~(1 << CS11);
            break;
     }
}