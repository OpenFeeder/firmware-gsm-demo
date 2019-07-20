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
#include <string.h>

#include "radio_alpha_trx_slave_api.h"
#include "led_status.h"
#include "app.h"

/******************************************************************************/
/******************************************************************************/
/********************* COUCHE APPLICATION DU SLAVE  ***************************/
/***************************                ***********************************/

/*****************                                         ********************/


/**-------------------------->> F R A M E - T O - S E N D <<-------------------*/





/**-------------------------->> V A R I A B L E S <<---------------------------*/
volatile uint8_t BUF_ERR[NB_ERR_BUF]; // sauve les erreur en transmettre 
volatile uint8_t pReadErrBuf = 0;
volatile uint8_t pWriteErrBuf = 0;
volatile uint8_t errOver = 0; // si on a ecraser une erreur gener
//for send data : mise a jour au fure et a mesure 
int8_t BUF_DATA[NB_DATA_BUF][SIZE_DATA]; // taille : NB_DATA_BUF*SIZE_DATA
int8_t nbFrameToSend = 0;
int8_t curseur = 1; //le curseur
int8_t ack_attedue = 1; //
int8_t windows = 1;
int8_t nbBlock = 0;

ACK_STATES lastSend = ACK_STATES_NOTHING;
//______________________________________________________________________________

void radioAlphaTRX_GetLogFromDisk() {
    //TODO transformer cette boucle en while 
    for (nbFrameToSend = 0; nbFrameToSend < NB_DATA_BUF; nbFrameToSend++) {
        strncpy(BUF_DATA[nbFrameToSend], "200119#132402#0700EE39BD0000100000", SIZE_DATA);
    }
}

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

int8_t radioAlphaTRX_SlaveSendMsgRF(uint8_t typeMsg,
                                    uint8_t * data,
                                    uint8_t idMsg,
                                    uint8_t nbRemaining) {
    //    radioAlphaTRX_SetSendMode(1); // j'annonce que je suis en mode transmission 
    Frame frameToSend;

    //_____________CREATE FRAME____________________________________________
    int8_t ret = 0;
    // en tete 
    frameToSend.Champ.dest = MASTER_ID;
    frameToSend.Champ.crc ^= frameToSend.Champ.dest;
    frameToSend.Champ.src = SLAVE_ID;
    frameToSend.Champ.crc ^= frameToSend.Champ.src;
    frameToSend.Champ.idMsg = idMsg;
    frameToSend.Champ.crc ^= frameToSend.Champ.idMsg;
    frameToSend.Champ.typeMsg = typeMsg;
    frameToSend.Champ.crc ^= frameToSend.Champ.typeMsg;
    frameToSend.Champ.nbR = nbRemaining;
    frameToSend.Champ.crc ^= frameToSend.Champ.nbR;

    // data
    int8_t i;
    frameToSend.Champ.size = strlen(data);
    for (i = 0; i < frameToSend.Champ.size; i++) {
        frameToSend.Champ.data[i] = data[i];
        frameToSend.Champ.crc ^= frameToSend.Champ.data[i];
    }
//    printf("src %d \ndes %d \ntypeMsg %d \nnbR %d \nidMsg %d\ncrc %d \nsize %d\n", 
//    frameToSend.Champ.src, frameToSend.Champ.dest, frameToSend.Champ.typeMsg, 
//           frameToSend.Champ.nbR, frameToSend.Champ.idMsg, frameToSend.Champ.crc,
//           frameToSend.Champ.);
//    int j;
//    for (j = 0; j < frameToSend.Champ.size; j++) {
//        printf("%d ",frameToSend.paquet[j]);
//
//    }
//    printf("\n");
    ///____________________________________________________________________
    if (radioAlphaTRX_SendMode()) {
        ret = radioAlphaTRX_SendData(frameToSend);
        ret = 1;
    } else {
#if defined(UART_DEBUG)
        printf("non Envoye\n");
#endif
    }
    radioAlphaTRX_ReceivedMode(); // je me remets en attente d'un msg
    return ret;
}

void radioAlphaTRX_SlaveSendErr(int8_t errToSend) {
    radioAlphaTRX_SlaveSendMsgRF(ERROR, "ERROR", errToSend, 1); // plus tard on evitera
}

void radioAlphaTRX_SlaveSendNothing() {
    radioAlphaTRX_SlaveSendMsgRF(NOTHING, "NOTHING", 1, 1);
}

void radioAlphaTRX_SlaveUpdateDate(uint8_t* date, int16_t derive) {
    Date d;
    int i;
    for (i = 0; i < 4; i++)
        d.date[i] = date[i];
    // deserealise
    struct tm time_set;
    time_set.tm_sec = d.Format.sec;
    time_set.tm_min = d.Format.min;
    time_set.tm_hour = d.Format.h;
    time_set.tm_mday = d.Format.day;
    time_set.tm_mon = d.Format.mm;
    time_set.tm_year = d.Format.yy;
    // mise a jour 
    time_set.tm_sec += 1;
    if (time_set.tm_sec == 60) {
        time_set.tm_sec = 0;
        time_set.tm_min += 1;
        if (time_set.tm_min == 60) {
            time_set.tm_min = 0;
            time_set.tm_hour += 1;
        }
    }
    RTCC_TimeSet((struct tm *) &time_set);
}


