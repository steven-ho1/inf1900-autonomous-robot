#include "bouton.h"

/*************************************************
Nom: Bouton()
Fonction: Constructeur de la classe Bouton
**************************************************/
Bouton::Bouton(volatile uint8_t interup , TYPE_PRESSION pression) {
	boutonPoussoir_ = interup;
	if (boutonPoussoir_ == 0)
		pressed_ = 0x04; // INT0 a pour masque 0x08 car il est situe sur PD2
	else if (boutonPoussoir_ == 1)
		pressed_ = 0x08; // INT1 a pour masque 0x08 car il est situe sur PD3
	initialisationIntExterne(pression);
}

/*************************************************
Nom: CheckIfPressed
Fonction: V�rifie si le bouton D est activ�.
Debounce int�gr�.
**************************************************/
bool Bouton::CheckIfPressed() {

	if (PIND & pressed_) { 		//
		_delay_ms(10);			// Debounce
		if (PIND & pressed_) {	
			return true;
		}
	}
	return false;
}

/*************************************************
Nom: initialisationIntExterne
Fonction: Permet les interruptions au niveau du bouton pressoir
**************************************************/
void Bouton::initialisationIntExterne( TYPE_PRESSION pression) {

	// Bloquer de nouvelles interuptions

	cli();
	volatile uint8_t setPression_0, setPression_1;
	// Masque qui permet les interruptions externes

	EIMSK |= (1 << boutonPoussoir_);

	// Sensibiliser les interruptions externes au niveau du bouton
	if (boutonPoussoir_ == 0) {
		setPression_0 = 0;
		setPression_1 = 1;
	}
	else if (boutonPoussoir_ == 1) {
		setPression_0 = 2;
		setPression_1 = 3;
	}
	pression_ = pression;

	switch (pression_)
	{
	case POUSSER:
		EICRA |= (1 << setPression_0);
		EICRA |= (1 << setPression_1);
		break;
	case RELACHER:
		EICRA |= (1 << setPression_1);
		break;
	case POUSSER_ET_RELACHER:
		EICRA |= (1 << setPression_0);
		break;
	}

	// Permettre de nouvelles interruptions

	sei();
}

/*************************************************
Nom: getBoutonPoussoir
Fonction: Retourne le statut (uint8_t) du boutton. Utilise dans le ISR d'interruption
**************************************************/
uint8_t Bouton::getBoutonPoussoir() {
	return boutonPoussoir_;
}

/*************************************************
Nom: getPressed
Fonction: Retourne le status (En bool) du bouton
**************************************************/
bool Bouton::getPressed() {
	return pressed_;
}