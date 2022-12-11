/* Classe suiveurLigne :
*  permet de lire les signaux recus par le capteur suiveur de
*  ligne et diriger le robot, a marcher droit
*  tourner gauche ou droit, en ajustant le pwm de ces roues.
*/
#pragma once
#ifndef F_CPU
# define F_CPU 8000000
#endif
 
#define EXTREME_GAUCHE (PINA & (1 << PA0)) 
#define MILIEU_GAUCHE  (PINA & (1 << PA1))
#define MILIEU         (PINA & (1 << PA2))
#define MILIEU_DROITE  (PINA & (1 << PA3))
#define EXTREME_DROITE (PINA & (1 << PA4))
#define VITESSE_PAR_DEFAUT 40
#define AJUSTEMENT_VIRAGE 15
#define AJUSTEMENT_ELEVE 25
#define AJUSTEMENT_MOYEN 15
#define AJUSTEMENT_FAIBLE 10
#define DELAI_CONFIRMATION_VIRAGE 100

#include <avr/io.h> 
#include <util/delay.h>
#include "moteur.h"
#include "timer1.h"
#include "piezo.h"

class SuiveurLigne
{
public:
    SuiveurLigne();
    ~SuiveurLigne();

    void deplacerRobot();

    bool detecterIntersection();
    bool detecterDepart();

    void partirStationner();
    void sortirBoucle();

    uint8_t obtenirCompteIntersections();

    void activerVirage();
    void desactiverVirage();

private:
    Moteur moteur_;
    Timer1 timer1_;
    Piezo piezo_;

    bool minuterieExpiree_;
    uint8_t nIntersections_;
    bool virageActive_;
};