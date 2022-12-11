#pragma once

#ifndef F_CPU
	# define F_CPU 8000000
#endif

#define DIRECTION_MOTEUR_GAUCHE 1 << PB2
#define DIRECTION_MOTEUR_DROIT 1 << PB1
#define AJUSTEMENT_VITESSE_GAUCHE 8 // Roue gauche plus lente

#define VITESSE_DEPART 60
#define DELAI_DEPART 200

#include <avr/io.h>
#include <util/delay.h>

enum class Direction
{
	AVANT,
	ARRIERE,
};

class Moteur
{

public:
	Moteur();

	~Moteur();

	void accelerer(uint8_t rapportCycliqueGauche, uint8_t rapportCycliqueDroit, 
				   Direction direction = Direction::AVANT);

	void accelerer(uint8_t rapportCyclique, Direction direction = Direction::AVANT);

	void arreter();
private:
	void ajusterPwm(uint8_t rapportCycliqueGauche, uint8_t rapportCycliqueDroit);
};
