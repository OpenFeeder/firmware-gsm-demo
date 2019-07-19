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
#include "app.h"
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
/****************** Driver ALPHA TRX433S - Interface SPI **********************/
/***************************                ***********************************/
/*****************                                       **********************/

/**-------------------------->> V A R I A B L E S <<---------------------------*/
CFG_SET_CMD_VAL RF_ConfigSet; // Configuration Setting Command
PWR_MGMT_CMD_VAL RF_PowerManagement; // Power Management Command
FQ_SET_CMD_VAL RF_FrequencySet; // Frequency Setting Command
RX_CTRL_CMD_VAL RF_ReceiverControl; // Receiver Control Command
RX_FIFO_READ_CMD_VAL RF_FIFO_Read; // Receiver FIFO Read
FIFO_RST_MODE_CMD_VAL RF_FIFOandResetMode; // FIFO and Reset Mode Command
TX_CONF_CTRL_CMD_VAL RF_TX_ConfCtrlCmd;
DATA_RATE_CMD_VAL RF_DataRate;
STATUS_READ_VAL RF_StatusRead; // Status Read Command

/**-------------------------->> D E F I N I T I O N <<-------------------------*/



unsigned int FSK_Transceiver_ConfigFq(unsigned char freqSelected) {
    unsigned int FrequencySet;
    //    unsigned char FrequencySet;

    /* Mise a jour du registre du module RF */
    switch (freqSelected) {
        case FQ_001:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            //FrequencySet = 0x0070; // F= 112 --> F0= 430.28 MHz
            FrequencySet = 0x0068; // F= 104 --> F0= 860.52 MHz FIXME : Warning 0x70 change to 0x68 !!!
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0068; // F= 104 --> F0= 430.26 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0064; // F= 100 --> F0= 430.25 MHz
#endif
            break;

        case FQ_002:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0090; // F= 144 --> F0= 430.36 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0078; // F= 120 --> F0= 430.30 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x006C; // F= 108 --> F0= 430.27 MHz
#endif
            break;

        case FQ_003:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x00B0; // F= 176 --> F0= 430.44 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0088; // F= 136 --> F0= 430.34 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0074; // F= 116 --> F0= 430.29 MHz
#endif
            break;

        case FQ_004:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x00D0; // F= 208 --> F0= 430.52 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0098; // F= 152 --> F0= 430.38 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x007C; // F= 124 --> F0= 430.31 MHz
#endif
            break;

        case FQ_005:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x00F0; // F= 240 --> F0= 430.60 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x00A8; // F= 168 --> F0= 430.42 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0084; // F= 132 --> F0= 430.33 MHz
#endif
            break;

        case FQ_006:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0110; // F= 272 --> F0= 430.68 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x00B8; // F= 184 --> F0= 430.46 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x008C; // F= 140 --> F0= 430.35 MHz
#endif
            break;

        case FQ_007:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0130; // F= 304 --> F0= 430.76 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x00C8; // F= 200 --> F0= 430.50 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0094; // F= 148 --> F0= 430.37 MHz
#endif
            break;

        case FQ_008:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0150; // F= 336 --> F0= 430.84 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)// Restriction 17,5-20
            FrequencySet = 0x00D8; // F= 216 --> F0= 430.54 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x009C; // F= 156 --> F0= 430.39 MHz
#endif
            break;

        case FQ_009:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0170; // F= 368 --> F0= 430.92 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x00E8; // F= 232 --> F0= 430.58 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x00A4; // F= 164 --> F0= 430.41 MHz
#endif
            break;

        case FQ_010:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0190; // F= 400 --> F0= 431.00 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x00F8; // F= 248 --> F0= 430.62 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x00AC; // F= 172 --> F0= 430.43 MHz
#endif
            break;

        case FQ_011:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x01B0; // F= 432 --> F0= 431,08 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0108; // F= 264 --> F0= 430.66 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x00B4; // F= 180 --> F0= 430.45 MHz
#endif
            break;

        case FQ_012:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x01D0; // F= 464 --> F0= 431.16 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0118; // F= 280 --> F0= 430.70 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x00BC; // F= 188 --> F0= 430.47 MHz
#endif
            break;

        case FQ_013:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x01F0; // F= 496 --> F0= 431.24 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0128; // F= 296 --> F0= 430.74 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x00C4; // F= 196 --> F0= 430.49 MHz
#endif
            break;

        case FQ_014:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0210; // F= 528 --> F0= 431.32 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0138; // F= 312 --> F0= 430.78 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x00CC; // F= 204 --> F0= 430.51 MHz
#endif
            break;

        case FQ_015:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0230; // F= 560 --> F0= 431.40 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0148; // F= 328 --> F0= 430.82 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x00D4; // F= 212 --> F0= 430.53 MHz
#endif
            break;

        case FQ_016:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0250; // F= 592 --> F0= 431.48 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0158; // F= 344 --> F0= 430.86 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x00DC; // F= 220 --> F0= 430.55 MHz
#endif
            break;

        case FQ_017:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0270; // F= 624 --> F0= 431.56 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0168; // F= 360 --> F0= 430.90 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x00E4; // F= 228 --> F0= 430.57 MHz
#endif
            break;

        case FQ_018:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0290; // F= 656 --> F0= 431.64 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0178; // F= 376 --> F0= 430.94 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x00EC; // F= 236 --> F0= 430.59 MHz
#endif
            break;

        case FQ_019:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x02B0; // F= 688 --> F0= 431.72 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0188; // F= 392 --> F0= 430.98 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x00F4; // F= 244 --> F0= 430.61 MHz
#endif
            break;

        case FQ_020:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x02D0; // F= 720 --> F0= 431.80 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0198; // F= 408 --> F0= 431.02 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x00FC; // F= 252 --> F0= 430.63 MHz
#endif
            break;

        case FQ_021:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x02F0; // F= 752 --> F0= 431.88 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x01A8; // F= 424 --> F0= 431.06 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0104; // F= 260 --> F0= 430.65 MHz
#endif
            break;

        case FQ_022:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0310; // F= 784 --> F0= 431.96 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x01B8; // F= 440 --> F0= 431.10 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x010C; // F= 268 --> F0= 430.67 MHz
#endif
            break;

        case FQ_023:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0330; // F= 816 --> F0= 432.04 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x01C8; // F= 456 --> F0= 431.14 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0114; // F= 276 --> F0= 430.69 MHz
#endif
            break;

        case FQ_024:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0350; // F= 848 --> F0= 432.12 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x01D8; // F= 472 --> F0= 431.18 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x011C; // F= 284 --> F0= 430.71 MHz
#endif
            break;

        case FQ_025:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0370; // F= 880 --> F0= 432.20 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x01E8; // F= 488 --> F0= 431.22 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0124; // F= 292 --> F0= 430.73 MHz
#endif
            break;

        case FQ_026:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0390; // F= 912 --> F0= 432.28 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x01F8; // F= 504 --> F0= 431.26 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x012C; // F= 300 --> F0= 430.75 MHz
#endif
            break;

        case FQ_027:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x03B0; // F= 944 --> F0= 432.36 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0208; // F= 520 --> F0= 431.30 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0134; // F= 308 --> F0= 430.77 MHz
#endif
            break;

        case FQ_028:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x03D0; // F= 976 --> F0= 432.44 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0218; // F= 536 --> F0= 431.34 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x013C; // F= 316 --> F0= 430.79 MHz
#endif
            break;

        case FQ_029:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x03F0; // F= 1008 --> F0= 432.52 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0228; // F= 552 --> F0= 431.38 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0144; // F= 324 --> F0= 430.81 MHz
#endif
            break;

        case FQ_030:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0410; // F= 1040 --> F0= 432.60 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0238; // F= 568 --> F0= 431.42 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x014C; // F= 332 --> F0= 430.83 MHz
#endif
            break;

        case FQ_031:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0430; // F= 1072 --> F0= 432.68 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0248; // F= 548 --> F0= 431.46 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0154; // F= 340 --> F0= 430.85 MHz
#endif
            break;

        case FQ_032:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0450; // F= 1104 --> F0= 432.76 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0258; // F= 600 --> F0= 431.50 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x015C; // F= 348 --> F0= 430.87 MHz
#endif
            break;

        case FQ_033:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0470; // F= 1136 --> F0= 432.84 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0268; // F= 616 --> F0= 431.54 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0164; // F= 356 --> F0= 430.89 MHz
#endif
            break;

        case FQ_034:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0490; // F= 1168 --> F0= 432.92 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0278; // F= 632 --> F0= 431.58 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x016C; // F= 364 --> F0= 430.91 MHz
#endif
            break;

        case FQ_035:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x04B0; // F= 1200 --> F0= 433.00 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0288; // F= 348 --> F0= 431.62 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0174; // F= 372 --> F0= 430.93 MHz
#endif
            break;

        case FQ_036:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x04D0; // F= 1232 --> F0= 433.08 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0298; // F= 664 --> F0= 431.66 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x017C; // F= 380 --> F0= 430.95 MHz
#endif
            break;

        case FQ_037:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x04F0; // F= 1264 --> F0= 433.16 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x02A8; // F= 680 --> F0= 431.70 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0184; // F= 388 --> F0= 430.97 MHz
#endif
            break;

        case FQ_038:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0510; // F= 1296 --> F0= 433.24 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x02B8; // F= 696 --> F0= 431.74 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x018C; // F= 396 --> F0= 430.99 MHz
#endif
            break;

        case FQ_039:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0530; // F= 1328 --> F0= 433.32 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x02C8; // F= 712 --> F0= 431.78 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0194; // F= 404 --> F0= 431.01 MHz
#endif
            break;

        case FQ_040:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0550; // F= 1360 --> F0= 433.40 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x02D8; // F= 728 --> F0= 431.82 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x019C; // F= 412 --> F0= 431.03 MHz
#endif
            break;

        case FQ_041:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0570; // F= 1392 --> F0= 433,48 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x02E8; // F= 744 --> F0= 433.42 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x01A4; // F= 420 --> F0= 431.83 MHz
#endif
            break;

        case FQ_042:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0590; // F= 1424 --> F0= 433,56 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x02F8; // F= 760 --> F0= 431,90 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x01AC; // F= 428 --> F0= 431,07 MHz
#endif
            break;

        case FQ_043:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x05B0; // F= 1456 --> F0= 433,64 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0308; // F= 776 --> F0= 431,94 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x01B4; // F= 436 --> F0= 431,09 MHz
#endif
            break;

        case FQ_044:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x05D0; // F= 1488 --> F0= 433,72 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0318; // F= 792 --> F0= 431,98 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x01BC; // F= 444 --> F0= 431,11 MHz
#endif
            break;

        case FQ_045:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x05F0; // F= 1520 --> F0= 433,80 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0328; // F= 808 --> F0= 432,02 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x01C4; // F= 452 --> F0= 431,13 MHz
#endif
            break;

        case FQ_046:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0610; // F= 1552 --> F0= 433,88 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0338; // F= 824 --> F0= 432,06 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x01CC; // F= 460 --> F0= 431,15 MHz
#endif
            break;

        case FQ_047:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0630; // F= 1584 --> F0= 433,96 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0348; // F= 840 --> F0= 432,10 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x01D4; // F= 468 --> F0= 431,17 MHz
#endif
            break;

        case FQ_048:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0650; // F= 1616 --> F0= 434,04 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0358; // F= 856 --> F0= 432,14 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x01DC; // F= 476 --> F0= 431,19 MHz
#endif
            break;

        case FQ_049:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0670; // F= 1648 --> F0= 434,12 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0368; // F= 872 --> F0= 432,18 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x01E4; // F= 484 --> F0= 431,21 MHz
#endif
            break;

        case FQ_050:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0690; // F= 1680 --> F0= 434,20 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0378; // F= 888 --> F0= 432,22 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x01EC; // F= 492 --> F0= 431,23 MHz
#endif
            break;

        case FQ_051:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x06B0; // F= 1712 --> F0= 434,28 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0388; // F= 904 --> F0= 432,26 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x01F4; // F= 500 --> F0= 431,25 MHz
#endif
            break;

        case FQ_052:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x06D0; // F= 1744 --> F0= 434,36 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0398; // F= 920 --> F0= 432,30 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x01FC; // F= 508 --> F0= 431,27 MHz
#endif
            break;

        case FQ_053:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x06F0; // F= 1776 --> F0= 434,44 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x03A8; // F= 936 --> F0= 432,34 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0204; // F= 516 --> F0= 431,29 MHz
#endif
            break;

        case FQ_054:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0710; // F= 1808 --> F0= 434,52 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x03B8; // F= 952 --> F0= 432,38 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x020C; // F= 524 --> F0= 431,31 MHz
#endif
            break;

        case FQ_055:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0730; // F= 1840 --> F0= 434,60 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x03C8; // F= 968 --> F0= 432,42 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0214; // F= 532 --> F0= 431,33 MHz
#endif
            break;

        case FQ_056:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0750; // F= 1872 --> F0= 434,68 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x03D8; // F= 984 --> F0= 432,46 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x021C; // F= 540 --> F0= 431,35 MHz
#endif
            break;

        case FQ_057:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0770; // F= 1904 --> F0= 434,76 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x03E8; // F= 1000 --> F0= 432,50 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0224; // F= 548 --> F0= 431,37 MHz
#endif
            break;

        case FQ_058:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0790; // F= 1936 --> F0= 434,84 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x03F8; // F= 1016 --> F0= 432,54 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x022C; // F= 556 --> F0= 431,39 MHz
#endif
            break;

        case FQ_059:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x07B0; // F= 1968 --> F0= 434,92 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0408; // F= 1032 --> F0= 432,58 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0234; // F= 564 --> F0= 431,41 MHz
#endif
            break;

        case FQ_060:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x07D0; // F= 2000 --> F0= 435,00 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0418; // F= 1048 --> F0= 432,62 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x023C; // F= 572 --> F0= 431,43 MHz
#endif
            break;

        case FQ_061:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x07F0; // F= 2032 --> F0= 435,08 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0428; // F= 1064 --> F0= 432,66 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0244; // F= 580 --> F0= 431,45 MHz
#endif
            break;

        case FQ_062:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0810; // F= 2064 --> F0= 435,16 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0438; // F= 1080 --> F0= 432,70 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x024C; // F= 588 --> F0= 431,47 MHz
#endif
            break;

        case FQ_063:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0830; // F= 2096 --> F0= 435,24 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0448; // F= 1096 --> F0= 432,74 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0254; // F= 596 --> F0= 431,49 MHz
#endif
            break;

        case FQ_064:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0850; // F= 2128 --> F0= 435,32 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0458; // F= 1112 --> F0= 432,78 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x025C; // F= 604 --> F0= 431,51 MHz
#endif
            break;

        case FQ_065:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0870; // F= 2160 --> F0= 435,40 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0468; // F= 1128 --> F0= 432,82 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0264; // F= 612 --> F0= 431,53 MHz
#endif
            break;

        case FQ_066:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0890; // F= 2192 --> F0= 435,48 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0478; // F= 1144 --> F0= 432,86 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x026C; // F= 620 --> F0= 431,55 MHz
#endif
            break;

        case FQ_067:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x08B0; // F= 2224 --> F0= 435,56 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0488; // F= 1160 --> F0= 432,90 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0274; // F= 628 --> F0= 431,57 MHz
#endif
            break;

        case FQ_068:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x08D0; // F= 2256 --> F0= 435,64 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0498; // F= 1176 --> F0= 432,94 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x027C; // F= 636 --> F0= 431,59 MHz
#endif
            break;

        case FQ_069:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x08F0; // F= 2288 --> F0= 435,72 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x04A8; // F= 1192 --> F0= 432,98 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0284; // F= 644 --> F0= 431,61 MHz
#endif
            break;

        case FQ_070:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0910; // F= 2320 --> F0= 435,80 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x04B8; // F= 1208 --> F0= 433,02 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x028C; // F= 652 --> F0= 431,63 MHz
#endif
            break;

        case FQ_071:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0930; // F= 2352 --> F0= 435,88 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x04C8; // F= 1224 --> F0= 433,06 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0294; // F= 660 --> F0= 431,65 MHz
#endif
            break;

        case FQ_072:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0950; // F= 2384 --> F0= 435,96 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x04D8; // F= 1240 --> F0= 433,10 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x029C; // F= 668 --> F0= 431,67 MHz
#endif
            break;

        case FQ_073:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0970; // F= 2416 --> F0= 436,04 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x04E8; // F= 1256 --> F0= 433,14 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x02A4; // F= 676 --> F0= 431,69 MHz
#endif
            break;

        case FQ_074:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0990; // F= 2448 --> F0= 436,12 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x04F8; // F= 1272 --> F0= 433,18 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x02AC; // F= 684 --> F0= 431,71 MHz
#endif
            break;

        case FQ_075:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x09B0; // F= 2480 --> F0= 436,20 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0508; // F= 1288 --> F0= 433,22 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x02B4; // F= 692 --> F0= 431,73 MHz
#endif
            break;

        case FQ_076:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x09D0; // F= 2512 --> F0= 436,28 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0518; // F= 1304 --> F0= 433,26 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x02BC; // F= 700 --> F0= 431,75 MHz
#endif
            break;

        case FQ_077:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x09F0; // F= 2544 --> F0= 436,36 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0528; // F= 1320 --> F0= 433,30 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x02C4; // F= 708 --> F0= 431,77 MHz
#endif
            break;

        case FQ_078:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0A10; // F= 2576 --> F0= 436,44 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0538; // F= 1336 --> F0= 433,34 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x02CC; // F= 716 --> F0= 431,79 MHz
#endif
            break;

        case FQ_079:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0A30; // F= 2608 --> F0= 436,52 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0548; // F= 1352 --> F0= 433,38 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x02D4; // F= 724 --> F0= 431,81 MHz
#endif
            break;

        case FQ_080:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0A50; // F= 2640 --> F0= 436.60 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0558; // F= 1368 --> F0= 433.42 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x02DC; // F= 732 --> F0= 431.83 MHz
#endif
            break;

        case FQ_081:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0A70; // F= 2672 --> F0= 436.68 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0568; // F= 1384 --> F0= 433.46 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x02E4; // F= 740 --> F0= 431.85 MHz
#endif
            break;

        case FQ_082:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0A90; // F= 2704 --> F0= 436.76 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0578; // F= 1400 --> F0= 433.50 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x02EC; // F= 748 --> F0= 431.87 MHz
#endif
            break;

        case FQ_083:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0AB0; // F= 2736 --> F0= 436.84 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0588; // F= 1416 --> F0= 433.54 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x02F4; // F= 756 --> F0= 431.89 MHz
#endif
            break;

        case FQ_084:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0AD0; // F= 2768 --> F0= 436.92 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0598; // F= 1432 --> F0= 433.58 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x02FC; // F= 764 --> F0= 431.91 MHz
#endif
            break;

        case FQ_085:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0AF0; // F= 2800 --> F0= 437.00 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x05A8; // F= 1448 --> F0= 433.62 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0304; // F= 772 --> F0= 431.93 MHz
#endif
            break;

        case FQ_086:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0B10; // F= 2832 --> F0= 437.08 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x05B8; // F= 1464 --> F0= 433.66 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x030C; // F= 780 --> F0= 431.95 MHz
#endif
            break;

        case FQ_087:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0B30; // F= 2864 --> F0= 436.16 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x05C8; // F= 1480 --> F0= 433.70 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0314; // F= 788 --> F0= 431.97 MHz
#endif
            break;

        case FQ_088:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0B50; // F= 2896 --> F0= 437.24 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x05D8; // F= 1496 --> F0= 433.74 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x031C; // F= 796 --> F0= 431.99 MHz
#endif
            break;

        case FQ_089:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0B70; // F= 2928 --> F0= 437.32 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x05E8; // F= 1512 --> F0= 433.78 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0324; // F= 804 --> F0= 432.01 MHz
#endif
            break;

        case FQ_090:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0B90; // F= 2960 --> F0= 437.40 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x05F8; // F= 1528 --> F0= 433.82 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x032C; // F= 812 --> F0= 432.03 MHz
#endif
            break;

        case FQ_091:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0BB0; // F= 2992 --> F0= 437.48 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0608; // F= 1544 --> F0= 433.86 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0334; // F= 820 --> F0= 432.05 MHz
#endif
            break;

        case FQ_092:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0BD0; // F= 3024 --> F0= 437.56 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0618; // F= 1560 --> F0= 433.90 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x033C; // F= 828 --> F0= 432.07 MHz
#endif
            break;

        case FQ_093:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0BF0; // F= 3056 --> F0= 437.64 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0628; // F= 1576 --> F0= 433.94 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0344; // F= 836 --> F0= 432.09 MHz
#endif
            break;

        case FQ_094:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0C10; // F= 3088 --> F0= 437.72 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0638; // F= 1592 --> F0= 433.98 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x034C; // F= 844 --> F0= 432.11 MHz
#endif
            break;

        case FQ_095:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0C30; // F= 3120 --> F0= 437.80 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0648; // F= 1608 --> F0= 434.02 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0354; // F= 852 --> F0= 431.13 MHz
#endif
            break;

        case FQ_096:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0C50; // F= 3152 --> F0= 437.88 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0658; // F= 1624 --> F0= 434.06 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x035C; // F= 860 --> F0= 432.15 MHz
#endif
            break;

        case FQ_097:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0C70; // F= 3184 --> F0= 437.96 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0668; // F= 1640 --> F0= 434.10 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0364; // F= 868 --> F0= 432.17 MHz
#endif
            break;

        case FQ_098:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0C90; // F= 3216 --> F0= 438.04 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0678; // F= 1656 --> F0= 434.14 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x036C; // F= 876 --> F0= 432.19 MHz
#endif
            break;

        case FQ_099:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0CB0; // F= 3248 --> F0= 438.12 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0688; // F= 1672 --> F0= 434.18 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0374; // F= 884 --> F0= 432.21 MHz
#endif
            break;

        case FQ_100:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0CD0; // F= 3280 --> F0= 438.20 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0698; // F= 1688 --> F0= 434.22 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x037C; // F= 892 --> F0= 432.23 MHz
#endif
            break;

        case FQ_101:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0CF0; // F= 3312 --> F0= 438.28 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x06A8; // F= 1704 --> F0= 434.26 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0384; // F= 900 --> F0= 432.25 MHz
#endif
            break;

        case FQ_102:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0D10; // F= 3344 --> F0= 438.36 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x06B8; // F= 1720 --> F0= 434.30 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x038C; // F= 908 --> F0= 432.27 MHz
#endif
            break;

        case FQ_103:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0D30; // F= 3376 --> F0= 438.44 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x06C8; // F= 1736 --> F0= 434.34 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0394; // F= 916 --> F0= 432.29 MHz
#endif
            break;

        case FQ_104:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0D50; // F= 3408 --> F0= 438.52 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x06D8; // F= 1752 --> F0= 434.38 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x039C; // F= 924 --> F0= 432.31 MHz
#endif
            break;

        case FQ_105:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0D70; // F= 3440 --> F0= 438.60 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x06E8; // F= 1768 --> F0= 434.42 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x03A4; // F= 932 --> F0= 423.33 MHz
#endif
            break;

        case FQ_106:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0D90; // F= 3472 --> F0= 438.68 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x06F8; // F= 1784 --> F0= 434.46 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x03AC; // F= 940 --> F0= 432.35 MHz
#endif
            break;

        case FQ_107:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0DB0; // F= 3504 --> F0= 438.76 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0708; // F= 1800 --> F0= 434.50 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x03B4; // F= 948 --> F0= 432.37 MHz
#endif
            break;

        case FQ_108:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0DD0; // F= 3536 --> F0= 438.84 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0718; // F= 1816 --> F0= 434.54 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x03BC; // F= 956 --> F0= 432.39 MHz
#endif
            break;

        case FQ_109:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0DF0; // F= 3568 --> F0= 438.92 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0728; // F= 1832 --> F0= 434.58 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x03C4; // F= 964 --> F0= 432.41 MHz
#endif
            break;

        case FQ_110:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0E10; // F= 3600 --> F0= 439.00 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0738; // F= 1848 --> F0= 434.62 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x03CC; // F= 972 --> F0= 432.43 MHz
#endif
            break;

        case FQ_111:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0E30; // F= 3632 --> F0= 439.08 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0748; // F= 1864 --> F0= 434.66 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x03D4; // F= 980 --> F0= 432.45 MHz
#endif
            break;

        case FQ_112:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0E50; // F= 3664 --> F0= 439.16 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0758; // F= 1880 --> F0= 434.70 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x03DC; // F= 988 --> F0= 432.47 MHz
#endif
            break;

        case FQ_113:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0E70; // F= 3696 --> F0= 439.24 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0768; // F= 1896 --> F0= 434.74 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x3E4; // F= 996 --> F0= 432.49 MHz
#endif
            break;

        case FQ_114:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0E90; // F= 3278 --> F0= 439.32 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0778; // F= 1912 --> F0= 434.78 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x03EC; // F= 1004 --> F0= 432.51 MHz
#endif
            break;

        case FQ_115:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0EB0; // F= 3760 --> F0= 439.40 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0788; // F= 1928 --> F0= 434.82 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x03F4; // F= 1012 --> F0= 432.53 MHz
#endif
            break;

        case FQ_116:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0ED0; // F= 3792 --> F0= 439.48 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x0798; // F= 1944 --> F0= 434.86 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x03FC; // F= 1020 --> F0= 432.55 MHz
#endif
            break;

        case FQ_117:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0EF0; // F= 3824 --> F0= 439.56 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x07A8; // F= 1960 --> F0= 434.90 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0404; // F= 1028 --> F0= 432.57 MHz
#endif
            break;

        case FQ_118:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0F10; // F= 3856 --> F0= 439.64 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x07B8; // F= 1976 --> F0= 434.94 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x040C; // F= 1036 --> F0= 432.59 MHz
#endif
            break;

        case FQ_119:
#if defined(FQ_RESTRICTION_LOW)        // Restriction 37,5-40
            FrequencySet = 0x0F30; // F= 3888 --> F0= 439.72 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)   // Restriction 17,5-20
            FrequencySet = 0x07C8; // F= 1992 --> F0= 434.98 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0414; // F= 1044 --> F0= 432.61 MHz
#endif
            break;

#if defined(FQ_RESTRICTION_MEDIUM) || defined(FQ_RESTRICTION_HIGH)

        case FQ_120:
#if defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x07D8; // F= 2008 --> F0= 435.02 MHz
#elif defined(FQ_RESTRICTION_HIGH)    // Restriction 7,5-10
            FrequencySet = 0x041C; // F= 1052 --> F0= 432.63 MHz
#endif
            break;

        case FQ_121:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x07E8; // F= 2024 --> F0= 435.06 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0424; // F= 1060 --> F0= 432.65 MHz
#endif
            break;

        case FQ_122:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x07F8; // F= 2040 --> F0= 435.10 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x042C; // F= 1068 --> F0= 432.67 MHz
#endif
            break;

        case FQ_123:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0808; // F= 2056 --> F0= 435.14 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0434; // F= 1076 --> F0= 432.69 MHz
#endif
            break;

        case FQ_124:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0818; // F= 2072 --> F0= 435.18 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x043C; // F= 1084 --> F0= 432.71 MHz
#endif
            break;

        case FQ_125:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0828; // F= 2088 --> F0= 435.22 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0444; // F= 1092 --> F0= 432.73 MHz
#endif
            break;

        case FQ_126:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0838; // F= 2104 --> F0= 435.26 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x044C; // F= 1100 --> F0= 432.75 MHz
#endif
            break;

        case FQ_127:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0848; // F= 2120 --> F0= 435.30 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0454; // F= 1108 --> F0= 432.77 MHz
#endif
            break;

        case FQ_128:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0858; // F= 2136 --> F0= 435.34 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x045C; // F= 1116 --> F0= 432,79 MHz
#endif
            break;

        case FQ_129:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0868; // F= 2152 --> F0= 435,38 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0464; // F= 1124 --> F0= 432,81 MHz
#endif
            break;

        case FQ_130:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0878; // F= 2168 --> F0= 435,42 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x046C; // F= 1132 --> F0= 432,83 MHz
#endif
            break;

        case FQ_131:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0888; // F= 2184 --> F0= 435,46 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0474; // F= 1140 --> F0= 432,85 MHz
#endif
            break;

        case FQ_132:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0898; // F= 2200 --> F0= 435,50 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x047C; // F= 1148 --> F0= 432,87 MHz
#endif
            break;

        case FQ_133:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x08A8; // F= 2216 --> F0= 435,54 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0484; // F= 1156 --> F0= 432,89 MHz
#endif
            break;

        case FQ_134:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x08B8; // F= 2232 --> F0= 435,58 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x048C; // F= 1164 --> F0= 432,91 MHz
#endif
            break;

        case FQ_135:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x08C8; // F= 2248 --> F0= 435,62 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0494; // F= 1172 --> F0= 432,93 MHz
#endif
            break;

        case FQ_136:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x08D8; // F= 2264 --> F0= 435,66 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x049C; // F= 1180 --> F0= 432,95 MHz
#endif
            break;

        case FQ_137:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x08E8; // F= 2280 --> F0= 435,70 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x04A4; // F= 1188 --> F0= 432,97 MHz
#endif
            break;

        case FQ_138:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x08F8; // F= 2296 --> F0= 435,74 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x04AC; // F= 1196 --> F0= 432,99 MHz
#endif
            break;

        case FQ_139:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0908; // F= 2312 --> F0= 435,78 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x04B4; // F= 1204 --> F0= 433,01 MHz
#endif
            break;

        case FQ_140:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0918; // F= 2328 --> F0= 435,82 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x04BC; // F= 1212 --> F0= 433,03 MHz
#endif
            break;

        case FQ_141:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0928; // F= 2344 --> F0= 435,86 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x04C4; // F= 1220 --> F0= 433,05 MHz
#endif
            break;

        case FQ_142:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0938; // F= 2360 --> F0= 435.90 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x04CC; // F= 1228 --> F0= 433,07 MHz
#endif
            break;

        case FQ_143:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0948; // F= 2376 --> F0= 435.94 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x04D4; // F= 1236 --> F0= 433.09 MHz
#endif
            break;

        case FQ_144:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0958; // F= 2392 --> F0= 435,98 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x04DC; // F= 1244 --> F0= 433,11 MHz
#endif
            break;

        case FQ_145:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0968; // F= 2408 --> F0= 436,02 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x04E4; // F= 1252 --> F0= 433,13 MHz
#endif
            break;

        case FQ_146:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0978; // F= 2424 --> F0= 436,06 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x04EC; // F= 1260 --> F0= 433,15 MHz
#endif
            break;

        case FQ_147:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0988; // F= 2440 --> F0= 436,10 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x04F4; // F= 1268 --> F0= 433,17 MHz
#endif
            break;

        case FQ_148:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0998; // F= 2456 --> F0= 436.14 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x04FC; // F= 1276 --> F0= 433,19 MHz
#endif
            break;

        case FQ_149:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x09A8; // F= 2472 --> F0= 436.18 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0504; // F= 1284 --> F0= 433.21 MHz
#endif
            break;

        case FQ_150:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x09B8; // F= 2488 --> F0= 436.22 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x050C; // F= 1292 --> F0= 433.23 MHz
#endif
            break;

        case FQ_151:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x09C8; // F= 2504 --> F0= 436.26 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0514; // F= 1300 --> F0= 433.25 MHz
#endif
            break;

        case FQ_152:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x09D8; // F= 2520 --> F0= 436.30 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x051C; // F= 1308 --> F0= 433.27 MHz
#endif
            break;

        case FQ_153:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x09E8; // F= 2536 --> F0= 436.34 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0524; // F= 1316 --> F0= 433.29 MHz
#endif
            break;

        case FQ_154:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x09F8; // F= 2552 --> F0= 436.38 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x052C; // F= 1324 --> F0= 433.31 MHz
#endif
            break;

        case FQ_155:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0A08; // F= 2568 --> F0= 436.42 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0534; // F= 1332 --> F0= 433.33 MHz
#endif
            break;

        case FQ_156:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0A18; // F= 2584 --> F0= 436.46 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x053C; // F= 1340 --> F0= 433.35 MHz
#endif
            break;

        case FQ_157:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0A28; // F= 2600 --> F0= 436.50 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0544; // F= 1348 --> F0= 433.37 MHz
#endif
            break;

        case FQ_158:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0A38; // F= 2616 --> F0= 436.54 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x054C; // F= 1356 --> F0= 433.39 MHz
#endif
            break;

        case FQ_159:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0A48; // F= 2632 --> F0= 436.58 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0554; // F= 1364 --> F0= 433.41 MHz
#endif
            break;

        case FQ_160:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0A58; // F= 2648 --> F0= 436.62 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x055C; // F= 1372 --> F0= 433.43 MHz
#endif
            break;

        case FQ_161:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0A68; // F= 2664 --> F0= 433.66 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0564; // F= 1380 --> F0= 433.45 MHz
#endif
            break;

        case FQ_162:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0A78; // F= 2680 --> F0= 436.70 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x056C; // F= 1388 --> F0= 433.47 MHz
#endif
            break;

        case FQ_163:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0A88; // F= 2696 --> F0= 436.74 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0574; // F= 1396 --> F0= 433.49 MHz
#endif
            break;

        case FQ_164:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0A98; // F= 2712 --> F0= 436.78 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x057C; // F= 1404 --> F0= 433.51 MHz
#endif
            break;

        case FQ_165:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0AA8; // F= 2728 --> F0= 436.82 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0584; // F= 1412 --> F0= 433.53 MHz
#endif
            break;

        case FQ_166:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0AB8; // F= 2744 --> F0= 436.86 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x058C; // F= 1420 --> F0= 433.55 MHz
#endif
            break;

        case FQ_167:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0AC8; // F= 2760 --> F0= 436.90 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0594; // F= 1428 --> F0= 433.57 MHz
#endif
            break;

        case FQ_168:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0AD8; // F= 2776 --> F0= 436.94 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x059C; // F= 1436 --> F0= 433.59 MHz
#endif
            break;

        case FQ_169:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0AE8; // F= 2792 --> F0= 436.98 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x05A4; // F= 1444 --> F0= 433.61 MHz
#endif
            break;

        case FQ_170:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0AF8; // F= 2808 --> F0= 437.02 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x05AC; // F= 1452 --> F0= 433.63 MHz
#endif
            break;

        case FQ_171:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0B08; // F= 2824 --> F0= 437.06 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x05B4; // F= 1460 --> F0= 433.65 MHz
#endif
            break;

        case FQ_172:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0B18; // F= 2840 --> F0= 437.10 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x05BC; // F= 1468 --> F0= 433.67 MHz
#endif
            break;

        case FQ_173:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0B28; // F= 2856 --> F0= 437.14 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x05C4; // F= 1476 --> F0= 433.69 MHz
#endif
            break;

        case FQ_174:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0B38; // F= 2872 --> F0= 437.18 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x05CC; // F= 1484 --> F0= 433.71 MHz
#endif
            break;

        case FQ_175:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0B48; // F= 2888 --> F0= 437.22 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x05D4; // F= 1492 --> F0= 433.73 MHz
#endif
            break;

        case FQ_176:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0B58; // F= 2904 --> F0= 437.26 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x05DC; // F= 1500 --> F0= 433.75 MHz
#endif
            break;

        case FQ_177:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0B68; // F= 2920 --> F0= 437.30 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x05E4; // F= 1508 --> F0= 433.77 MHz
#endif
            break;

        case FQ_178:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0B78; // F= 2936 --> F0= 437.34 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x05EC; // F= 1516 --> F0= 433.79 MHz
#endif
            break;

        case FQ_179:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0B88; // F= 2952 --> F0= 437.38 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x05F4; // F= 1524 --> F0= 433.81 MHz
#endif
            break;

        case FQ_180:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0B98; // F= 2968 --> F0= 437.42 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x05FC; // F= 1532 --> F0= 433.83 MHz
#endif
            break;

        case FQ_181:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0BA8; // F= 2984 --> F0= 437.46 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0604; // F= 1540 --> F0= 433.85 MHz
#endif
            break;

        case FQ_182:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0BB8; // F= 3000 --> F0= 437.50 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x060C; // F= 1548 --> F0= 433.87 MHz
#endif
            break;

        case FQ_183:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0BC8; // F= 3016 --> F0= 437.54 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0614; // F= 1556 --> F0= 433.89 MHz
#endif
            break;

        case FQ_184:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0BD8; // F= 3032 --> F0= 437.58 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x061C; // F= 1564 --> F0= 433.91 MHz
#endif
            break;

        case FQ_185:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0BE8; // F= 3048 --> F0= 437.62 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0624; // F= 1572 --> F0= 433.93 MHz
#endif
            break;

        case FQ_186:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0BF8; // F= 3064 --> F0= 437.66 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x062C; // F= 1580 --> F0= 433.95 MHz
#endif
            break;

        case FQ_187:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0C08; // F= 3080 --> F0= 437.70 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0634; // F= 1588 --> F0= 433.97 MHz
#endif
            break;

        case FQ_188:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0C18; // F= 3096 --> F0= 437.74 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x063C; // F= 1596 --> F0= 433.99 MHz
#endif
            break;

        case FQ_189:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0C28; // F= 3112 --> F0= 437.78 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0644; // F= 1604 --> F0= 434.01 MHz
#endif
            break;

        case FQ_190:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0C38; // F= 3128 --> F0= 437.82 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x064C; // F= 1612 --> F0= 434.03 MHz
#endif
            break;

        case FQ_191:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0C48; // F= 3144 --> F0= 437.86 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0654; // F= 1620 --> F0= 434.05 MHz
#endif
            break;

        case FQ_192:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0C58; // F= 3160 --> F0= 437.90 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x065C; // F= 1628 --> F0= 434.07 MHz
#endif
            break;

        case FQ_193:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0C68; // F= 3176 --> F0= 437.94 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0664; // F= 1636 --> F0= 434.09 MHz
#endif
            break;

        case FQ_194:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0C78; // F= 3192 --> F0= 437.98 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x066C; // F= 1644 --> F0= 434.11 MHz
#endif
            break;

        case FQ_195:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0C88; // F= 3208 --> F0= 438.02 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0674; // F= 1652 --> F0= 434.13 MHz
#endif
            break;

        case FQ_196:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0C98; // F= 3224 --> F0= 438.06 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x067C; // F= 1660 --> F0= 434.15 MHz
#endif
            break;

        case FQ_197:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0CA8; // F= 3240 --> F0= 438.10 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0684; // F= 1668 --> F0= 434.17 MHz
#endif
            break;

        case FQ_198:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0CB8; // F= 3256 --> F0= 438.14 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x068C; // F= 1676 --> F0= 434.19 MHz
#endif
            break;

        case FQ_199:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0CC8; // F= 3272 --> F0= 438.18 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0694; // F= 1684 --> F0= 434.21 MHz
#endif
            break;

        case FQ_200:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0CD8; // F= 3288 --> F0= 438.22 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x069C; // F= 1692 --> F0= 434.23 MHz
#endif
            break;

        case FQ_201:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0CE8; // F= 3304 --> F0= 438.26 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x06A4; // F= 1700 --> F0= 434.25 MHz
#endif
            break;

        case FQ_202:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0CF8; // F= 3320 --> F0= 438.30 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x06AC; // F= 1708 --> F0= 434.27 MHz
#endif
            break;

        case FQ_203:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0D08; // F= 3336 --> F0= 438.34 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x06B4; // F= 1716 --> F0= 434.29 MHz
#endif
            break;

        case FQ_204:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0D18; // F= 3352 --> F0= 438.38 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x06BC; // F= 1724 --> F0= 434.31 MHz
#endif
            break;

        case FQ_205:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0D28; // F= 3368 --> F0= 438.42 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x06C4; // F= 1732 --> F0= 434.33 MHz
#endif
            break;

        case FQ_206:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0D38; // F= 3384 --> F0= 438.46 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x06CC; // F= 1740 --> F0= 434.35 MHz
#endif
            break;

        case FQ_207:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0D48; // F= 3400 --> F0= 438.50 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x06D4; // F= 1748 --> F0= 434.37 MHz
#endif
            break;

        case FQ_208:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0D58; // F= 3416 --> F0= 438.54 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x06DC; // F= 1756 --> F0= 434.39 MHz
#endif
            break;

        case FQ_209:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0D68; // F= 3432 --> F0= 438.58 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x06E4; // F= 1764 --> F0= 434.41 MHz
#endif
            break;

        case FQ_210:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0D78; // F= 3448 --> F0= 438.62 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x06EC; // F= 1772 --> F0= 434.43 MHz
#endif
            break;

        case FQ_211:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0D88; // F= 3464 --> F0= 438.66 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x06F4; // F= 1780 --> F0= 434.45 MHz
#endif
            break;

        case FQ_212:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0D98; // F= 3480 --> F0= 438.70 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x06FC; // F= 1788 --> F0= 434.47 MHz
#endif
            break;

        case FQ_213:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0DA8; // F= 3496 --> F0= 438.74 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0704; // F= 1796 --> F0= 434.49 MHz
#endif
            break;

        case FQ_214:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0DB8; // F= 3512 --> F0= 438.78 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x070C; // F= 1804 --> F0= 434.51 MHz
#endif
            break;

        case FQ_215:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0DC8; // F= 3528 --> F0= 438.82 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0714; // F= 1812 --> F0= 434.53 MHz
#endif
            break;

        case FQ_216:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0DD8; // F= 3544 --> F0= 438.86 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x071C; // F= 1820 --> F0= 434.55 MHz
#endif
            break;

        case FQ_217:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0DE8; // F= 3560 --> F0= 438.90 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0724; // F= 1828 --> F0= 434.57 MHz
#endif
            break;

        case FQ_218:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0DF8; // F= 3576 --> F0= 434.94 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x072C; // F= 1836 --> F0= 434.59 MHz
#endif
            break;

        case FQ_219:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0E08; // F= 3592 --> F0= 438.98 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0734; // F= 1844 --> F0= 434.61 MHz
#endif
            break;

        case FQ_220:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0E18; // F= 3608 --> F0= 439.02 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x073C; // F= 1852 --> F0= 434.63 MHz
#endif
            break;

        case FQ_221:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0E28; // F= 3624 --> F0= 439.06 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0744; // F= 1860 --> F0= 434.65 MHz
#endif
            break;

        case FQ_222:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0E38; // F= 3640 --> F0= 439.10 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x074C; // F= 1868 --> F0= 434.67 MHz
#endif
            break;

        case FQ_223:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0E48; // F= 3656 --> F0= 439.14 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0754; // F= 1876 --> F0= 434.69 MHz
#endif
            break;

        case FQ_224:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0E58; // F= 3672 --> F0= 439.18 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x075C; // F= 1884 --> F0= 434.71 MHz
#endif
            break;

        case FQ_225:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0E68; // F= 3688 --> F0= 439.22 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0764; // F= 1892 --> F0= 434.73 MHz
#endif
            break;

        case FQ_226:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0E78; // F= 3704 --> F0= 439.26 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x076C; // F= 1900 --> F0= 434.75 MHz
#endif
            break;

        case FQ_227:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0E88; // F= 3720 --> F0= 439.30 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0774; // F= 1908 --> F0= 434.77 MHz
#endif
            break;

        case FQ_228:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0E98; // F= 3736 --> F0= 439.34 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x077C; // F= 1916 --> F0= 434.79 MHz
#endif
            break;

        case FQ_229:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0EA8; // F= 3752 --> F0= 439.38 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0784; // F= 1924 --> F0= 434.81 MHz
#endif
            break;

        case FQ_230:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0EB8; // F= 3768 --> F0= 439.42 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x078C; // F= 1932 --> F0= 434.83 MHz
#endif
            break;

        case FQ_231:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0EC8; // F= 3784 --> F0= 439.46 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x0794; // F= 1940 --> F0= 434.85 MHz
#endif
            break;

        case FQ_232:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0ED8; // F= 3800 --> F0= 439.50 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x079C; // F= 1948 --> F0= 434.87 MHz
#endif
            break;

        case FQ_233:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0EE8; // F= 3816 --> F0= 439.54 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x07A4; // F= 1956 --> F0= 434.89 MHz
#endif
            break;

        case FQ_234:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0EF8; // F= 3832 --> F0= 439.58 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x07AC; // F= 1964 --> F0= 434.91 MHz
#endif
            break;

        case FQ_235:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0F08; // F= 3848 --> F0= 439.62 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x07B4; // F= 1972 --> F0= 434.93 MHz
#endif
            break;

        case FQ_236:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0F18; // F= 3864 --> F0= 439.66 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x07BC; // F= 1980 --> F0= 434.95 MHz
#endif
            break;

        case FQ_237:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0F28; // F= 3880 --> F0= 439.70 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x07C4; // F= 1988 --> F0= 434.97 MHz
#endif
            break;

        case FQ_238:
#if defined(FQ_RESTRICTION_MEDIUM)     // Restriction 17,5-20
            FrequencySet = 0x0F38; // F= 3896 --> F0= 439.74 MHz
#elif defined(FQ_RESTRICTION_HIGH)     // Restriction 7,5-10
            FrequencySet = 0x07CC; // F= 1996 --> F0= 434.99 MHz
#endif
            break;


#endif

#if defined(FQ_RESTRICTION_HIGH)

        case FQ_239:
            FrequencySet = 0x07D4; // F= 2004 --> F0= 435.01 MHz
            break;

        case FQ_240:
            FrequencySet = 0x07DC; // F= 2012 --> F0= 435.03 MHz
            break;

        case FQ_241:
            FrequencySet = 0x07E4; // F= 2020 --> F0= 435.05 MHz
            break;

        case FQ_242:
            FrequencySet = 0x07EC; // F= 2028 --> F0= 435.07 MHz
            break;

        case FQ_243:
            FrequencySet = 0x07F4; // F= 2036 --> F0= 435.09 MHz
            break;

        case FQ_244:
            FrequencySet = 0x07FC; // F= 2044 --> F0= 435.11 MHz
            break;

        case FQ_245:
            FrequencySet = 0x0804; // F= 2052 --> F0= 435.13 MHz
            break;

        case FQ_246:
            FrequencySet = 0x080C; // F= 2060 --> F0= 435.15 MHz
            break;

        case FQ_247:
            FrequencySet = 0x0814; // F= 2068 --> F0= 435.17 MHz
            break;

        case FQ_248:
            FrequencySet = 0x081C; // F= 2076 --> F0= 435.19 MHz
            break;

        case FQ_249:
            FrequencySet = 0x0824; // F= 2084 --> F0= 435.21 MHz
            break;

        case FQ_250:
            FrequencySet = 0x082C; // F= 2092 --> F0= 435.23 MHz
            break;

        case FQ_251:
            FrequencySet = 0x0834; // F= 2100 --> F0= 435.25 MHz
            break;

        case FQ_252:
            FrequencySet = 0x083C; // F= 2108 --> F0= 435.27 MHz
            break;

        case FQ_253:
            FrequencySet = 0x0844; // F= 2116 --> F0= 435.29 MHz
            break;

        case FQ_254:
            FrequencySet = 0x084C; // F= 2124 --> F0= 435.31 MHz
            break;

        case FQ_255:
            FrequencySet = 0x0854; // F= 2132 --> F0= 435.33 MHz
            break;

        case FQ_256:
            FrequencySet = 0x085C; // F= 2140 --> F0= 435.35 MHz
            break;

        case FQ_257:
            FrequencySet = 0x0864; // F= 2148 --> F0= 435.37 MHz
            break;

        case FQ_258:
            FrequencySet = 0x086C; // F= 2156 --> F0= 435.39 MHz
            break;

        case FQ_259:
            FrequencySet = 0x0874; // F= 2164 --> F0= 435.41 MHz
            break;

        case FQ_260:
            FrequencySet = 0x087C; // F= 2172 --> F0= 435.43 MHz
            break;

        case FQ_261:
            FrequencySet = 0x0884; // F= 2180 --> F0= 435.45 MHz
            break;

        case FQ_262:
            FrequencySet = 0x088C; // F= 2188 --> F0= 435.47 MHz
            break;

        case FQ_263:
            FrequencySet = 0x0894; // F= 2196 --> F0= 435.49 MHz
            break;

        case FQ_264:
            FrequencySet = 0x089C; // F= 2204 --> F0= 435.51 MHz
            break;

        case FQ_265:
            FrequencySet = 0x08A4; // F= 2212 --> F0= 435.53 MHz
            break;

        case FQ_266:
            FrequencySet = 0x08AC; // F= 2220 --> F0= 435.55 MHz
            break;

        case FQ_267:
            FrequencySet = 0x08B4; // F= 2228 --> F0= 435.57 MHz
            break;

        case FQ_268:
            FrequencySet = 0x08BC; // F= 2236 --> F0= 435.59 MHz
            break;

        case FQ_269:
            FrequencySet = 0x08C4; // F= 2244 --> F0= 435.61 MHz
            break;

        case FQ_270:
            FrequencySet = 0x08CC; // F= 2252 --> F0= 435.63 MHz
            break;

        case FQ_271:
            FrequencySet = 0x08D4; // F= 2260 --> F0= 435.65 MHz
            break;

        case FQ_272:
            FrequencySet = 0x08DC; // F= 2268 --> F0= 435.67 MHz
            break;

        case FQ_273:
            FrequencySet = 0x08E4; // F= 2276 --> F0= 435.69 MHz
            break;

        case FQ_274:
            FrequencySet = 0x08EC; // F= 2284 --> F0= 435.71 MHz
            break;

        case FQ_275:
            FrequencySet = 0x08F4; // F= 2292 --> F0= 435.73 MHz
            break;

        case FQ_276:
            FrequencySet = 0x08FC; // F= 2300 --> F0= 435.75 MHz
            break;

        case FQ_277:
            FrequencySet = 0x0904; // F= 2308 --> F0= 435.77 MHz
            break;

        case FQ_278:
            FrequencySet = 0x090C; // F= 2316 --> F0= 435.79 MHz
            break;

        case FQ_279:
            FrequencySet = 0x0914; // F= 2324 --> F0= 435.81 MHz
            break;

        case FQ_280:
            FrequencySet = 0x091C; // F= 2332 --> F0= 435.83 MHz
            break;

        case FQ_281:
            FrequencySet = 0x0924; // F= 2340 --> F0= 435.85 MHz
            break;

        case FQ_282:
            FrequencySet = 0x092C; // F= 2348 --> F0= 435.87 MHz
            break;

        case FQ_283:
            FrequencySet = 0x0934; // F= 2356 --> F0= 435.89 MHz
            break;

        case FQ_284:
            FrequencySet = 0x093C; // F= 2364 --> F0= 435.91 MHz
            break;

        case FQ_285:
            FrequencySet = 0x0944; // F= 2372 --> F0= 435.93 MHz
            break;

        case FQ_286:
            FrequencySet = 0x094C; // F= 2380 --> F0= 435.95 MHz
            break;

        case FQ_287:
            FrequencySet = 0x0954; // F= 2388 --> F0= 435.97 MHz
            break;

        case FQ_288:
            FrequencySet = 0x095C; // F= 2396 --> F0= 435.99 MHz
            break;

        case FQ_289:
            FrequencySet = 0x0964; // F= 2404 --> F0= 436.01 MHz
            break;

        case FQ_290:
            FrequencySet = 0x096C; // F= 2412 --> F0= 436.03 MHz
            break;

        case FQ_291:
            FrequencySet = 0x0974; // F= 2420 --> F0= 436.05 MHz
            break;

        case FQ_292:
            FrequencySet = 0x097C; // F= 2428 --> F0= 436.07 MHz
            break;

        case FQ_293:
            FrequencySet = 0x0984; // F= 2436 --> F0= 436.09 MHz
            break;

        case FQ_294:
            FrequencySet = 0x098C; // F= 2444 --> F0= 436.11 MHz
            break;

        case FQ_295:
            FrequencySet = 0x0994; // F= 2452 --> F0= 436.13 MHz
            break;

        case FQ_296:
            FrequencySet = 0x099C; // F= 2460 --> F0= 436.15 MHz
            break;

        case FQ_297:
            FrequencySet = 0x09A4; // F= 2468 --> F0= 436.17 MHz
            break;

        case FQ_298:
            FrequencySet = 0x09AC; // F= 2476 --> F0= 436.19 MHz
            break;

        case FQ_299:
            FrequencySet = 0x09B4; // F= 2484 --> F0= 436.21 MHz
            break;

        case FQ_300:
            FrequencySet = 0x09BC; // F= 2492 --> F0= 436.23 MHz
            break;

        case FQ_301:
            FrequencySet = 0x09C4; // F= 2500 --> F0= 436.25 MHz
            break;

        case FQ_302:
            FrequencySet = 0x09CC; // F= 2508 --> F0= 436.27 MHz
            break;

        case FQ_303:
            FrequencySet = 0x09D4; // F= 2516 --> F0= 436.29 MHz
            break;

        case FQ_304:
            FrequencySet = 0x09DC; // F= 2524 --> F0= 436.31 MHz
            break;

        case FQ_305:
            FrequencySet = 0x09E4; // F= 2532 --> F0= 436.33 MHz
            break;

        case FQ_306:
            FrequencySet = 0x09EC; // F= 2540 --> F0= 436.35 MHz
            break;

        case FQ_307:
            FrequencySet = 0x09F4; // F= 2548 --> F0= 436.37 MHz
            break;

        case FQ_308:
            FrequencySet = 0x09FC; // F= 2556 --> F0= 436.39 MHz
            break;

        case FQ_309:
            FrequencySet = 0x0A04; // F= 2564 --> F0= 436.41 MHz
            break;

        case FQ_310:
            FrequencySet = 0x0A0C; // F= 2572 --> F0= 436.43 MHz
            break;

        case FQ_311:
            FrequencySet = 0x0A14; // F= 2580 --> F0= 436.45 MHz
            break;

        case FQ_312:
            FrequencySet = 0x0A1C; // F= 2588 --> F0= 436.47 MHz
            break;

        case FQ_313:
            FrequencySet = 0x0A24; // F= 2596 --> F0= 436.49 MHz
            break;

        case FQ_314:
            FrequencySet = 0x0A2C; // F= 2604 --> F0= 436.51 MHz
            break;

        case FQ_315:
            FrequencySet = 0x0A34; // F= 2612 --> F0= 436.53 MHz
            break;

        case FQ_316:
            FrequencySet = 0x0A3C; // F= 2620 --> F0= 436.55 MHz
            break;

        case FQ_317:
            FrequencySet = 0x0A44; // F= 2628 --> F0= 436.57 MHz
            break;

        case FQ_318:
            FrequencySet = 0x0A4C; // F= 2636 --> F0= 436.59 MHz
            break;

        case FQ_319:
            FrequencySet = 0x0A54; // F= 2644 --> F0= 436.61 MHz
            break;

        case FQ_320:
            FrequencySet = 0x0A5C; // F= 2652 --> F0= 436.63 MHz
            break;

        case FQ_321:
            FrequencySet = 0x0A64; // F= 2660 --> F0= 436.65 MHz
            break;

        case FQ_322:
            FrequencySet = 0x0A6C; // F= 2668 --> F0= 436.67 MHz
            break;

        case FQ_323:
            FrequencySet = 0x0A74; // F= 2676 --> F0= 436.69 MHz
            break;

        case FQ_324:
            FrequencySet = 0x0A7C; // F= 2684 --> F0= 436.71 MHz
            break;

        case FQ_325:
            FrequencySet = 0x0A84; // F= 2692 --> F0= 436.73 MHz
            break;

        case FQ_326:
            FrequencySet = 0x0A8C; // F= 2700 --> F0= 436.75 MHz
            break;

        case FQ_327:
            FrequencySet = 0x0A94; // F= 2708 --> F0= 436.77 MHz
            break;

        case FQ_328:
            FrequencySet = 0x0A9C; // F= 2716 --> F0= 436.79 MHz
            break;

        case FQ_329:
            FrequencySet = 0x0AA4; // F= 2724 --> F0= 436.81 MHz
            break;

        case FQ_330:
            FrequencySet = 0x0AAC; // F= 2732 --> F0= 436.83 MHz
            break;

        case FQ_331:
            FrequencySet = 0x0AB4; // F= 2740 --> F0= 436.85 MHz
            break;

        case FQ_332:
            FrequencySet = 0x0ABC; // F= 2748 --> F0= 436.87 MHz
            break;

        case FQ_333:
            FrequencySet = 0x0AC4; // F= 2756 --> F0= 436.89 MHz
            break;

        case FQ_334:
            FrequencySet = 0x0ACC; // F= 2764 --> F0= 436.91 MHz
            break;

        case FQ_335:
            FrequencySet = 0x0AD4; // F= 2772 --> F0= 436.93 MHz
            break;

        case FQ_336:
            FrequencySet = 0x0ADC; // F= 2780 --> F0= 436.95 MHz
            break;

        case FQ_337:
            FrequencySet = 0x0AE4; // F= 2788 --> F0= 436.97 MHz
            break;

        case FQ_338:
            FrequencySet = 0x0AEC; // F= 2796 --> F0= 436.99 MHz
            break;

        case FQ_339:
            FrequencySet = 0x0AF4; // F= 2804 --> F0= 437.01 MHz
            break;

        case FQ_340:
            FrequencySet = 0x0AFC; // F= 2812 --> F0= 437.03 MHz
            break;

        case FQ_341:
            FrequencySet = 0x0B04; // F= 2820 --> F0= 437.05 MHz
            break;

        case FQ_342:
            FrequencySet = 0x0B0C; // F= 2828 --> F0= 437.07 MHz
            break;

        case FQ_343:
            FrequencySet = 0x0B14; // F= 2836 --> F0= 437.09 MHz
            break;

        case FQ_344:
            FrequencySet = 0x0B1C; // F= 2844 --> F0= 437.11 MHz
            break;

        case FQ_345:
            FrequencySet = 0x0B24; // F= 2852 --> F0= 437.13 MHz
            break;

        case FQ_346:
            FrequencySet = 0x0B2C; // F= 2860 --> F0= 437.15 MHz
            break;

        case FQ_347:
            FrequencySet = 0x0B34; // F= 2868 --> F0= 437.17 MHz
            break;

        case FQ_348:
            FrequencySet = 0x0B3C; // F= 2876 --> F0= 437.19 MHz
            break;

        case FQ_349:
            FrequencySet = 0x0B44; // F= 2884 --> F0= 437.21 MHz
            break;

        case FQ_350:
            FrequencySet = 0x0B4C; // F= 2892 --> F0= 437.23 MHz
            break;

        case FQ_351:
            FrequencySet = 0x0B54; // F= 2900 --> F0= 437.25 MHz
            break;

        case FQ_352:
            FrequencySet = 0x0B5C; // F= 2908 --> F0= 437.27 MHz
            break;

        case FQ_353:
            FrequencySet = 0x0B64; // F= 2916 --> F0= 437.29 MHz
            break;

        case FQ_354:
            FrequencySet = 0x0B6C; // F= 2924 --> F0= 437.31 MHz
            break;

        case FQ_355:
            FrequencySet = 0x0B74; // F= 2932 --> F0= 437.33 MHz
            break;

        case FQ_356:
            FrequencySet = 0x0B7C; // F= 2940 --> F0= 437.35 MHz
            break;

        case FQ_357:
            FrequencySet = 0x0B84; // F= 2948 --> F0= 437.37 MHz
            break;

        case FQ_358:
            FrequencySet = 0x0B8C; // F= 2956 --> F0= 437.39 MHz
            break;

        case FQ_359:
            FrequencySet = 0x0B94; // F= 2964 --> F0= 437.41 MHz
            break;

        case FQ_360:
            FrequencySet = 0x0B9C; // F= 2972 --> F0= 437.43 MHz
            break;

        case FQ_361:
            FrequencySet = 0x0BA4; // F= 2980 --> F0= 437.45 MHz
            break;

        case FQ_362:
            FrequencySet = 0x0BAC; // F= 2988 --> F0= 437.47 MHz
            break;

        case FQ_363:
            FrequencySet = 0x0BB4; // F= 2996 --> F0= 437.49 MHz
            break;

        case FQ_364:
            FrequencySet = 0x0BBC; // F= 3004 --> F0= 437.51 MHz
            break;

        case FQ_365:
            FrequencySet = 0x0BC4; // F= 3012 --> F0= 437.53 MHz
            break;

        case FQ_366:
            FrequencySet = 0x0BCC; // F= 3020 --> F0= 437.55 MHz
            break;

        case FQ_367:
            FrequencySet = 0x0BD4; // F= 3028 --> F0= 437.57 MHz
            break;

        case FQ_368:
            FrequencySet = 0x0BDC; // F= 3036 --> F0= 437.59 MHz
            break;

        case FQ_369:
            FrequencySet = 0x0BE4; // F= 3044 --> F0= 437.61 MHz
            break;

        case FQ_370:
            FrequencySet = 0x0BEC; // F= 3052 --> F0= 437.63 MHz
            break;

        case FQ_371:
            FrequencySet = 0x0BF4; // F= 3060 --> F0= 437.65 MHz
            break;

        case FQ_372:
            FrequencySet = 0x0BFC; // F= 3068 --> F0= 437.67 MHz
            break;

        case FQ_373:
            FrequencySet = 0x0C04; // F= 3076 --> F0= 437.69 MHz
            break;

        case FQ_374:
            FrequencySet = 0x0C0C; // F= 3084 --> F0= 437.71 MHz
            break;

        case FQ_375:
            FrequencySet = 0x0C14; // F= 3092 --> F0= 437.73 MHz
            break;

        case FQ_376:
            FrequencySet = 0x0C1C; // F= 3100 --> F0= 437.75 MHz
            break;

        case FQ_377:
            FrequencySet = 0x0C24; // F= 3108 --> F0= 437.77 MHz
            break;

        case FQ_378:
            FrequencySet = 0x0C2C; // F= 3116 --> F0= 437.79 MHz
            break;

        case FQ_379:
            FrequencySet = 0x0C34; // F= 3124 --> F0= 437.81 MHz
            break;

        case FQ_380:
            FrequencySet = 0x0C3C; // F= 3132 --> F0= 437.83 MHz
            break;

        case FQ_381:
            FrequencySet = 0x0C44; // F= 3140 --> F0= 437.85 MHz
            break;

        case FQ_382:
            FrequencySet = 0x0C4C; // F= 3148 --> F0= 437.87 MHz
            break;

        case FQ_383:
            FrequencySet = 0x0C54; // F= 3156 --> F0= 437.89 MHz
            break;

        case FQ_384:
            FrequencySet = 0x0C5C; // F= 3164 --> F0= 437.91 MHz
            break;

        case FQ_385:
            FrequencySet = 0x0C64; // F= 3172 --> F0= 437.93 MHz
            break;

        case FQ_386:
            FrequencySet = 0x0C6C; // F= 3180 --> F0= 437.95 MHz
            break;

        case FQ_387:
            FrequencySet = 0x0C74; // F= 3188 --> F0= 437.97 MHz
            break;

        case FQ_388:
            FrequencySet = 0x0C7C; // F= 3196 --> F0= 437.99 MHz
            break;

        case FQ_389:
            FrequencySet = 0x0C84; // F= 3204 --> F0= 438.01 MHz
            break;

        case FQ_390:
            FrequencySet = 0x0C8C; // F= 3212 --> F0= 438.03 MHz
            break;

        case FQ_391:
            FrequencySet = 0x0C94; // F= 3220 --> F0= 438.05 MHz
            break;

        case FQ_392:
            FrequencySet = 0x0C9C; // F= 3228 --> F0= 438.07 MHz
            break;

        case FQ_393:
            FrequencySet = 0x0CA4; // F= 3236 --> F0= 438.09 MHz
            break;

        case FQ_394:
            FrequencySet = 0x0CAC; // F= 3244 --> F0= 438.11 MHz
            break;

        case FQ_395:
            FrequencySet = 0x0CB4; // F= 3252 --> F0= 438.13 MHz
            break;

        case FQ_396:
            FrequencySet = 0x0CBC; // F= 3260 --> F0= 438.15 MHz
            break;

        case FQ_397:
            FrequencySet = 0x0CC4; // F= 3268 --> F0= 438.17 MHz
            break;

        case FQ_398:
            FrequencySet = 0x0CCC; // F= 3276 --> F0= 438.19 MHz
            break;

        case FQ_399:
            FrequencySet = 0x0CD4; // F= 3284 --> F0= 438.21 MHz
            break;

        case FQ_400:
            FrequencySet = 0x0CDC; // F= 3292 --> F0= 438.23 MHz
            break;

        case FQ_401:
            FrequencySet = 0x0CE4; // F= 3300 --> F0= 438.25 MHz
            break;

        case FQ_402:
            FrequencySet = 0x0CEC; // F= 3308 --> F0= 438.27 MHz
            break;

        case FQ_403:
            FrequencySet = 0x0CF4; // F= 3316 --> F0= 438.29 MHz
            break;

        case FQ_404:
            FrequencySet = 0x0CFC; // F= 3324 --> F0= 438.31 MHz
            break;

        case FQ_405:
            FrequencySet = 0x0D04; // F= 3332 --> F0= 438.33 MHz
            break;

        case FQ_406:
            FrequencySet = 0x0D0C; // F= 3340 --> F0= 438.35 MHz
            break;

        case FQ_407:
            FrequencySet = 0x0D14; // F= 3348 --> F0= 438.37 MHz
            break;

        case FQ_408:
            FrequencySet = 0x0D1C; // F= 3356 --> F0= 438.39 MHz
            break;

        case FQ_409:
            FrequencySet = 0x0D24; // F= 3364 --> F0= 438.41 MHz
            break;

        case FQ_410:
            FrequencySet = 0x0D2C; // F= 3372 --> F0= 438.43 MHz
            break;

        case FQ_411:
            FrequencySet = 0x0D34; // F= 3380 --> F0= 438.45 MHz
            break;

        case FQ_412:
            FrequencySet = 0x0D3C; // F= 3388 --> F0= 438.47 MHz
            break;

        case FQ_413:
            FrequencySet = 0x0D44; // F= 3396 --> F0= 438.49 MHz
            break;

        case FQ_414:
            FrequencySet = 0x0D4C; // F= 3404 --> F0= 438.51 MHz
            break;

        case FQ_415:
            FrequencySet = 0x0D54; // F= 3412 --> F0= 438.53 MHz
            break;

        case FQ_416:
            FrequencySet = 0x0D5C; // F= 3420 --> F0= 438.55 MHz
            break;

        case FQ_417:
            FrequencySet = 0x0D64; // F= 3428 --> F0= 438.57 MHz
            break;

        case FQ_418:
            FrequencySet = 0x0D6C; // F= 3436 --> F0= 438.59 MHz
            break;

        case FQ_419:
            FrequencySet = 0x0D74; // F= 3444 --> F0= 438.61 MHz
            break;

        case FQ_420:
            FrequencySet = 0x0D7C; // F= 3452 --> F0= 438.63 MHz
            break;

        case FQ_421:
            FrequencySet = 0x0D84; // F= 3460 --> F0= 438.65 MHz
            break;

        case FQ_422:
            FrequencySet = 0x0D8C; // F= 3468 --> F0= 438.67 MHz
            break;

        case FQ_423:
            FrequencySet = 0x0D94; // F= 3476 --> F0= 438.69 MHz
            break;

        case FQ_424:
            FrequencySet = 0x0D9C; // F= 3484 --> F0= 438.71 MHz
            break;

        case FQ_425:
            FrequencySet = 0x0DA4; // F= 3492 --> F0= 438.73 MHz
            break;

        case FQ_426:
            FrequencySet = 0x0DAC; // F= 3500 --> F0= 438.75 MHz
            break;

        case FQ_427:
            FrequencySet = 0x0DB4; // F= 3508 --> F0= 438.77 MHz
            break;

        case FQ_428:
            FrequencySet = 0x0DBC; // F= 3516 --> F0= 438.79 MHz
            break;

        case FQ_429:
            FrequencySet = 0x0DC4; // F= 3524 --> F0= 438.81 MHz
            break;

        case FQ_430:
            FrequencySet = 0x0DCC; // F= 3532 --> F0= 438.83 MHz
            break;

        case FQ_431:
            FrequencySet = 0x0DD4; // F= 3540 --> F0= 438.85 MHz
            break;

        case FQ_432:
            FrequencySet = 0x0DDC; // F= 3548 --> F0= 438.87 MHz
            break;

        case FQ_433:
            FrequencySet = 0x0DE4; // F= 3556 --> F0= 438.89 MHz
            break;

        case FQ_434:
            FrequencySet = 0x0DEC; // F= 3564 --> F0= 438.91 MHz
            break;

        case FQ_435:
            FrequencySet = 0x0DF4; // F= 3572 --> F0= 438.93 MHz
            break;

        case FQ_436:
            FrequencySet = 0x0DFC; // F= 3580 --> F0= 438.95 MHz
            break;

        case FQ_437:
            FrequencySet = 0x0E04; // F= 3588 --> F0= 438.97 MHz
            break;

        case FQ_438:
            FrequencySet = 0x0E0C; // F= 3596 --> F0= 438.99 MHz
            break;

        case FQ_439:
            FrequencySet = 0x0E14; // F= 3604 --> F0= 439.01 MHz
            break;

        case FQ_440:
            FrequencySet = 0x0E1C; // F= 3612 --> F0= 439.03 MHz
            break;

        case FQ_441:
            FrequencySet = 0x0E24; // F= 3620 --> F0= 439.05 MHz
            break;

        case FQ_442:
            FrequencySet = 0x0E2C; // F= 3628 --> F0= 439.07 MHz
            break;

        case FQ_443:
            FrequencySet = 0x0E34; // F= 3636 --> F0= 439.09 MHz
            break;

        case FQ_444:
            FrequencySet = 0x0E3C; // F= 3644 --> F0= 439.11 MHz
            break;

        case FQ_445:
            FrequencySet = 0x0E44; // F= 3652 --> F0= 439.13 MHz
            break;

        case FQ_446:
            FrequencySet = 0x0E4C; // F= 3660 --> F0= 439.15 MHz
            break;

        case FQ_447:
            FrequencySet = 0x0E54; // F= 3668 --> F0= 439.17 MHz
            break;

        case FQ_448:
            FrequencySet = 0x0E5C; // F= 3676 --> F0= 439.19 MHz
            break;

        case FQ_449:
            FrequencySet = 0x0E64; // F= 3684 --> F0= 439.21 MHz
            break;

        case FQ_450:
            FrequencySet = 0x0E6C; // F= 3692 --> F0= 439.23 MHz
            break;

        case FQ_451:
            FrequencySet = 0x0E74; // F= 3700 --> F0= 439.25 MHz
            break;

        case FQ_452:
            FrequencySet = 0x0E7C; // F= 3708 --> F0= 439.27 MHz
            break;

        case FQ_453:
            FrequencySet = 0x0E84; // F= 3716 --> F0= 439.29 MHz
            break;

        case FQ_454:
            FrequencySet = 0x0E8C; // F= 3724 --> F0= 439.31 MHz
            break;

        case FQ_455:
            FrequencySet = 0x0E94; // F= 3732 --> F0= 439.33 MHz
            break;

        case FQ_456:
            FrequencySet = 0x0E9C; // F= 3740 --> F0= 439.35 MHz
            break;

        case FQ_457:
            FrequencySet = 0x0EA4; // F= 3748 --> F0= 439.37 MHz
            break;

        case FQ_458:
            FrequencySet = 0x0EAC; // F= 3756 --> F0= 439.39 MHz
            break;

        case FQ_459:
            FrequencySet = 0x0EB4; // F= 3764 --> F0= 439.41 MHz
            break;

        case FQ_460:
            FrequencySet = 0x0EBC; // F= 3772 --> F0= 439.43 MHz
            break;

        case FQ_461:
            FrequencySet = 0x0EC4; // F= 3780 --> F0= 439.45 MHz
            break;

        case FQ_462:
            FrequencySet = 0x0ECC; // F= 3788 --> F0= 439.47 MHz
            break;

        case FQ_463:
            FrequencySet = 0x0ED4; // F= 3796 --> F0= 439.49 MHz
            break;

        case FQ_464:
            FrequencySet = 0x0EDC; // F= 3804 --> F0= 439.51 MHz
            break;

        case FQ_465:
            FrequencySet = 0x0EE4; // F= 3812 --> F0= 439.53 MHz
            break;

        case FQ_466:
            FrequencySet = 0x0EEC; // F= 3820 --> F0= 439.55 MHz
            break;

        case FQ_467:
            FrequencySet = 0x0EF4; // F= 3828 --> F0= 439.57 MHz
            break;

        case FQ_468:
            FrequencySet = 0x0EFC; // F= 3836 --> F0= 439.59 MHz
            break;

        case FQ_469:
            FrequencySet = 0x0F04; // F= 3844 --> F0= 439.61 MHz
            break;

        case FQ_470:
            FrequencySet = 0x0F0C; // F= 3852 --> F0= 439.63 MHz
            break;

        case FQ_471:
            FrequencySet = 0x0F14; // F= 3860 --> F0= 439.65 MHz
            break;

        case FQ_472:
            FrequencySet = 0x0F1C; // F= 3868 --> F0= 439.67 MHz
            break;

        case FQ_473:
            FrequencySet = 0x0F24; // F= 3876 --> F0= 439.69 MHz
            break;

        case FQ_474:
            FrequencySet = 0x0F2C; // F= 3884 --> F0= 439.71 MHz
            break;

        case FQ_475:
            FrequencySet = 0x0F34; // F= 3892 --> F0= 439.73 MHz
            break;

        case FQ_476:
            FrequencySet = 0x0F3C; // F= 3900 --> F0= 439.75 MHz
            break;
#endif

            /* RegModuleRF = valeur_du_registre_par_defaut_pour_case_non_valide; */
        case FQ_NOT_DEFINE:
        default:
#if defined(FQ_RESTRICTION_LOW)         // Restriction 37,5-40
            FrequencySet = 0x0070; // F= 112 --> F0= 430.28 MHz
#elif defined(FQ_RESTRICTION_MEDIUM)    // Restriction 17,5-20
            FrequencySet = 0x0068; // F= 104 --> F0= 430.26 MHz
#elif defined(FQ_RESTRICTION_HIGH)      // Restriction 7,5-10
            FrequencySet = 0x0064; // F= 100 --> F0= 430.25 MHz
#else
#error "You must define a valid Fq configuration !"
#endif
    }

    FrequencySet |= FQ_SET_CMD_CODE; // ajouter le code commande ? la valeur de la Fq

    return (FrequencySet);

} // end of FSK_Transceiver_ConfigFq()

