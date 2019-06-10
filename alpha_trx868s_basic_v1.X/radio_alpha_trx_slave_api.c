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
#include "app.h"

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
//for send data : mise a jour au fure et a mesure 
int8_t BUF_DATA[NB_DATA_BUF][SIZE_DATA]; // taille : NB_DATA_BUF*SIZE_DATA
int8_t curseur = 1; //le curseur
int8_t ack_attedue = 1; //
int8_t windows = 1;
int8_t nbBlock = 1;

ACK_STATES lastSend = ACK_STATES_NOTHING;
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


/**-------------------------->> S E N D - L O G S <<---------------------------*/
// attention on compte a partir de 1, mais l'indice du table et de la boucle 
// c'est a partir de 0

void radioAlphaTRX_SlaveSendLog() {
    int8_t i = 0;
    while (i < windows) {
        TMR_DelayMs(LAPS); // on attends avant de retransmettre un autre msg 
        radioAlphaTRX_SlaveSendMsgRF(srv_data(), BUF_DATA[i + curseur - 1], i + curseur);
        i++;
    }
    ack_attedue = i + curseur;
    lastSend = ACK_STATES_DATA;
    radioAlphaTRX_ReceivedMode(); // on se met en mode reception 
    APP_DATA.state = APP_STATE_IDLE; // on rend la main 
}

void radioAlphaTRX_SlaveUpdateSendLogParam(uint8_t numSeq) {
    curseur = numSeq;
    if (numSeq == ack_attedue) { // tous les msg envoyees ont ete aquitte
        if (windows < MAX_W) windows += 1;
    } else {
        windows/2; // on diminue la fenetre d'emission 
        // il serait interressent de faire des statistique du nombre d'echec constate
    }
    if (curseur-1 == NB_DATA_BUF) {
        APP_DATA.state = APP_STATE_RADIO_SEND_END_BLOCK; // on demande l'envoie d'un msg de fin de block
    }else {
        APP_DATA.state = APP_STATE_RADIO_SEND_DATA; // on se remet en transmission de donnee 
    }
}

void radioAlphaTRX_SlaveSendEndBlok() {
    radioAlphaTRX_SlaveSendMsgRF(srv_end_block(), "END BLOCK", nbBlock);
    lastSend = ACK_STATES_END_BLOCK;
    appData.state = APP_STATE_IDLE; 
}

/**-------------------------->>                   <<---------------------------*/

void radioAlphaTRX_SlaveBehaviourWhenMsgReceived() {
    Frame msgReceive;
    uint16_t timeout = TMR_GetMsgRecuTimeout();
    if (srv_decode_packet_rf(radioAlphaTRX_ReadBuf(), &msgReceive,
                             radioAlphaTRX_GetSizeBuf(), srv_getID_Slave()) > 0) {
        if (msgReceive.Type_Msg == srv_horloge()) {
#if defined(UART_DEBUG)
            printf("Msg synch recu \n");
#endif
            radioAlphaTRX_SlaveUpdateDate(msgReceive.data, timeout);
        } else if (msgReceive.Type_Msg == srv_infos() && timeout) { // on est dans la journee
            int8_t err = radioAlphaTRX_SlaveGetError();
#if defined(UART_DEBUG)
            printf("Demande d'infos recu || timeout %d\n", timeout);
#endif  
            if (err) { //l'error a transmettre 
                radioAlphaTRX_SlaveSendErr(err); // source probalble d'err
                lastSend = ACK_STATES_ERROR;
            } else {
                radioAlphaTRX_SlaveSendNothing();
                lastSend = ACK_STATES_NOTHING;
            }
            radioAlphaTRX_ReceivedMode(); // je me remets en attente d'un msg
        } else if (msgReceive.Type_Msg == srv_ack()) { // un ack confirmant le dernier paquet envoyer 
            switch (lastSend) {
                case ACK_STATES_ERROR:
                    radioAlphaTRX_SlaveUpdatePtrErrBuf();
                    break;
                case ACK_STATES_DATA:
                    break;
                default:
                    break;
            }
        } else if (msgReceive.Type_Msg == srv_data()) { // on me demande transmettre des données 
            //on est donc le soir et je bascule dans la recuperation des données
            appData.state = APP_STATE_RADIO_SEND_DATA; // je lui demande transmettre 
        }
    }
}

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/
//end file