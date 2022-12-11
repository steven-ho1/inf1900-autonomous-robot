#include "selecteur.h"

Selecteur::Selecteur():
	boutonBlanc_(INT1),
    boutonInterrupt_(INT0, Bouton::RELACHE),
    del_(Del::VERT, &PORTC, 2, 3),
    parcours_(Point::A)
{
    // Rien
}

Selecteur::~Selecteur()
{
    // Rien
}

// Méthode bloquante
void Selecteur::initialiser()
{
    do 
    {
        afficherParcours();
    } while (parcoursConfirme_ == false); 

    // 4 clignotements par seconde = 1 clignotement par 250 ms
    del_.clignoter(FREQUENCE_CLIGNOTEMENT, DUREE);
    boutonBlanc_.toutDesactiver();
}

void Selecteur::afficherParcours()
{
    switch (parcours_)
    {
        case Point::A :
            del_.changerCouleur(Del::VERT);
            break;
        case Point::B :
            del_.changerCouleur(Del::ROUGE);
            break;
        case Point::S :
            del_.changerCouleur(Del::AMBRE);
            break;
        default :
            // Rien
            break;
    }
}

void Selecteur::changerParcours(Point& parcours)
{
    switch (parcours)
    {
        case Point::A :
            parcours = Point::B;
            break;
        case Point::B :
            parcours = Point::S;
            break;
        case Point::S :
            parcours = Point::A;

            // Évite une dernière allumage rouge visible durant la transition
            del_.changerCouleur(Del::VERT);
            break;
        default :
            // Rien
            break;
    }

    parcours_ = parcours;
}

void Selecteur::confirmerParcours()
{
    parcoursConfirme_ = true;
}