#include "./tx.h"
#include "rx.h"
#include "Services.h"

void configure_tx() {
    //pour tx : 12pF 
    clear_rx();
    ecrire_reg(0x80A7);
}

void clear_tx(){
    ecrire_reg(0x8008);
}

int open_tx(int delais) {
    //registre d'activation d'ecriture
    ecrire_reg(0x8239);
    return rdy(delais); // c'est arbitraire et on reviendra 
}

void close_tx() {
    //registre d'activation d'ecriture
    ecrire_reg(0x8209);
}

int send(uint8_t* buffer, int size, int delais) {
    // a changé au cas ou 
    int ecr = 0;
    if (open_tx(1000) != 0) {
        ecr = send_data(buffer, size, delais);
        close_tx();
    }
    return ecr;
}

int send_data(uint8_t* bytes, int size, int delais) {
    int i;

    //preamble
    send_byte(0xAA, delais);
    send_byte(0xAA, delais);
    send_byte(0xAA, delais);
    
    //peut générer des probleme à surveiller
    //synchro pattern
    send_byte(0x2D, delais);
    send_byte(0xD4, delais);
    for (i = 0; i < size; i++) {
         // cela veux dire que nous n'avons pas pu transmetre un octe 
         // alors les données sont erronnée 
        if (send_byte(bytes[i], delais) == 0)
            break;
    }

    //dummy bytes
    send_byte(0x00, delais);
    send_byte(0x00, delais);
    return i;
}

int send_byte(uint8_t byte, int delais) {
    registre reg;
    reg.octets.g = 0xB8;
    reg.octets.d = byte;
    ecrire_reg(reg.reg);
    return rdy(delais); // arbitraire on reviendra changer plutar 
}



