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


