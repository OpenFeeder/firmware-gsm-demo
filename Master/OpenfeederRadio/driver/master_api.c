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
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "master_api.h"
#include "timer.h"

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
    if (true == nRES_GetValue()) {
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
MASTER_DATA appData; /* Global application data. */
MASTER_ERROR appError; /* Errors application data */


//DEbug
int8_t print = 0;

/**-------------------------->> M A C R O S <<--------------------------------*/
#define GETpREAD(prio) appData.ptr[prio][PTR_READ] // recupere le ponteur de lecture
#define GETpWRITE(prio) appData.ptr[prio][PTR_WRITE] // recupere le pointeur d'ecriture
#define GET_OVFF(prio)  appData.ptr[prio][PTR_OVFF] // recipere le pointeur d'overFlow

#define INCpREAD(prio) (appData.ptr[prio][PTR_READ] = (appData.ptr[prio][PTR_READ]+1) % NB_BEHAVIOR_PER_PRIO)
#define INCpWRITE(prio) (appData.ptr[prio][PTR_WRITE] = (appData.ptr[prio][PTR_WRITE]+1) % NB_BEHAVIOR_PER_PRIO)
#define SET_0VFF(prio, set) (appData.ptr[prio][PTR_OVFF] = set)

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
    frameToSend.Champ.src = appData.masterId;
    frameToSend.Champ.crc ^= frameToSend.paquet[0];
    frameToSend.Champ.station = appData.station;
    frameToSend.Champ.crc ^= frameToSend.paquet[1];
    frameToSend.Champ.nbR = nbR;
    frameToSend.Champ.typeMsg = typeMsg;
    frameToSend.Champ.crc ^= frameToSend.paquet[2];
    frameToSend.Champ.idMsg = idMsg;
    frameToSend.Champ.crc ^= frameToSend.paquet[3];
    // data
    int8_t i;
    frameToSend.Champ.size = sizeData;
    frameToSend.Champ.crc ^= frameToSend.paquet[4];
    for (i = 0; i < frameToSend.Champ.size; i++) {
        frameToSend.Champ.data[i] = data[i];
        frameToSend.Champ.crc ^= frameToSend.paquet[i + 6]; // penser ? changer le 6 en generique 
    }
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
    //____________________________
    //________UPDATE DATE_________
    // with uart 
    //TODO : use GSM function to update date here
    //____________________________
    //________GET DATE____________
    if (getDateTime()) {
        Date d;
        memset(d.date, 0, 5);
        d.dateVal = 0;
        d.Format.yy = appData.current_time.tm_year;
        d.Format.mon = appData.current_time.tm_mon;
        d.Format.day = appData.current_time.tm_mday;
        d.Format.h = appData.current_time.tm_hour;
        d.Format.min = appData.current_time.tm_min;
        d.Format.sec = appData.current_time.tm_sec;
        //____________________________
        //________SEND DATE___________
#if defined(UART_DEBUG)
        printf("SEND DATE\n");
#endif
        return MASTER_SendMsgRF(appData.broadCastId, HORLOGE, 1, 1, d.date, 5); // visualiser cette valeur 
    }
    return 0;
}

//______________________________________________________________________________
//______________________________________________________________________________
//________________________________STATE MACHINE FUNCTION________________________

bool MASTER_StoreBehavior(MASTER_STATES state, PRIORITY prio) {
    appData.behavior[prio][GETpWRITE(prio)] = state; // on ecrit le comportement 
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
    if (GETpREAD(PRIO_EXEPTIONNEL) != GETpWRITE(PRIO_EXEPTIONNEL)) {
        // on ne peut avoir l'egalite et a voir un comportement present 
        state = appData.behavior[PRIO_EXEPTIONNEL][GETpREAD(PRIO_EXEPTIONNEL)];
        INCpREAD(PRIO_EXEPTIONNEL);
    } else if (GETpREAD(PRIO_HIGH) != GETpWRITE(PRIO_HIGH)) {
        // on ne peut avoir l'egalite et a voir un comportement present 
        state = appData.behavior[PRIO_HIGH][GETpREAD(PRIO_HIGH)];
        INCpREAD(PRIO_HIGH);
    } else if (GETpREAD(PRIO_MEDIUM) != GETpWRITE(PRIO_MEDIUM)) {
        state = appData.behavior[PRIO_MEDIUM][GETpREAD(PRIO_MEDIUM)];
        INCpREAD(PRIO_MEDIUM);
    } else if (GETpREAD(PRIO_LOW) != GETpWRITE(PRIO_LOW)) {
        state = appData.behavior[PRIO_LOW][GETpREAD(PRIO_LOW)];
        INCpREAD(PRIO_LOW);
    }
    return state;
}

