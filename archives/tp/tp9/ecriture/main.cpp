/*
Auteurs : Nazim et Yassine
Travail : TP9
Section # : 01
Équipe # : 0314

Description du programme :
Ce programme attend une transmission via uart du fichier .bin pour l'ecrire dans la memoire externe du robot 

Identifications matérielles (Broches I/O) :
connection USB entre le robot et notre PC 
*/

#define F_CPU 8000000
#define DELAY_LECTURE_ECRITURE 5

#include <avr/io.h>
#include <util/delay.h>
#include "../../lib_dir/memoire24.h"
#include "../../lib_dir/uart.h"

void lireEcrire(uint8_t& octet, Memoire24& memoire, uint8_t& adresse, Uart& uart)
{
    octet = uart.recevoirViaUart();
    memoire.ecriture(adresse, octet);
    adresse++;

    _delay_ms(DELAY_LECTURE_ECRITURE);
}


/*************************************************
Nom: ecritureByteCode
Fonction: Fonction qui lit le contennu du UART et qui l'écrit sur la mémoire externe
**************************************************/
void ecritureByteCode() {
    Memoire24 memoire;
    Uart uart = Uart();
    uart.initialiserUart();
    uint16_t adresse = 0x00;
    uint8_t octet = 0;

    
    lireEcrire(octet, memoire, adresse, uart); // lecture et ecriture du 1er octet (le plus significatif)

    uint16_t taille = octet;
    taille = taille << 8; //bitshift de 8 vers la gauche car la premiere lecture lis les 8 bits les plus significatifs
    
    lireEcrire(octet, memoire, adresse, uart); // lecture et ecriture du 2eme octet (le moins significatif)

    taille |= octet;

     while (adresse < taille)
    {
         lireEcrire(octet, memoire, adresse, uart); // lecture et ecriture de chaque octet dans la memoire
    }
}

int main() {
    ecritureByteCode();
    return 0;
}