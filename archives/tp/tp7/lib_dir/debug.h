#pragma once

#include "uart.h"

#ifdef DEBUG
    #define INITIALISER_UART() Uart uartDebug;\
                                uartDebug.initialiserUart();
    #define DEBUG_TRANSMETTRE(x) uartDebug.transmettreViaUart(x);
#else
    #define INITIALISER_UART() do {} while (0)
    #define DEBUG_TRANSMETTRE(x) do {} while (0)
#endif