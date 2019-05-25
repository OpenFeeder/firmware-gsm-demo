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
    set_time(hf);
}


void radioAlphaTRX_slave_behaviour_of_daytime() {
    Paquet msg_receive;
    
    //TOASK : est ce que on traite un msg et on rend la main ou on traite toutes les msg presente ?
    //TOASK : Pour l'instant je traite tout car pas d'importance 

    while (radioAlphaTRX_msg_receive()) { // tant qu'un msg est présent 
        //recuperer le msg | le decapsuler | verifier s'il est mien
        int i = getB_Read();
        incB_Read();
        
        if(srv_decode_packet_rf(getBuf(i), &msg_receive, getSizeBuf(i), 37) > 0) { // poo test 37
            if (msg_receive.typeDePaquet == srv_horloge()) {
                radioAlphaTRX_slave_update_date(msg_receive.data, get_tmr_msg_recu_timeout(i));
            }// il y'aura d'autes type de paquet ici 
        }
    }
}