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

#include "master_api.h"
#include "timer.h"

/******************************************************************************/
/******************************************************************************/
/********************* COUCHE APPLICATION DU MASTER  **************************/
/***************************                ***********************************/

/*****************                                         ********************/

/**-------------------------->> S T R U C T U R E -- S L A V E <<--------------*/
typedef struct sSlave {
    int16_t idSlave;
    SLAVE_STATE state;
    int8_t curseur; // le paquet attendu, tanq que je ne suis pas en fin de bloc
    int8_t nbTimeout;
    int8_t tryToConnect;
    int8_t nbBlocs;
} Slave;


/**-------------------------->> V A R I A B L E S <<---------------------------*/
volatile int8_t msgReceiveRF = 0; // informe de l'arriver d'un msg rf 
volatile int8_t msgReceiveGSM = 0; // informe de l'arriver d'un msg GSM

MSTR_STATE_GENERAL mstrState = MSTR_STATE_GENERAL_BEFOR_DAYTIME; //modifier par rtc plus tard 
MSTR_STATE_GENERAL mstrPrevState = MSTR_STATE_GENERAL_ERROR; //pour les besoin des teste on simule cela avec des interuption 

MSTR_STATE_GET_LOG mstrStateGetLog = MSTR_STATE_GET_LOG_SELECT_SLAVE; // init state 

//TOASK : je pense utiliser Deux buffeurs pour pouvoir faire du pseudo
//TOASK : paralleliseme, pendant que je recup�re en RF je transmettrais en GSM
uint8_t BUF_DATA[NB_DATA_BUF][SIZE_DATA] = {
    {0}
};

Slave ensSlave[NB_SLAVE]; //ensemble des openfeeder sur le site 
int16_t slaveID[NB_SLAVE];
int8_t slaveSlected; // l'of en cours d'interogation 
//______________________________________________________________________________

void MASTER_Init() {
    int8_t i;
    for (i = 0; i < NB_SLAVE; i++) {
        ensSlave[i].curseur = 0;
        ensSlave[i].nbBlocs = 0;
        ensSlave[i].nbTimeout = MAX_TIMEOUT;
        ensSlave[i].state = SLAVE_STATE_DESELCTED;
        ensSlave[i].tryToConnect = MAX_TRY_TO_SYNC;
        ensSlave[i].idSlave = SLAVE1_ID; // attention c'est un test penser a changer 
    }

}

//chisit un autre slave a qestionner

int8_t MASTER_SelectSlave() {
    //TODO : choix du slave 
    int8_t i = slaveSlected + 1 % NB_SLAVE;
    int8_t stop = 0;
    int8_t ret = 0;
    do {
        if (ensSlave[i].state == SLAVE_STATE_ERROR ||
            ensSlave[i].state == SLAVE_STATE_COLLECT_END) {
            i = i + 1 % NB_SLAVE;
            if (i == slaveSlected) {
                stop = 1; // pas de slave operationel  
            }
        } else {
            stop = 1; // on a trouve un slave
            ensSlave[i].state = SLAVE_STATE_SELECTED;
#if defined(UART_DEBUG)
            printf("slave %d selected\n", ensSlave[i].idSlave);
#endif
            slaveSlected = i;
            ret = 1;
        }
    } while (!stop);

    return ret;
}

void MASTER_SetMsgReceiveRF(uint8_t set) {
    msgReceiveRF = set;
}

int8_t Master_SendMsgRF(uint16_t idSlave, uint8_t typeMsg, uint8_t * data, uint8_t idMsg) {
    //    radioAlphaTRX_SetSendMode(1);
    uint8_t data_send[FRAME_LENGTH];
    int8_t ret = 0;
    int8_t size_h = srv_create_paket_rf(data_send, data, idSlave,
                                        MASTER_ID, typeMsg, idMsg);
    if (radioAlphaTRX_SendMode()) {
        radioAlphaTRX_SendData(data_send, size_h);
        ret = 1;
    }
    //    radioAlphaTRX_SetSendMode(0);
    radioAlphaTRX_ReceivedMode();
    return ret;
}

int8_t MASTER_SendDateRF() {
    //TOASK je ne sais pas si je dois d'abord mettre a jour l'heure du master 
    // avant de la transmetre au autres 
    struct heure_format hf;
    uint8_t date[14]; // cas particulier
    get_time(&hf);
    //creation de du format pour l'envoie  ensEsclave[0].logRecup = 0;
    serial_buffer(date, hf);
    return Master_SendMsgRF(srv_getBroadcast(), srv_horloge(), (uint8_t *)date, 1); // a voir
}

