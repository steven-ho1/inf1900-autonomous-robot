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

    Timer1(uint16_t ocrValue = 7812, uint16_t prescaler = 1024, Mode mode = CTC, uint16_t initialCount = 0);

    ~Timer1() {};

    void start();
    void stop();

    // Les méthodes setters doivent être suivies de start() pour prendre effet
    void setMode(Mode mode);
    void setPrescaler(uint16_t prescaler);
    void setOcrValue(uint16_t ocrValue);

private:
    void configurateMode();
    void configuratePrescaler();

    Mode mode_;
    uint16_t initialCount_;
    uint16_t ocrValue_;
    uint16_t prescaler_;
};


