#pragma once

#ifndef F_CPU
# define F_CPU 8000000
#endif

#define POTEAU_PROCHE 0
#define POTEAU_LOIN 1

#define N_ECHANTILLONS 25
#define MAX_LOIN 155 // 60 cm
#define MAX_PROCHE 300 // 30 cm
#define MIN_PROCHE 600 // 15 cm

#define DELAI_ACCES_MEMOIRE 5
#define ADRESSE_N_POTEAUX 5

#define NOTE_AIGUE 80
#define NOTE_GRAVE 50
#define DELAI_SON 1000

#include <avr/io.h> 
#include <stdio.h>
#include <util/delay.h>
#include "memoire24.h"
#include "piezo.h"
#include "moteur.h"
#include "can.h"
#include "point.h"
#include "timer1.h"

class CapteurDistance
{
public:
    CapteurDistance(uint8_t positionBroche = PA6, bool lecturePermise = true);
    ~CapteurDistance();

    void capter();
    void desactiverLecture();
    void activerLecture();

    uint8_t obtenirNombrePoteaux();

private:
    void ecrireMemoire(uint8_t positionPoteau);

    Memoire24 memoire_; 
    Piezo piezo_;
    Moteur moteur_;
    Can convertisseur_;
    Timer1 timer1_;

    uint8_t positionBroche_;

    bool    lecturePermise_;
    uint8_t positionPoteau_;
    uint8_t nPoteaux_;

    // Prendre des échantillons et déterminer la plus petite
    uint16_t echantillons_[N_ECHANTILLONS];
    uint8_t noEchantillon_ = 0;
    bool debuterEchantillonnage_ = false;
};