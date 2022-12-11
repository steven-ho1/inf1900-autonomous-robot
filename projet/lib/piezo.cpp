#include "piezo.h"

Piezo::Piezo()
{
    DDRD |= (1 << PD7) | (1 << PD6); // PD7 piezo, PD6 pour ground
    PORTD &= ~(1 << PD6);
}

void Piezo::genererPwm(uint8_t duree)
{
    TCNT2 = 0;

    // Seule la période change, pas le rapport cyclique
    OCR2A = duree;

    // « Toggle OCnA on Compare Match »
    TCCR2A |= 1 << COM2A0;

    // Mode CTC
    TCCR2A |= 1 << WGM21;

    // Horloge divisée par 256
    TCCR2B |= (1 << CS22) | (1 << CS21);
}

void Piezo::arreterSon()
{
    TCCR2A &= ~(1 << COM2A0); // Désactiver la sortie OC2A
}

void Piezo::jouerNote(uint8_t operande)
{
    uint16_t frequence = 0; 
    uint16_t duree = 0;

    switch (operande)
    {
        case A2  :
            frequence = 110; 
            break;       
        case A_2 :        
            frequence = 117;
            break;       
        case B2  :         
            frequence = 123;
            break;       
        case C3  :         
            frequence = 131;
            break;       
        case C_3 :        
            frequence = 139;
            break;       
        case D3  :         
            frequence = 147;
            break;       
        case D_3 :        
            frequence = 156;
            break;      
        case E3  :         
            frequence = 165;
            break;       
        case F3  :         
            frequence = 175;
            break;       
        case F_3 :        
            frequence = 185;
            break;       
        case G3  :         
            frequence = 196;
            break;       
        case G_3 :        
            frequence = 208;
            break;       
        case A3  :         
            frequence = 220;
            break;       
        case A_3 :        
            frequence = 233;
            break;     
        case B3  :         
            frequence = 247;
            break;       
        case C4  :         
            frequence = 262;
            break;       
        case C_4 :        
            frequence = 277;
            break;       
        case D4  :         
            frequence = 294;
            break;       
        case D_4 :        
            frequence = 311;
            break;       
        case E4  :         
            frequence = 330;
            break;       
        case F4  :         
            frequence = 349;
            break;       
        case F_4 :        
            frequence = 370;
            break;       
        case G4  :         
            frequence = 392;
            break;       
        case G_4 :        
            frequence = 415;
            break;       
        case A4  :         
            frequence = 440;
            break;       
        case A_4 :        
            frequence = 466;
            break;       
        case B4  :         
            frequence = 494;
            break;       
        case C5  :         
            frequence = 523;
            break;       
        case C_5 :        
            frequence = 554;
            break;       
        case D5  :         
            frequence = 587;
            break;       
        case D_5 :        
            frequence = 622;
            break;       
        case E5  :         
            frequence = 659;
            break;       
        case F5  :         
            frequence = 698;
            break;       
        case F_5 :        
            frequence = 740;
            break;       
        case G5  :         
            frequence = 784;
            break;       
        case G_5 :       
            frequence = 831;
            break;       
        case A5  :         
            frequence = 880;
            break;
    }
    // Formule initiale : F_CPU_SUR_PRESCALIER * periode / 2 
    duree = F_CPU_SUR_PRESCALER / (F_TEMPS_ACTIF); 
    genererPwm(duree);
}

/* Trouver ça prend combien d'itérations du compteur pour arriver au temps actif

*/