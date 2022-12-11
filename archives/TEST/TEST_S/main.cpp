#include "suiveurLigne.h"
#include "timer1.h"

volatile bool gFinCouloir = false;

ISR(TIMER1_COMPA_vect)
{
    Timer1 timer1;
    timer1.stop();
    gFinCouloir = true;
}

int main()
{
    SuiveurLigne sv;
    Moteur moteur;
    
    Timer1 timer1(40000); // 4s avant le couloir
    timer1.start();
    while (true)
    {
        sv.deplacerRobot();
        if (gFinCouloir == true)
            {
                gFinCouloir = false;
                break;
            }
    }
    // Pour forcer le premier virage du couloir
    moteur.accelerer(50, 15);
    timer1.setOcrValue(62500); // 12s pour le couloir
    timer1.start();
    while(true)
    {
        if ((!EXTREME_GAUCHE && !MILIEU_GAUCHE && !MILIEU &&
             !MILIEU_DROITE && EXTREME_DROITE))
        {
            if (gFinCouloir == true)
            {
                gFinCouloir = false;
                break;
            }
            moteur.accelerer(25, 45);

        }
        else if (EXTREME_GAUCHE && !MILIEU_GAUCHE && !MILIEU &&
             !MILIEU_DROITE && !EXTREME_DROITE)
        {
            moteur.accelerer(50, 25);
        }
    }

    // Ajuster timer pour réactivation arrêt 
    //78125 pour 10 sec
    //93750 pour 12 sec
    timer1.setOcrValue(31250);
    timer1.start();
    
    while (true)
    {
        sv.deplacerRobot();
        if ((gFinCouloir == true) && 
            (!EXTREME_GAUCHE && !MILIEU_GAUCHE && !MILIEU &&
            !MILIEU_DROITE && !EXTREME_DROITE))
            break;
        
    }
    moteur.arreter();
    _delay_ms(1000);
    moteur.accelerer(50, Direction::ARRIERE);
    _delay_ms(900);
    moteur.accelerer(50, 10, Direction::ARRIERE);
    _delay_ms(1100);
    moteur.accelerer(50, Direction::ARRIERE);
    _delay_ms(1350);
    moteur.arreter();
}
