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

#include "radio_alpha_trx_slave_api.h"

void radioAlphaTRX_slave_update_date(uint8_t* date, int16_t derive) {
    struct heure_format hf;
    deserial_buffer(date, &hf);
    hf.s += 1;
    //TOASK : A voir si c'est nécessaire 
    if (hf.s == 60){ 
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
    //recuperer le msg | le decapsuler | verifier s'il est mien   
    if(srv_decode_packet_rf(radioAlphaTRX_read_buf(), &msg_receive, radioAlphaTrx_get_size_buf(), 37) > 0) { // poo test 37
        if (msg_receive.Type_Msg == srv_horloge()) {
            radioAlphaTRX_slave_update_date(msg_receive.data, get_tmr_msg_recu_timeout());
        }// il y'aura d'autes type de paquet ici 
    }
}