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
    //do {
        RF_StatusRead.Val = radioAlphaTRX_Command( STATUS_READ_CMD ); // intitial SPI transfer added to avoid power-up problem
        printf( "status: 0x%04X\r\n", RF_StatusRead.Val );
//#if defined(UART_DEBUG)
        //printf( "An other Wait until RFM12B is out of power-up reset, status: 0x%04X\r\n", RF_StatusRead.Val );
//#endif
    //}while ( RF_StatusRead.bits.b14_POR );
    
    /**-------------> Frequency Setting Command @ 433 MHz <--------------------*/
    
    //    ALPHA_TRX433S_Control(0xA640); // Set operation frequency: Fc= 430+F*0.0025 , soit 430+1600*0.0025= 434 MHz avec 0x640 --> 110 0100 0000
    RF_FrequencySet.Val = FQ_SET_CMD_POR;
    RF_StatusRead.Val = radioAlphaTRX_Command(RF_FrequencySet.Val); // Set operation frequency: Fc= 430+F*0.0025 , soit 430+1600*0.0025= 434 MHz avec 0x640 --> 110 0100 0000 
    printf( "status: 0x%04X\r\n", RF_StatusRead.Val );
    
    /**-------------> FReceiver Control Command <------------------------------*/
    // Interrupt,FAST,200kHz,20dBm,-103dBm
    // p16 - Interrupt input (bit 10)
    // d.  - Fast
    // i.  - Receiver baseband bandwidth (BW) select: 200 KHz
    // g.  - Gain relative to maximum               :  20 dBm
    // r.  - RSSI detector threshold                : -103 dBm
    RF_ReceiverControl.Val = RX_CTRL_CMD_POR;
    RF_ReceiverControl.REGbits.RSSIsetth = RSSIsetth_n103;
    RF_ReceiverControl.REGbits.GLNA = GAIN_n20_dB;
    RF_ReceiverControl.REGbits.RX_BW_Select = BW_200_KHz;
    RF_ReceiverControl.REGbits.VDI_RespSetting = FAST;
    RF_ReceiverControl.REGbits.Pin16_function = INTERRUPT_INPUT;
    radioAlphaTRX_Command(RF_ReceiverControl.Val);
    
    /** 0xC2AC - Data Filter Command */
    // AL,!ml,DIG,DQD4
    // al - Clock recovery (CR) auto lock control : auto mode
    // ml - Clock recovery lock control           : slow mode, slow attack and slow release
    // s  - Digital filter
    // f  - DQD threshold parameter               : 4
    radioAlphaTRX_Command(0xC2AC); // al=1; ml=0;Data Filter: Digital filter, DATA QUALITY THRESHOLD 4
    
    /**-------------> 0xCA83 - FIFO and Reset Mode Command <-------------------*/
    //  bit  15  14  13  12  11  10  9  8   7   6   5   4   3   2   1   0   POR
    //        1   1   0   0   1   0  1  0  f3  f2  f1  f0  sp  al  ff  dr   CA80h
    // f<3:0> - FIFO IT level                   : 8
    // sp     - Length of the synchron pattern  : byte 1: 2Dh and byte 0: D4h
    // al     - FIFO fill start condition       : Synchron pattern
    // ff     - Input of the FIFO fill start con: FIFO fill will be enabled after synchron pattern reception
    // dr     - Disable highly sens. RESET mode : Non-sensitive reset
    //    ALPHA_TRX433S_Control(0xCA83); // FIFO interrupt level 8, sp = 0 Synchron pattern: 0x2D + 0xD4, al=0 SYNC, disable FIFO fill (FIFO fill will be enabled after synchron pattern reception)
    RF_FIFOandResetMode.Val = FIFO_RST_MODE_CMD_POR;
    RF_FIFOandResetMode.bits.b0_dr = 1; // Non-sensitive reset
    RF_FIFOandResetMode.bits.b1_ff = 0; // FIFO fill will be enabled after synchron pattern reception
    radioAlphaTRX_Command( RF_FIFOandResetMode.Val ); // al=1; ml=0;Data Filter: Digital filter, DATA QUALITY THRESHOLD 6

    
    /* 0xE000 - Wake-Up Timer Command */
    // NOT USE
    // T_wakeup = 0 ms
    radioAlphaTRX_Command(0xE000); // Wake-Up Timer NOT USE :: pour l'instant on fait �a on changera 
    
    /* 0xC800 - Low Duty-Cycle Command */
    // NOT USE
    // duty-cyle                    : 0 %
    // low duty-cycle mode enabled  : off
    radioAlphaTRX_Command(0xC800); // disable low duty cycle mode --> NOT USE
    __delay32(SLEEP_AFTER_INIT);
}

