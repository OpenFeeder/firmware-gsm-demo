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

//______________________________________________________________________________
//______________________________DEBUG___________________________________________
volatile uint8_t dayTime = 0; // 0 before; 1 during; 2 night

void modif(int val) { // changement de journee ==> selection de slave
    dayTime = val;
    MASTER_StoreBehavior(MASTER_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
#if defined(UART_DEBUG)
    printPointeur(PRIO_HIGH);
    printPointeur(PRIO_MEDIUM);
    printPointeur(PRIO_LOW);
    printf("moment %d \n", dayTime);
#endif
}

void display_STATUS_register_from_RF_module(void) {
    if (false == CMD_3v3_RF_GetValue()) {
        /* Read RF module STATUS register */
        RF_StatusRead.Val = radioAlphaTRX_Command(STATUS_READ_CMD);
        printf("Read RF STATUS register: 0x%04X\n", RF_StatusRead.Val); // 4.1. ?criture format?e de donn?es --> https://www.ltam.lu/cours-c/prg-c42.htm
        printf("Bit [ 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 ]\n");
        printf("    [  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u ]\n", \
                        RF_StatusRead.bits.b15_RGIT_FFIT, \
                        RF_StatusRead.bits.b14_POR, \
                        RF_StatusRead.bits.b13_RGUR_FFOV, \
                        RF_StatusRead.bits.b12_WKUP, \
                        RF_StatusRead.bits.b11_EXT, \
                        RF_StatusRead.bits.b10_LBD, \
                        RF_StatusRead.bits.b9_FFEM, \
                        RF_StatusRead.bits.b8_ATSS, \
                        RF_StatusRead.bits.b7_ATS_RSSI, \
                        RF_StatusRead.bits.b6_DQD, \
                        RF_StatusRead.bits.b5_CRL, \
                        RF_StatusRead.bits.b4_ATGL, \
                        RF_StatusRead.bits.b3_OFFS_Sign, \
                        RF_StatusRead.bits.b2_OFFS_b2, \
                        RF_StatusRead.bits.b1_OFFS_b1, \
                        RF_StatusRead.bits.b0_OFFS_b0 \
                        );
        printf("     FFIT| POR|FFOV|WKUP| EXT| LBD|FFEM|RSSI| DQD| CRL|ATGL|OFFS|OFFS|OFFS|OFFS|OFFS|\n"); // transceiver in receive (RX) mode, bit 'er' is set
        printf("     RGIT      RGUR                      ATS                 <6>  <3>  <2>  <1>  <0>\n"); // bit 'er' is cleared
    } else {
        printf("Can't read status register, RF module is power OFF!\n");
    }
}
//______________________________________________________________________________

/**-------------------------->> V A R I A B L E S <<---------------------------*/
volatile uint8_t behavior[MAX_LEVEL_PRIO][NB_BEHAVIOR_PER_PRIO]; // tableau des comportement 
//
// les pointeurs d'ecriture et de l'ecture des buffer circulaire 
volatile uint8_t ptr[MAX_LEVEL_PRIO][3]; //READ - WRITE - OVFF (overflow)



//buffer de collecte de donnees 
uint8_t BUFF_COLLECT[NB_DATA_BUF][SIZE_DATA];

uint8_t publicSlaveAdress[NB_SLAVE]; // id reel des slave 

//ensembele de slave de ce site, adresse local 
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

#define GET_PUBLIC_ID_SLAVE(localIdSlave) (publicSlaveAdress[localIdSlave])
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
        frameToSend.Champ.crc ^= frameToSend.paquet[i + 5]; // penser ? changer le 5 en generique 
    }

    //    for (i = 0; i < frameToSend.Champ.size + 5; i++) {
    //        printf("%d ", frameToSend.paquet[i]);
    //    }
    //#if defined(UART_DEBUG)
    //    printf("\nsize\n");
    //#endif

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
    if (!app_UpdateRtcTimeFromGSM()) {
#if defined(UART_DEBUG)
        printf("No update from GSM\n");
#endif
        return 0;
    }
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
#if defined(UART_DEBUG)
        printf("SEND DATE\n");
#endif
        return MASTER_SendMsgRF(BROADCAST_ID, HORLOGE, 1, 1, d.date, 5); // visualiser cette valeur 
    }
    return 0;
}


//______________________________________________________________________________
//________________________________STATE MACHINE FUNCTION________________________

uint8_t MASTER_GetSlaveSelected() {
    return (ensSlave[slaveSelected].idSlave);
}

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
                printf("> MASTER_STATE_INITIALIZE\n");
