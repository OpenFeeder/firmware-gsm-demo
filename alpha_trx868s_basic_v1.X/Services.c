/*
 * File:   Services.c
 * Author: mmadi
 *
 * Created on 16 mars 2019, 19:03
 */


#include "xc.h"
#include "Services.h"
#include "led_status.h"
#include <string.h>

/******************************************************************************/
/******************************************************************************/
/****************** FONCTIONNALITEES COMMUNES AU ALPHA TRX ********************/
/***************************                ***********************************/

/*****************                                 ****************************/


//on calcule la somme de controle

uint8_t srv_checksum(uint8_t* paquet, int size) {
    uint8_t somme = 0;
    int i;
    for (i = 0; i < size; ++i) {
        somme ^= paquet[i];
    }
    return somme;
}

bool srv_test_cheksum(uint8_t* paquet, int size, uint8_t somme_ctrl) {
    return (srv_checksum(paquet, size) == somme_ctrl);
}


int8_t srv_DecodePacketRF(uint8_t* buffer, Frame *frameReceive, uint8_t size) {
    int8_t i = 0;
    frameReceive->id.code = buffer[i++];
    if (frameReceive->id.id.dest != SLAVE_ID)
        return 0;
    
    if (!srv_test_cheksum(buffer, size-2, buffer[size-1]))
        return 0;
    
    frameReceive->rfTandNBR.code = buffer[i++];
    frameReceive->idMsg = buffer[i++];
    int8_t j;
    for (j = 0; j < size-2; j++) 
        frameReceive->data[j] = buffer[i++];
    frameReceive->sumCtrl = buffer[i++];
    return i;
}

int8_t srv_CreatePaketRF(Frame frame, uint8_t *packetToSend) {
    uint8_t i = 0;
    packetToSend[i++] = frame.id.code;
    packetToSend[i++] = frame.rfTandNBR.code;
    packetToSend[i++] = frame.idMsg;
    int8_t j = 0;
    for (; j < strlen(frame.data); j++)
        packetToSend[i++] = frame.data[j];
    packetToSend[i++] = frame.sumCtrl;
    return i;
}


/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/