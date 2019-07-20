/* *****************************************************************************
 * 
 * _____________________________________________________________________________
 *
 *                            MASTER API  (.c)
 * _____________________________________________________________________________
 *
 * Titre            : Mise en oeuvre de l'api et la machie a etat du master   
 * Version          : v00
 * Date de creation : 26/05/2019
 * Auteur           : MMADI Anzilane 
 * Contact          : anzilan@hotmail.fr
 * Web page         : 
 * Collaborateur    : ...
 * Processor        : PIC24
 * Tools used       : MPLAB X IDE v5.15 and MPLAB Code Configurator (MCC) Version: 3.36
 * Compiler         : Microchip XC16 v1.35
 * Programmateur    : PICkit 3
 *******************************************************************************
 *******************************************************************************/

/**------------------------>> I N C L U D E <<---------------------------------*/
#include <stdio.h>
#include <stdbool.h>
#include "master_api.h"
#include "timer.h"
#include "app.h"

/******************************************************************************/
/******************************************************************************/
/********************* COUCHE APPLICATION DU MASTER  **************************/
/***************************                ***********************************/
/**************************                         ***************************/
/*****************                                         ********************/

/**-------------------------->> V A R I A B L E S <<---------------------------*/
uint8_t behavior[MAX_LEVEL_PRIO][NB_BEHAVIOR_PER_PRIO]; // tableau des comportement 
//
// les pointeurs d'ecriture et de l'ecture des buffer circulaire 
uint8_t ptr[MAX_LEVEL_PRIO][3]; //READ - WRITE - OVFF (overflow)

//previous behavior
MASTER_STATES prevBehavior = MASTER_STATE_NONE;

//DEbug
int8_t noPrint = 0;

/**-------------------------->> M A C R O S <<--------------------------------*/
#define GETpREAD(prio) ptr[prio][READ] // recupere le ponteur de lecture
#define GETpWRITE(prio) ptr[prio][WRITE] // recupere le pointeur d'ecriture
#define GET_OVFF(prio)  ptr[prio][OVFF] // recipere le pointeur d'overFlow

#define INCpREAD(prio) (ptr[prio][READ] = (ptr[prio][READ]+1) % NB_BEHAVIOR_PER_PRIO)
#define INCpWRITE(prio) (ptr[prio][WRITE] = ptr[prio][WRITE]+1 % NB_BEHAVIOR_PER_PRIO)
#define SET_0VFF(prio, set) (ptr[prio][OVFF] = set)

/**-------------------------->> S T R U C T U R E -- S L A V E <<--------------*/

/**-------------------------->> V A R I A B L E S <<---------------------------*/

//______________________________________________________________________________

/**-------------------------->> L O C A L -- F O N C T I O N S <<--------------*/

//_______________________________ IMPLEMENTATION________________________________

int8_t MASTER_SendMsgRF(uint8_t dest,
                        uint8_t typeMsg,
                        uint8_t idMsg,
                        uint8_t nbR,
                        uint8_t * data,
                        uint8_t sizeData) {
    //    radioAlphaTRX_SetSendMode(1); // j'annonce que je suis en mode transmission 
    Frame frameToSend;

    //_____________CREATE FRAME____________________________________________
    int8_t ret = 0;
    // en tete 
    frameToSend.Champ.dest = dest;
    frameToSend.Champ.crc ^= frameToSend.Champ.dest;
    frameToSend.Champ.src = MASTER_ID;
    frameToSend.Champ.crc ^= frameToSend.Champ.src;
    frameToSend.Champ.idMsg = idMsg;
    frameToSend.Champ.crc ^= frameToSend.Champ.idMsg;
    frameToSend.Champ.typeMsg = typeMsg;
    frameToSend.Champ.crc ^= frameToSend.Champ.typeMsg;
    frameToSend.Champ.nbR = nbR;
    frameToSend.Champ.crc ^= frameToSend.Champ.nbR;

    // data
    int8_t i;
    frameToSend.Champ.size = sizeData;
    for (i = 0; i < frameToSend.Champ.size; i++) {
        frameToSend.Champ.data[i] = data[i];
        frameToSend.Champ.crc ^= frameToSend.Champ.data[i];
    }
    
    ///____________________________________________________________________
    if (radioAlphaTRX_SendMode()) {
        ret = radioAlphaTRX_SendData(frameToSend);
    } else {
#if defined(UART_DEBUG)
        printf("non Envoye\n");
#endif
    }
    radioAlphaTRX_ReceivedMode(); // je me remets en attente d'un msg
    return ret;
}