void radioAlphaTRX_Init(void) {
    //
    RF_StatusRead.Val = 0;
    //    RF_StatusRead.Val = radioAlphaTRX_Command(STATUS_READ_CMD); // intitial SPI transfer added to avoid power-up problem
    /**-------------> Frequency Setting Command @ 433 MHz <--------------------*/

    do {
        RF_StatusRead.Val = radioAlphaTRX_Command(STATUS_READ_CMD); // intitial SPI transfer added to avoid power-up problem
#if defined(UART_DEBUG)
        printf("A other Wait until RFM12B is out of power-up reset, status: 0x%04X\r\n", RF_StatusRead.Val);
#endif
    } while (RF_StatusRead.bits.b14_POR);


    /**-------------> Power Management Command(2) <---------------------------*/
    //  bit  15  14  13  12  11  10   9   8   7    6   5   4   3   2   1   0   POR
    //        1   0   0   0   0   0   1   0  er  ebb  et  es  ex  eb  ew  dc   0x8208
    // --------------------- 0x82D9 ---------------------
    // er  - Enabled the whole receiver chain	    : on
    // ebb - The recv. baseband circuit can be sep. : on
    // et  - Switches on the PLL, the PA, starts tr.: off
    // es  - Turns on the synthesizer               : on
    // ex  - Turns on the crystal oscillator        : on
    // eb  - Enabled the low battery detector       : off
    // ew  - Enabled the wake-up timer              : off
    // dc  - Disabled the clock output (pin 8)      : on
    // 0x82A0
    // The ebb, es, and exbits are provided to optimize the TX to RX or RX to TX turnaround time.
    //    ALPHA_TRX433S_Control(0x82D9); // enable receive, enable base band block, enable synthesizer, enable crystal oscillator, disable clock output of CLK pin
    RF_PowerManagement.Val = PWR_MGMT_CMD_POR; // enable receive OFF, enable base band block OFF, enable synthesizer OFF, enable crystal oscillator ON, dc = 0 --> enable clock output of CLK pin
    RF_PowerManagement.bits.b7_er = 1; // Enabled the whole receiver chain
    RF_PowerManagement.bits.b6_ebb = 1; // The receiver baseband circuit can be separately switched on
    RF_PowerManagement.bits.b5_et = 0; // Switch on the PLL
    RF_PowerManagement.bits.b4_es = 1; // Turns on the synthesizer
    RF_PowerManagement.bits.b0_dc = 1; // Disable clock output of CLK pin
    radioAlphaTRX_Command(RF_PowerManagement.Val);



    /**-------------> Frequency Setting Command (3) <--------------------------*/
    // 96 < F < 3903 = 1600
    // 433 band: Fc= 430+F*0.0025 MHz ou 10*(43+F/4000)=
    // 430+1600*0.0025= 434 MHz (433 MHz band, 2.5 KHz step)
    // 1600d --> 0x640
    //    ALPHA_TRX433S_Control(0xA640); // Set operation frequency: Fc= 430+F*0.0025 , soit 430+1600*0.0025= 434 MHz avec 0x640 --> 110 0100 0000
    RF_FrequencySet.Val = FQ_SET_CMD_POR;
    //    RF_FrequencySet.REGbits.SetOperationFrequency_H = 0x6;
    //    RF_FrequencySet.REGbits.SetOperationFrequency_L = 0x40;

    //    RF_FrequencySet.Val = FSK_Transceiver_ConfigFq( Switch_Read( ) );
    //    RF_FrequencySet.Val = FSK_Transceiver_ConfigFq(FQ_001);
    radioAlphaTRX_Command(RF_FrequencySet.Val); // Set operation frequency: Fc= 430+F*0.0025 , soit 430+1600*0.0025= 434 MHz avec 0x640 --> 110 0100 0000


    /**-------------> Data Rate Command (4) <---------------------------------*/
    // BR = 10000000/29/(r+1)/(1+cs*7)
    // BR = 10000000/29/(35+1)/(1+0*7)= 9.579 Kbps
    // Approximately 9.579 Kbps
    //    radioAlphaTRX_Command(0xC623); // Data rate command 9600 bauds
    //    R= (10000 / 29 / (1+cs * 7) / BR)-1
    //le CS permet d'affiner le bpts
    //    radioAlphaTRX_Command(0xC647);
    //    radioAlphaTRX_Command(0xC602); //115.2
    RF_DataRate.Val = DATA_RATE_CMD_POR;
    RF_DataRate.REGbits.SetCs = CS_HIGH;
    RF_DataRate.REGbits.SetRate = 4;
    radioAlphaTRX_Command(RF_DataRate.Val); // 0xC684 == 4800

    /**-------------> FReceiver Control Command (5) <--------------------------*/
    // Interrupt,FAST,200kHz,20dBm,-103dBm
    // p16 - Interrupt input (bit 10)
    // d.  - Fast
    // i.  - Receiver baseband bandwidth (BW) select:  67 KHz
    // g.  - Gain relative to maximum               :  20 dBm
    // r.  - RSSI detector threshold                : -103 dBm
    RF_ReceiverControl.Val = RX_CTRL_CMD_POR;
    RF_ReceiverControl.REGbits.RSSIsetth = RSSIsetth_n103;
    RF_ReceiverControl.REGbits.GLNA = GAIN_n20_dB;
    RF_ReceiverControl.REGbits.RX_BW_Select = BW_67_KHz;
    RF_ReceiverControl.REGbits.VDI_RespSetting = FAST;
    RF_ReceiverControl.REGbits.Pin16_function = INTERRUPT_INPUT;
    radioAlphaTRX_Command(RF_ReceiverControl.Val);

    /**-------------> TX Configuration Control Command (11) <-----------------*/
    // FSK modulation           : 90 KHz frequency deviation
    // 434000+(6*15)= 434090 KHz soit F0= 434 MHz + dFfsk= 90 KHz
    // avec mp= 0, on aura:
    //   la frequence FSK pour representer le 0 sera de F0-dFfsk= 433,910 MHz
    //   la frequence FSK pour representer le 1 sera de F0+dFfsk= 434,090 MHz
    //  434,090-433,910= 0,180 MHz --> band passante BW par rapport a F0
    // Relative output power	:  0 dB
    //force de sortie -21  TX configure controle commande 
    RF_TX_ConfCtrlCmd.Val = TX_CONF_CTRL_CMD_POR;
    RF_TX_ConfCtrlCmd.REGbits.SetDeviation = FSQ_DEV_45_KHZ;
    RF_TX_ConfCtrlCmd.REGbits.SetOutputPower = OUTPUT_PWER_21_dB;
    radioAlphaTRX_CaptureFrame(RF_TX_ConfCtrlCmd.Val); // 9847




    /**-------------> Data Filter Command (6) <--------------------------------*/
    /** 0xC2AC - Data Filter Command */
    // AL,!ml,DIG,DQD4
    // al - Clock recovery (CR) auto lock control : auto mode
    // ml - Clock recovery lock control           : slow mode, slow attack and slow release
    // s  - Digital filter
    // f  - DQD threshold parameter : 4
    //  bit  15  14  13  12 # 11  10  9  8 #  7    6     5    4   #  3     2    1     0   POR
    //        1   1   0   0 #  1   0  1  0 # al=1  ml=0  1    s=0 #  1    f2=1  f1=1  f0?0   CA80h
    radioAlphaTRX_Command(0xC2AC); // al=1; ml=0;Data Filter: Digital filter, DATA QUALITY THRESHOLD 4

    
    /**-------------> 0xCA83 - FIFO and Reset Mode Command (7) <---------------*/
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
    radioAlphaTRX_Command(RF_FIFOandResetMode.Val); // al=1; ml=0;Data Filter: Digital filter, DATA QUALITY THRESHOLD 6
    
    /**-------------> Synchron Pattern Command (8) <--------------------------*/
    // 0xCE00 | group
    // SYNC = 2DXX
    // byte 0 of the synchron patern (group 212 = D4h)
    //    result = rf12_xferCmd(fd, 0xCE00 | jl_group);
    //    printCmdAndResult(0xCE00 | jl_group, result, "SYNC=2DXX");
    radioAlphaTRX_Command(0xCED4); // Synchron Pattern Byte0 = 0xD4

    /**-------------> AFC Command (10) <--------------------------------------*/
    // Automatic operation mode selector    : Keep the f_offset only during receiving (VDI=high)
    // Range limits                         : No restriction
    // Strobe edge                          : off
    // High accuracy mode                   : off
    // Enable frequency offset register     : on
    // Enable calculation of offset freq    : on
    // AFC auto-mode: Keep offset indepently value VDI hi, [range limit: +15/-16,] 
    //st goes hi will store offset into output register, Enable AFC output register, Enable AFC function
    radioAlphaTRX_Command(0xC4D7); //0xC4F7
    
    
    /**-------------> PLL Setting Command (12) <-----------------*/
    // Microcontroller output clock buffer rise and fall time   : 5 or 10 Mhz (recommended)
    // Switches on the delay in the phase detector              : off
    // Disabled the dithering in the PLL loop                   : on
    // PLL bandwidth                                            : Max bit rate: 256 kbps, Phase noise at 1 Mhz offset: -102 dBc/Hc
    radioAlphaTRX_Command(0xCC7A); // 0xCC7A Pll Setting
    
    /* 0xE000 - Wake-Up Timer Command */
    // NOT USE
    // T_wakeup = 0 ms
    radioAlphaTRX_Command(0xE000); // Wake-Up Timer NOT USE :: pour l'instant on fait ?a on changera 

    /* 0xC800 - Low Duty-Cycle Command */
    // NOT USE
    // duty-cyle                    : 0 %
    // low duty-cycle mode enabled  : off
    radioAlphaTRX_Command(0xC800); // disable low duty cycle mode --> NOT USE
    __delay32(SLEEP_AFTER_INIT);
}