void MASTER_HandlerMsgRF() {
    Frame dataReceive;
    int8_t sizeData;
    if ((sizeData = srv_decode_packet_rf(radioAlphaTRX_ReadBuf(),
                                         &dataReceive,
                                         radioAlphaTRX_GetSizeBuf(),
                                         MASTER_ID)) > 0) {
        if (dataReceive.Type_Msg == srv_err()) {
#if defined(UART_DEBUG)
            LED_GREEN_Toggle();
            printf("erreur recu ==> transfere gsm :: %s %d\n",
                   dataReceive.data,
                   dataReceive.ID_Msg);
#endif
            Master_SendMsgRF(dataReceive.ID_Src,
                             srv_ack(), (uint8_t *)("ACK"),
                             dataReceive.Type_Msg); // pour l'instant
            //TODO Transmettre Par GSM 
        } else if (dataReceive.Type_Msg == srv_infos()) { // pour l'instant on traite pas ce cas 
#if defined(UART_DEBUG)
            printf("info recu transmission au autres \n");
#endif
        } else if (dataReceive.Type_Msg == srv_nothing()) {
#if defined(UART_DEBUG)
            printf("Le slave [%d] n'a rien a transmettre\n", dataReceive.ID_Src);
#endif
        } else if (dataReceive.Type_Msg == srv_data()) {
            if (dataReceive.ID_Msg == ensSlave[slaveSlected].curseur) { // si c'est la donnee attendu 
                srv_cpy(BUF_DATA[ensSlave[slaveSlected].curseur++], dataReceive.data, sizeData); // on recupere la donnee
                //on change d'etat si c'est le premier paquet attendu 
                if (ensSlave[slaveSlected].state == SLAVE_STATE_SYNC) {
                    ensSlave[slaveSlected].tryToConnect = MAX_TRY_TO_SYNC; // on reset le nombre de demande de connxion 
                } else
                    ensSlave[slaveSlected].nbTimeout = MAX_TIMEOUT;
                TMR_SetHorlogeTimeout(SEND_HORLOG_TIMEOUT); // on demarre le timeout  
                mstrStateGetLog = MSTR_STATE_GET_LOG_WAIT_EVENT;
            } else {
                mstrStateGetLog = MSTR_STATE_GET_LOG_ERROR;
            }
        } else if (dataReceive.Type_Msg == srv_end_block()) {
            mstrStateGetLog = MSTR_STATE_GET_LOG_SEND_FROM_GSM;
        } else if (dataReceive.Type_Msg == srv_end_trans()) {
            mstrStateGetLog = MSTR_STATE_GET_LOG_SEND_FROM_GSM; //pour l'instant 
        }
    }
}


void MASTER_StateMachineOfDaytime() {
#if defined(UART_DEBUG)
    struct tm t;
    RTCC_TimeGet(&t);
    if (!TMR_GetTimeout()) {
        printf("heur master ==> %dh:%dmin:%ds\n", t.tm_hour, t.tm_min, t.tm_sec);
        TMR_SetTimeout(5000); // 5s
    }
#endif
    // ici il est important de respecter la hierarchie des test pour le bon 
    // fonctionnement l'appli 
    if (!TMR_GetHorlogeTimeout()) { // on doit envoyer l'horloge en mode broadcast
        MASTER_SendDateRF();
        TMR_SetHorlogeTimeout(SEND_HORLOG_TIMEOUT);
        TMR_Delay(AFTER_SEND_HORLOGE); //on attends 
    } else if (msgReceiveRF == 1) {
        MASTER_HandlerMsgRF();
        // a decommenter lorsqu'il y'a plusieurs of connecte
        // TMR_SetWaitRqstTimeout(0); // a pour effet d'arreter le temporisateur 
    } else if (!TMR_GetWaitRqstTimeout()) {
        MASTER_SelectSlave();
        TMR_SetWaitRqstTimeout(TIME_OUT_WAIT_RQST); //demarre le temporisateur
    }
}

