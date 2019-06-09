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

/**-------------------------->> S T R U C T U R E -- S L A V E <<--------------*/
typedef struct sSlave {
} Slave;


/**-------------------------->> V A R I A B L E S <<---------------------------*/
int8_t msg_receive_rf = 0; // 


//______________________________________________________________________________

/******************************************************************************/
/******************************************************************************/
/********************* COUCHE APPLICATION DU MASTER  **************************/
/***************************                ***********************************/

/*****************                                         ********************/

void master_set_msg_receive_rf(uint8_t set) {
    msg_receive_rf = set;
}

int8_t master_send_msg_rf(uint16_t idSlave, uint8_t type_msg, uint8_t * data, uint8_t id_msg) {
    radioAlphaTRX_set_send_mode(1);
    uint8_t data_send[FRAME_LENGTH];
    int8_t ret = 0;
    int8_t size_h = srv_create_paket_rf(data_send, data, idSlave,
                                        srv_getIDM(), type_msg, id_msg);
    if (radioAlphaTRX_Send_Init()) {
        radioAlphaTRX_Send_data(data_send, size_h);
        ret = 1;
    }
    radioAlphaTRX_set_send_mode(0);
    radioAlphaTRX_Received_Init();
    return ret;
}

int8_t master_send_date_rf() {
    struct heure_format hf;
    uint8_t date[14]; // cas particulier
    get_time(&hf);
    //creation de du format pour l'envoie  ensEsclave[0].logRecup = 0;
    serial_buffer(date, hf);
    return master_send_msg_rf(srv_getBroadcast(), srv_horloge(), date, 1); // a voir
}

void master_handle_msg_rf() {
    Frame data_receive;
    if (srv_decode_packet_rf(radioAlphaTRX_read_buf(), &data_receive, radioAlphaTRX_get_size_buf(), srv_getIDM()) > 0) {
        if (data_receive.Type_Msg == srv_err()) {
#if defined(UART_DEBUG)
            LED_GREEN_Toggle();
            printf("erreur recu ==> transfere gsm :: %d %d\n", data_receive.data, data_receive.Type_Msg);
            master_send_msg_rf(data_receive.ID_Src, srv_ack(), "ACK", data_receive.Type_Msg); // pour l'instant
#endif
        } else if (data_receive.Type_Msg == srv_infos()) {
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

void master_select_slave() {
    //choix du slave 
    int16_t idSlave = 37; // pour l'instant
#if defined(UART_DEBUG)
    printf("demande d'infos au slave %d\n", idSlave);
#endif
    master_send_msg_rf(idSlave, srv_infos(), "INFOS", 1);
}

void master_state_machine_of_daytime() {
    while (1) {
#if defined(UART_DEBUG)
        struct tm t;
        RTCC_TimeGet(&t);
        if (!get_tmr_timeout()) {
            printf("heur master ==> %dh:%dmin:%ds\n", t.tm_hour, t.tm_min, t.tm_sec);
            set_tmr_timeout(5000); // 5s
        }
#endif
        if (!get_tmr_horloge_timeout()) { // on doit envoyer l'horloge en mode broadcast
            master_send_date_rf();
            set_tmr_horloge_timeout_x1000_ms(SEND_HORLOG_TIMEOUT);
        } else if (msg_receive_rf == 1) { // || ou module GSM
            master_handle_msg_rf();
        } else if (!get_tmr_wait_rqst_timeout()) { //le temps d'attente d'une reponse a ecouler on choisi un autre slave
            master_select_slave();
            set_tmr_wait_rqst_timeout(TIME_OUT_WAIT_RQST);
        }
    }
}


/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/