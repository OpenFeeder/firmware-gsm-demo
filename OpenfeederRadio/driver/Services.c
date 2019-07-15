/*
 * File:   Services.c
 * Author: mmadi
 *
 * Created on 16 mars 2019, 19:03
 */


#include "xc.h"
#include "Services.h"
#include <string.h>

/******************************************************************************/
/******************************************************************************/
/****************** FONCTIONNALITEES COMMUNES AU ALPHA TRX ********************/
/***************************                ***********************************/

/*****************                                 ****************************/

//on calcule la somme de controle

uint8_t srv_Checksum(uint8_t* paquet, int size) {
    uint8_t somme = 0;
    int i;
    for (i = 0; i < size; ++i) {
        somme ^= paquet[i];
    }
    return somme;
}

bool srv_TestCheksum(uint8_t* paquet, int size, uint8_t somme_ctrl) {
    return (srv_Checksum(paquet, size) == somme_ctrl);
}


int8_t srv_DecodePacketRF(uint8_t* buffer, Frame *frameReceive, uint8_t size) {
    int8_t i = 0;
    frameReceive->id.code = buffer[i++];
//    if (frameReceive->id.id.dest != MASTER_ID)
//        return 0;
    //source d'erreur -2 ou -1
    if (!srv_TestCheksum(buffer, size-1, buffer[size-1]))
        return 0;
    
    frameReceive->rfTandNBR.code = buffer[i++];
    frameReceive->idMsg = buffer[i++];
    int8_t j; int8_t lenData = size-i-1;
    for (j = 0; j < lenData; j++) 
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
    frame.sumCtrl = srv_Checksum(packetToSend, i);
    packetToSend[i++] = frame.sumCtrl;
    return i;
}



/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/