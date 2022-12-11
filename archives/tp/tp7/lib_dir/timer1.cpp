#include "timer1.h"

Timer1::Timer1(Mode mode, uint16_t initialCount, uint16_t ocrValue, uint16_t prescaler)
{
    mode_ = mode; 
    initialCount_ = initialCount;
    ocrValue_ = ocrValue;
}

void Timer1::start()
{
    TCNT1 = initialCount_;
    configurateMode();
    OCR1A = ocrValue_;
    configuratePrescaler();
    enableInterrupt(); // ISR nécessaire
}

void Timer1::setMode(Mode mode)
{
    mode_ = mode;
    configurateMode();   
}

void Timer1::configurateMode()
{
    switch(mode_) 
    {
        case Normal :  
            // Mode by default
            break;
        case CTC :  
            // CTC mode, OCnA/OCnB disconnected
            TCCR1B |= 1 << WGM12;
            break;
    }
}    

void Timer1::setOcrValue()
{   
    OCR1A = ocrValue_;  
}

void Timer1::setPrescaler(uint16_t prescaler)
{
    prescaler_ = prescaler;
}

void Timer1::configuratePrescaler()
{
     switch(prescaler_) 
     {
        case 0 :
            // CS12, CS11 and CS10 equal to 0 by default
            break;
        case 1 :
            TCCR1B |= (1 << CS10);
            break;
        case 8 :
            TCCR1B |= (1 << CS11);
            break;
        case 64 :
            TCCR1B |= (1 << CS11) | (1 << CS10);
            break;
        case 256 :
            TCCR1B |= (1 << CS12);
            break;
        case 1024 :
            TCCR1B |= (1 << CS12) | (1 << CS10);
            break;
     }
}

void Timer1::enableInterrupt()
{
    TIMSK1 |= (1 << OCIE1A);
}