uint8_t MASTER_SendDateRF() {
    struct tm picDate;
    //____________________________
    //________UPDATE DATE_________
    // with GSM module 
    //TODO : use GSM function to update date here
    //____________________________
    //________GET DATE____________
    if (RTCC_TimeGet(&picDate)) {
        Date d;
        d.dateVal = 0;
        d.Format.yy = picDate.tm_year;
        d.Format.mom = picDate.tm_mon;
        d.Format.day = picDate.tm_mday;
        d.Format.h = picDate.tm_hour;
        d.Format.min = picDate.tm_min;
        d.Format.sec = picDate.tm_sec;
        //____________________________
        //________SEND DATE___________
        return MASTER_SendMsgRF(ID_BROADCAST, HORLOGE, 1, 1, d.date, 5); // visualiser cette valeur 
    }
    return 0;
}


//______________________________________________________________________________
//________________________________STATE MACHINE FUNCTION________________________

bool MASTER_StoreBehavior(MASTER_STATES state, PRIORITY prio) {
    behavior[prio][GETpWRITE(prio)] = state; // on ecrit le comportement 
    INCpWRITE(prio);
    if (GETpREAD(prio) == GETpWRITE(prio)) { // Je viens d'ecraser un comportement 
        //        SET_0VFF(prio, 1); // overflow
        INCpREAD(prio);
    }
    return true; // 
}

MASTER_STATES MASTER_GetBehavior() {
    // on cmmence par chercher un comportement de prio eleve
    MASTER_STATES state = MASTER_STATE_IDLE;
    // l'ordre des condition est important car on respect la priorite
    if (GETpREAD(HIGH) != GETpWRITE(HIGH)) {
        // on ne peut avoir l'egalite et a voir un comportement present 
        state = behavior[HIGH][GETpREAD(HIGH)];
        INCpREAD(HIGH);
    } else if (GETpREAD(MEDIUM) != GETpWRITE(MEDIUM)) {
        state = behavior[MEDIUM][GETpREAD(MEDIUM)];
        INCpREAD(MEDIUM);
    } else if (GETpREAD(LOW) != GETpWRITE(LOW)) {
        state = behavior[LOW][GETpREAD(LOW)];
        INCpREAD(LOW);
    }
    return state;
}

void MASTER_AppTask() { // machiine a etat general
    MASTER_STATES state = MASTER_GetBehavior();
    switch (state) {
        case MASTER_STATE_INIT:
            if (state != prevBehavior) {
                prevBehavior = state;
#if defined (UART_DEBUG)
                displayBootMessage();
                printf("> MASTER_STATE_INITIALIZE\n");
                powerRFEnable();
                // Check the power statut of the RF module
                if (CMD_3v3_RF_GetValue() == false) {
                    printf("RF Module enable.\n");
                    radioAlphaTRX_Init();
                    radioAlphaTRX_ReceivedMode(); // receive mode actived
                } else {
                    printf("RF Module disable.\n");
                    printf("Send 'T' to change power state of radio module.\n");
                }
#endif
            }
            printf("Go to MASTER_STATE_IDLE...\n");
            break;
            /* -------------------------------------------------------------- */
        case MASTER_STATE_IDLE:
        {
            if (state != prevBehavior) {
                prevBehavior = state;
#if defined(UART_DEBUG)
                printf(">MASTER STATE IDLE\n");
#endif
            }
            struct tm t;
            LedsStatusBlink(LED_GREEN, 20, 1980);
#if defined(UART_DEBUG)
            RTCC_TimeGet(&t);
            if (t.tm_sec % 10 == 0 && noPrint) {
                noPrint = 0;
                printf("[heur ==> %dh:%dmin:%ds]\n", t.tm_hour, t.tm_min, t.tm_sec);

            } else if (t.tm_sec % 10 != 0 && !noPrint) {
                noPrint = 1;
            }
            APP_SerialDebugTasks();
#endif     
            break;
        }
            /* -------------------------------------------------------------- */
        case MASTER_STATE_MSG_RF_RECEIVE:
            break;
            /* -------------------------------------------------------------- */
        case MASTER_STATE_SEND_DATE:
            break;
            /* -------------------------------------------------------------- */
        case MASTER_STATE_SEND_REQUEST_INFOS:
            break;
            /* -------------------------------------------------------------- */
        case MASTER_STATE_ERROR:
            break;
            /* -------------------------------------------------------------- */
        default:

            break;
            /* -------------------------------------------------------------- */
    }
}

void MASTER_AppInit() {
    MASTER_StoreBehavior(MASTER_STATE_INIT, HIGH);
}

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/