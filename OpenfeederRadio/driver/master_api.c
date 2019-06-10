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
#include "master_api.h"
#include "timer.h"

/******************************************************************************/
/******************************************************************************/
/********************* COUCHE APPLICATION DU MASTER  **************************/
/***************************                ***********************************/

/*****************                                         ********************/

/**-------------------------->> S T R U C T U R E -- S L A V E <<--------------*/
typedef struct sSlave {
} Slave;


/**-------------------------->> V A R I A B L E S <<---------------------------*/
int8_t msgReceiveRF = 0; // 

MSTR_STATE_GENERAL mstrStat; //modifier par rtc
//______________________________________________________________________________

void MASTER_SetMsgReceiveRF(uint8_t set) {
    msgReceiveRF = set;
}

int8_t Master_SendMsgRF(uint16_t idSlave, uint8_t typeMsg, uint8_t * data, uint8_t idMsg) {
    radioAlphaTRX_SetSendMode(1);
    uint8_t data_send[FRAME_LENGTH];
    int8_t ret = 0;
    int8_t size_h = srv_create_paket_rf(data_send, data, idSlave,
                                        srv_getID_Master(), typeMsg, idMsg);
    if (radioAlphaTRX_SendMode()) {
        radioAlphaTRX_SendData(data_send, size_h);
        ret = 1;
    }
    radioAlphaTRX_SetSendMode(0);
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
    return Master_SendMsgRF(srv_getBroadcast(), srv_horloge(), date, 1); // a voir
}

void MASTER_HandlerMsgRF() {
    Frame data_receive;
    if (srv_decode_packet_rf(radioAlphaTRX_ReadBuf(), 
                             &data_receive, radioAlphaTRX_GetSizeBuf(), srv_getID_Master()) > 0) {
        if (data_receive.Type_Msg == srv_err()) {
#if defined(UART_DEBUG)
            LED_GREEN_Toggle();
            printf("erreur recu ==> transfere gsm :: %s %d\n", data_receive.data, data_receive.ID_Msg);
#endif
            Master_SendMsgRF(data_receive.ID_Src, srv_ack(), "ACK", data_receive.Type_Msg); // pour l'instant
            //TODO Transmettre Par GSM 
        } else if (data_receive.Type_Msg == srv_infos()) { // pour l'instant on traite pas ce cas 
#if defined(UART_DEBUG)
            printf("info recu transmission au autres \n");
#endif
        } else if (data_receive.Type_Msg == srv_nothing()) {
#if defined(UART_DEBUG)
            printf("Le slave [%d] n'a rien a transmettre\n", data_receive.ID_Src);
#endif
        }
    }
}

//chisit un autre slave a qestionner

void MASTER_SelectSlave() {
    //choix du slave 
    int16_t idSlave = 37; // pour l'instant
#if defined(UART_DEBUG)
    printf("demande d'infos au slave %d\n", idSlave);
#endif
    Master_SendMsgRF(idSlave, srv_infos(), "INFOS", 1);
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
    if (!TMR_GetHorlogeTimeout()) { // on doit envoyer l'horloge en mode broadcast
        MASTER_SendDateRF();
        TMR_SetHorlogeTimeout(SEND_HORLOG_TIMEOUT);
    } else if (msgReceiveRF == 1) {
        MASTER_HandlerMsgRF();
    } else if (!TMR_GetWaitRqstTimeout()) { //le temps d'attente d'une reponse
        MASTER_SelectSlave();
        TMR_SetWaitRqstTimeout(TIME_OUT_WAIT_RQST);
    }
}

void MASTER_Task() {

    switch (mstrStat) {

        case MSTR_STATE_GENERAL_BEFOR_DAYTIME:
#if defined(UART_DEBUG)
            printf("Master on est 2h avant le debut de la journée \n");
#endif
            //TODO : ce que je dois faire avant le debut des hostilite 
            break;
            /* -------------------------------------------------------------- */
        case MSTR_STATE_GENERAL_DAYTIME:
#if defined(UART_DEBUG)
            printf("Master on est le debut de la journée \n");
#endif
            MASTER_StateMachineOfDaytime();
            break;
            /* -------------------------------------------------------------- */
        case MSTR_STATE_GENERAL_AFTER_DAYTIME:
            //TODO : ce que je dois faire avant le debut des hostilite 
#if defined(UART_DEBUG)
            printf("Master on est a la fin de la journée \n");
#endif
            break;
            /* -------------------------------------------------------------- */
        default:
            break;
    }


}

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/