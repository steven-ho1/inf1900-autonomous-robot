#include "suiveurLigne.h"
#include "delay.h"

SuiveurLigne::SuiveurLigne():
    minuterieExpiree_(false),
    nIntersections_(0),
    virageActive_(true)
{
    DDRA &= ~(1 << PA0 | 
              1 << PA1 |
              1 << PA2 |
              1 << PA3 |
              1 << PA4 |
              1 << PA5);
}

SuiveurLigne::~SuiveurLigne() 
{
    // Rien
}

// NOTE IMPORTANT: La roue gauche est plus faible que la droite
void SuiveurLigne::deplacerRobot()
{
    // Centré ou départ détecté
    if ((!EXTREME_GAUCHE && !MILIEU_GAUCHE && MILIEU &&
         !MILIEU_DROITE && !EXTREME_DROITE))
    {
        moteur_.accelerer(VITESSE_PAR_DEFAUT);
    }
    /*                          VIRER                      */
    // 90 gauche
    else if (virageActive_ && ((EXTREME_GAUCHE && MILIEU_GAUCHE && !MILIEU &&
             !MILIEU_DROITE && !EXTREME_DROITE) ||
             (EXTREME_GAUCHE && MILIEU_GAUCHE && MILIEU &&
             !MILIEU_DROITE && !EXTREME_DROITE) ||
             (EXTREME_GAUCHE && MILIEU_GAUCHE && MILIEU &&
             MILIEU_DROITE && !EXTREME_DROITE)))
    {
        moteur_.accelerer(AJUSTEMENT_VIRAGE, 
                          VITESSE_PAR_DEFAUT);
        nIntersections_++;                    
        _delay_ms(DELAI_CONFIRMATION_VIRAGE); 

    }
    // 90 droit
    else if (virageActive_ && ((!EXTREME_GAUCHE && !MILIEU_GAUCHE && !MILIEU &&
             MILIEU_DROITE && EXTREME_DROITE) ||
             (!EXTREME_GAUCHE && !MILIEU_GAUCHE && MILIEU &&
             MILIEU_DROITE && EXTREME_DROITE) ||
             (!EXTREME_GAUCHE && MILIEU_GAUCHE && MILIEU &&
             MILIEU_DROITE && EXTREME_DROITE)))
    {
        moteur_.accelerer(VITESSE_PAR_DEFAUT,
                            AJUSTEMENT_VIRAGE);

        nIntersections_++;
        _delay_ms(DELAI_CONFIRMATION_VIRAGE);
    }
    
    /*       POUSSER LE ROBOT DANS LA ZONE DE CAPTAGE        */
    // État du robot:

    // Fortement à droite
    else if ((EXTREME_GAUCHE && !MILIEU_GAUCHE && !MILIEU &&
             !MILIEU_DROITE && !EXTREME_DROITE))
    {
        moteur_.accelerer(VITESSE_PAR_DEFAUT - AJUSTEMENT_ELEVE, 
                          VITESSE_PAR_DEFAUT);
    }
    // Fortement à gauche
    else if ((!EXTREME_GAUCHE && !MILIEU_GAUCHE && !MILIEU &&
             !MILIEU_DROITE && EXTREME_DROITE))
    {
        moteur_.accelerer(VITESSE_PAR_DEFAUT, 
                          VITESSE_PAR_DEFAUT - AJUSTEMENT_ELEVE);
    }
    // Moyennement à droite
    else if (!EXTREME_GAUCHE && MILIEU_GAUCHE && !MILIEU &&
             !MILIEU_DROITE && !EXTREME_DROITE)
    {
        moteur_.accelerer(VITESSE_PAR_DEFAUT - AJUSTEMENT_MOYEN, 
                          VITESSE_PAR_DEFAUT);
    }
    // Moyennement à gauche
    else if (!EXTREME_GAUCHE && !MILIEU_GAUCHE && !MILIEU &&
             MILIEU_DROITE && !EXTREME_DROITE)
    {
        moteur_.accelerer(VITESSE_PAR_DEFAUT, 
                          VITESSE_PAR_DEFAUT - AJUSTEMENT_MOYEN);
    }
    // Légèrement à droite
    else if (!EXTREME_GAUCHE && MILIEU_GAUCHE && MILIEU &&
             !MILIEU_DROITE && !EXTREME_DROITE)
    {
        moteur_.accelerer(VITESSE_PAR_DEFAUT - AJUSTEMENT_FAIBLE, 
                          VITESSE_PAR_DEFAUT);
    }
    // Légèrement à gauche
    else if (!EXTREME_GAUCHE && !MILIEU_GAUCHE && MILIEU &&
             MILIEU_DROITE && !EXTREME_DROITE)
    {
        moteur_.accelerer(VITESSE_PAR_DEFAUT, 
                          VITESSE_PAR_DEFAUT - AJUSTEMENT_FAIBLE);
    }
}

