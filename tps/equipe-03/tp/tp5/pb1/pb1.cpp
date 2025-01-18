#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "memoire_24.h"

void initialiserRegistres()
{
    DDRA |= (1 << PA0) | (1 << PA1);
}

void estVert()
{
    PORTA |= 1 << PA0;
    PORTA &= ~(1 << PA1);
}

void estRouge()
{
    PORTA |= 1 << PA1;
    PORTA &= ~(1 << PA0);
}

int main()
{
    initialiserRegistres();
    char donnees[] = "P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
    char donneesLues[100];

    Memoire24CXXX memoire;
    
    uint16_t adresse = 0x0000;
    uint8_t longueur = sizeof(donnees); //strlen ou sizeof

    memoire.ecriture(adresse, (uint8_t*)donnees, longueur);     
    memoire.lecture(adresse, (uint8_t*)donneesLues, longueur);
    
    if (strcmp(donnees, donneesLues) == 0) 
        estVert();
    else 
        estRouge();
}