// Initialiser la detection d'une nouvelle donnee

void radioAlphaTRX_ReceivedMode(void) {
    //close TX mode 
    RF_PowerManagement.Val = PWR_MGMT_CMD_POR;
    RF_PowerManagement.bits.b0_dc = 1;
    radioAlphaTRX_Command(RF_PowerManagement.Val);//0x8209
    radioAlphaTRX_Command(RX_FIFO_READ_CMD_POR);

    /**-------------> Configuration Setting Command (2) <----------------------*/
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
    RF_PowerManagement.Val = PWR_MGMT_CMD_POR;
    RF_PowerManagement.bits.b7_er = 1;
    RF_PowerManagement.bits.b6_ebb = 1;
    RF_PowerManagement.bits.b3_ex = 1;
    RF_PowerManagement.bits.b0_dc = 1;
    radioAlphaTRX_Command(RF_PowerManagement.Val); //0x82C9 

    RF_FIFOandResetMode.bits.b1_ff = 1; // FIFO fill will be enabled after synchronize pattern reception
    RF_StatusRead.Val = radioAlphaTRX_Command(RF_FIFOandResetMode.Val); // --> 0xCA83
    //#if defined(UART_DEBUG)
    //        printf("status 0x%04X\n", RF_StatusRead.Val);
    //#endif
    radioAlphaTRX_SetSendMode(0); // on n'est plus en mode emission
}

