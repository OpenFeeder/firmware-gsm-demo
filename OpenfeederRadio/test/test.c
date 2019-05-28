/*

 */

#include "./test.h"
#include "../alpha_trx_driver/radio_alpha_trx.h"
#include "../driver/radio_alpha_trx_slave_api.h"
#include "../driver/timer.h"
#include "../driver/config_system_com.h"


/* vérifier que le state register se remet à 0x4000 après un reset
 * pendant le fonctionnement
 */
void test_state_register() {
    uint16_t state_reg = ecrire_reg(0x0000);
    __delay32(PITCH_TRANS);

    state_reg = ecrire_reg(0x0000);
    __delay32(PITCH_TRANS);
    nRES_SetLow();
    __delay32(PITCH_TRANS);
    nRES_SetHigh();
    __delay32(PITCH_TRANS);

    state_reg = ecrire_reg(0x0000);
}

//simulation d'une horloge

void test_clock_seal() {
    nRES_SetHigh();
    __delay32(1000);
    nRES_SetLow();
    __delay32(1000);
}

uint8_t test_serial_datetime(uint8_t* datetime) {
    struct heure_format hf_serial, hf_deserial;
    uint8_t data_serial[7];
    /*
    hf_serial = conv_datetime(datetime);
    serial_buffer(data_serial, hf_serial);
    deserial_buffer(data_serial, &hf_deserial);
    return hf_serial == hf_deserial;
    */
}

//test 
void test_rx() {
    uint8_t paquet[20];
    
    radioAlphaTRX_Received_Init();
    while (1) {
        if (getFlag()==1){
            resetFlag();
            if (radioAlphaTRX_is_receive_msg()) {
                printf("tps %d \n", TIME_OUT_GET_FRAME-get_tmr_msg_recu_timeout());
                printf("recu %s \n", radioAlphaTRX_read_buf());
            }
        }
    }
    
}

void test_tx() {
    uint8_t *paquet = "PAQUET ENVOIE TEST";
    uint8_t date_send[50];
    int8_t size_h = srv_create_paket_rf(date_send, paquet, srv_getBroadcast(), srv_getIDM(), 
                    srv_horloge(), '0');
    while (1) {
        if (getFlag()==1) {
            printf("envoie\n");
            resetFlag(); // on remet le flag a 0 pour eviter d'envoyer indefiniment 
            if (radioAlphaTRX_Send_Init()){
                radioAlphaTRX_Send_data(paquet, size_h);
            }
        }
    }
}

void test_timer() {
    set_tmr_nIRQ_low_timeout(100);
    LED_GREEN_Toggle();
    while (1) {
        if(get_tmr_nIRQ_low_timeout() == 0) {
            LED_BLUE_Toggle();
            LED_GREEN_Toggle();
            LED_RED_Toggle();
            set_tmr_nIRQ_low_timeout(1000);
        }
    }

}

void test_update_date_send() {
    uint8_t date_send[50];
    struct tm t;
    struct heure_format hf;
    uint8_t date[14]; // cas particulier
//#if defined(UART_DEBUG)
//        RTCC_TimeGet(&t); 
//        printf("HEURE ==> %dh:%dmin:%ds\n", t.tm_hour, t.tm_min, t.tm_sec);
//#endif
    while (1) {
        if (getFlag()) {
            resetFlag();
#if defined(UART_DEBUG)
            RTCC_TimeGet(&t); 
            printf("Fin ==> %dh:%dmin:%ds\n", t.tm_hour, t.tm_min, t.tm_sec);
#endif
        }else if (!get_tmr_horloge_timeout()) {
//#if defined(UART_DEBUG)
//            printf("Master : date envoye...\n");
//#endif
            get_time(&hf);
            //creation de du format pour l'envoie  ensEsclave[0].logRecup = 0;
            serial_buffer(date, hf);
            int8_t size_h = srv_create_paket_rf(date_send, date, srv_getBroadcast(), srv_getIDM(), 
                srv_horloge(), '0');
//#if defined(UART_DEBUG)
//         RTCC_TimeGet(&t); 
//         printf("trans : %dh:%dm:%ds\n", t.tm_hour, t.tm_min, t.tm_sec);
//#endif
            if (radioAlphaTRX_Send_Init()) {
                radioAlphaTRX_Send_data(date_send, size_h);
            }
            set_tmr_horloge_timeout_x1000_ms(SEND_HORLOG_TIMEOUT);
        }
    }

}
void test_update_date_receive() {
    printf("anzilane\n");
    struct tm t;
    radioAlphaTRX_Received_Init();
#if defined(UART_DEBUG)
    RTCC_TimeGet(&t); 
    printf("HEURE ==> %dh:%dmin:%ds\n", t.tm_hour, t.tm_min, t.tm_sec);
#endif
    while (1) {
#if defined(UART_DEBUG)
        RTCC_TimeGet(&t);
        if (!get_tmr_timeout()) {
            printf("heur slave ==> %dh:%dmin:%ds\n", t.tm_hour, t.tm_min, t.tm_sec);
            set_tmr_timeout(1000); // 1s
        }
#endif
        if (getFlag()) {
            resetFlag();
            // simule une activité
#if defined(UART_DEBUG)
            RTCC_TimeGet(&t); 
            printf("heur slave ==> %dh:%dmin:%ds\n", t.tm_hour, t.tm_min, t.tm_sec);
#endif
        }else if (radioAlphaTRX_is_receive_msg()) {
            radioAlphaTRX_slave_behaviour_of_daytime();
        }
    }
}

// verifie une idée