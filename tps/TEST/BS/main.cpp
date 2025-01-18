#define F_CPU 8000000
#define DELAI_VIRAGE 1000

#include <avr/io.h>
#include <util/delay.h>
#include "del.h"
#include "piezo.h"
#include "moteur.h"
#include "suiveurLigne.h"
#include "memoire24.h"
#include "timer1.h"

Timer1 timer1(39062); // 5s
enum class Point
{
	B,
	E,
	J,
	M,
	P,
	S
};

volatile bool gMinuterieExpiree = false;
ISR(TIMER1_COMPA_vect)
{
	timer1.stop();
	gMinuterieExpiree = true;
}

int main()
{
	SuiveurLigne sv;
	Memoire24 memoire;
	Moteur moteur;
	uint8_t nbPoto_ = 2;
	uint8_t potoRestant = nbPoto_;
	uint8_t positionPoto;
	Point position = Point::B;
	Piezo piezo;
	Del del(Del::ETEINT, &PORTC, 2, 3);

	while (true)
	{
		sv.deplacerRobot();

		switch (position)
		{
		case Point::B:
			if (sv.detecterDepart())
			{
				moteur.accelerer(60);
				_delay_ms(500);
			}
			if (sv.detecterIntersection())
				position = Point::E;
			break;
		case Point::E:
			piezo.jouerNote(45);
			_delay_ms(50);
			piezo.arreterSon();

			potoRestant -= 1;
			memoire.lecture(potoRestant, &positionPoto);
			_delay_ms(5);
			if (positionPoto == 0)
				moteur.accelerer(40, 15);
			else if (positionPoto == 1)
				moteur.accelerer(15, 40);
			_delay_ms(DELAI_VIRAGE);
			// ce delai existe pour pouvoir sortir de la zone ou il capte 5
			// chute
			position = Point::J;
			break;
		case Point::J:
			if (MILIEU_GAUCHE && MILIEU && MILIEU_DROITE)
			{
				timer1.start();

				piezo.jouerNote(55);
				_delay_ms(50);
				piezo.arreterSon();
			}

			if (gMinuterieExpiree)
			{
				moteur.arreter();
				_delay_ms(500);
				if (nbPoto_ == 3)
				{
					potoRestant -= 1;
					memoire.lecture(potoRestant, &positionPoto);
					_delay_ms(5);
					if (positionPoto == 0)
					{
						moteur.accelerer(40, 0);
						_delay_ms(1000);
						moteur.arreter();
						_delay_ms(1000);
						moteur.accelerer(0, 40);
					}
					else if (positionPoto == 1)
					{
						moteur.accelerer(0, 40);
						_delay_ms(1000);
						moteur.arreter();
						_delay_ms(1000);
						moteur.accelerer(40, 0);
					}
					nbPoto_ = 0; // on n'a plus besoin et comme ca on ne fait qu'une fois le if
				}
				else if (nbPoto_ == 2) // je suis pas sure de ca
				{
					moteur.accelerer(60, Direction::ARRIERE);
					_delay_ms(500); // � corriger en fct de la distance
					moteur.arreter();
					_delay_ms(1000);
					moteur.accelerer(60);
					nbPoto_ = 0;
				}
				gMinuterieExpiree = false;
			}
			// fin du do once

			if (sv.detecterIntersection())
				position = Point::M;
			break;
		case Point::M:
			piezo.jouerNote(65);
			_delay_ms(50);
			piezo.arreterSon();

			potoRestant -= 1;
			memoire.lecture(potoRestant, &positionPoto);
			_delay_ms(5);
			if (positionPoto == 0)
				moteur.accelerer(40, 15);
			else if (positionPoto == 1)
				moteur.accelerer(15, 40);
			_delay_ms(DELAI_VIRAGE); // je vais transformer ca en fonction plus tard on doit tester avant

			position = Point::P;
			break;
		case Point::P:
			if (sv.detecterDepart())
			{
				moteur.arreter();
				piezo.jouerNote(65);
				_delay_ms(2000);
				piezo.arreterSon();
				position = Point::S;
			}
			break;
		case Point::S:
			del.changerCouleur(Del::ROUGE);
			break;
		}
	}
}