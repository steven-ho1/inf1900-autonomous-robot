#include "del.h"

Del::Del(Couleur couleur, volatile uint8_t* port,
		 uint8_t positionBroche1, uint8_t positionBroche2):
	port_(port),
	positionBroche1_(positionBroche1),
	positionBroche2_(positionBroche2)
{
	determinerDdr();
	changerCouleur(couleur);
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

void Del::changerCouleur(Couleur couleur)
{
	couleur_ = couleur;

	switch (couleur)
	{
		case ETEINT :
			eteindre();
			break;
		case VERT :
			allumerVert();
			break;
		case ROUGE :
			allumerRouge();
			break;
		case AMBRE : // À utiliser avec un while 
			allumerAmbre();
			break;
	}
}

void Del::clignoter(uint16_t frequenceClignotement, uint16_t duree)
{
	uint16_t dureeClignotement = SEC_EN_MS / frequenceClignotement;  

	switch (couleur_) 
	{
		case ETEINT:
			// Rien à faire.
			break;
		case VERT :
			// Fallthrough
		case ROUGE :
			for (uint16_t i = 0; i < frequenceClignotement * duree; i++)
			{
				changerCouleur(couleur_);
				creerDelaiMs(TEMPS_ACTIF); 
				eteindre();
				creerDelaiMs(TEMPS_ACTIF); 
			}
			break;
		case AMBRE :
			for (uint16_t i = 0; i < frequenceClignotement * duree; i++)
			{
				for (uint16_t j = 0; j < TEMPS_ACTIF / DELAI_TOTAL_AMBRE; j++)
				{
					allumerAmbre();
				}
				eteindre();
				creerDelaiMs(TEMPS_ACTIF); 
			}
			break;
	}
}

void Del::eteindre()
{
	*port_ &= ~(1 << positionBroche1_ | 1 << positionBroche2_);
}

void Del::allumerVert()
{
	*port_ |= 1 << positionBroche1_;
	*port_ &= ~(1 << positionBroche2_);
}

void Del::allumerRouge()
{
	*port_ &= ~(1 << positionBroche1_);
	*port_ |= 1 << positionBroche2_;
}

void Del::allumerAmbre()
{
	allumerRouge();
    _delay_ms(DELAI_AMBRE);
    allumerVert();
    _delay_ms(DELAI_AMBRE);
}