void SuiveurLigne::partirStationner() 
{
    // 4.75s avant d'entrer dans le couloir
    timer1_.setOcrValue(37109);
    timer1_.start();
    moteur_.accelerer(VITESSE_PAR_DEFAUT);
    _delay_ms(DELAI_DEPART);
    
    while (true)
    {
        deplacerRobot();
        if (minuterieExpiree_ == true)
        {
            minuterieExpiree_ = false;
            break;
        }
    }

    // Pour forcer le premier virage du couloir
    moteur_.accelerer(50, 15);
    timer1_.setOcrValue(65535); // 8.39s dans le couloir
    timer1_.start();
    while(true)
    {
        if ((!EXTREME_GAUCHE && !MILIEU_GAUCHE && !MILIEU &&
             !MILIEU_DROITE && EXTREME_DROITE) || 
             (!EXTREME_GAUCHE && !MILIEU_GAUCHE && !MILIEU &&
             MILIEU_DROITE && EXTREME_DROITE))
        {
            if (minuterieExpiree_ == true)
            {
                minuterieExpiree_ = false;
                break;
            }
            moteur_.accelerer(25, 40);
        }
        else if ((EXTREME_GAUCHE && !MILIEU_GAUCHE && !MILIEU &&
                 !MILIEU_DROITE && !EXTREME_DROITE) ||
                 (EXTREME_GAUCHE && MILIEU_GAUCHE && !MILIEU &&
                 !MILIEU_DROITE && !EXTREME_DROITE))
        {
            moteur_.accelerer(45, 25);
        }
    }
    
    // 4s avant d'activer l'arrêt
    timer1_.setOcrValue(31250);
    timer1_.start();
    
    while (true)
    {
        deplacerRobot();
        if ((minuterieExpiree_ == true) && 
            (!EXTREME_GAUCHE && !MILIEU_GAUCHE && !MILIEU &&
            !MILIEU_DROITE && !EXTREME_DROITE))
            break;
    }

    moteur_.arreter();
    _delay_ms(1000);
    moteur_.accelerer(60, Direction::ARRIERE);
    _delay_ms(700);
    moteur_.accelerer(60, 10, Direction::ARRIERE);
    _delay_ms(1000);
    moteur_.accelerer(60, Direction::ARRIERE);
    _delay_ms(1100);
    moteur_.arreter();
    _delay_ms(1000);
    piezo_.jouerNote(80);
    _delay_ms(1000);
    piezo_.arreterSon();
}

bool SuiveurLigne::detecterIntersection()
{
    return EXTREME_GAUCHE && EXTREME_DROITE; 
}

bool SuiveurLigne::detecterDepart()
{
    return EXTREME_GAUCHE && MILIEU_GAUCHE && 
           MILIEU && MILIEU_DROITE && EXTREME_DROITE;
}

void SuiveurLigne::sortirBoucle()
{
    minuterieExpiree_ = true;
}

uint8_t SuiveurLigne::obtenirCompteIntersections()
{
    return nIntersections_;
}

void SuiveurLigne::activerVirage()
{
    virageActive_ = true;
}

void SuiveurLigne::desactiverVirage()
{
    virageActive_ = false;
}