/**-------------------------->> S E N D - L O G S <<---------------------------*/
// attention on compte a partir de 1, mais l'indice du table et de la boucle 
// c'est a partir de 0

void radioAlphaTRX_SlaveSendLog() {
    int8_t i = 0;
    while (i < windows && (i + curseur - 1) < nbFrameToSend) {
        int8_t nbRemaining = windows - i;
        if (nbBlock >= NB_BLOC) { // le dernier bloc
            if ((i + curseur - 1) == nbFrameToSend - 1) { // si le dernier paquet 
                nbRemaining = MAX_W + 1; // signifie qu'il n'y a plus de bloc
            }
        } else { // je ne suis pas le dernier bloc
            if ((i + curseur - 1) == nbFrameToSend - 1) { // je suis a la fin d'un bloc
                nbRemaining = MAX_W; // signifie qu'il n'y a plus de bloc
            }
        }
        TMR_Delay(LAPS); // on attends avant de retransmettre un autre msg 
        radioAlphaTRX_SlaveSendMsgRF(DATA,
                                     BUF_DATA[i + curseur - 1],
                                     i + curseur, nbRemaining);
        i++;

    }
    ack_attedue = i + curseur;
    lastSend = ACK_STATES_DATA;
    appData.state = APP_STATE_IDLE; // on rend la main et on attends un ack 
    radioAlphaTRX_ReceivedMode(); // on se met en mode reception 
    TMR_SetWaitRqstTimeout(TIME_OUT_WAIT_ACK); //j'active le timeout 
}

/*********************************************************************
 * Function:        void radioAlphaTRX_SlaveUpdateSendLogParam(uint8_t numSeq)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
void radioAlphaTRX_SlaveUpdateSendLogParam(uint8_t numSeq) {
    curseur = numSeq; // on met a jour le curseur
    if (numSeq == ack_attedue) { // tous les msg envoyees ont ete aquitte
        if (windows < MAX_W - 1) windows += 1;
    } else if (!TMR_GetWaitRqstTimeout()) {
        if (windows > 1)
            windows = windows / 2; // on diminue la fenetre d'emission 
        // il serait interressent de faire des statistique du nombre d'echec constate
    }
    TMR_SetWaitRqstTimeout(-1); // je le desactive 
    if (numSeq >= nbFrameToSend) {
        appData.state = APP_STATE_IDLE; // on demande l'envoie d'un msg de fin de block
    } else {
        appData.state = APP_STATE_RADIO_SEND_DATA; // on se remet en transmission de donnee 
    }
}


/**-------------------------->>                   <<---------------------------*/

/*********************************************************************
 * Function:        void radioAlphaTRX_SlaveAckHundler(Frame msgReceive)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
void radioAlphaTRX_SlaveAckHundler(Frame msgReceive) {
#if defined(UART_DEBUG)
    printf("ACK RECU \n");
#endif
    switch (lastSend) {
        case ACK_STATES_ERROR:
            radioAlphaTRX_SlaveUpdatePtrErrBuf();
            appData.state = APP_STATE_IDLE; // on rend la main
            radioAlphaTRX_ReceivedMode(); // on se met en mode reception 
            break;
        case ACK_STATES_DATA:
            radioAlphaTRX_SlaveUpdateSendLogParam(msgReceive.Champ.idMsg);
            break;
        default:
            appData.state = APP_STATE_IDLE; // on rend la main
            break;
    }
}

/*********************************************************************
 * Function:        radioAlphaTRX_SlaveHundlerMsgReceived()
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
void radioAlphaTRX_SlaveHundlerMsgReceived() {
    Frame msgReceive;
    int16_t timeout = TMR_GetMsgRecuTimeout();
    int8_t err;
    switch (msgReceive.Champ.typeMsg) {
        case DATA:
            if (msgReceive.Champ.idMsg <= NB_BLOC) {
                if (msgReceive.Champ.idMsg > nbBlock) { // on recharge un nouveau blocs
                    nbBlock = msgReceive.Champ.idMsg;
                    //TODO recharge un nouveau block en calclant a partir du numero de bloc
#if defined(UART_DEBUG)
                    printf("nume bloc a envoyer %d, recharge d'un bloc\n", msgReceive.Champ.idMsg);
#endif
                    curseur = 1;
                }
                appData.state = APP_STATE_RADIO_SEND_DATA; // je lui demande transmettre 
            } else {
                appData.state = APP_STATE_IDLE;
            }
            break;
        case INFOS:
#if defined(UART_DEBUG)
            printf("Demande d'infos recu || timeout %d\n", timeout);
#endif  
            if ((err = radioAlphaTRX_SlaveGetError()) > 0) { //l'error a transmettre 
                radioAlphaTRX_SlaveSendErr(err);
                lastSend = ACK_STATES_ERROR;
            } else {
                TMR_Delay(100);
                radioAlphaTRX_SlaveSendNothing();
                lastSend = ACK_STATES_NOTHING;
            }
            radioAlphaTRX_ReceivedMode(); // je me remets en attente d'un msg
            appData.state = APP_STATE_IDLE; // etat endormie 
            break;
        case ACK:
            radioAlphaTRX_SlaveAckHundler(msgReceive);
            break;
        default:
            appData.state = APP_STATE_IDLE;
            break;
    }
}

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/
//end file