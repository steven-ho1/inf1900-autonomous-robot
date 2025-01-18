#include "del.h"

Del::Del(Couleur couleur, volatile uint8_t* port,
		 uint8_t positionBroche1, uint8_t positionBroche2)
{
	couleur_ = couleur;
	port_    = port;
	positionBroche1_ = positionBroche1;
	positionBroche2_ = positionBroche2;

	estActive_ = false;
	determinerDdr();
}

void Del::determinerDdr()
{
	// Détermine automatiquement le DDR associé
	if (port_ == &PORTA)
		DDR_ = &DDRA;
	else if (port_ == &PORTB) 
		DDR_ = &DDRB;
	else if (port_ == &PORTC)
		DDR_ = &DDRC;
	else if (port_ == &PORTD)
		DDR_ = &DDRD;

	// Met automatiquement les broches choisies en sortie
	*DDR_ |= (1 << positionBroche1_) | (1 << positionBroche2_); 
}

void Del::fixerCouleur(Couleur couleur)
{
	couleur_ = couleur;

	if (estActive_)
	{
		changerCouleur();
	}
}

void Del::activer()
{
	estActive_ = true;
	changerCouleur();
}

void Del::changerCouleur()
{
	switch (couleur_)
	{
		case ETEINT :
			*port_ &= ~(1 << positionBroche1_ | 1 << positionBroche2_); 
			break;
		case VERT :
			*port_ |= 1 << positionBroche1_;
			*port_ &= ~(1 << positionBroche2_);
			break;
		case ROUGE :
			*port_ &= ~(1 << positionBroche1_);
			*port_ |= 1 << positionBroche2_;
			break;
	}
}

void Del::desactiver()
{
	estActive_ = false;
	couleur_   = ETEINT;
	changerCouleur();
}