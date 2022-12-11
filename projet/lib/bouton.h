#pragma once

#ifndef F_CPU
# define F_CPU 8000000
#endif

#define DELAI_ANTI_REBOND 30

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

class Bouton {
public:
	enum PRESSION
	{
		APPUIE,
		RELACHE,
		APPUIE_RELACHE
	};

	Bouton(uint8_t noInterruption = INT0, PRESSION pression = APPUIE);
	~Bouton() {};

	void determinerMasque();

	void toutDesactiver();

	bool lireEtat();
	bool obtenirEtat();

private:
	void initialiserInterruption(PRESSION pression);
	void initialiserPort();

	uint8_t noInterruption_;
	uint8_t masqueBouton_;
	bool estAppuye_;
};