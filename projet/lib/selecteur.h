#pragma once

#ifndef F_CPU
	# define F_CPU 8000000
#endif

#define FREQUENCE_CLIGNOTEMENT 4
#define DUREE 2

#include <avr/io.h>
#include "bouton.h"
#include "del.h"
#include "point.h"

class Selecteur
{
public:
	Selecteur();

	~Selecteur();

	void initialiser();

	void changerParcours(Point& parcours);

	void confirmerParcours();
	
private:
	void afficherParcours();

	Bouton boutonBlanc_;
	Bouton boutonInterrupt_;
	Del del_;

	Point parcours_;
	bool parcoursConfirme_;
};
