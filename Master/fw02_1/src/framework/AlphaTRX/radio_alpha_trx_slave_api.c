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


//to logger
struct tm oldTime;
struct tm updateTime;
int8_t slaveId = 1;

uint8_t radioAlphaTRX_SlaveGetSlaveID(){ return slaveId; } 
void radioAlphaTRX_SlaveSetSlaveID(uint8_t idSlave) { slaveId = idSlave; }
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
                                    uint8_t nbRemaining,
                                    uint8_t frmaeSize) {
    Frame frameToSend;
    memset(frameToSend.paquet, 0, FRAME_LENGTH);
    //_____________CREATE FRAME____________________________________________
    int8_t ret = 0;
    // en tete 
    frameToSend.Champ.dest = MASTER_ID;
    frameToSend.Champ.src = SLAVE_ID;
    frameToSend.Champ.crc ^= frameToSend.paquet[0];
    frameToSend.Champ.typeMsg = typeMsg;
    frameToSend.Champ.nbR = nbRemaining;
    frameToSend.Champ.crc ^= frameToSend.paquet[1];
    frameToSend.Champ.idMsg = idMsg;
    frameToSend.Champ.crc ^= frameToSend.paquet[2];
    // data
    int8_t i;
    frameToSend.Champ.size = frmaeSize;
    frameToSend.Champ.crc ^= frameToSend.paquet[3];
    for (i = 0; i < frameToSend.Champ.size; i++) {
        frameToSend.Champ.data[i] = data[i];
        frameToSend.Champ.crc ^= frameToSend.paquet[i + 5]; // penser ? changer le 5 en generique 
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

void radioAlphaTRX_SlaveSendErr(int8_t errToSend) {
//    radioAlphaTRX_SlaveSendMsgRF(ERROR, "ERROR", errToSend, 1); // plus tard on evitera
}

void radioAlphaTRX_SlaveSendNothing() {
//    radioAlphaTRX_SlaveSendMsgRF(NOTHING, "NOTHING", 1, 1);
}

void radioAlphaTRX_SlaveUpdateDate(uint8_t* date) {
    Date d;
    d.dateVal = 0;
    int i;
    for (i = 0; i < 5; i++) {
        d.date[i] = date[i];
    }

    // deserealise
    struct tm time_set;
    RTCC_TimeGet(&time_set);
    oldTime = time_set;
    time_set.tm_sec = d.Format.sec;
    time_set.tm_min = d.Format.min;
    time_set.tm_hour = d.Format.h;
    time_set.tm_mday = d.Format.day;
    time_set.tm_mon = d.Format.mm;
    time_set.tm_year = d.Format.yy;
    updateTime = time_set;
    // mise a jour 
    RTCC_TimeSet((struct tm *) &time_set);
    appData.state = APP_STATE_STORE_HOUR;
    LED_STATUS_R_Toggle();
    LED_STATUS_B_Toggle();
    LED_STATUS_Y_Toggle();
}

FILEIO_RESULT radioAlphaTRX_SlaveUpdateDatelog() {
    setLogDate(TIME_TO_LOG_DATE);
    
    FILEIO_OBJECT file;
    FILEIO_ERROR_TYPE errF;
    char buf[250];
    int flag;
    size_t numDataWritten;
    bool needToUnmount;

    if (USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status) {
        needToUnmount = false;
    } else {
        if (USB_DRIVE_NOT_MOUNTED == usbMountDrive()) {
            return FILEIO_RESULT_FAILURE;
        }
        needToUnmount = true;
    }

    /* Log event if required */
    if (true == appDataLog.log_events) {
        store_event(OF_UPDATE_DATE_LOG_FAIL);
    }

    if (FILEIO_RESULT_FAILURE == FILEIO_Open(&file,
                                             TEST_SYNCHRO_FILE,
                                             FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_APPEND)) {
        errF = FILEIO_ErrorGet('A');
        sprintf(appError.message, "Unable to open error log file (%u)", errF);
        appError.current_line_number = __LINE__;
        sprintf(appError.current_file_name, "%s", __FILE__);
        FILEIO_ErrorClear('A');
        appError.number = ERROR_ERROR_FILE_OPEN;
        return FILEIO_RESULT_FAILURE;
    }

    memset(buf, '\0', sizeof ( buf));
    getDateTime();
    flag = sprintf(buf, "%c%c,OF%c%c,%02d/%02d/%04d,%02d:%02d:%02d : actuelle\n",
                   appData.siteid[0],
                   appData.siteid[1],
                   appData.siteid[2],
                   appData.siteid[3],
                   oldTime.tm_mday,
                   oldTime.tm_mon,
                   2000 + oldTime.tm_year,
                   oldTime.tm_hour,
                   oldTime.tm_min,
                   oldTime.tm_sec);
    
    if ( flag > 0 ) {
        numDataWritten = FILEIO_Write( buf, 1, flag, &file );
    }
    
    if ( numDataWritten < flag ) {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to write error in log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear('A');
        appError.number = ERROR_ERROR_FILE_WRITE;
        return FILEIO_RESULT_FAILURE;
    }
    
    
    memset(buf, '\0', sizeof ( buf));
    getDateTime();
    flag = sprintf(buf, "%c%c,OF%c%c,%02d/%02d/%04d,%02d:%02d:%02d : update\n\n",
                   appData.siteid[0],
                   appData.siteid[1],
                   appData.siteid[2],
                   appData.siteid[3],
                   updateTime.tm_mday,
                   updateTime.tm_mon,
                   2000 + updateTime.tm_year,
                   updateTime.tm_hour,
                   updateTime.tm_min,
                   updateTime.tm_sec);
    
    if ( flag > 0 ) {
        numDataWritten = FILEIO_Write( buf, 1, flag, &file );
    }
    
    if ( numDataWritten < flag ) {
        errF = FILEIO_ErrorGet( 'A' );
        sprintf( appError.message, "Unable to write error in log file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear('A');
        appError.number = ERROR_ERROR_FILE_WRITE;
        return FILEIO_RESULT_FAILURE;
    }
    
    
    if (FILEIO_RESULT_FAILURE == FILEIO_Close(&file))
    {
        errF = FILEIO_ErrorGet('A');
        sprintf( appError.message, "Unable to close error file (%u)", errF );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        FILEIO_ErrorClear( 'A' );
        appError.number = ERROR_ERROR_FILE_CLOSE;
        return FILEIO_RESULT_FAILURE;
    }

    if (true == needToUnmount)
    {
        if (USB_DRIVE_MOUNTED == usbUnmountDrive( ) )
        {
            return FILEIO_RESULT_FAILURE;
        }
    }

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_LOG_INFO)
    printf("\tError to file success\n");
#endif 

    return FILEIO_RESULT_SUCCESS;
}


/**-------------------------->> S E N D - L O G S <<---------------------------*/
// attention on compte a partir de 1, mais l'indice du table et de la boucle 
// c'est a partir de 0

void radioAlphaTRX_SlaveSendLog() {
    int8_t i = 0;
#if defined(UART_DEBUG)
    printf("w = %d\ncurseur = %d, nbFrameToSend %d\n", windows, curseur, nbFrameToSend);
#endif
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
        printf("send %d \n",radioAlphaTRX_SlaveSendMsgRF(DATA,
                                     BUF_DATA[i + curseur - 1],
                                     i + curseur, nbRemaining, SIZE_DATA));
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
void radioAlphaTRX_SlaveHundlerMsgReceived(Frame msgReceive) {
    appData.state = APP_STATE_IDLE; // etat endormie
    int16_t timeout = TMR_GetMsgRecuTimeout();
    switch (msgReceive.Champ.typeMsg) {
        case DATA:
#if defined(UART_DEBUG)
            printf("---->DEMANDE DE DATA\n");
            printf("id recu %d,  nb block %d\n", msgReceive.Champ.idMsg, nbBlock);
#endif
            if (msgReceive.Champ.idMsg <= NB_BLOC) {
                if (msgReceive.Champ.idMsg != nbBlock) { // on recharge un nouveau blocs
                    nbBlock = msgReceive.Champ.idMsg;
                    //TODO recharge un nouveau block en calclant a partir du numero de bloc
#if defined(UART_DEBUG)
                    printf("nume bloc a envoyer %d, recharge d'un bloc\n", msgReceive.Champ.idMsg);
#endif
                    curseur = 1;
                    windows = 1;
                }
                appData.state = APP_STATE_RADIO_SEND_DATA; // je lui demande transmettre 
            }
            break;
        case INFOS:
        {
            int8_t err;
#if defined(UART_DEBUG)
            printf("Demande d'infos recu || timeout %d\n", timeout);
#endif  
            if ((err = radioAlphaTRX_SlaveGetError()) > 0) { //l'error a transmettre 
                //                radioAlphaTRX_SlaveSendErr(err);
                radioAlphaTRX_SlaveSendMsgRF(ERROR, "ERROR", err, 1, 5);
                lastSend = ACK_STATES_ERROR;
            } else {
                TMR_Delay(100);
                radioAlphaTRX_SlaveSendMsgRF(NOTHING, "NOTHING", 1, 1, 7);
                lastSend = ACK_STATES_NOTHING;
            }
            break;
        }
        case ACK:
        {
#if defined(UART_DEBUG)
            printf("ACK RECU \n");
#endif
            switch (lastSend) {
                case ACK_STATES_ERROR:
                    radioAlphaTRX_SlaveUpdatePtrErrBuf();
                    radioAlphaTRX_ReceivedMode(); // on se met en mode reception 
                    break;
                case ACK_STATES_DATA:
                    curseur = msgReceive.Champ.idMsg; // on met a jour le curseur
                    if (msgReceive.Champ.idMsg == ack_attedue) { // tous les msg envoyees ont ete aquitte
//                        if (windows < MAX_W - 1) windows += 1;
                        windows = MAX_W-1;
                    } else if (!TMR_GetWaitRqstTimeout()) {
                        if (windows > 1)
                            windows = windows / 2; // on diminue la fenetre d'emission 
                        // il serait interressent de faire des statistique du nombre d'echec constate
                    }
                    TMR_SetWaitRqstTimeout(-1); // je desactive le timer 
                    if (msgReceive.Champ.idMsg > nbFrameToSend) {
                        appData.state = APP_STATE_IDLE; // on demande l'envoie d'un msg de fin de block
                    } else {
                        appData.state = APP_STATE_RADIO_SEND_DATA; // on se remet en transmission de donnee 
                    }
                    //                    radioAlphaTRX_SlaveUpdateSendLogParam(msgReceive.Champ.idMsg);
                    break;
                default:
                    break;
            }
        }
        default:
            break;
    }
}
/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/
//end file