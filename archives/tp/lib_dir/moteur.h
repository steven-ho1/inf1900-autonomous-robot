#pragma once

#ifndef F_CPU
	# define F_CPU 8000000
#endif

#define DELAI_VIRAGE_90 300 // Déterminé par essai et erreur (en ms)
#define RAPPORT_CYCLIQUE_75 75 
#define RAPPORT_CYCLIQUE_0 0  
#define DIRECTION_MOTEUR_GAUCHE 1 << PB2
#define DIRECTION_MOTEUR_DROIT 1 << PB5 

#include <avr/io.h>
#include <util/delay.h>

class Moteur
{

public:
	enum DirectionVerticale
	{
		AVANT,
		ARRIERE,
	};

	enum DirectionHorizontale
	{
		GAUCHE,
		DROITE
	};

	Moteur();

	~Moteur() {};

	void accelerer(DirectionVerticale direction, uint8_t rapportCyclique);

	void virer(uint8_t rapportCycliqueGauche, uint8_t rapportCycliqueDroit);
	void virer90(DirectionHorizontale direction);

	void arreter();

private:
	void ajusterPwm(uint8_t rapportCycliqueGauche, uint8_t rapportCycliqueDroit);
};
