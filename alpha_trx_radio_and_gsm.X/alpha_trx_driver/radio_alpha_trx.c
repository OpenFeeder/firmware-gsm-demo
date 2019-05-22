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

/**-------------------------->> V A R I A B L E S <<---------------------------*/
CFG_SET_CMD_VAL RF_ConfigSet; // Configuration Setting Command
PWR_MGMT_CMD_VAL RF_PowerManagement; // Power Management Command
FQ_SET_CMD_VAL RF_FrequencySet; // Frequency Setting Command
RX_CTRL_CMD_VAL RF_ReceiverControl; // Receiver Control Command
RX_FIFO_READ_CMD_VAL RF_FIFO_Read; // Receiver FIFO Read
FIFO_RST_MODE_CMD_VAL RF_FIFOandResetMode; // FIFO and Reset Mode Command
STATUS_READ_VAL RF_StatusRead; // Status Read Command


/**-------------------------->> D E F I N I T I O N <<-------------------------*/

void radioAlphaTRX_Init(void) {
    
    RF_StatusRead.Val = 0;
    do {
        RF_StatusRead.Val = radioAlphaTRX_Command( STATUS_READ_CMD ); // intitial SPI transfer added to avoid power-up problem
//#if defined(UART_DEBUG)
        //printf( "An other Wait until RFM12B is out of power-up reset, status: 0x%04X\r\n", RF_StatusRead.Val );
//#endif
    }while ( RF_StatusRead.bits.b14_POR );    

}

uint16_t radioAlphaTRX_Command(uint16_t cmd_write) {
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