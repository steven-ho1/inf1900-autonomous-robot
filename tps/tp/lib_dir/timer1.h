#pragma once

#ifndef F_CPU
    #define F_CPU 8000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

class Timer1
{
public:
    enum Mode 
    {
        Normal,
        CTC,
    };

    Timer1(Mode mode, uint16_t initialCount, uint16_t ocrValue, uint16_t prescaler);

    ~Timer1() {};

    void start();
    void setMode(Mode mode);
    void setPrescaler(uint16_t prescaler);
    void setOcrValue(uint16_t ocrValue);
    
private:
    void configurateMode();
    void configuratePrescaler();
    void enableInterrupt();

    Mode mode_;
    uint16_t initialCount_;
    uint16_t ocrValue_;
    uint16_t prescaler_;
};


