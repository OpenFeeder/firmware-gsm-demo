/* *****************************************************************************
 * 
 * _____________________________________________________________________________
 *
 *                    COURCHE APPLICATION (OF) SLAVE (.c)
 * _____________________________________________________________________________
 *
 * Titre            : Mise en oeuvre de la couche application du slave 
 * Version          : v00
 * Date de creation : 24/05/2019
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
#include "radio_alpha_trx_slave_api.h"
#include "led_status.h"

/******************************************************************************/
/******************************************************************************/
/********************* COUCHE APPLICATION DU SLAVE  ***************************/
/***************************                ***********************************/

/*****************                                         ********************/

/**-------------------------->> V A R I A B L E S <<---------------------------*/
volatile uint8_t BUF_ERR[NB_ERR_BUF]; // sauve les erreur en transmettre 
volatile uint8_t pReadErrBuf = 0;
volatile uint8_t pWriteErrBuf = 0;
volatile uint8_t errOver = 0; // si on a ecraser une erreur gener
uint8_t lastSend = 2; // 0 si erreur | 1 si data | 2 ack   
//______________________________________________________________________________

void radioAlphaTRX_SlaveSaveError(int8_t numError) { // vraiment a voir 
    LED_STATUS_R_Toggle();
    BUF_ERR[pWriteErrBuf] = numError;
    pWriteErrBuf = (pWriteErrBuf + 1) % NB_ERR_BUF;
    if (pReadErrBuf == pWriteErrBuf) {
        errOver = 1;
    } else if (((pReadErrBuf == pWriteErrBuf - 1) || (pWriteErrBuf - 1 < 0)) && errOver) {
        pReadErrBuf = (pReadErrBuf + 1) % NB_ERR_BUF;
    }
}
//ret 0 alors pas d'erreur 

int8_t radioAlphaTRX_SlaveGetError() {
    if (pReadErrBuf != pWriteErrBuf || errOver) { // il y' a une err
        return BUF_ERR[pReadErrBuf];
    }
    return 0;
}

void radioAlphaTRX_SlaveUpdatePtrErrBuf() {
    pReadErrBuf = (pReadErrBuf + 1) % NB_ERR_BUF;
    if (pReadErrBuf == pWriteErrBuf && errOver)
        errOver = 0;
}

int8_t radioAlphaTRX_SlaveSendMsgRF(uint8_t typeMsg, uint8_t * data, uint8_t idMsg) {
    //    radioAlphaTRX_SetSendMode(1); // j'annonce que je suis en mode transmission 

    uint8_t dataToSend[FRAME_LENGTH];
    int8_t ret = 0;
    int8_t size = srv_create_paket_rf(dataToSend, data, srv_getID_Master(), srv_getID_Slave(), typeMsg, idMsg);
    if (radioAlphaTRX_SendMode()) {
        radioAlphaTRX_SendData(dataToSend, size);
        ret = 1;
    }

    //    radioAlphaTRX_SetSendMode(0); // je suis en mode transmission 
    radioAlphaTRX_ReceivedMode(); // je me remets en attente d'un msg
    return ret;
}

void radioAlphaTRX_SlaveSendErr(int8_t errToSend) {
    radioAlphaTRX_SlaveSendMsgRF(srv_err(), "ERROR", errToSend); // plus tard on evitera
}

void radioAlphaTRX_SlaveSendNothing() {
    radioAlphaTRX_SlaveSendMsgRF(srv_nothing(), "NOTHING", 1);
}

void radioAlphaTRX_SlaveUpdateDate(uint8_t* date, int16_t derive) {
    struct heure_format hf;
    deserial_buffer(date, &hf);
    hf.s += 1;
    if (hf.s == 60) {
        hf.s = 0;
        hf.m += 1;
        if (hf.m == 60) {
            hf.m = 0;
            hf.h += 1;
        }
    }
    set_time(hf);
}

void radioAlphaTRX_SlaveBehaviour() {
    Frame msgReceive;
    uint16_t timeout = TMR_GetMsgRecuTimeout();
    if (srv_decode_packet_rf(radioAlphaTRX_ReadBuf(), &msgReceive,
                             radioAlphaTRX_GetSizeBuf(), srv_getID_Slave()) > 0) {
        if (msgReceive.Type_Msg == srv_horloge()) {
#if defined(UART_DEBUG)
            printf("Msg synch recu \n");
#endif
            radioAlphaTRX_SlaveUpdateDate(msgReceive.data, timeout);
        } else if (msgReceive.Type_Msg == srv_infos() && timeout) { // demande de transmission de d'erreur ou de paquet
            int8_t err = radioAlphaTRX_SlaveGetError();
#if defined(UART_DEBUG)
            printf("Demande d'infos recu || timeout %d\n", timeout);
#endif
            if (err) { //l'error a transmettre 
                radioAlphaTRX_SlaveSendErr(err); // source probalble d'err
                lastSend = 0;
            } else {
                lastSend = 2;
                radioAlphaTRX_SlaveSendNothing();
            }
        } else if (msgReceive.Type_Msg == srv_ack()) { // un ack confirmant le dernier paquet envoyer 
            if (lastSend == 0) {
#if defined(UART_DEBUG)
                printf("mise a jour du buf d'err\n");
#endif
                radioAlphaTRX_SlaveUpdatePtrErrBuf();
            }
        }
    }
}

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/
//end file