#endif
                radioAlphaTRX_Init();
                radioAlphaTRX_ReceivedMode(); // receive mode actived
            }
            MASTER_StoreBehavior(MASTER_STATE_SELECTE_SLAVE, PRIO_HIGH);
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
#if defined(UART_DEBUG)
            if (RTCC_TimeGet(&t)) {
                if (t.tm_sec % 10 == 0 && noPrint) {
                    noPrint = 0;
                    printf("[heur ==> %02d:%02d:%02d]\n", t.tm_hour, t.tm_min, t.tm_sec);

                } else if (t.tm_sec % 10 != 0 && !noPrint) {
                    noPrint = 1;
                }
            }
            if (TMR_GetWaitRqstTimeout() == -1) {
                TMR_SetWaitRqstTimeout(TIME_OUT_WAIT_RQST);
            }
            APP_SerialDebugTasks();
#endif     
            break;
        }
            /* -------------------------------------------------------------- */

        case MASTER_STATE_TIMEOUT: //if an event has occurred with the timer 
            if (state != prevBehavior) {
                prevBehavior = state;
#if defined(UART_DEBUG)
                printf(">MASTER STATE TIMEOUT\n");
#endif
            }
        {
            switch (ensSlave[slaveSelected].state) {
                case SLAVE_SYNC:
#if defined(UART_DEBUG)
                    printf("hundler error phase de synchro try to connect%d \n",
                           ensSlave[slaveSelected].tryToConnect);
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
                        if (ensSlave[slaveSelected].nbError == 0) {
#if defined(UART_DEBUG)
                            printf("hundler slave %d communication corompu\n ERROR ==> SELECT SLAVE\n",
                                   ensSlave[slaveSelected].idSlave);
#endif
                            ensSlave[slaveSelected].state = SLAVE_ERROR;
                            MASTER_StoreBehavior(MASTER_STATE_ERROR, PRIO_HIGH); // traitement de l'erreur 
                        } else {
                            MASTER_StoreBehavior(MASTER_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
                        }
                    }
                    break;
                    /*-----------------------------------------------------------------*/
                case SLAVE_COLLECT:
#if defined(UART_DEBUG)
                    printf("hundler error phase de collect\n");
#endif
                    if (--ensSlave[slaveSelected].nbTimeout) {
#if defined(UART_DEBUG)
                        printf("envoit d'ack N? %d \n", ensSlave[slaveSelected].index);
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

                case SLAVE_DAYTIME:
                    --ensSlave[slaveSelected].nbTimeout;
                    if (ensSlave[slaveSelected].nbTimeout > 0) {
                        MASTER_StoreBehavior(MASTER_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
                    } else {
                        --ensSlave[slaveSelected].nbError;
                        if (ensSlave[slaveSelected].nbError < 0) {
                            ensSlave[slaveSelected].state = SLAVE_ERROR;
                            MASTER_StoreBehavior(MASTER_STATE_ERROR, PRIO_HIGH);
                        }
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
                            TMR_SetWaitRqstTimeout(-1);
                            MASTER_StoreBehavior(MASTER_STATE_SEND_FROM_GSM, PRIO_HIGH);
                            ensSlave[slaveSelected].state = SLAVE_COLLECT_END_BLOCK;
#if defined(UART_DEBUG)
                            printf("END BLOC\n");
#endif  
                        } else if (receive.Champ.nbR == MAX_W + 1) { // fin de trans
                            TMR_SetWaitRqstTimeout(-1);
                            MASTER_StoreBehavior(MASTER_STATE_SEND_FROM_GSM, PRIO_HIGH);
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
                                TMR_SetWaitRqstTimeout(0); // deactive timer up to (until) request 
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
                {
                    bool b = true;
                    uint8_t bufErrorMSG [256];
                    //                    TMR_SetWaitRqstTimeout(0); // declencher le l'interuption logiciel  
#if defined(UART_DEBUG)
                    printf("ERROR RECEIVE %d\n", receive.Champ.idMsg);
#endif
                    //TODO : traitement d'erreur send ack
                    switch (receive.Champ.idMsg) {
                            //different type d'erreur 
                        case ERROR_NONE:
#if defined(UART_DEBUG)
                            printf("transmission d'une erreur inconnue\n");
#endif
                            sprintf(bufErrorMSG, "OF %d du site %d ERREUR CRITIQUE INCONNUE",
                                    GET_PUBLIC_ID_SLAVE(slaveSelected), STATION);
                            break;
                        case ERROR_LOW_BATTERY:
#if defined(UART_DEBUG)
                            printf("transmission de niveau de batterie faible\n");
#endif
                            sprintf(bufErrorMSG, "OF %d du site %d niveau de batterie faible",
                                    GET_PUBLIC_ID_SLAVE(slaveSelected), STATION);
                            break;
                        case ERROR_LOW_FOOD:
#if defined(UART_DEBUG)
                            printf("transmission de niveau de nourriture faible\n");
#endif
                            sprintf(bufErrorMSG, "OF %d du site %d niveau de nourriture faible",
                                    GET_PUBLIC_ID_SLAVE(slaveSelected), STATION);
                            break;
                        case ERROR_LOW_VBAT:
#if defined(UART_DEBUG)
                            printf("transmission de ERROR_LOW_VBAT\n");
#endif
                            sprintf(bufErrorMSG, "OF %d du site %d niveau de .... faible: ERROR_LOW_VBAT",
                                    GET_PUBLIC_ID_SLAVE(slaveSelected), STATION);
                            break;
                        case ERROR_DOOR_CANT_CLOSE:
#if defined(UART_DEBUG)
                            printf("transmission de PORTE OUVERTE\n");
#endif      

                            sprintf(bufErrorMSG, "OF %d du site %d PORTE OUVERTE",
                                    GET_PUBLIC_ID_SLAVE(slaveSelected), STATION);
                            b = true;
                            break;
                        default:
#if defined(UART_DEBUG)
                            b = false;
                            printf("Erreur non connue\n");
#endif
                            break;
                    }
                    if (b) { // aquitter 
                        if (app_SendSMS(bufErrorMSG)) {
                            MASTER_SendMsgRF(ensSlave[slaveSelected].idSlave,
                                         ACK, receive.Champ.idMsg, 1, "ACK", 3);
                        }else {
#if defined(UART_DEBUG)
                            printf("ERROR non transmisi\n");
#endif
                        }
                    } else {
#if defined(UART_DEBUG)
                        printf("ERROR non reconuue \n");
#endif
                    }
                    TMR_SetWaitRqstTimeout(0); // fin timeout
                }
                    break;
                    /* -------------------------------------------------------*/
                case NOTHING:
                    //                    TMR_SetWaitRqstTimeout(0); // on laisse le timer se terminer pour les test
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
            if (state != prevBehavior) {
                prevBehavior = state;
#if defined(UART_DEBUG)
                printf(">MASTER STATE SEND DATE\n");
#endif
            }
#if defined(UART_DEBUG)
            printf("send date %d \n", MASTER_SendDateRF());
#else
            MASTER_SendDateRF();
#endif
            TMR_SetHorlogeTimeout(SEND_HORLOG_TIMEOUT);
            break;
            /* -------------------------------------------------------------- */
        case MASTER_STATE_SEND_REQUEST_INFOS:
            if (state != prevBehavior) {
                prevBehavior = state;
#if defined(UART_DEBUG)
                printf(">MASTER STATE SEND REQUEST INFOS\n");
#endif
            }
            //pour tout transmission RF hors la date, on passe par cet etat: niveau medium
            MASTER_SendMsgRF(ensSlave[slaveSelected].idSlave,
                             INFOS,
                             1, 1, (uint8_t *) ("INFOS"), 5); // demande du paquet attendu 
            TMR_SetWaitRqstTimeout(TIME_OUT_WAIT_RQST); // on demare le timer 
            break;
            /* -------------------------------------------------------------- */

        case MASTER_STATE_SELECTE_SLAVE:
            if (state != prevBehavior) {
                prevBehavior = state;
#if defined(UART_DEBUG)
                printf(">MASTER STATE SELECT SLAVE\n");
#endif
            }
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
#if defined(UART_DEBUG)
                //en foction de l'heur de la journee on change d'etat 
                if (dayTime == 0) {
                    ensSlave[i].state = SLAVE_CONFIG;
                    TMR_SetWaitRqstTimeout(0);
                } else if (dayTime == 1) {
                    ensSlave[i].state = SLAVE_DAYTIME;
                    MASTER_StoreBehavior(MASTER_STATE_SEND_REQUEST_INFOS, PRIO_MEDIUM);
                } else if (dayTime == 2) {
                    ensSlave[slaveSelected].tryToConnect = MAX_TRY_TO_SYNC;
                    ensSlave[i].state = SLAVE_SYNC;
                    TMR_SetWaitRqstTimeout(0); // declenche une interuption logiciel 
                }
#else
                struct tm t;
                if (RTCC_TimeGet(&t)) {
                    if (t.tm_hour < TIME_LIMIT_OF_CONFIG) {
                        ensSlave[i].state = SLAVE_CONFIG;
                    } else if (t.tm_hour < TIME_LIMIT_TO_GET_INFOS) {
                        ensSlave[i].state = SLAVE_DAYTIME;
                    } else {
                        ensSlave[i].state = SLAVE_SYNC;
                    }
                    TMR_SetWaitRqstTimeout(0); // declenche une interuption logiciel 
                } else {
#if defined(UART_DEBUG)
                    printf("La date n'est pas bonne \n");
#endif
                    MASTER_StoreBehavior(MASTER_STATE_ERROR, PRIO_HIGH); // error 
                }
#endif
            } else {
#if defined(UART_DEBUG)
                printf("Aucun slave en selection\n");
#endif
                MASTER_StoreBehavior(MASTER_STATE_END, PRIO_HIGH);
            }

            break;
            /* -------------------------------------------------------------- */
        }
        case MASTER_STATE_SEND_FROM_GSM:
        {
            if (state != prevBehavior) {
                prevBehavior = state;
#if defined(UART_DEBUG)
                printf(">MASTER_STATE_SEND_FROM_GSM\n");
#endif
            }
            int8_t j = 0;
#if defined(UART_DEBUG)
            printf("ind %d\n", ensSlave[slaveSelected].index);
#endif
            for (; j < ensSlave[slaveSelected].index - 1; j++) {
                printf("[%d]  -> %s\n", j, BUFF_COLLECT[j]);
            }
            //TODO : go to select slave 
            MASTER_StoreBehavior(MASTER_STATE_SELECTE_SLAVE, MEDIUM);
            // prepare l'attente d'un nouveau bloc ou c'est la fin 
            if (ensSlave[slaveSelected].state == SLAVE_COLLECT_END_BLOCK) {
                ensSlave[slaveSelected].index = 1;
                ensSlave[slaveSelected].nbBloc++;
            }
        }
            break;
            /* -------------------------------------------------------------- */
        case MASTER_STATE_ERROR: // NIVEAU HIGH, etat de traitement des erreur lie au master 
            if (state != prevBehavior) {
                prevBehavior = state;
                // deux type d'erreur ici ceux lie au slave, et ceux lie au master lui meme 
                // erreur liee aux slaves : 
                //  * connexion interrompu avec un slave apres plusieurs tentative de connexion 
                //    le master sais que c'est le slave et non lui, car les autre marche slaves 
                //    communique avec lui 
                //
                //  ==> Traitement, envoie d'un sms pour anoncer l'etat du slave
                //
                // erreur liee au master :
                //  * connexion interrompus avec tous les slaves: ils sont en etat d'erreur tous 
                //    les 4 (8) et aucun ne repond.
                //
                //  ==> traitement, envoie d'un sms renseignant de l'etat de connexion entre 
                //   le master et les slaves, (erreur critique car meme la syncro ne se ferra pas)
                //  
                //  * Module GSM non fonctionnel, soit plus de sms, plus de data, impossible a 
                //    l'allumer, on capte plus de reseau, carte sim infonctionnel 
                //
                //  ==> je ne sais pas ce qu'il faut faire dans ce cas (a discuter avec le responsable)
#if defined(UART_DEBUG)
                printf(">MASTER STATE ERROR\n");
#endif
            }
            break;
            /* -------------------------------------------------------------- */

        case MASTER_STATE_END:
            if (state != prevBehavior) {
                prevBehavior = state;
#if defined(UART_DEBUG)
                printf(">MASTER STATE END\n");
#endif
            }
            //TODO : gestion de fin
            break;
            /* -------------------------------------------------------------- */
        default:
            if (state != prevBehavior) {
                prevBehavior = state;
#if defined(UART_DEBUG)
                printf(">MASTER STATE DEFAULT\n");
#endif
            }
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
    ensSlave[0].idSlave = 1;
    ensSlave[1].idSlave = 2;

    for (i = 0; i < NB_DATA_BUF; i++) {
        memset(BUFF_COLLECT[i], 0, SIZE_DATA);
    }

    publicSlaveAdress[0] = SLAVE_ID_PUB1;
    publicSlaveAdress[1] = SLAVE_ID_PUB2;
    publicSlaveAdress[2] = SLAVE_ID_PUB3;
    publicSlaveAdress[3] = SLAVE_ID_PUB4;
    if (NB_SLAVE > 4) {
        publicSlaveAdress[4] = SLAVE_ID_PUB5;
        publicSlaveAdress[5] = SLAVE_ID_PUB6;
        publicSlaveAdress[6] = SLAVE_ID_PUB7;
        publicSlaveAdress[7] = SLAVE_ID_PUB8;
    }

    MASTER_StoreBehavior(MASTER_STATE_INIT, PRIO_HIGH);
}

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/