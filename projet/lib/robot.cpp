#include "robot.h"
Robot Robot::robot_;
Robot& Robot::controler()
{
    return robot_;
}
Robot::Robot():
    capteurDistance_(PA6, false),
    point_(Point::A),
    avancePermise_(true),
    arretActive_(false),
    detectionIntersectionActive_(false),
    doitSaluer_(false),
    salueTermine_(false)
{
    // Rien
}

Robot::~Robot()
{
    // Rien
}

// Méthode bloquante
void Robot::initialiser()
{
    selecteur_.initialiser();
}

void Robot::deplacer()
{
    if (avancePermise_)
        suiveurLigne_.deplacerRobot();
    else
    {
        moteur_.arreter();
        capteurDistance_.desactiverLecture();
    }
}

void Robot::effectuerParcours()
{
    deplacer();

    bool departTrouve = suiveurLigne_.detecterDepart();
    switch (point_)
    {
        case Point::A :
            if (departTrouve)
            {
                if (arretActive_)
                    avancePermise_ = false;
                else
                {
                    // 1s avant d'activer l'arrêt à B et le captage
                    timer1_.setOcrValue(7812); 
                    timer1_.start();
                    moteur_.accelerer(VITESSE_DEPART);
                    _delay_ms(DELAI_DEPART);
                }
            }
            capteurDistance_.capter();
            break;
        case Point::B :
            if (departTrouve && detectionIntersectionActive_ == false)
            {
                // 8.39s avant d'activer la détection d'intersection
                timer1_.setOcrValue(65535); 
                timer1_.start();

                nPoteaux_ = capteurDistance_.obtenirNombrePoteaux();
                poteauxRestants_ = nPoteaux_;

                moteur_.accelerer(VITESSE_DEPART);
                _delay_ms(DELAI_DEPART);
            }

            // Mesures pour éviter détection précoce d'intersections
            if (suiveurLigne_.detecterIntersection() && 
                suiveurLigne_.obtenirCompteIntersections() > 2 &&
                detectionIntersectionActive_)
				point_ = Point::E;

            break;
        
        case Point::E:
            poteauxRestants_--;
            memoire_.lecture(poteauxRestants_, &positionPoteau_);
            _delay_ms(DELAI_ACCES_MEMOIRE);

            if (positionPoteau_ == POTEAU_PROCHE)
                moteur_.accelerer(VITESSE_PAR_DEFAUT, AJUSTEMENT_VIRAGE);
            else if (positionPoteau_ == POTEAU_LOIN)
                moteur_.accelerer(AJUSTEMENT_VIRAGE, VITESSE_PAR_DEFAUT);
            _delay_ms(DELAI_VIRAGE_INTERSECTION);
            
            suiveurLigne_.desactiverVirage();
            point_ = Point::J;
            break;
        case Point::J:
            if (MILIEU_GAUCHE && MILIEU && MILIEU_DROITE && 
                salueTermine_ == false)
            {
                // 3s avant de saluer
                timer1_.setOcrValue(23437);
                timer1_.start();
            }

            if (doitSaluer_)
            {
                moteur_.arreter();
                _delay_ms(DELAI_PREPARATION_SALUT);
                if (nPoteaux_ == 3)
                {
                    poteauxRestants_--;
                    memoire_.lecture(poteauxRestants_, &positionPoteau_);
                    _delay_ms(DELAI_ACCES_MEMOIRE);

                    if (positionPoteau_ == POTEAU_PROCHE)
                    {
                        moteur_.accelerer(0, VITESSE_DEPART);
                        _delay_ms(DELAI_VIRAGE_SALUT);
                        moteur_.arreter();
                        _delay_ms(DELAI_ARRET_SALUT);
                        moteur_.accelerer(VITESSE_DEPART, 0);
                    }
                    else if (positionPoteau_ == POTEAU_LOIN)
                    {
                        moteur_.accelerer(VITESSE_DEPART, 0);
                        _delay_ms(DELAI_VIRAGE_SALUT);
                        moteur_.arreter();
                        _delay_ms(DELAI_ARRET_SALUT);
                        moteur_.accelerer(0, VITESSE_DEPART);
                    }
                    nPoteaux_ = 0;
                }
                else if (nPoteaux_ == 2) 
                {
                    moteur_.accelerer(VITESSE_DEPART, Direction::ARRIERE);
                    _delay_ms(DELAI_RECUL_SALUT); 
                    moteur_.arreter();
                    _delay_ms(DELAI_ARRET_SALUT);
                    moteur_.accelerer(VITESSE_DEPART);
                    _delay_ms(DELAI_DEPART);
                    nPoteaux_ = 0;
                }
                doitSaluer_ = false;
                salueTermine_ = true;
            }
        
            if (suiveurLigne_.detecterIntersection() && salueTermine_)
            {
                // Mesure pour éviter la double détection d'intersection: 8.39s 
                detectionIntersectionActive_ = false;
                timer1_.setOcrValue(65535);
                timer1_.start();
                point_ = Point::M;
            }
            break;
        case Point::M:
            poteauxRestants_--;
            memoire_.lecture(poteauxRestants_, &positionPoteau_);
            _delay_ms(DELAI_ACCES_MEMOIRE);

            if (positionPoteau_ == POTEAU_PROCHE)
                moteur_.accelerer(VITESSE_PAR_DEFAUT, AJUSTEMENT_VIRAGE);
            else if (positionPoteau_ == POTEAU_LOIN)
                moteur_.accelerer(AJUSTEMENT_VIRAGE, VITESSE_PAR_DEFAUT);
            _delay_ms(DELAI_VIRAGE_INTERSECTION); 

            suiveurLigne_.activerVirage();
            point_ = Point::P;
            break;
        case Point::P:
            if (suiveurLigne_.detecterDepart() &&
                suiveurLigne_.obtenirCompteIntersections() > 4 && 
                detectionIntersectionActive_)
				point_ = Point::S;
            break;
        case Point::S :
            suiveurLigne_.partirStationner();
            avancePermise_ = false;
            
            //A revoir
            do
            {
                moteur_.arreter();
            } while (true);
            break;
    }
}

void Robot::gererInterruptionBouton(uint8_t noInterruption, volatile bool gboutonAppuye)
{
    cli();
    switch (noInterruption) 
    {
        case INT0 :
            if (gboutonAppuye)
                selecteur_.confirmerParcours();
            break;
        case INT1 :   
            if (gboutonAppuye) 
                selecteur_.changerParcours(point_);
            break;
    }
    // Éviter des interruptions consécutives
    EIFR |= (1 << INTF0);
    sei();
}

void Robot::gererInterruptionMinuterie()  
{
    timer1_.stop();
    switch (point_)
    {
        case Point::A :
            arretActive_ = true;
            capteurDistance_.activerLecture();
            break;
        case Point::B :
            detectionIntersectionActive_ = true;
            break;
        case Point::J :
            doitSaluer_ = true;
            break;
        case Point::P :
            detectionIntersectionActive_ = true;
            break;
        case Point::S :
            suiveurLigne_.sortirBoucle();
            break;
        default :
            // Rien
            break;
    }
}

Timer1& Robot::accederTimer1()
{
    return timer1_;
}