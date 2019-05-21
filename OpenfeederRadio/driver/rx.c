#include "./trx.h"
#include "tx.h"
#include <time.h>

void open_rx() {
    //registre d'activation d'ecriture
    ecrire_reg(0x82C9);

    ecrire_reg(0xCA83);
}

void configure_rx() {
    //pour rx
    ecrire_reg(0x8067);
}

void clear_rx() {
    //pour rx
    ecrire_reg(0x8008);
}

void close_rx() {
    //registre d'activation d'ecriture
    ecrire_reg(0x8209);
}

int receive(uint8_t* buffer, int size, int delais) {
    //printf("Attente ...\n");
    int lu = 0;
    open_rx();
    lu = read_data(buffer, size, delais);
    close_rx();
    return lu;
}

void wait_clock(int* compteur) {
    do {
        debug_datetime(compteur);
    } while (nIRQ_GetValue());
}

int read_data(uint8_t* buffer, int size, int delais) {
    registre reg_in;
    int i = 0;
    for (i = 0; i < size; i++) {
        if (0 == rdy(delais)) {
            return 0;
        }
        reg_in.reg = ecrire_reg(0xB000);
        buffer[i] = reg_in.octets.d;
        if (reg_in.octets.d == 0)
            return i;
    }
    return i;
}

void receive_datetime(uint8_t* buffer) {
    
    struct heure_format heure_master;
    printf("Attente ...\n");
    open_rx();
    registre reg_in;
    int i;
    int compteur = 0;
    //buffer
    for (i = 0; i < 7; i++) {
        wait_clock(&compteur);
        reg_in.reg = ecrire_reg(0xB000);
        buffer[i] = reg_in.octets.d;
    }
    close_rx();
    deserial_buffer(buffer, &heure_master);
    set_time(heure_master);
    while (1) {
        debug_datetime(&compteur);
    }

}


//test 
void oepn_receive() {
    configure_rx();
    open_rx();
    //init buffer 
}

int8_t readData() {
    registre reg_in;
    reg_in.reg = ecrire_reg(0xB000);
    return reg_in.octets.d;
}