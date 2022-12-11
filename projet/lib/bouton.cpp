#include "bouton.h"

/*************************************************
Nom: Bouton()
Fonction: Constructeur de la classe Bouton
**************************************************/
Bouton::Bouton(uint8_t noInterruption, PRESSION pression):
	noInterruption_(noInterruption)
{
	initialiserPort();
	initialiserInterruption(pression);
	determinerMasque();
}

void Bouton::initialiserPort()
{
	if (noInterruption_ == INT0)
	{
		DDRD &= ~(1 << PD2);
	}
	else if (noInterruption_ == INT1)
	{
		DDRD &= ~(1 << PD3);
	}
}

/*************************************************
Nom: initialiserInterruption
Fonction: Permet les interruptions au niveau du bouton pressoir
**************************************************/
void Bouton::initialiserInterruption(PRESSION pression) 
{
	// Bloquer de nouvelles interuptions
	cli();

	// Masque qui permet les interruptions externes
	EIMSK |= (1 << noInterruption_);
	
	switch (pression)
	{
		case APPUIE:
			if (noInterruption_ == INT0)
				EICRA |= (1 << ISC01) | (1 << ISC00);
			else if (noInterruption_ == INT1)
				EICRA |= (1 << ISC11) | (1 << ISC10);
			break;
		case RELACHE:
			if (noInterruption_ == INT0)
			{
				EICRA |= 1 << ISC01;
				EICRA &= ~(1 << ISC00);
			}
			else if (noInterruption_ == INT1)
			{
				EICRA |= 1 << ISC11;
				EICRA &= ~(1 << ISC10);
			}
			break;
		case APPUIE_RELACHE:
			if (noInterruption_ == INT0)
			{
				EICRA |= 1 << ISC00;
				EICRA &= ~(1 << ISC01);
			}
			else if (noInterruption_ == INT1)
			{
				EICRA |= 1 << ISC10;
				EICRA &= ~(1 << ISC11);
			}
			break;
	}

	// Permettre de nouvelles interruptions
	sei();
}

void Bouton::determinerMasque()
{
	if (noInterruption_ == INT0)
		masqueBouton_ = 1 << PD2; // INT0 se trouve sur PD2
	else if (noInterruption_ == INT1)
		masqueBouton_ = 1 << PD3; // INT0 se trouve sur PD3
}

void Bouton::toutDesactiver()
{
	EIMSK &= ~(1 << INT1 | 1 << INT0);
}

/*************************************************
Nom: lireBouton
Fonction: Vérifie si le bouton est appuyé ou non.
		  Utilisé dans le ISR.
**************************************************/
bool Bouton::lireEtat() 
{
	_delay_ms(DELAI_ANTI_REBOND);		
	if (PIND & masqueBouton_) 
		return estAppuye_ = true;
	return estAppuye_ = false;
}

/*************************************************
Nom: obtenirEtat
Fonction: Retourne le statut (bool) du boutton. 
**************************************************/
bool Bouton::obtenirEtat() 
{
	return estAppuye_;
}
