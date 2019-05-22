
#include "./trx.h"

void init_trx() {
    nRES_SetHigh();  //reset du bus nRes == 1 
    ecrire_reg(0x0000);

    ecrire_reg(0xA640); //fHz setting
    ecrire_reg(0x9098); //receive controle command
    ecrire_reg(0xC2AC); //data filter command
    ecrire_reg(0xCA81); //fifo and reset  mode command

    ecrire_reg(0xE000); //wake-up Timer command 

    ecrire_reg(0xC800); //low Duty-cycle commande
    
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

