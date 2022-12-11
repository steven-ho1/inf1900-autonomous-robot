#pragma once

#ifndef F_CPU
# define F_CPU 8000000
#endif

#define DELAI_AMBRE 3
#define DELAI_TOTAL_AMBRE 10
#define SEC_EN_MS 1000
#define TEMPS_ACTIF dureeClignotement / 2

#include <avr/io.h>
#include "delay.h"

class Del
{
public:
	// Ressource consultée: 
	// https://www.learncpp.com/cpp-tutorial/nested-types-in-classes/
	enum Couleur
	{
		ETEINT,
		VERT,
		ROUGE,
		AMBRE
	};

	Del(Couleur couleur = ETEINT, volatile uint8_t* port = &PORTA,
		uint8_t positionBroche1 = 0, uint8_t positionBroche2 = 1);

	~Del() {};
	
	void changerCouleur(Couleur couleur);
	void clignoter(uint16_t frequenceClignotement, uint16_t duree);
	
private:
	void determinerDdr();
	void eteindre();
	void allumerVert();
	void allumerRouge();
	void allumerAmbre();

	volatile uint8_t* port_;
	volatile uint8_t* DDR_;
	uint8_t positionBroche1_;
	uint8_t positionBroche2_;
	Couleur couleur_;
};