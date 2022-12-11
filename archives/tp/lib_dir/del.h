#pragma once

#include <avr/io.h>

class Del
{
public:
	// Ressource consultée: 
	// https://www.learncpp.com/cpp-tutorial/nested-types-in-classes/
	enum Couleur
	{
		ETEINT,
		VERT,
		ROUGE
	};

	Del(Couleur couleur = ETEINT, volatile uint8_t* port = &PORTA,
		uint8_t positionBroche1 = 0, uint8_t positionBroche2 = 1);

	~Del() {};
	
	void activer();
	void fixerCouleur(Couleur couleur);
	void desactiver();
	
private:
	void changerCouleur();
	void determinerDdr();

	Couleur couleur_;
	bool 	estActive_;
	volatile uint8_t* port_;
	volatile uint8_t* DDR_;
	uint8_t positionBroche1_;
	uint8_t positionBroche2_;
};