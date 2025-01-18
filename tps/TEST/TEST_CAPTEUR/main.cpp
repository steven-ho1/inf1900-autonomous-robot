#define F_CPU 8000000
#define N_ECHANTILLONS 20
#define MAX_LOIN 155 // 60 cm
#define MIN_LOIN 200 // 45 cm
#define MAX_PROCHE 300 // 30 cm
#define MIN_PROCHE 600 // 15 cm
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "piezo.h"
#include "can.h"
#include "debug.h"
#include "suiveurLigne.h"
#include "moteur.h"
#include "timer1.h"
volatile bool lecturePermise = true;
Timer1 timer1(7812); // 1 s
ISR(TIMER1_COMPA_vect)
{
    timer1.stop();
    lecturePermise = true;
}
int main()
{
    Piezo piezo_;
    Can convertisseur_;
    SuiveurLigne suiveurLigne;
    Moteur moteur_;
    INITIALISER_UART();
    uint16_t echantillons[N_ECHANTILLONS];
    uint8_t noEchantillon = 0;
    bool debuterEchantillonnage = false;
    while (true)
    {
        suiveurLigne.deplacerRobot();
        if (lecturePermise)
        {
            uint16_t tensionNumerique = convertisseur_.lecture(PA6);
            _delay_ms(15);
            char stringTensionNumerique[20];
            sprintf(stringTensionNumerique, "%hu", tensionNumerique);
            DEBUG_TRANSMETTRE(("Tension numérique: "));
            DEBUG_TRANSMETTRE((stringTensionNumerique));
            DEBUG_TRANSMETTRE(('\n'));
            if (MAX_LOIN < tensionNumerique && tensionNumerique < MIN_PROCHE)
            {
                debuterEchantillonnage = true;
            }
            if (debuterEchantillonnage == true)
            {
                echantillons[noEchantillon] = tensionNumerique;
                noEchantillon++;
                if (noEchantillon == N_ECHANTILLONS)
                {
                    uint16_t meilleurEchantillon = echantillons[0];
                    for (uint8_t i = 0; i < N_ECHANTILLONS; i++)
                    {
                        if (echantillons[i] > meilleurEchantillon && echantillons[i] != 0)
                        {
                            meilleurEchantillon = echantillons[i];
                        }
                    }
                    if (MAX_PROCHE < meilleurEchantillon && meilleurEchantillon < MIN_PROCHE)
                    {
                        moteur_.arreter();
                        piezo_.jouerNote(80);
                        _delay_ms(1000);
                        piezo_.arreterSon();
                        _delay_ms(1000);
                        moteur_.accelerer(60);
                        _delay_ms(200);
                    }
                    else if (MAX_LOIN < meilleurEchantillon && meilleurEchantillon < MAX_PROCHE)
                    {
                        moteur_.arreter();
                        piezo_.jouerNote(50);
                        _delay_ms(1000);
                        piezo_.arreterSon();
                        _delay_ms(1000);
                        moteur_.accelerer(60);
                        _delay_ms(200);
                    }
                    char stringDistance[20];
                    sprintf(stringDistance, "%hu", meilleurEchantillon);
                    DEBUG_TRANSMETTRE(("Meilleur Échantillon: "));
                    DEBUG_TRANSMETTRE((stringDistance));
                    DEBUG_TRANSMETTRE(('\n'));
                    timer1.start();
                    lecturePermise = false;
                    debuterEchantillonnage = false;
                    noEchantillon = 0;
                    meilleurEchantillon = 0;
                }
            }
        }
    }
}