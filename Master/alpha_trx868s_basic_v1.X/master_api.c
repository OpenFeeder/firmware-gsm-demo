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
volatile uint8_t behavior[MAX_LEVEL_PRIO][NB_BEHAVIOR_PER_PRIO]; // tableau des comportement 
//
// les pointeurs d'ecriture et de l'ecture des buffer circulaire 
volatile uint8_t ptr[MAX_LEVEL_PRIO][3]; //READ - WRITE - OVFF (overflow)


//buffer de collecte de donnees 
uint8_t BUFF_COLLECT[NB_DATA_BUF][SIZE_DATA];

//ensembele de slave de ce site 
SlaveState ensSlave[NB_SLAVE];

//slave en cours d'interogation
int8_t slaveSelected = 0;

//previous behavior
MASTER_STATES prevBehavior = MASTER_STATE_NONE;

//DEbug
int8_t noPrint = 0;

/**-------------------------->> M A C R O S <<--------------------------------*/
#define GETpREAD(prio) ptr[prio][READ] // recupere le ponteur de lecture
#define GETpWRITE(prio) ptr[prio][WRITE] // recupere le pointeur d'ecriture
#define GET_OVFF(prio)  ptr[prio][OVFF] // recipere le pointeur d'overFlow

#define INCpREAD(prio) (ptr[prio][READ] = (ptr[prio][READ]+1) % NB_BEHAVIOR_PER_PRIO)
#define INCpWRITE(prio) (ptr[prio][WRITE] = (ptr[prio][WRITE]+1) % NB_BEHAVIOR_PER_PRIO)
#define SET_0VFF(prio, set) (ptr[prio][OVFF] = set)

/**-------------------------->> S T R U C T U R E -- S L A V E <<--------------*/

/**-------------------------->> D E B U G <<----------------------------------*/
void printPointeur(PRIORITY prio) {
    printf("prio %d : \npREAD %d\nWRITE %d\n", prio, GETpREAD(prio), GETpWRITE(prio));
}
//______________________________________________________________________________



/**-------------------------->> L O C A L -- F O N C T I O N S <<--------------*/

//_______________________________ IMPLEMENTATION________________________________

int8_t MASTER_SendMsgRF(uint8_t dest,
                        uint8_t typeMsg,
                        uint8_t idMsg,
                        uint8_t nbR,
                        uint8_t * data,
                        uint8_t sizeData) {
    Frame frameToSend;
    memset(frameToSend.paquet, 0, FRAME_LENGTH);
    //_____________CREATE FRAME____________________________________________
    int8_t ret = 0;
    // en tete 
    frameToSend.Champ.dest = dest;
    frameToSend.Champ.src = MASTER_ID;
    frameToSend.Champ.crc ^= frameToSend.paquet[0];
    frameToSend.Champ.nbR = nbR;
    frameToSend.Champ.typeMsg = typeMsg;
    frameToSend.Champ.crc ^= frameToSend.paquet[1];
    frameToSend.Champ.idMsg = idMsg;
    frameToSend.Champ.crc ^= frameToSend.paquet[2];
    // data
    int8_t i;
    frameToSend.Champ.size = sizeData;
    frameToSend.Champ.crc ^= frameToSend.paquet[3];
    for (i = 0; i < frameToSend.Champ.size; i++) {
        frameToSend.Champ.data[i] = data[i];
        frameToSend.Champ.crc ^= frameToSend.paquet[i + 5]; // penser � changer le 5 en generique 
    }

    for (i = 0; i < frameToSend.Champ.size + 5; i++) {
        printf("%d ", frameToSend.paquet[i]);
    }
#if defined(UART_DEBUG)
    printf("\nsize\n");
#endif

    //____________________________________________________________________
    //________________________TRANSMSISSION_______________________________
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
        return MASTER_SendMsgRF(BROADCAST_ID, HORLOGE, 1, 1, d.date, 5); // visualiser cette valeur 
    }
    return 0;
}


//______________________________________________________________________________
//________________________________STATE MACHINE FUNCTION________________________

bool MASTER_GetNextSlave() {
    int8_t i = (slaveSelected + 1) % NB_SLAVE;
    bool stop = false;
    bool b = false;
    do {
        if (ensSlave[i].state == SLAVE_ERROR ||
            ensSlave[i].state == SLAVE_COLLECT_END) {
            i = (i + 1) % NB_SLAVE;
            if (i == slaveSelected) {
                stop = true; // pas de slave operationel  
            }
        } else {
            stop = true; // on a trouve un slave
            ensSlave[i].state = SLAVE_SYNC;
            slaveSelected = i;
            b = true;
        }
    } while (!stop);
#if defined(UART_DEBUG)
    printf("slave %d selected\n", slaveSelected + 1);
#endif
}

