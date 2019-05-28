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
uint8_t BUF_ERR[NB_ERR_BUF]; // sauve les erreur en transmettre 
uint8_t p_read_err_buf = 0;
uint8_t p_write_err_buf = 0;
uint8_t err_evo = 0;
uint8_t last_send; // 0 si erreur | 1 si infos   
//______________________________________________________________________________

/**-------------------------->> I M P L E M E N T <<---------------------------*/

void radioAlphaTRX_save_error(int8_t num_error) { // vraiment a voir 
    BUF_ERR[p_write_err_buf] = num_error;
    p_write_err_buf = (p_write_err_buf + 1) % NB_ERR_BUF;
    if (p_read_err_buf == p_write_err_buf) {
        err_evo = 1;
    } else if (p_read_err_buf == p_write_err_buf - 1 && err_evo) {
        p_read_err_buf = (p_read_err_buf + 1) % NB_ERR_BUF;
    }
}

//ret 0 alors pas d'erreur  

int8_t radioAlphaTRX_slave_get_error() {
    int8_t i = 0;
    if (p_read_err_buf != p_write_err_buf || err_evo) { // il y' a une err
        return BUF_ERR[p_read_err_buf];
    }
    return i;
}

void radioAlphaTRX_slave_update_buf_ptr() {
    p_read_err_buf = (p_read_err_buf + 1) % NB_ERR_BUF;
    if (p_read_err_buf == p_write_err_buf && err_evo)
        err_evo = 0;
}

void radioAlphaTRX_slave_send_err(int8_t *err_to_send) {
    uint8_t date_send[ERROR_LENGTH];
    int8_t size_err = srv_create_paket_rf(date_send, err_to_send, srv_getIDM(),
                                          srv_getIDS1(), srv_err(), '0');
    if (radioAlphaTRX_Send_Init()) {
        radioAlphaTRX_Send_data(date_send, size_err);
    }
    radioAlphaTRX_Init();
    radioAlphaTRX_Received_Init();
}

void radioAlphaTRX_slave_send_nothing() {
    uint8_t date_send[ERROR_LENGTH];
    int8_t size_err = srv_create_paket_rf(date_send, "n", srv_getIDM(),
                                          srv_getIDS1(), srv_nothing(), '0');
    if (radioAlphaTRX_Send_Init()) {
        radioAlphaTRX_Send_data(date_send, size_err);
    }
    radioAlphaTRX_Init();
    radioAlphaTRX_Received_Init();
}

void radioAlphaTRX_slave_update_date(uint8_t* date, int16_t derive) {
    struct heure_format hf;
    deserial_buffer(date, &hf);
    hf.s += 1;
    //TOASK : A voir si c'est nécessaire 
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
    if (srv_decode_packet_rf(radioAlphaTRX_read_buf(), &msg_receive, radioAlphaTrx_get_size_buf(), srv_getIDS1()) > 0) { // 
        if (msg_receive.Type_Msg == srv_horloge()) {
            radioAlphaTRX_slave_update_date(msg_receive.data, get_tmr_msg_recu_timeout());
        } else if (msg_receive.Type_Msg == srv_infos() && get_tmr_msg_recu_timeout()) { // demande de transmission de d'erreur ou de paquet
            int8_t err = radioAlphaTRX_slave_get_error();
            if (err) { //l'error a transmettre 
                radioAlphaTRX_slave_send_err(&err); // source probalble d'err
                last_send = 0;
            } else {
                nothing = 0;
            }
        } else if (msg_receive.Type_Msg == srv_ack()) { // un ack confirmant le dernier paquet envoyer 
            if (!last_send) {
                radioAlphaTRX_slave_update_buf_ptr();
            } else { // pour l'instant on fait ca 
                nothing = 0;
            }
        } else {
            nothing = 0;
        }
    }
    if (!nothing) {
        last_send = 1;
        radioAlphaTRX_slave_send_nothing();
    }

}

//end file