// Configuration en mode TX avant l'envoie de donnee

int8_t radioAlphaTRX_SendMode(void) {
    //close Rx mode 
    RF_PowerManagement.Val = PWR_MGMT_CMD_POR;
    RF_PowerManagement.bits.b0_dc = 1;
    radioAlphaTRX_Command(RF_PowerManagement.Val);//0x8209
    radioAlphaTRX_Command(RX_FIFO_READ_CMD_POR);


//    //    radioAlphaTRX_CaptureFrame(RX_FIFO_READ_CMD);
//    /**-------------> Configuration Setting Command (1)<-----------------------*/
//    //  bit  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0   POR
//    //  Val   1   0   0   0   0   0   0   0  el  ef  b1  b0  x3  x2  x1  x0   0x8008
//    // --------------------- 0x80?? ---------------------
//    // el     - Enabled the internal data register : off
//    // ef     - Enabled the FIFO mode              : on
//    // b<1:0> - Set up the band                    : 868 MHz
//    // x<3:0> -                                    : 12.5 pF
//    RF_ConfigSet.Val = CFG_SET_CMD_POR; // initialiser la variable globale avec la valeur interne pr?sente lors du Power On Reset
//    //    radioAlphaTRX_Command(RF_ConfigSet.Val);
//    //    el = 1;
//    //    SelectBand = BAND_868 ==> initialer module at 868 Mhz
//    //    SelectCrystalCapacitor = LOAD_C_12_0pF  
    
//    RF_ConfigSet.Val = 0x80A7;
//    radioAlphaTRX_Command(RF_ConfigSet.Val);

    /**-------------> Configuration Setting Command (2) <----------------------*/
    //  bit  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0   POR
    //  Val   1   0   0   0   0   0   0   0  el  ef  b1  b0  x3  x2  x1  x0   0x8008
    // --------------------- 0x80?? ---------------------
    // el     - Enabled the internal data register : off
    // ef     - Enabled the FIFO mode              : on
    // b<1:0> - Set up the band                    : 868 MHz
    // x<3:0> -                                    : 12.5 pF
    RF_ConfigSet.Val = CFG_SET_CMD_POR; // initialiser la variable globale avec la valeur interne pr?sente
    //active receive mode (RX)lors du Power On Reset
    RF_ConfigSet.bits.b7_el = 1; // e enables the internal data register 
    RF_ConfigSet.REGbits.SelectBand = BAND_868; // initialer module at 868 Mhz
    RF_ConfigSet.REGbits.SelectCrystalCapacitor = LOAD_C_12_0pF;
    radioAlphaTRX_Command(RF_ConfigSet.Val);
    
    
    //    Power Management Command
    //    b5_et = 1; // Enabling the Transmitter preloads the TX latch with 0xAAAA
    //    b4_es = 1;
    //    b3_ex = 1; 
    //    b0_dc = 1;
    RF_PowerManagement.Val = PWR_MGMT_CMD_POR;
    RF_PowerManagement.bits.b5_et = 1;
    RF_PowerManagement.bits.b4_es = 1;
    RF_PowerManagement.bits.b3_ex = 1;
    RF_PowerManagement.bits.b0_dc = 1;
    RF_StatusRead.Val = radioAlphaTRX_Command(RF_PowerManagement.Val); //0x8239

    radioAlphaTRX_SetSendMode(1); // on est en mode transmission
    return radioAlphaTRX_WaitLownIRQ(SEND_TIME_OUT); // arbitraire 
}


