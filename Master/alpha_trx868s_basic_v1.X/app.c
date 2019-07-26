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
// dont 0A est interpréter comme la valeur suivante !
// --> pour résoudre cela dans Docklight, il faut appuyer sur la combinaison de touches
//     [Ctrl]+[Enter] pour n'envoyer que la valeur 0D (Carriage Return <CR>)
//     https://en.wikipedia.org/wiki/Control_character

// TODO: RE02/SPARE_4 => SP1 à relier sur nIRQ pin 2 du ALPHA-TRX433S


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
//int8_t noPrint = 0;

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
//void APP_Tasks(void) {
//    struct tm t;
//    /* Check the Application State. */
//    /* Green status LED blinks in idle mode. */
//    LedsStatusBlink(LED_GREEN, 20, 1980);
//#if defined(UART_DEBUG)
//    RTCC_TimeGet(&t);
//    if (t.tm_sec % 10 == 0 && noPrint) {
//        noPrint = 0;
//        printf("[heur ==> %dh:%dmin:%ds]\n", t.tm_hour, t.tm_min, t.tm_sec);
//
//    } else if (t.tm_sec % 10 != 0 && !noPrint) {
//        noPrint = 1;
//    }
//    APP_SerialDebugTasks();
//#endif     
//    switch (appData.state) {
//        case APP_STATE_INITIALIZE:
//        {
//            /**
//             * Initializing the application.
//             * (en) Application initialization when starting the main power.
//             * (fr) Initialisation de l'application lors du démarrage de l'alimentation principale.
//             */
//            if (appData.state != appData.previous_state) {
//                appData.previous_state = appData.state;
//#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
//                displayBootMessage();
//                printf("> APP_STATE_INITIALIZE\n");
//                powerRFEnable();
//                // Check the power statut of the RF module
//                if (CMD_3v3_RF_GetValue() == false) {
//                    printf("RF Module enable.\n");
//                    radioAlphaTRX_Init();
//                    radioAlphaTRX_ReceivedMode(); // receive mode actived
//                } else {
//                    printf("RF Module disable.\n");
//                    printf("Send 'T' to change power state of radio module.\n");
//                }
//#endif
//            }
//            printf("Go to APP_STATE_IDLE...\n");
//            appData.state = MSTR_STATE_GENERAL_BEFOR_DAYTIME;
//            break;
//        }
//            /* -------------------------------------------------------------- */
//
//        case MSTR_STATE_GENERAL_BEFOR_DAYTIME:
//             if (appData.state != appData.previous_state) {
//                appData.previous_state = appData.state;
//#if defined(UART_DEBUG)
//                printf("Master on est 2h avant le debut de la journee\n");
//#endif
//
//                MASTER_Init();
//            }
//            //TODO : ce que je dois faire avant le debut des hostilite 
//            break;
//            /* -------------------------------------------------------------- */
//        case MSTR_STATE_GENERAL_DAYTIME:
//             if (appData.state != appData.previous_state) {
//                appData.previous_state = appData.state;
//#if defined(UART_DEBUG)
//                printf("Master on est le debut de la journee \n");
//#endif
//            }
//            MASTER_StateMachineOfDaytime();
//            break;
//            /* -------------------------------------------------------------- */
//        case MSTR_STATE_GENERAL_AFTER_DAYTIME:
//            //TODO : ce que je dois faire avant le debut des hostilite 
//             if (appData.state != appData.previous_state) {
//                appData.previous_state = appData.state;
//#if defined(UART_DEBUG)
//                printf("Master on est a la fin de la journee \n");
//#endif  
//            }
//
//            MASTER_GetLog();
//            break;
//            /* -------------------------------------------------------------- */
//        case MSTR_STATE_GENERAL_END:
//             if (appData.state != appData.previous_state) {
//                appData.previous_state = appData.state;
//#if defined(UART_DEBUG)
//                printf("Fin de la journe: afficher le status du master \n");
//#endif
//            }
//        default:
//            //            Nop( );
//#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
//            printf("> APP_STATE_DEFAULT\n");
//#endif
//            setLedsStatusColor(LED_RED);
//            break;
//    }
//}

void APP_Initialize(void) {
    /* APP state task initialize */
    appData.state = APP_STATE_INITIALIZE;
    appData.previous_state = APP_STATE_ERROR;

}


/*******************************************************************************
End of File
 */
