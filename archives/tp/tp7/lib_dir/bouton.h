#pragma once

#ifndef F_CPU
# define F_CPU 8000000
#endif

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

class Bouton {
public:
	enum TYPE_PRESSION
	{
		POUSSER,
		RELACHER,
		POUSSER_ET_RELACHER,
	};
	Bouton(volatile uint8_t interup = 0 , TYPE_PRESSION pression = POUSSER_ET_RELACHER);

	~Bouton() {};

	bool CheckIfPressed();
	uint8_t getBoutonPoussoir();
	bool getPressed();
	void setVect();

private:
	void initialisationIntExterne(TYPE_PRESSION pression);

	TYPE_PRESSION pression_;
	uint8_t boutonPoussoir_;
	bool pressed_;
	volatile uint8_t* vect_;
	
};