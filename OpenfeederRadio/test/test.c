/*

 */

#include "./test.h"
#include "../alpha_trx_driver/radio_alpha_trx.h"
#include "../driver/timer.h"
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
            if (getB_Read() != getB_Write()) {
                printf("tps %d \n", 3000-get_tmr_msg_recu_timeout(getB_Read()));
                printf("recu %s \n", getBuf(getB_Read()));
                setB_Read((getB_Read()+1)%4);
                printf("B_Read %d vs B_Write %d\n", getB_Read(), getB_Write());
            }else if (getError_FFOV()) {
                printf("tps %d \n", 3000-get_tmr_msg_recu_timeout(getB_Read()));
                resetError_FFOV();
                printf("recu %s \n", getBuf(getB_Read()));
                printf("B_Read %d vs B_Write %d\n", getB_Read(), getB_Write());
            }
        }
    }
    
}

void test_tx() {
    uint8_t *paquet = "PAQUET ENVOIE TEST";
    while (1) {
        if (getFlag()==1) {
            printf("envoie\n");
            resetFlag(); // on remet le flag a 0 pour eviter d'envoyer indefiniment 
            if (radioAlphaTRX_Send_Init()){
                radioAlphaTRX_Send_data(paquet, 18);
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