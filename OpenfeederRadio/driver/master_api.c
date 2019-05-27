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
 /****************** FONCTIONNALITEES COMMUNES AU ALPHA TRX ********************/
 /***************************                ***********************************/
 /*****************                                 ****************************/


int8_t master_send_date_rf() {
    uint8_t date_send[FRAME_LENGTH];
    struct tm t;
    int8_t ret = 0;
    struct heure_format hf;
    uint8_t date[14]; // cas particulier
    get_time(&hf);
    //creation de du format pour l'envoie  ensEsclave[0].logRecup = 0;
    serial_buffer(date, hf);
    int8_t size_h = srv_create_paket_rf(date_send, date, srv_getBroadcast(), 
            srv_getIDM(), srv_horloge(), '0');
    if (radioAlphaTRX_Send_Init()) {
        radioAlphaTRX_Send_data(date_send, size_h);
        set_tmr_horloge_timeout_x1000_ms(SEND_HORLOG_TIMEOUT); // on attend a vouveau
        ret = 1;
    }
    // pour l'instant on fait ça on verra si on trouve une solution propre plus tard 
    //on se remet en ecoute 
    radioAlphaTRX_Init();
    radioAlphaTRX_Received_Init();
    return ret; // a voir 
}

void master_state_machine() {
    while(1) {
        if (!get_tmr_horloge_timeout()) { // on doit envoyer l'horloge en mode broadcast
            LED_GREEN_Toggle();
            master_send_date_rf();
        }// il y'aura d'autres cas ici 
    }
}


/****************                                         *********************/
 /*************************                     ********************************/
 /******************************************************************************/
 /******************************************************************************/