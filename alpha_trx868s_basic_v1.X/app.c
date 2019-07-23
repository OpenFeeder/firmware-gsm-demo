// https://en.wikipedia.org/wiki/Javadoc
// TODO: Documentation of the microcontroller program to achieve with the documentation generation tool "Doxygen"

/**
 * @file name.ext
 * @brief Titre function
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date dd/mm/yyyy
 * Revision history:
 * @param name usage
 * @return usage
 */

/* Software */
// Docklight COM connections (RS232 terminal to communicate with USB Serial Port)
// Downloads: http://docklight.de/
// Lors de l'appuis dans Docklight sur la touche [Enter] le PC envoie 0D 0A soit 2 octets,
// dont 0A est interpr�ter comme la valeur suivante !
// --> pour r�soudre cela dans Docklight, il faut appuyer sur la combinaison de touches
//     [Ctrl]+[Enter] pour n'envoyer que la valeur 0D (Carriage Return <CR>)
//     https://en.wikipedia.org/wiki/Control_character

// TODO: RE02/SPARE_4 => SP1 � relier sur nIRQ pin 2 du ALPHA-TRX433S


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
 */

APP_DATA appData; /* Global application data. */

volatile int8_t msgReceive = 0;
int8_t noPrint = 0; 
int8_t APP_isMsgReceive() {
    return msgReceive;
}

void APP_setMsgReceive(int8_t set) {
    msgReceive = set;
}

/******************************************************************************
  Function:
    void APP_Tasks( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Tasks(void) {
    struct tm t;
    /* Check the Application State. */
    switch (appData.state) {
        case APP_STATE_INITIALIZE:
        {
            /**
             * Initializing the application.
             * (en) Application initialization when starting the main power.
             * (fr) Initialisation de l'application lors du d�marrage de l'alimentation principale.
             */
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                displayBootMessage();
                printf("> APP_STATE_INITIALIZE\n");
                powerRFEnable();
                // Check the power statut of the RF module
                if (CMD_3v3_RF_GetValue() == false) {
                    printf("RF Module enable.\n");
                    radioAlphaTRX_Init();
                    radioAlphaTRX_ReceivedMode(); // receive mode actived
                    radioAlphaTRX_GetLogFromDisk();
                } else {
                    printf("RF Module disable.\n");
                    printf("Send 'T' to change power state of radio module.\n");
                }
#endif
            }
            printf("Go to APP_STATE_IDLE...\n");
            appData.state = APP_STATE_IDLE;
            break;
        }
            /* -------------------------------------------------------------- */

        case APP_STATE_IDLE:
            //NewFunction();
            /**
             * Application idle state.
             *  - waiting for a event during a timeout period
             *  - if user detached USB key go to error state
             *  - event PIR sensor, detecting movement near the system
             *  - after the timeout period go into sleep mode
             */
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                
                printf("> APP_STATE_IDLE\n");
#endif
            }

            /* Green status LED blinks in idle mode. */
            LedsStatusBlink(LED_GREEN, 20, 1980);
#if defined(UART_DEBUG)
            RTCC_TimeGet(&t);
            if (t.tm_sec%10 == 0 && noPrint) {
                noPrint = 0;
                printf("[heur ==> %dh:%dmin:%ds]\n", t.tm_hour, t.tm_min, t.tm_sec);
                
            }else if (t.tm_sec%10 != 0 && !noPrint){
                noPrint = 1;
            }
#endif      
            if (APP_isMsgReceive() == 1) {
                APP_setMsgReceive(0); // on effce le flag 
                appData.state = APP_STATE_RADIO_RECEIVED;
            } else {
#if defined (USE_UART1_SERIAL_INTERFACE)
                /* Get interaction with the serial terminal. */
                APP_SerialDebugTasks();
#endif
            }
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_RADIO_RECEIVED:
            if (appData.state != appData.previous_state) {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf("> APP_STATE_RADIO_RECEIVED\n");
#endif
                appData.previous_state = appData.state;
            }
            radioAlphaTRX_SlaveHundlerMsgReceived(radioAlphaTRX_GetFrame());
            break;
            /* -------------------------------------------------------------- */
        case APP_STATE_RADIO_SEND_DATA:
            if (appData.state != appData.previous_state) {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf("> APP_STATE_RADIO_SEND_DATA\n");
#endif
                appData.previous_state = appData.state;
            }
            radioAlphaTRX_SlaveSendLog();
            //apelle la fonction qui s'occupe de transmettre les donnees 
            break;
            /* -------------------------------------------------------------- */
        default:
            //            Nop( );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
            printf("> APP_STATE_DEFAULT\n");
#endif
            setLedsStatusColor(LED_RED);
            break;
    }
}

void APP_Initialize(void) {
    /* APP state task initialize */
    appData.state = APP_STATE_INITIALIZE;
    appData.previous_state = APP_STATE_ERROR;

}


/*******************************************************************************
End of File
 */
