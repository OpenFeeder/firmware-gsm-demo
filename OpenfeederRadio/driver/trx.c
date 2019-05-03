
#include "./trx.h"

void init_trx() {
    nRES_SetHigh();  //reset du bus nRes == 1 
    ecrire_reg(0x0000);

    ecrire_reg(0xA640);
    ecrire_reg(0x9098);
    ecrire_reg(0xC2AC);
    ecrire_reg(0xCA81);

    ecrire_reg(0xE000);

    ecrire_reg(0xC800);
    
    __delay32(SLEEP_AFTER_INIT);
}

uint16_t ecrire_reg(uint16_t commande) {
    registre reg_in, res;
    reg_in.reg = commande;
    CS_SetLow();
    while (CS_GetValue()) {

    }
    res.octets.g = SPI1_Exchange8bit(reg_in.octets.g);
    res.octets.d = SPI1_Exchange8bit(reg_in.octets.d);
    CS_SetHigh();
    while (!CS_GetValue()) {

    }
    // __delay32(PITCH_TRANS);
    return res.reg;
}


// cette fonction pemet d'attedre le nIRQ
int rdy(short delais) {
    short i = 0;
    TMR1_Counter16BitSet(0);
    while (nIRQ_GetValue()) {
        if (TMR1_Counter16BitGet() == TMR1_Period16BitGet()){ // conteur 
            TMR1_Counter16BitSet(0);
            i++;
        }
        if (i == delais) {
            return 0;
        }
    }
    return 1;
}