void MASTER_AppTask() { // machiine a etat general
    MASTER_STATES state = MASTER_GetBehavior();
    int i;
    switch (state) {
        case MASTER_STATE_INIT:
        {
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined (UART_DEBUG)
                printf("> APP_STATE_INITIALIZE\n");
#endif
            }

            int8_t i = 0;
            while (i < 10 && !appData.RfModuleInit) {
                if (radioAlphaTRX_Init()) {
#if defined  (UART_DEBUG)
                    printf("RF Module Enable And INIT\n");
#endif
                    radioAlphaTRX_ReceivedMode(); // receive mode actived
                    appData.RfModuleInit = true;
                } else
                    i++;
            }
            if (i >= 10) { // module n
#if defined(UART_DEBUG)
                printf("RF module not power on \n");
#endif  
                MASTER_StoreBehavior(MASTER_STATE_ERROR, PRIO_EXEPTIONNEL);
                break;
            }

            MASTER_StoreBehavior(MASTER_STATE_SELECTE_SLAVE, PRIO_HIGH);
        }
            break;
            /* -------------------------------------------------------------- */
        case MASTER_STATE_IDLE:
        {
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined (UART_DEBUG)
                printf("> APP_STATE_IDLE\n");
#endif
            }
#if defined(UART_DEBUG)
            if (getDateTime()) {
                if (appData.current_time.tm_sec % 30 == 0 && (print == true)) {
                    print = false;
                    printDateTime(appData.current_time);
                } else if (appData.current_time.tm_sec % 30 != 0 && (print == false)) {
                    print = true;
                }
            }
#endif     
            break;
        }
            /* -------------------------------------------------------------- */

        case MASTER_STATE_TIMEOUT: //if an event has occurred with the timer 
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined(UART_DEBUG)
                printf(">MASTER STATE TIMEOUT\n");
