#pragma once
#ifndef F_CPU
	# define F_CPU 8000000
#endif

#define F_CPU_SUR_PRESCALER F_CPU/256
#define F_TEMPS_ACTIF frequence * 2

#include <avr/io.h>

class Piezo
{
public:

    enum Note
    {
        // Basé sur https://pages.mtu.edu/~suits/notefreqs.html
        // '_' = '#' dans ce cas-ci
        A2  = 45,
        A_2 = 46,
        B2  = 47,
        C3  = 48,
        C_3 = 49,
        D3  = 50,
        D_3 = 51,
        E3  = 52,
        F3  = 53,
        F_3 = 54,
        G3  = 55,
        G_3 = 56,
        A3  = 57,
        A_3 = 58,
        B3  = 59,
        C4  = 60,
        C_4 = 61,
        D4  = 62,
        D_4 = 63,
        E4  = 64,
        F4  = 65,
        F_4 = 66,
        G4  = 67,
        G_4 = 68,
        A4  = 69,
        A_4 = 70,
        B4  = 71,
        C5  = 72,
        C_5 = 73,
        D5  = 74,
        D_5 = 75,
        E5  = 76,
        F5  = 77,
        F_5 = 78,
        G5  = 79,
        G_5 = 80,
        A5  = 81
    };

    // Mets les broches PD7 et PD6 en mode sortie, et envoie 0V dans PD6.
    Piezo();

    ~Piezo() {};

    void arreterSon();
    void jouerNote(uint8_t operande);

private:
    // Utilisation du Timer2 pour générer du PWM.
   void genererPwm(uint8_t duree);
};


