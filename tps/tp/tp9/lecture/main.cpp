/*
Auteurs : Steven Ho, Massoud Ibrahim et Nazim Bertouche
Travail : TP9
Section # : 01
Équipe # : 0314

Description du programme :
ce code lit dans la memoire externe du robot et execute les commandes qui y sont ecrite grace au programme "ecrire"

Identifications matérielles (Broches I/O) :
Nous utilisons la Del qui est connectee aux ports A1 & A2, les ports B3 et B6 sont connectes aux moteurs gauche et droit respectivement
*/

#define F_CPU 8000000
#define OPERANDE_VERT  1
#define OPERANDE_ROUGE 2
#define BASE_BINAIRE   2
#define OPERANDE_MINIMALE 0 
#define OPERANDE_MAXIMALE 255
#define DELAI_MINIMAL_ATT 25
#define OPERANDE_MINIMALE_SGO 45
#define OPERANDE_MAXIMALE_SGO 81
#define RAPPORT_CYCLIQUE operande / 255 * 100

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "../../lib_dir/del.h"
#include "../../lib_dir/moteur.h"
#include "../../lib_dir/memoire24.h"
#include "../../lib_dir/piezo.h"
#include "../../lib_dir/delay.h"


enum Mnemonique
{
    DBT  = 0x01,
    ATT  = 0x02,
    DAL  = 0x44,
    DET  = 0x45,
    SGO  = 0x48,
    SAR  = 0x09,
    MAR1 = 0x60,
    MAR2 = 0x61,
    MAV  = 0x62,
    MRE  = 0x63,
    TRD  = 0x64,
    TRG  = 0x65,
    DBC  = 0xC0,
    FBC  = 0xC1,
    FIN  = 0xFF
};

void desactivaterComposantes(Del& del, Moteur& moteur, Piezo& piezo)
{
    del.desactiver();
    moteur.arreter();
    piezo.arreterSon();
}

void executerInstruction(uint16_t& adresse, 
                         uint8_t instruction, uint8_t operande, 
                         Del& del, Moteur& moteur, 
                         Piezo& piezo)
{
    static uint8_t nRepetitions = 0;
    static uint16_t debutBoucle = 0;
    static uint8_t compteur = 0;
    
    switch (instruction) 
    {
        case ATT :
            if (operande >= OPERANDE_MINIMALE && operande <= OPERANDE_MAXIMALE)
                creerDelaiMs(DELAI_MINIMAL_ATT * operande);
            break;
        case DAL :
            if (operande == OPERANDE_VERT)
                del.fixerCouleur(Del::VERT);
            else if (operande == OPERANDE_ROUGE)
                del.fixerCouleur(Del::ROUGE);
            // else fera rien (si operande autre que 1 ou 2)
            break;
        case DET :
            del.fixerCouleur(Del::ETEINT);
            break;
        case SGO :
            if (operande >= OPERANDE_MINIMALE_SGO && operande <= OPERANDE_MAXIMALE_SGO)
                piezo.jouerNote(operande);
            break;
        case SAR :
            piezo.arreterSon();
            break;
        case MAR1 :
            // Fallthrough
        case MAR2:
            moteur.arreter();
            break;
        case MAV :
            if (operande >= OPERANDE_MINIMALE && operande <= OPERANDE_MAXIMALE)
                moteur.accelerer(Moteur::AVANT, RAPPORT_CYCLIQUE);
            break;
        case MRE :
            if (operande >= OPERANDE_MINIMALE && operande <= OPERANDE_MAXIMALE)
                moteur.accelerer(Moteur::ARRIERE, RAPPORT_CYCLIQUE);
            break;
        case TRD :
            moteur.virer90(Moteur::DROITE);
            break;
        case TRG :
            moteur.virer90(Moteur::GAUCHE);
            break;
        case DBC :
            if (operande > OPERANDE_MINIMALE && operande <= OPERANDE_MAXIMALE)
            {
                debutBoucle = adresse;
                nRepetitions = operande;
            }
            break;
        case FBC :
            if (compteur < nRepetitions)
            {
                adresse = debutBoucle;
                compteur++ ;
            }
            else
                compteur = 0;
            break;
        case FIN :
            desactivaterComposantes(del, moteur, piezo);
            break;
    }
}

int main()
{
    Del del;
    del.activer(); 
    Moteur moteur;
    Memoire24 memoire;
    Piezo piezo;

    bool lectureEstActif = false;

    uint16_t adresse = 0x00;
    uint8_t instruction = 0;
    uint8_t operande = 0;
    uint8_t octet = 0;
    // Lire les 16 premiers bits.
    memoire.lecture(adresse, &octet); // lecture du 1er ocet (les 8 bits les plus significatis)
    adresse ++ ;

    uint16_t nInstructions = octet;
    nInstructions = nInstructions << 8; //bitshift de 8 vers la gauche car la premiere lecture lis les 8 bits les plus significatifs

    memoire.lecture(adresse, &octet); // lecture du 2eme ocet (les 8 bits les moins significatis)
    adresse ++ ;

    nInstructions |= octet; //le bitshift de 8 vers la gauche et le OU= permettent de concatener les 2 octets 

    while (true)
    {
        memoire.lecture(adresse, &instruction);
        adresse ++;

        memoire.lecture(adresse, &operande);
        adresse ++;

        if (instruction == DBT)
            lectureEstActif = true;

        if (lectureEstActif)
        {
            executerInstruction(adresse, instruction,
                                operande, del,     
                                moteur, piezo);
        }

        if (instruction == FIN)
        {
            lectureEstActif = false;
            break;
        }
    }
}