// Transmission d'une donnee par le module RF

int8_t radioAlphaTRX_SendByte(uint8_t data_send, int8_t timeout) {
    WORD_VAL_T sendData;
    sendData.byte.high = 0xB8; // c'est le bit de poid fort d'abord 
    sendData.byte.low = data_send;
    radioAlphaTRX_Command(sendData.word);
    return radioAlphaTRX_WaitLownIRQ(timeout);
}

int8_t radioAlphaTRX_SendData(uint8_t* bytes, int8_t size) {
    int i;

    //preamble
    radioAlphaTRX_SendByte(0xAA, SEND_TIME_OUT);
    radioAlphaTRX_SendByte(0xAA, SEND_TIME_OUT);
    radioAlphaTRX_SendByte(0xAA, SEND_TIME_OUT);

    //peut g?n?rer des probleme ? surveiller
    //synchro pattern
    radioAlphaTRX_SendByte(0x2D, SEND_TIME_OUT);
    radioAlphaTRX_SendByte(0xD4, SEND_TIME_OUT);
    //transmission des octet 
    for (i = 0; i < size; i++) {
        if (radioAlphaTRX_SendByte(bytes[i], SEND_TIME_OUT) == 0)
            break;
    }

    //dummy bytes
    radioAlphaTRX_SendByte(0x00, SEND_TIME_OUT);
    radioAlphaTRX_SendByte(0x00, SEND_TIME_OUT);
    // clear TX
    //    RF_PowerManagement.Val = 0x8209;
    //    radioAlphaTRX_Command(RF_PowerManagement.Val);
    return i;
}