void MASTER_HundlerError() {
    //time out pour l'instant on ne traite que ca 
    switch (ensSlave[slaveSlected].state) {
        case SLAVE_STATE_SYNC:

            if (--ensSlave[slaveSlected].tryToConnect) {
                Master_SendMsgRF(ensSlave[slaveSlected].idSlave, srv_data(),(uint8_t *)("DATA"), 1); // demande de data 
                TMR_SetWaitRqstTimeout(TIME_OUT_WAIT_RQST);
                //j'attends � nouveau un evenement 
                mstrStateGetLog = MSTR_STATE_GET_LOG_WAIT_EVENT;
            } else { // on ne peut pas se connecter � ce slave 
                ensSlave[slaveSlected].state = SLAVE_STATE_ERROR;
                mstrStateGetLog = MSTR_STATE_GET_LOG_SELECT_SLAVE; // oui car je n'ai rien recupere 
            }
            break;
            /*-----------------------------------------------------------------*/
        case SLAVE_STATE_COLLECT:
            if (--ensSlave[slaveSlected].nbTimeout) {
                Master_SendMsgRF(ensSlave[slaveSlected].idSlave, srv_ack(), 
                                 (uint8_t *)("ACK"), 
                                 ensSlave[slaveSlected].curseur); // demande du paquet attendu 
                TMR_SetWaitRqstTimeout(TIME_OUT_WAIT_RQST);
                //j'attends � nouveau un evenement 
                mstrStateGetLog = MSTR_STATE_GET_LOG_WAIT_EVENT;
            } else { // on une interuption dans la collect
                ensSlave[slaveSlected].state = SLAVE_STATE_ERROR;
                mstrStateGetLog = MSTR_STATE_GET_LOG_SEND_FROM_GSM; // on transmet le peut qu'on a recu 
            }
            break;
            /*-----------------------------------------------------------------*/
    }

}

void MASTER_GetLog() { // ces etats sont consideres comme des phases
    switch (mstrStateGetLog) {
        case MSTR_STATE_GET_LOG_SELECT_SLAVE:

            MASTER_SelectSlave();
            //mstrState = MSTR_STATE_GENERAL_END;
            break;
            /*-----------------------------------------------------------------*/
        case MSTR_STATE_GET_LOG_WAIT_EVENT:
            //TODO : select the slave to collect log 
            if (msgReceiveRF + msgReceiveGSM) { // > 0 ==> msg recive, but we don't know what type of msg 
                mstrStateGetLog = MSTR_STATE_GET_LOG_MSG_RECEIVE;
            } else if (TMR_GetWaitRqstTimeout()) { // time out  aucun msg n'est recu 
                mstrStateGetLog = MSTR_STATE_GET_LOG_ERROR; // we have a timeout 
            }
            break;
            /*-----------------------------------------------------------------*/
        case MSTR_STATE_GET_LOG_ERROR: // dans cet etat, on s'occupe de la demande 
            MASTER_HundlerError(); // traitement de l'ensemble des erreur survenues : timeout 
            break;
            /*-----------------------------------------------------------------*/
        case MSTR_STATE_GET_LOG_MSG_RECEIVE:
            if (msgReceiveRF) {
                msgReceiveRF = 0; // reset msgReceive
                MASTER_HandlerMsgRF();
            } else if (msgReceiveGSM) {
                msgReceiveGSM = 0;
                //TODO : MASTER_HandlerMsgGSM();
            }

            break;
            /*-----------------------------------------------------------------*/
        case MSTR_STATE_GET_LOG_SEND_FROM_GSM:
            printf("Je transfere au serveur les donnee du slave %d\n", ensSlave[slaveSlected].idSlave);
            int8_t i = 0;
            for (; i < ensSlave[slaveSlected].curseur; i++) {
                printf("recu : %d << %s >>\n", i, BUF_DATA[i]);
            }
            mstrStateGetLog = MSTR_STATE_GET_LOG_SELECT_SLAVE;
            break;
            /*-----------------------------------------------------------------*/
        default:
            break;
    }
}

void MASTER_Task() {

    switch (mstrState) {

        case MSTR_STATE_GENERAL_BEFOR_DAYTIME:
            if (mstrState != mstrPrevState) {
                mstrPrevState = mstrState;
#if defined(UART_DEBUG)
                printf("Master on est 2h avant le debut de la journee\n");
#endif
            }
            MASTER_Init();
            //TODO : ce que je dois faire avant le debut des hostilite 
            break;
            /* -------------------------------------------------------------- */
        case MSTR_STATE_GENERAL_DAYTIME:
            if (mstrState != mstrPrevState) {
                mstrPrevState = mstrState;
#if defined(UART_DEBUG)
                printf("Master on est le debut de la journee \n");
#endif
            }
            MASTER_StateMachineOfDaytime();
            break;
            /* -------------------------------------------------------------- */
        case MSTR_STATE_GENERAL_AFTER_DAYTIME:
            //TODO : ce que je dois faire avant le debut des hostilite 
            if (mstrState != mstrPrevState) {
                mstrPrevState = mstrState;
#if defined(UART_DEBUG)
                printf("Master on est a la fin de la journee \n");
#endif  
            }

            MASTER_GetLog();
            break;
            /* -------------------------------------------------------------- */
        case MSTR_STATE_GENERAL_END:
#if defined(UART_DEBUG)
            printf("Fin de la journe: afficher le status du master \n");
#endif
            while (1) {
            }
        default:
            break;
    }


}

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/