#endif
            }
        {
            switch (appData.ensSlave[appData.slaveSelected].state) {
                case SLAVE_CONFIG:
#if defined(UART_DEBUG)
                    printf("Config slave with rf and gsm communication\n");
#endif

                case SLAVE_SYNC:
#if defined(UART_DEBUG)
                    printf("hundler error phase de synchro try to connect %d \n",
                            appData.ensSlave[appData.slaveSelected].tryToConnect);
#endif              
                    --appData.ensSlave[appData.slaveSelected].tryToConnect;
                    if (appData.ensSlave[appData.slaveSelected].tryToConnect > 0) {
#if defined( UART_DEBUG )
                        printf("[demande bloc %d]\n", appData.ensSlave[appData.slaveSelected].nbBloc - 1);
#endif
                        MASTER_SendMsgRF(appData.ensSlave[appData.slaveSelected].idSlave,
                                DATA,
                                appData.ensSlave[appData.slaveSelected].nbBloc, 1,
                                (uint8_t *) ("DATA"),
                                4); // attention a changer
                        TMR_SetWaitRqstTimeout(TIME_OUT_COLLECT_LOG); // active timer 
                    } else { // on ne peut pas se connecter ? ce slave 
                        appData.ensSlave[appData.slaveSelected].nbError--;
                        appData.ensSlave[appData.slaveSelected].tryToConnect = MAX_TRY_TO_SYNC;
                        if (appData.ensSlave[appData.slaveSelected].nbError == 0) {
#if defined(UART_DEBUG)
                            printf("hundler slave %d communication corompu\n ERROR ==> SELECT SLAVE\n",
                                    appData.ensSlave[appData.slaveSelected].idSlave);
#endif
                            appData.ensSlave[appData.slaveSelected].state = SLAVE_ERROR;
                        }
                        MASTER_StoreBehavior(MASTER_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
                    }
                    break;
                    /*-----------------------------------------------------------------*/
                case SLAVE_COLLECT:
                {
#if defined(UART_DEBUG)
                    printf("==>COLLECT LOG\n");
#endif  
                    --appData.ensSlave[appData.slaveSelected].nbTimeout;
                    if (appData.ensSlave[appData.slaveSelected].nbTimeout <= 0) {
                        appData.ensSlave[appData.slaveSelected].nbError--;
                        if (appData.ensSlave[appData.slaveSelected].nbError <= 0) {
                            appData.ensSlave[appData.slaveSelected].state = SLAVE_ERROR; // informer le master 
                            MASTER_StoreBehavior(MASTER_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
                            break;
                        } else {
                            appData.ensSlave[appData.slaveSelected].nbTimeout = MAX_TIMEOUT;
                        }
                    }
                    MASTER_SendMsgRF(appData.ensSlave[appData.slaveSelected].idSlave,
                            ACK,
                            appData.ensSlave[appData.slaveSelected].index, 1,
                            (uint8_t *) ("ACK"), 3); // demande du paquet attendu 
                    TMR_SetWaitRqstTimeout(TIME_OUT_COLLECT_LOG);
                }
                    break;
                    /*-----------------------------------------------------------------*/

                case SLAVE_DAYTIME:
                case SLAVE_NO_REQUEST:
                {
#if defined(UART_DEBUG)
                    printf("==>DAYTIME OR SLAVE NO REQUEST\n");
#endif
                    if (appData.ensSlave[appData.slaveSelected].state != SLAVE_NO_REQUEST) {
                        --appData.ensSlave[appData.slaveSelected].nbTimeout;
                        if (appData.ensSlave[appData.slaveSelected].nbTimeout <= 0) {
                            --appData.ensSlave[appData.slaveSelected].nbError;
                            if (appData.ensSlave[appData.slaveSelected].nbError <= 0) {
                                //peut etre c'est le master qui a un probleme 
                                //ou c'est effectivement le slave 
                                //si les 4 slave ne repondent pas c'est probablement le master 
#if defined(UART_DEBUG)
                                printf("SLAVE NO REQUEST %d\n", appData.ensSlave[appData.slaveSelected].uidSlave);
#endif
                                appData.ensSlave[appData.slaveSelected].state = SLAVE_NO_REQUEST;
                                sprintf(appError.message, "Error Slave No Request");
                                appError.current_line_number = __LINE__;
                                sprintf(appError.current_file_name, "%s", __FILE__);
                                appError.number = ERROR_SLAVE_NO_REQUEST;
                                MASTER_StoreBehavior(MASTER_STATE_ERROR, PRIO_EXEPTIONNEL);
                                break;
                            } else
                                appData.ensSlave[appData.slaveSelected].nbTimeout = MAX_TIMEOUT;
                        }
                    }
                    MASTER_StoreBehavior(MASTER_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
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
            appData.receive = radioAlphaTRX_GetFrame();
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf(">MASTER_STATE_MSG_RECEIVE\n");
#endif

                appData.ensSlave[appData.slaveSelected].nbError = MAX_ERROR;
                appData.ensSlave[appData.slaveSelected].nbTimeout = MAX_TIMEOUT;

                switch (appData.receive.Champ.typeMsg) {
                    case DATA:
                        if (appData.dayTime == GOOD_NIGHT) {
                            if (appData.ensSlave[appData.slaveSelected].state == SLAVE_SYNC) {
#if defined(UART_DEBUG)
                                printf("Slave %d connect\n", appData.ensSlave[appData.slaveSelected].idSlave);
#endif
                                appData.ensSlave[appData.slaveSelected].tryToConnect = MAX_TRY_TO_SYNC; // on reset le nombre de demande de connxion 
                                appData.ensSlave[appData.slaveSelected].state = SLAVE_COLLECT; //changement d'etat
                            }

                            if (appData.receive.Champ.idMsg == appData.ensSlave[appData.slaveSelected].index) {
                                appData.ensSlave[appData.slaveSelected].state = SLAVE_COLLECT;
                                uint8_t buf[SIZE_DATA];
                                memset(buf, 0, SIZE_DATA);
                                sprintf(buf, "%s\n", appData.receive.Champ.data);
                                strncpy(appData.BUFF_COLLECT +
                                        appData.receive.Champ.size * (appData.ensSlave[appData.slaveSelected].index - 1),
                                        buf, appData.receive.Champ.size); // save data
                                if (appData.receive.Champ.nbR == MAX_W) {
                                    TMR_SetWaitRqstTimeout(-1);
                                    MASTER_StoreBehavior(MASTER_STATE_SEND_FROM_UART, PRIO_HIGH);
                                    appData.ensSlave[appData.slaveSelected].state = SLAVE_COLLECT_END_BLOCK;
#if defined(UART_DEBUG)
                                    printf("END BLOC\n");
#endif  
                                } else if (appData.receive.Champ.nbR == MAX_W + 1) { // fin de trans
                                    TMR_SetWaitRqstTimeout(-1);
                                    MASTER_StoreBehavior(MASTER_STATE_SEND_FROM_UART, PRIO_HIGH);
                                    appData.ensSlave[appData.slaveSelected].state = SLAVE_COLLECT_END;
#if defined(UART_DEBUG)
                                    printf("END BLOC Trans\n");
#endif
                                } else {
                                    appData.ensSlave[appData.slaveSelected].index += 1;
                                    // on demarre le timeout
                                    if (appData.receive.Champ.nbR > 1) { // je suis plus en attente d'un paquet
                                        TMR_SetWaitRqstTimeout(TIME_OUT_COLLECT_LOG);
                                    } else {
                                        TMR_SetWaitRqstTimeout(0); // deactive timer up to (until) request 
                                    }
                                }
                            } else {
#if defined(UART_DEBUG)
                                printf("numSeq attendu %d vs %d recu \n",
                                        appData.ensSlave[appData.slaveSelected].index, appData.receive.Champ.idMsg);
                                TMR_SetWaitRqstTimeout(0);
#endif
                            }
                        }
                        break;
                        /* -------------------------------------------------------*/
                    case ERROR:
                    {
#if defined( USE_UART1_SERIAL_INTERFACE )
                        printf("ERROR RECEVE OF %d num Error %d\n", appData.ensSlave[appData.slaveSelected].uidSlave,
                                appData.receive.Champ.idMsg);
#endif
                        //                        appError.errorSend = false;
                        appData.ensSlave[appData.slaveSelected].state = SLAVE_ERROR;
                        sprintf(appError.message, "Error Receive from Alpha TRX module");
                        appError.current_line_number = __LINE__;
                        sprintf(appError.current_file_name, "%s", __FILE__);
                        appError.number = appData.receive.Champ.idMsg;
                        MASTER_StoreBehavior(MASTER_STATE_ERROR, PRIO_EXEPTIONNEL);
                        TMR_SetWaitRqstTimeout(-1); // desactive le timer 
                    }
                        break;
                        /* -------------------------------------------------------*/
                    case NOTHING:
                        if (appData.ensSlave[appData.slaveSelected].state == SLAVE_SYNC) {
                            appData.ensSlave[appData.slaveSelected].state = SLAVE_COLLECT_END;
                            MASTER_StoreBehavior(MASTER_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
                            // on n'a pas pu recuperer un block car c'est fini 
                        } else {
#if defined( USE_UART1_SERIAL_INTERFACE )
                            printf("Slave %d Nothing to send\n", appData.ensSlave[appData.slaveSelected].idSlave);
#endif
                        }
                        break;
                        /* ----------------------------------------------------*/
                        /* -------------------------------------------------------*/
                }
                break;
            }
        }
            break;
            /*-----------------------------------------------------------------*/
        case MASTER_STATE_SEND_DATE: // niveau HIGH
        {
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined (UART_DEBUG)
                printf("> MASTER_APP_STATE_SEND_DATE\n");
#endif
                MASTER_SendDateRF();
            }
        }
            break;
            /* -------------------------------------------------------------- */
        case MASTER_STATE_SEND_REQUEST_INFOS:
            if (appData.state != appData.previous_state) {
#if defined UART_DEBUG
                printf(">MASTER STATE SEND REQUEST INFOS\n");
#endif
            }
            //pour tout transmission RF hors la date, on passe par cet etat: niveau medium
            MASTER_SendMsgRF(appData.ensSlave[appData.slaveSelected].idSlave,
                    INFOS,
                    1, 1, (uint8_t *) ("INFOS"), 5); // demande du paquet attendu 
            TMR_SetWaitRqstTimeout(TIME_OUT_WAIT_RQST); // on demare le timer 
            break;
            /* -------------------------------------------------------------- */
        case MASTER_STATE_SELECTE_SLAVE:
        {
            if (appData.state != appData.previous_state) {
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf(">MASTER STATE SELECT SLAVE\n");
#endif
                appData.previous_state = appData.state;
            }

            //test si tous les slaves sont sans reponse 
            uint8_t i;
            bool b = false;
            for (i = 0; i < appData.nbSlaveOnSite; i++) {
                if (appData.ensSlave[i].state != SLAVE_NO_REQUEST &&
                        appData.ensSlave[i].state != SLAVE_ERROR) {
                    b = true;
                    break;
                }
            }

            if (b) {
                i = (appData.slaveSelected + 1) % appData.nbSlaveOnSite;
                b = false;
                bool stop = false;
                do {
                    if (appData.ensSlave[i].state == SLAVE_ERROR ||
                            appData.ensSlave[i].state == SLAVE_COLLECT_END) {
                        i = (i + 1) % appData.nbSlaveOnSite;
                        if (i == appData.slaveSelected)
                            if (appData.ensSlave[i].state == SLAVE_COLLECT_END ||
                                    appData.ensSlave[i].state == SLAVE_ERROR) {
                                stop = true; // pas de slave operationel  
                            }
                    } else {
                        stop = true; // on a trouve un slave
                        appData.slaveSelected = i;
                        b = true;
                    }
                } while (!stop);

                if (b) {
                    //en foction de l'heur de la journee on change d'etat 
                    switch (appData.dayTime) {
                        case GOOD_MORNING:
                        {
                            appData.ensSlave[appData.slaveSelected].state = SLAVE_CONFIG;
                            TMR_SetWaitRqstTimeout(-1);
                        }
                            break;
                        case GOOD_DAY:
                        {
                            if (appData.ensSlave[appData.slaveSelected].state != SLAVE_NO_REQUEST)
                                appData.ensSlave[appData.slaveSelected].state = SLAVE_DAYTIME;
                            MASTER_StoreBehavior(MASTER_STATE_SEND_REQUEST_INFOS, PRIO_MEDIUM);
                        }
                            break;
                        case GOOD_NIGHT:
                        {
                            appData.ensSlave[appData.slaveSelected].tryToConnect = MAX_TRY_TO_SYNC;
                            appData.ensSlave[appData.slaveSelected].state = SLAVE_SYNC;
                            TMR_SetWaitRqstTimeout(0); // declenche une interuption logiciel 
                        }
                            break;
                        default:
                            break;
                    }
                    break; // fin de la selection du slave 
                }
            }
#if defined(UART_DEBUG)
            printf("Aucun slave en selection\n");
#endif
            MASTER_StoreBehavior(MASTER_STATE_END, PRIO_LOW);
        }
            break;
            /* -------------------------------------------------------------- */
        case MASTER_STATE_SEND_FROM_UART:
        {
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf(">MASTER_STATE_SEND_FROM_GSM\n");
#endif
            }
            appData.BUFF_COLLECT[appData.ensSlave[appData.slaveSelected].index * (SIZE_DATA - 1)] = '#';
            appData.BUFF_COLLECT[appData.ensSlave[appData.slaveSelected].index * (SIZE_DATA - 1) + 1] = appData.ensSlave[appData.slaveSelected].idSlave + 48;
            appData.BUFF_COLLECT[appData.ensSlave[appData.slaveSelected].index * (SIZE_DATA - 1) + 2] = '#';
            appData.BUFF_COLLECT[appData.ensSlave[appData.slaveSelected].index * (SIZE_DATA - 1) + 3] = 1 + 48;
            appData.BUFF_COLLECT[appData.ensSlave[appData.slaveSelected].index * (SIZE_DATA - 1) + 4] = '#';
            appData.BUFF_COLLECT[appData.ensSlave[appData.slaveSelected].index * (SIZE_DATA - 1) + 5] = appData.station + 48;
            appData.BUFF_COLLECT[appData.ensSlave[appData.slaveSelected].index * (SIZE_DATA - 1) + 6] = '#';
            appData.BUFF_COLLECT[appData.ensSlave[appData.slaveSelected].index * (SIZE_DATA - 1) + 7] = appData.ensSlave[appData.slaveSelected].nbBloc + 48;

            //TODO :  
        }
            break;
            /* -------------------------------------------------------------- */
        case MASTER_STATE_ERROR: // NIVEAU HIGH, etat de traitement des erreur lie au master 
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined UART_DEBUG
                printf("> APP_STATE_ERROR\n");
#endif

                //#if defined( USE_UART1_SERIAL_INTERFACE )
                //                printf("error number %d, errorSend %d\n", appError.number, appError.errorSend);
                //#endif
                if ((appError.number < ERROR_CRITICAL || appError.number > ERROR_TOO_MANY_SOFTWARE_RESET)
                        && !appError.errorSend && appError.number != ERROR_GSM_NO_POWER_ON) {
                    MASTER_StoreBehavior(MASTER_STATE_SEND_ERROR_TO_UART, PRIO_EXEPTIONNEL);
                    break;
                }

                while (!RTCC_TimeGet(&appError.time)) {
                    Nop();
                }


//#if defined UART_DEBUG
//                printError();
//#endif
//                rtcc_stop_alarm();
//
//
//                /* Unmount drive on USB device before power it off. */
//                if (USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status) {
//                    usbUnmountDrive();
//                }
//
//                if (appError.number != ERROR_GSM_NO_POWER_ON &&
//                        !appError.OfInCriticalError) { // indique que c'est la premiere fois qu'on entre dedans 
//                    TMR_Delay(30000); //permet d'attendre la fin des tache du gsm avant de couper l'alime  
//                    app_PowerDown(true);
//                }
//                USBHostShutdown();
//                powerUsbGSMDisable();
//
//                /* Reset the system after DELAY_BEFORE_RESET milli-seconds if non critical error occurred */
//                if (appError.number > ERROR_CRITICAL && appError.number < ERROR_TOO_MANY_SOFTWARE_RESET) {
//                    if (MAX_NUM_RESET <= appData.dsgpr0.bit_value.num_software_reset) {
//#if defined ( USE_UART1_SERIAL_INTERFACE ) 
//                        printf("\t/!\\ The system reset %u times => Critical error\n", MAX_NUM_RESET);
//#endif                       
//                        appError.number = appError.number + ERROR_TOO_MANY_SOFTWARE_RESET;
//                        appData.previous_state = MASTER_APP_STATE_IDLE;
//                        break;
//                    }
//
//                    TMR3_Start();
//                    setDelayMs(DELAY_BEFORE_RESET);
//#if defined ( USE_UART1_SERIAL_INTERFACE ) 
//                    printf("\t/!\\ The system will reset in %ums\n", DELAY_BEFORE_RESET);
//#endif
//                } else {
//                    rtcc_set_alarm(0, 0, 0, EVERY_10_SECONDS);
//                    rtcc_start_alarm();
//                }
//            }
//
//            if (appError.number > ERROR_CRITICAL && appError.number < ERROR_TOO_MANY_SOFTWARE_RESET) {
//                /* A non critical error occurred                      */
//                /* Wait DELAY_BEFORE_RESET milli-seconds and reset    */
//#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined (ENABLE_ERROR_LEDS)
//                LedsStatusBlink(appError.led_color_1, appError.led_color_2, 150, 150);
//#endif
//                if (isDelayMsEnding()) {
//
//                    appData.dsgpr0.bit_value.num_software_reset += 1;
//                    DSGPR0 = appData.dsgpr0.reg;
//                    DSGPR0 = appData.dsgpr0.reg;
//
//                    TMR3_Stop();
//
//                    __asm__ volatile ( "reset");
//
//                }
//            } else {
//                /* A critical error occured */
//                /* Stop the openfeeder to save battery */
//                /* Make status LEDs blink at long interval */
//
//                if (!appError.OfInCriticalError) {
//#if defined( USE_UART1_SERIAL_INTERFACE )
//                    printf("RF MODULE POWER OFF\n");
//#endif 
//                    // on a un bug ici, que je ne parviens pas a expliquer encore 
//                    // donc pour palier a cela on eteint pas le module radio
//                    // on l'empeche juste de ne plus recevoir ni d'emettre 
//                    //                    powerRFDisable();
//                    RF_PowerManagement.Val = PWR_MGMT_CMD_POR;
//                    radioAlphaTRX_Command(RF_PowerManagement.Val);
//                    appError.OfInCriticalError = true;
//                }
//                Sleep();
            }
            break;
            /* -------------------------------------------------------------- */

        case MASTER_STATE_END:
//            if (state != prevBehavior) {
//                prevBehavior = state;
//#if defined(UART_DEBUG)
//                printf(">MASTER STATE END\n");
//#endif
//            }
//            //TODO : gestion de fin
//            break;
//            /* -------------------------------------------------------------- */
//        default:
//            if (state != prevBehavior) {
//                prevBehavior = state;
//#if defined(UART_DEBUG)
//                printf(">MASTER STATE DEFAULT\n");
//#endif
//            }
            break;
            /* -------------------------------------------------------------- */
    }
}

void MASTER_AppInit() {
    
}

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/