// Envoyer une commande au module RF (par liaison SPI)

uint16_t radioAlphaTRX_Command(uint16_t cmdWrite) {
    WORD_VAL_T receiveData;
    WORD_VAL_T sendData;
    sendData.word = cmdWrite;

    RF_nSEL_SetLow(); // debut de la communication SPI avec le Slave
    while (RF_nSEL_GetValue()) {
    } // a eviter je pense, mais pour l'instant je le garde 
    receiveData.byte.high = SPI1_Exchange8bit(sendData.byte.high);
    receiveData.byte.low = SPI1_Exchange8bit(sendData.byte.low);
    RF_nSEL_SetHigh(); // fin de la communication SPI avec le SLave 
    while (!RF_nSEL_GetValue()) {
    } // a eviter mais pour l'instant je le garde 

    return receiveData.word;
}

int8_t radioAlphaTRX_WaitLownIRQ(int timeout) {
    TMR_SetnIRQLowTimeout(timeout);
    while (RF_nIRQ_GetValue()) {
        if (TMR_GetnIRQLowTimeout() == 0) {
            return 0;
        }
    }
    TMR_SetnIRQLowTimeout(0);
    return 1;
}

/******************************************************************************/
/******************************************************************************/
/******************** PARAMETRE DE CAPTURE DE LA TRAME RECU *******************/
/******************************************************************************/
/******************************************************************************/
// 4 buffer remplie de circulairement 
volatile uint8_t BUF[FRAME_LENGTH];
volatile uint8_t sizeBuf = 0;
volatile uint8_t sendMode = 0; // O receve mode    1 send mode  

