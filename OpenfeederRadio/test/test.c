/*

 */

#include "./test.h"
#include "../alpha_trx_driver/radio_alpha_trx.h"
#include "../driver/timer.h"
#include "../driver/config_system_com.h"

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



volatile uint8_t msg_receive = 0; // juste pour les test

void test_set_msg_receive(uint8_t set) {
    msg_receive = set;
} // juste pour les test 


//test 

void test_rx() {
    uint8_t paquet[20];

    radioAlphaTRX_ReceivedMode();
    while (1) {
    }

}

void test_tx() {
    uint8_t *paquet = "PAQUET ENVOIE TEST";
    uint8_t date_send[50];
    int8_t size_h = srv_create_paket_rf(date_send, paquet,
                                        srv_getBroadcast(), 
                                        MASTER_ID, 
                                        srv_horloge(), '0');
    while (1) {
    }
}

void test_timer() {
    TMR_SetnIRQLowTimeout(100);
    LED_GREEN_Toggle();
    while (1) {
        if (TMR_GetnIRQLowTimeout(100) == 0) {
            LED_BLUE_Toggle();
            LED_GREEN_Toggle();
            LED_RED_Toggle();
            TMR_SetnIRQLowTimeout(1000);
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
        if (!TMR_GetHorlogeTimeout()) {
            get_time(&hf);
            //creation de du format pour l'envoie  ensEsclave[0].logRecup = 0;
            serial_buffer(date, hf);
            int8_t size_h = srv_create_paket_rf(date_send,
                                                date, srv_getBroadcast(), 
                                                MASTER_ID, 
                                                srv_horloge(), '0');
            if (radioAlphaTRX_SendMode()) {
                radioAlphaTRX_SendData(date_send, size_h);
            }
            TMR_GetHorlogeTimeout(SEND_HORLOG_TIMEOUT);
        }
    }

}

void test_update_date_receive() {
    struct tm t;
    radioAlphaTRX_ReceivedMode();
#if defined(UART_DEBUG)
    RTCC_TimeGet(&t);
    printf("HEURE ==> %dh:%dmin:%ds\n", t.tm_hour, t.tm_min, t.tm_sec);
#endif
    while (1) {
#if defined(UART_DEBUG)
        RTCC_TimeGet(&t);
        if (!TMR_GetTimeout()) {
            printf("heur slave ==> %dh:%dmin:%ds\n", t.tm_hour, t.tm_min, t.tm_sec);
            TMR_SetTimeout(5000); // 1s
        }
#endif       
    }
}

// verifie une idée