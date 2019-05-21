/*

 */

#include "./test.h"

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
    
    oepn_receive();
    while (1) {
        if(srv_receive_rf(paquet, 20, 20) > 0) {
            printf("recu : %s\n", paquet);
            LED_BLUE_SetLow();
        }
//        if(getflagFFIT() == 1) {
//            LED_BLUE_SetLow();
//            restFladFFIT();
//        }
    }
    
}

void test_tx() {
    uint8_t *paquet = "PAQUET ENVOIE TEST";
    while (1) {
        if (getFlag()==1) {
            printf("envoie\n");
            resetFlag(); // on remet le flag a 0 pour eviter d'envoyer indefiniment 
            srv_send_rf(paquet, 18, 20, 1);
//            send_byte('A', 20);
        }
    }
}