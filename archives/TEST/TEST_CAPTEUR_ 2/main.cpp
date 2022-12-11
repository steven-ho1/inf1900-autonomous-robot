#define F_CPU 8000000
#define N_ECHANTILLONS 15
#define VALEUR_MAX 80
#define ZONE_GRISE 35
#define VALEUR_MIN 15

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "piezo.h"
#include "can.h"
#include "debug.h"
#include "suiveurLigne.h"
#include "moteur.h"

volatile bool lecturePermise = true;
Timer1 timer1(11718); // 1.5 s

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

    // Prendre une échantillon de 4 valeurs et déterminer la plus petite
    uint16_t echantillons[N_ECHANTILLONS];
    uint8_t noEchantillon = 0;
    bool debuterEchantillonnage = false;

    while (true)
    {
        suiveurLigne.deplacerRobot();
        if (lecturePermise)
        {
            _delay_ms(200);
            uint16_t tensionNumerique = convertisseur_.lecture(PA6);
            uint16_t tension = tensionNumerique * 3.0 * 1000 / 1023; // Revoir graphique, AREF et 3.1
            uint16_t distance = 24047 / (tension - 143);
            // char stringTensionNumerique[20];
            // char stringTension[20];
            char stringDistance[20];

            // sprintf(stringTensionNumerique, "%hu", tensionNumerique);
            // sprintf(stringTension, "%hu", tension);
            sprintf(stringDistance, "%hu", distance);

            // DEBUG_TRANSMETTRE(("Tension numérique: "));
            // DEBUG_TRANSMETTRE((stringTensionNumerique));
            // DEBUG_TRANSMETTRE(('\n'));
            // DEBUG_TRANSMETTRE(("Tension (mV): "));
            // DEBUG_TRANSMETTRE((stringTension));
            // DEBUG_TRANSMETTRE(('\n'));
            //DEBUG_TRANSMETTRE(("Distance (cm): "));
            DEBUG_TRANSMETTRE((stringDistance));
            DEBUG_TRANSMETTRE(('\n'));
            //DEBUG_TRANSMETTRE(('\n'));

            
        }
    }
}
