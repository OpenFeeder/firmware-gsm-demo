/* *****************************************************************************
 * 
 * _____________________________________________________________________________
 *
 *                    Driver ALPHA TRX433S - Interface SPI
 * _____________________________________________________________________________
 *
 * Titre            : Mise en oeuvre du module ALPHA TRXxxxS
 * Version          : v00
 * Date de creation : 22/05/2019
 * Auteur           : MMADI Anzilane (A partir du code de Arnauld BIGANZOLI (AB))
 * Contact          : anzilan@hotmail.fr
 * Web page         : 
 * Collaborateur    : ...
 * Processor        : PIC24
 * Tools used       : MPLAB X IDE v5.15 and MPLAB Code Configurator (MCC) Version: 3.36
 * Compiler         : Microchip XC16 v1.35
 * Programmateur    : PICkit 3
 *******************************************************************************
 *******************************************************************************
 */

 /**------------------------->> I N C L U D E S <<-----------------------------*/
#include <stdio.h>

#include "radio_alpha_trx.h"

 /******************************************************************************/
//
void radioAlphaTRX_Init(void) {
#if defined(UART_DEBUG)
    printf("Anzilane\n");
#endif
}

uint16_t radioTransceiver_Command(uint16_t cmd_write) {
    WORD_VAL_T receiveData;
    WORD_VAL_T sendData;
    sendData.word = cmd_write;
    
    RF_nSEL_SetLow(); // debut de la communication SPI avec le Slave
    while (RF_nSEL_GetValue()) { } // a eviter je pense, mais pour l'instant je le garde 
    receiveData.byte.high = SPI1_Exchange8bit( sendData.byte.high );
    receiveData.byte.low = SPI1_Exchange8bit( sendData.byte.low );
    RF_nSEL_SetHigh(); // fin de la communication SPI avec le SLave 
    while (!RF_nSEL_GetValue()) { } // a eviter mais pour l'instant je le garde 
    
    return receiveData.word;
}