int8_t radioAlphaTRX_IsSendMode() {
    return sendMode;
}

void radioAlphaTRX_SetSendMode(int8_t modeRF) {
    sendMode = modeRF;
}

uint8_t radioAlphaTRX_GetSizeBuf() {
    return sizeBuf;
}

uint8_t * radioAlphaTRX_ReadBuf() {
    return BUF;
}

int8_t radioAlphaTRX_receive(uint8_t buffer[FRAME_LENGTH]) {
    WORD_VAL_T receiveData;
    uint8_t i = 0;
    for (i = 0; i < FRAME_LENGTH; i++) {
        if (0 == radioAlphaTRX_WaitLownIRQ(TIME_OUT_nIRQ)) {
            return 0;
        }
        receiveData.word = radioAlphaTRX_Command(0xB000);
        buffer[i] = receiveData.byte.low;
        idOF id;
        id.code = receiveData.byte.low;
        if (id.id.dest != SLAVE_ID && id.id.dest != ID_BROADCAST && i == 0) {
            return 0;
        } else if (receiveData.byte.low == 0) {
            break;
        }
    }
    return i;
}

bool radioAlphaTRX_updateDate(uint8_t date[14]) {
    int8_t i = 1;
    RF_Type_And_nbRemaining paquet;
    paquet.code = BUF[i++];
    if (paquet.ret.typePaquet != HORLOGE) return false;
    if (!srv_TestCheksum(BUF, sizeBuf - 1, BUF[sizeBuf - 1])) return false;
    i++;
    int8_t j = 0;
    for (j; j < sizeBuf - i; j++) date[j] = BUF[i++];
    return true;
}

void radioAlphaTRX_CaptureFrame() {
    if ((sizeBuf = radioAlphaTRX_receive(BUF)) > 0) {
        uint8_t date[14];
        if (radioAlphaTRX_updateDate(date)) {
            LED_STATUS_Y_Toggle();
            radioAlphaTRX_SlaveUpdateDate(date);
        } else {
            LED_STATUS_B_Toggle();
            setLedsStatusColor(LED_BLUE);
            APP_setMsgReceive(1);
            TMR_SetMsgRecuTimeout(TIME_OUT_GET_FRAME); // on demare le timer, car le bufer est probablement remplie 
        }
    }
    //on se remet en ecoute 
    radioAlphaTRX_ReceivedMode();
}

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/

//End file