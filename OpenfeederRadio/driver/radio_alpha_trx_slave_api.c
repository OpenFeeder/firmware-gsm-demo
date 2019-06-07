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

/**-------------------------->> V A R I A B L E S <<---------------------------*/
volatile uint8_t BUF_ERR[NB_ERR_BUF]; // sauve les erreur en transmettre 
volatile uint8_t p_read_err_buf = 0;
volatile uint8_t p_write_err_buf = 0;
volatile uint8_t err_evo = 0;
uint8_t last_send = 2; // 0 si erreur | 1 si infos 2 ack   
//______________________________________________________________________________

/******************************************************************************/
/******************************************************************************/
/********************* COUCHE APPLICATION DU SLAVE  ***************************/
/***************************                ***********************************/

/*****************                                         ********************/

void radioAlphaTRX_save_error(int8_t num_error) { // vraiment a voir 
    LED_GREEN_Toggle();
    BUF_ERR[p_write_err_buf] = num_error;
    p_write_err_buf = (p_write_err_buf + 1) % NB_ERR_BUF;
    if (p_read_err_buf == p_write_err_buf) {
        err_evo = 1;
    } else if (((p_read_err_buf == p_write_err_buf - 1)||(p_write_err_buf - 1 < 0)) && err_evo) {
        p_read_err_buf = (p_read_err_buf + 1) % NB_ERR_BUF;
    }
}
//ret 0 alors pas d'erreur 
int8_t radioAlphaTRX_slave_get_error() {
    if (p_read_err_buf != p_write_err_buf || err_evo) { // il y' a une err
        return BUF_ERR[p_read_err_buf];
    }
    return 0;
}

void radioAlphaTRX_slave_update_buf_err_ptr() {
    p_read_err_buf = (p_read_err_buf + 1) % NB_ERR_BUF;
    if (p_read_err_buf == p_write_err_buf && err_evo)
        err_evo = 0;
}

int8_t radioAlphaTRX_slave_send_msg_rf(uint8_t type_msg, uint8_t * data, uint8_t id_msg) {
    radioAlphaTRX_set_send_mode(1);
    
    uint8_t data_send[FRAME_LENGTH];
    int8_t ret = 0;
    int8_t size = srv_create_paket_rf(data_send, data, srv_getIDM(), srv_getIDS1(), type_msg, id_msg);
    if (radioAlphaTRX_Send_Init()) {
        radioAlphaTRX_Send_data(data_send, size);
        ret = 1;
    }
    radioAlphaTRX_set_send_mode(0);
    radioAlphaTRX_Received_Init();
    return ret;
}

void radioAlphaTRX_slave_send_err(int8_t *err_to_send) {
    radioAlphaTRX_slave_send_msg_rf (srv_err(), err_to_send, 1);
}

void radioAlphaTRX_slave_send_nothing() {
#if defined(UART_DEBUG)
    printf("J'ai rien a transmettre ==> nothing\n");
#endif
    radioAlphaTRX_slave_send_msg_rf (srv_nothing(), "NOTHING", 1);
}

void radioAlphaTRX_slave_update_date(uint8_t* date, int16_t derive) {
    struct heure_format hf;
    deserial_buffer(date, &hf);

    //TOASK : etant donner qu'on est a la seconde pres, selon mecalcule 
    //TOASK : je dois ajouter 1 pour etre à peut pres synchrone 
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

void radioAlphaTRX_slave_behaviour_of_daytime() {
    Frame msg_receive;
    uint16_t timeout = get_tmr_msg_recu_timeout();
    int8_t nothing = 1;
    //recuperer le msg | le decapsuler | verifier s'il est mien 
    if (srv_decode_packet_rf(radioAlphaTRX_read_buf(), &msg_receive, radioAlphaTrx_get_size_buf(), srv_getIDS1()) > 0) { // est il à moi
        if (msg_receive.Type_Msg == srv_horloge()) {
#if defined(UART_DEBUG)
            printf("Msg synch recu \n");
#endif
            radioAlphaTRX_slave_update_date(msg_receive.data, get_tmr_msg_recu_timeout());
        } else if (msg_receive.Type_Msg == srv_infos() ) { // demande de transmission de d'erreur ou de paquet
            int8_t err = radioAlphaTRX_slave_get_error();
#if defined(UART_DEBUG)
            printf("Demande d'infos recu\n");
            printf("err = %d\n", err);
#endif
            if (err) { //l'error a transmettre 
                radioAlphaTRX_slave_send_err(err+1); // source probalble d'err
                last_send = 0;
            } else {
                last_send = 2;
                radioAlphaTRX_slave_send_nothing();
            }
        } else if (msg_receive.Type_Msg == srv_ack()) { // un ack confirmant le dernier paquet envoyer 
            if (last_send == 0) {
#if defined(UART_DEBUG)
                printf("mise a jour du buf d'err\n");
#endif
                radioAlphaTRX_slave_update_buf_err_ptr();
            }
        }
    }
}

//end file