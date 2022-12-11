#include "capteurDistance.h"

CapteurDistance::CapteurDistance(uint8_t positionBroche, bool lecturePermise):
    timer1_(11718), // 1.5s avant de réactiver la lecture
    lecturePermise_(lecturePermise),
    positionPoteau_(0),
    nPoteaux_(0)
{
    positionBroche_ = positionBroche;
    DDRA &= ~(1 << positionBroche);
}

CapteurDistance::~CapteurDistance() 
{
    // Rien
}

void CapteurDistance::capter()
{
    if (lecturePermise_)
    {
        uint16_t tensionNumerique = convertisseur_.lecture(PA6);
        _delay_ms(20);
        if (MAX_LOIN < tensionNumerique && tensionNumerique < MIN_PROCHE)
        {
            debuterEchantillonnage_ = true;
        }
        if (debuterEchantillonnage_ == true)
        {
            echantillons_[noEchantillon_] = tensionNumerique;
            noEchantillon_++;
            if (noEchantillon_ == N_ECHANTILLONS)
            {
                uint16_t meilleurEchantillon = echantillons_[0];
                for (uint8_t i = 0; i < N_ECHANTILLONS; i++)
                {
                    if (echantillons_[i] > meilleurEchantillon && echantillons_[i] != 0)
                    {
                        meilleurEchantillon = echantillons_[i];
                    }
                }
                if (MAX_PROCHE < meilleurEchantillon && meilleurEchantillon < MIN_PROCHE)
                {
                    ecrireMemoire(POTEAU_PROCHE);
                }
                else if (MAX_LOIN < meilleurEchantillon && meilleurEchantillon < MAX_PROCHE)
                {
                    ecrireMemoire(POTEAU_LOIN);
                }
                timer1_.start();
                lecturePermise_ = false;
                debuterEchantillonnage_ = false;
                noEchantillon_ = 0;
                meilleurEchantillon = 0;
            }
        }
    }
}

void CapteurDistance::ecrireMemoire(uint8_t positionPoteau)
{
    positionPoteau_ = positionPoteau; 
    moteur_.arreter();

    if (positionPoteau_ == POTEAU_PROCHE)
        piezo_.jouerNote(NOTE_AIGUE);
    else if (positionPoteau_ == POTEAU_LOIN)
        piezo_.jouerNote(NOTE_GRAVE);
        
    _delay_ms(DELAI_SON);
    piezo_.arreterSon();
    _delay_ms(DELAI_SON);
    moteur_.accelerer(VITESSE_DEPART);
    _delay_ms(DELAI_DEPART);
    memoire_.ecrire(positionPoteau_);
    _delay_ms(DELAI_ACCES_MEMOIRE);
    nPoteaux_++;

    // Nombre de poteaux à l'adresse 5
    memoire_.ecriture(ADRESSE_N_POTEAUX, nPoteaux_);
}

void CapteurDistance::desactiverLecture()
{
    lecturePermise_ = false;
}

void CapteurDistance::activerLecture()
{
    lecturePermise_ = true;
}

uint8_t CapteurDistance::obtenirNombrePoteaux()
{
    memoire_.lecture(ADRESSE_N_POTEAUX, &nPoteaux_);
    _delay_ms(DELAI_ACCES_MEMOIRE);
    return nPoteaux_;
}