// Initialiser la detection d'une nouvelle donnee
void radioAlphaTRX_Received_Init(void) {
    
    /**-------------> Configuration Setting Command <--------------------------*/
    //  bit  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0   POR
    //  Val   1   0   0   0   0   0   0   0  el  ef  b1  b0  x3  x2  x1  x0   0x8008
    // --------------------- 0x80?? ---------------------
    // el     - Enabled the internal data register : off
    // ef     - Enabled the FIFO mode              : on
    // b<1:0> - Set up the band                    : 868 MHz
    // x<3:0> -                                    : 12.5 pF
    RF_ConfigSet.Val = CFG_SET_CMD_POR; // initialiser la variable globale avec la valeur interne pr?sente
    //active receive mode (RX)lors du Power On Reset
    RF_ConfigSet.bits.b6_ef = 1;
    RF_ConfigSet.REGbits.SelectBand = BAND_868; // initialer module at 868 Mhz
    RF_ConfigSet.REGbits.SelectCrystalCapacitor = LOAD_C_12_0pF;  
    radioAlphaTRX_Command(RF_ConfigSet.Val);
    //active the 
    RF_FIFOandResetMode.bits.b1_ff = 0; // FIFO fill stops
    radioAlphaTRX_Command(RF_FIFOandResetMode.Val);
    RF_FIFOandResetMode.bits.b1_ff = 1; // FIFO fill will be enabled after synchronize pattern reception
    radioAlphaTRX_Command(RF_FIFOandResetMode.Val); // --> 0xCA83
}

// Configuration en mode TX avant l'envoie de donnee
void radioAlphaTRX_Send_Init(void) {
    //clear 
    /**-------------> Configuration Setting Command <--------------------------*/
    //  bit  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0   POR
    //  Val   1   0   0   0   0   0   0   0  el  ef  b1  b0  x3  x2  x1  x0   0x8008
    // --------------------- 0x80?? ---------------------
    // el     - Enabled the internal data register : off
    // ef     - Enabled the FIFO mode              : on
    // b<1:0> - Set up the band                    : 868 MHz
    // x<3:0> -                                    : 12.5 pF
    RF_ConfigSet.Val = CFG_SET_CMD_POR; // initialiser la variable globale avec la valeur interne pr?sente lors du Power On Reset
    radioAlphaTRX_Command(RF_ConfigSet.Val);
    RF_ConfigSet.bits.b7_el = 1;
    RF_ConfigSet.REGbits.SelectBand = BAND_868; // initialer module at 868 Mhz
    RF_ConfigSet.REGbits.SelectCrystalCapacitor = LOAD_C_12_0pF;  
    radioAlphaTRX_Command(RF_ConfigSet.Val);
    printf( "status: 0x%04X\r\n", RF_ConfigSet.Val);
    
    // Power Management Command
    RF_PowerManagement.Val = PWR_MGMT_CMD_POR;
    RF_PowerManagement.bits.b5_et = 1; // Enabling the Transmitter preloads the TX latch with 0xAAAA
    RF_PowerManagement.bits.b4_es = 1;
    RF_PowerManagement.bits.b3_ex = 1; 
    RF_PowerManagement.bits.b0_dc = 1;
    radioAlphaTRX_Command(RF_PowerManagement.Val);
    printf( "status: 0x%04X\r\n", RF_PowerManagement.Val);
}


// Envoyer une commande au module RF
uint16_t radioAlphaTRX_Command(uint16_t cmd_write) {
    WORD_VAL_T receiveData;
    WORD_VAL_T sendData;
    sendData.word = cmd_write;
    
    RF_nSEL_SetLow(); // debut de la communication SPI avec le Slave
    while (RF_nSEL_GetValue()) { } // a eviter je pense, mais pour l'instant je le garde 
    receiveData.byte.high = SPI1_Exchange8bit(sendData.byte.high);
    receiveData.byte.low = SPI1_Exchange8bit(sendData.byte.low);
    RF_nSEL_SetHigh(); // fin de la communication SPI avec le SLave 
    while (!RF_nSEL_GetValue()) { } // a eviter mais pour l'instant je le garde 
    
    return receiveData.word;
}