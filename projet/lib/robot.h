/* Classe suiveurLigne :
*  permet de lire les signaux recus par le capteur suiveur de
*  ligne et diriger le robot, a marcher droit
*  tourner gauche ou droit, en ajustant le pwm de ces roues.
*/

#pragma once

#ifndef F_CPU
# define F_CPU 8000000
#endif

#define DELAI_VIRAGE_INTERSECTION 700
#define DELAI_PREPARATION_SALUT 500
#define DELAI_VIRAGE_SALUT 500
#define DELAI_RECUL_SALUT 500
#define DELAI_ARRET_SALUT 1000

#include <avr/io.h> 
#include <stdlib.h>
#include "capteurDistance.h"
#include "selecteur.h"
#include "moteur.h"
#include "suiveurLigne.h"
#include "timer1.h"

class Robot
{
public:
    static Robot& controler();

    void initialiser();
    void effectuerParcours();
    
    void gererInterruptionBouton(uint8_t noInterruption, volatile bool gBoutonAppute);
	void gererInterruptionMinuterie();

    Timer1& accederTimer1();

private:
    Robot();
    ~Robot();
    void deplacer();

    static Robot robot_;

    CapteurDistance capteurDistance_;
    Selecteur selecteur_;
    SuiveurLigne suiveurLigne_;
    Timer1 timer1_;
    Moteur moteur_;
    Memoire24 memoire_;

    Point point_;

    bool avancePermise_;
    bool arretActive_;
    bool detectionIntersectionActive_;
    uint8_t nPoteaux_;
    uint8_t poteauxRestants_;
    uint8_t positionPoteau_;
    bool doitSaluer_;
    bool salueTermine_;
};