bool MASTER_StoreBehavior(MASTER_STATES state, PRIORITY prio) {
    behavior[prio][GETpWRITE(prio)] = state; // on ecrit le comportement 
    INCpWRITE(prio);
    if (GETpREAD(prio) == GETpWRITE(prio)) { // Je viens d'ecraser un comportement 
        //        SET_0VFF(prio, 1); // overflow
        INCpREAD(prio);
    }
    LED_STATUS_R_Toggle();
    return true; // 
}

MASTER_STATES MASTER_GetBehavior() {
    // on cmmence par chercher un comportement de prio eleve
    MASTER_STATES state = MASTER_STATE_IDLE;
    // l'ordre des condition est important car on respect la priorite
    if (GETpREAD(PRIO_HIGH) != GETpWRITE(PRIO_HIGH)) {
        // on ne peut avoir l'egalite et a voir un comportement present 
        state = behavior[PRIO_HIGH][GETpREAD(PRIO_HIGH)];
        INCpREAD(PRIO_HIGH);
    } else if (GETpREAD(PRIO_MEDIUM) != GETpWRITE(PRIO_MEDIUM)) {
        state = behavior[PRIO_MEDIUM][GETpREAD(PRIO_MEDIUM)];
        INCpREAD(PRIO_MEDIUM);
    } else if (GETpREAD(PRIO_LOW) != GETpWRITE(PRIO_LOW)) {
        state = behavior[PRIO_LOW][GETpREAD(PRIO_LOW)];
        INCpREAD(PRIO_LOW);
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
#endif
                powerRFEnable();
                // Check the power statut of the RF module
                if (CMD_3v3_RF_GetValue() == false) {
#if defined (UART_DEBUG)
                    printf("RF Module enable. ----> power off\n");
#endif
                    radioAlphaTRX_Init();
                    radioAlphaTRX_ReceivedMode(); // receive mode actived
                } else {
                    MASTER_StoreBehavior(MASTER_STATE_ERROR, PRIO_HIGH);
#if defined (UART_DEBUG)
                    printf("RF Module disable.----> no power on\n");
#endif
                }

            }
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

        case MASTER_STATE_TIMEOUT: //if an event has occurred with the timer 
        {
            switch (ensSlave[slaveSelected].state) {
                case SLAVE_SYNC:
#if defined(UART_DEBUG)
                    printf("hundler error phase de synchro\n");
#endif
                    if (--ensSlave[slaveSelected].tryToConnect) {
                        MASTER_SendMsgRF(ensSlave[slaveSelected].idSlave,
                                         DATA,
                                         ensSlave[slaveSelected].nbBloc, 1,
                                         (uint8_t *) ("DATA"),
                                         4); // attention a changer
                        TMR_SetWaitRqstTimeout(TIME_OUT_COLLECT_LOG); // active timer 
                    } else { // on ne peut pas se connecter ? ce slave 
                        ensSlave[slaveSelected].nbError--;
                        if (!ensSlave[slaveSelected].nbError) {
#if defined(UART_DEBUG)
                            printf("hundler slave %d communication corompu\n ERROR ==> SELECT SLAVE\n",
                                   ensSlave[slaveSelected].idSlave);
#endif
                            ensSlave[slaveSelected].state = SLAVE_ERROR;
                        }
                        MASTER_StoreBehavior(MASTER_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
                    }
                    break;
                    /*-----------------------------------------------------------------*/
                case SLAVE_COLLECT:
#if defined(UART_DEBUG)
                    printf("hundler error phase de collect\n");
#endif
                    if (--ensSlave[slaveSelected].nbTimeout) {
#if defined(UART_DEBUG)
                        printf("envoit d'ack \n");
#endif
                        MASTER_SendMsgRF(ensSlave[slaveSelected].idSlave,
                                         ACK,
                                         ensSlave[slaveSelected].index, 1,
                                         (uint8_t *) ("ACK"), 3); // demande du paquet attendu 
                        TMR_SetWaitRqstTimeout(TIME_OUT_COLLECT_LOG);
                    } else { // on une interuption dans la collect
                        ensSlave[slaveSelected].nbError--;
                        if (!ensSlave[slaveSelected].nbError) {
#if defined(UART_DEBUG)
                            printf("slave %d communication corompu\n",
                                   ensSlave[slaveSelected].idSlave);
#endif
                            ensSlave[slaveSelected].state = SLAVE_ERROR; // informer le master 
                            MASTER_StoreBehavior(MASTER_STATE_ERROR, PRIO_HIGH);
                        }
                    }
                    break;
                    /*-----------------------------------------------------------------*/

                case SLAVE_DAYTIME: // j'aurais pu utiliser le neutre
                    ensSlave[slaveSelected].nbTimeout++;
                    if (--ensSlave[slaveSelected].nbTimeout)
                        MASTER_StoreBehavior(MASTER_STATE_SEND_REQUEST_INFOS, PRIO_MEDIUM);
                    else
                        if (!(--ensSlave[slaveSelected].nbError)) {
                        ensSlave[slaveSelected].state = SLAVE_ERROR;
                        MASTER_StoreBehavior(MASTER_STATE_ERROR, PRIO_HIGH);
                    }
                    break;
                default:
                    break;
            }
        }
            break;
            /* -------------------------------------------------------------- */
        case MASTER_STATE_MSG_RF_RECEIVE:
        {
            Frame receive = radioAlphaTRX_GetFrame();
            if (state != prevBehavior) {
                prevBehavior = state;
#if defined(UART_DEBUG)
                printf(">MASTER_STATE_MSG_RECEIVE\n");
                printf("recu %s\n", receive.Champ.data);
#endif
            }
            ensSlave[slaveSelected].nbError = MAX_ERROR;
            ensSlave[slaveSelected].nbTimeout = MAX_TIMEOUT;

            switch (receive.Champ.typeMsg) {
                case DATA:
#if defined(UART_DEBUG)
                    printf("DATA RECEIVE\n");
#endif
                    if (ensSlave[slaveSelected].state == SLAVE_SYNC) {
#if defined(UART_DEBUG)
                        printf("Slave %d connect\n", ensSlave[slaveSelected].idSlave);
#endif
                        ensSlave[slaveSelected].tryToConnect = MAX_TRY_TO_SYNC; // on reset le nombre de demande de connxion 
                        ensSlave[slaveSelected].state = SLAVE_COLLECT; //changement d'etat
                    }

                    if (receive.Champ.idMsg == ensSlave[slaveSelected].index) {
                        ensSlave[slaveSelected].state = SLAVE_COLLECT;
                        strncpy(BUFF_COLLECT[ensSlave[slaveSelected].index - 1],
                                receive.Champ.data, receive.Champ.size); // save data

                        if (receive.Champ.nbR == MAX_W) {
                            MASTER_StoreBehavior(MASTER_STATE_SEND_FROME_GSM, PRIO_HIGH);
                            ensSlave[slaveSelected].state = SLAVE_COLLECT_END_BLOCK;
#if defined(UART_DEBUG)
                            printf("END BLOC\n");
#endif  
                        } else if (receive.Champ.nbR == MAX_W + 1) { // fin de trans
                            MASTER_StoreBehavior(MASTER_STATE_SEND_FROME_GSM, PRIO_HIGH);
                            ensSlave[slaveSelected].state = SLAVE_COLLECT_END;
#if defined(UART_DEBUG)
                            printf("END BLOC Trans\n");
#endif
                        } else {
                            ensSlave[slaveSelected].index += 1;
                            // on demarre le timeout
                            if (receive.Champ.nbR > 1) { // je suis plus en attente d'un paquet
                                TMR_SetWaitRqstTimeout(TIME_OUT_COLLECT_LOG);
                            } else {
                                TMR_SetWaitRqstTimeout(-1); // deactive timer up to (until) request 
                            }
                        }
                    } else {
#if defined(UART_DEBUG)
                        printf("numSeq attendu %d vs %d recu \n",
                               ensSlave[slaveSelected].index, receive.Champ.idMsg);
                        TMR_SetWaitRqstTimeout(0);
#endif
                    }
                    break;
                    /* -------------------------------------------------------*/
                case ERROR:
                    TMR_SetWaitRqstTimeout(0); // declencher le l'interuption logiciel  
#if defined(UART_DEBUG)
                    printf("ERROR RECEIVE %d\n", receive.Champ.idMsg);
#endif
                    //TODO : traitement d'erreur send ack 
                    break;
                    /* -------------------------------------------------------*/
                case NOTHING:
                    TMR_SetWaitRqstTimeout(0);
#if defined(UART_DEBUG )
                    printf("Slave %d Nothing to send\n", ensSlave[slaveSelected].idSlave);
#endif
                    break;
                    /* -------------------------------------------------------*/
            }
            break;
        }
            /* -------------------------------------------------------------- */
        case MASTER_STATE_SEND_DATE: // niveau HIGH
#if defined(UART_DEBUG)
            printf("send date %d \n", MASTER_SendDateRF());
#else
            MASTER_SendDateRF();
#endif
            TMR_SetHorlogeTimeout(SEND_HORLOG_TIMEOUT);
            break;
            /* -------------------------------------------------------------- */
        case MASTER_STATE_SEND_REQUEST_INFOS:
            //pour tout transmission RF hors la date, on passe par cet etat: niveau medium
            MASTER_SendMsgRF(ensSlave[slaveSelected].idSlave,
                             INFOS,
                             1, 1, (uint8_t *) ("INFOS"), 5); // demande du paquet attendu 
            TMR_SetWaitRqstTimeout(TIME_OUT_WAIT_RQST); // on demare le timer 
            break;
            /* -------------------------------------------------------------- */

        case MASTER_STATE_SELECTE_SLAVE:
        {
            int8_t i = (slaveSelected + 1) % NB_SLAVE;
            bool stop = false;
            bool b = false;
            do {
                if (ensSlave[i].state == SLAVE_ERROR ||
                    ensSlave[i].state == SLAVE_COLLECT_END) {
                    i = (i + 1) % NB_SLAVE;
                    if (i == slaveSelected) {
                        stop = true; // pas de slave operationel  
                    }
                } else {
                    stop = true; // on a trouve un slave
                    slaveSelected = i;
                    b = true;
                }
            } while (!stop);

            if (b) {
#if defined(UART_DEBUG)
                printf("slave %d selected\n", slaveSelected + 1);
#endif
                //en foction de l'heur de la journee on change d'etat 
                struct tm t; 
                if (RTCC_TimeGet(&t)) {
                    if (t.tm_hour < TIME_LIMIT_OF_CONFIG) {
                        ensSlave[i].state = SLAVE_CONFIG;
                    }else if (t.tm_hour < TIME_LIMIT_TO_GET_INFOS) {
                        ensSlave[i].state = SLAVE_DAYTIME;
                    }else {
                        ensSlave[i].state = SLAVE_SYNC;
                    }
                    TMR_SetWaitRqstTimeout(0); // declenche une interuption logiciel 
                }else {
#if defined(UART_DEBUG)
                    printf("La date n'est pas bonne \n");
#endif
                    MASTER_StoreBehavior(MASTER_STATE_ERROR, PRIO_HIGH); // error 
                }
            }else {
#if defined(UART_DEBUG)
                printf("Aucun slave en selection\n");
#endif
                MASTER_StoreBehavior(MASTER_STATE_END, PRIO_HIGH);
            }

            break;
            /* -------------------------------------------------------------- */
        }
        case MASTER_STATE_ERROR: // NIVEAU HIGH, etat de traitement des erreur lie au master 
            break;
            /* -------------------------------------------------------------- */
            
        case MASTER_STATE_END:
            //TODO : gestion de fin
            break;
            /* -------------------------------------------------------------- */
        default:

            break;
            /* -------------------------------------------------------------- */
    }
}

void MASTER_AppInit() {
    ptr[PRIO_HIGH][READ] = 0;
    ptr[PRIO_HIGH][WRITE] = 0;
    ptr[PRIO_MEDIUM][READ] = 0;
    ptr[PRIO_MEDIUM][WRITE] = 0;
    ptr[PRIO_LOW][READ] = 0;
    ptr[PRIO_LOW][WRITE] = 0;

    //init master param
    int i = 0;
    for (; i < NB_SLAVE; i++) {
        ensSlave[i].idSlave = i + 1;
        ensSlave[i].index = 1;
        ensSlave[i].nbBloc = 1;
        ensSlave[i].state = SLAVE_NONE;
        ensSlave[i].nbError = MAX_ERROR;
        ensSlave[i].nbTimeout = MAX_TIMEOUT;
        ensSlave[i].tryToConnect = MAX_TRY_TO_SYNC;
    }

    MASTER_StoreBehavior(MASTER_STATE_INIT, PRIO_HIGH);
}

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/