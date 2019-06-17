/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

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
 * 
 * 
 * 
 * Information sur le module ALPHA TRX868 :
 * 
 * PIN CONNECTIONS (DIL connector)
 *      Pin Number | Pin name | Function
 *      ---------------------------------------------------------
 *               1 |      SDO | Serial data output with bus hold 
 *               2 |     nIRQ | Interrupts request output (active low) --> vers INT0
 *               3 | FSK/DATA/nFFS | Transmit FSK data input / Received data output (FIFO not used)/ FIFO select
 *               6 |     nRES | Reset Input (active low)
 *               7 |      GND | Power ground
 *               8 |      ANT | Antenna Connection
 *               9 |      VDD | Positive power supply (2.2V to 3.8V)
 *              10 |      GND | Power ground
 *              11 | nINT/VDI | Interrupt input (active low)/Valid data indicator
 *              12 |      SDI | SPI data input (MOSI)
 *              13 |      SCK | SPI clock input (SCLK)
 *              14 |     nSEL | Chip select (active low)
 *
 * * Internal POR timeout: 100 ms
 * Crystal oscillator startup time: 5 ms
 * Max Timing SPI CLK: 50 ns --> FCLK: 20 MHz
 * Max node:
 * FSK frequency deviation (Programmable in 15 KHz steps)
 * . Programmable TX frequency deviation (from 15 to 240 KHz)
 * Programmable receiver bandwidth (from 67 to 400 KHz)
 * Il n'est pas necessaire de modifier le cablage du module lors de l'utilisation
 * d'applications a sauts de frequence (frequency-hopping applications), le changement
 * de Fq se fait via le registre "Frequency Setting Command"
 * 433 MHz band, 2.5 KHz step: 430.24 <= Frequency >= 439.75
 *
 * nFFS (pin 3)
 * This code will work at non FIFO transmission mode
 * --> mettre la broche a VDD pour desactiver l'acces directe au registre FIFO (se comporte comme un CS)
 * 
 * nIRQ pin low on the following events :
 * . Power-On Reset (POR)
 * . Wake-up timer timeout (WKUP)
 * . TX register is ready to receive the next byte (RGIT)
 * . RX FIFO has received the pre-programmed amount of bits (FFIT)
 * . RX FIFO overflow (FFOV)/TX register under run (RGUR)
 * . Negative pulse on the interrupt input pin nINT (EXT)
 * . Supply voltage below the pre-programmed value is detected (LBD)
 * --> FFIT and FFOV are applicable when the RX FIFO is enable.
 * --> RGIT and RGUR are applicable only when the TX register is enable.
 * --> To identify the source of the IT, the status bits should be read out.
 *
 * SPI software :
 * le module RF fait la lecture du signal MOSI sur le front montant de l'horloge (SCLK)
 *
 * Lecture du registre Status lorsque la broche nIRQ passe a 0
 */

/**
 * Software SPI pin attribution :
 * Pin | Name | SPI Signal | Signal Name 
 * ----+------+------------+--------------------
 *  14 | nSEL |     SS     | Slave Select
 *   1 |  SDO |    MISO    | Master-In-Slave-Out
 *  12 |  SDI |    MOSI    | Master-Out-Slave-In
 *  14 |  SCK |    SCLK    | Serial Clock
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

/**------------------------>> I N C L U D E <<---------------------------------*/
#include <xc.h> // include processor files - each processor file is guarded.  
#include "mcc_generated_files/pin_manager.h"
#include "Services.h"

/******************************************************************************/
/******************************************************************************/
/****************** Driver ALPHA TRX433S - Interface SPI **********************/
/***************************                ***********************************/
/*****************                                       **********************/

/**------------------------>> D E F I N I T I O N S <<-------------------------*/

/** Detection du signal nIRQ du module RF */

/** Pin 2 "nIRQ": Interrupts ReQuest output (active low) */
// nIRQ_Pin_Read() --> nIRQ_RF_GetValue(), MCU INPUT pin: lecture de la broche nIRQ du module Transceiver


/** ------------------------>> ALPHA TRX433S Register <<-----------------------*/

// Control Register Default Values after Power-On Reset (POR):
// ---- Control Register -------------- Power-On Reset Value -------------- Control Command -------------------- Related control bits
#define CFG_SET_CMD_POR                 (0x8008)    // 1000 0000 0000 1000, Configuration Setting Command ...... el, ef, b1 to b0, x3 to x0
#define PWR_MGMT_CMD_POR                (0x8208)    // 1000 0010 0000 1000, Power Management Command ........... er, ebb, et, es, ex, eb, ew, dc
//#define FQ_SET_CMD_POR                  (0xA680)    // 1010 0110 1000 0000, Frequency Setting Command .......... f11 to f0
#define FQ_SET_CMD_POR                  (0xA640)    // 1010 0110 1000 0000, Frequency Setting Command .......... f11 to f0
#define DATA_RATE_CMD_POR               (0xC623)    // 1100 0110 0010 0011, Data Rate Command .................. cs, r6 to r0
#define RX_CTRL_CMD_POR                 (0x9080)    // 1001 0000 1000 0000, Receiver Control Command ........... p16, d1 to d0, i2 to i0, g1 to g0, r2 to r0
#define DATA_FILTER_CMD_POR             (0xC22C)    // 1100 0010 0010 1100, Data Filter Command ................ al, ml, s, f2 to f0
#define FIFO_RST_MODE_CMD_POR           (0xCA80)    // 1100 1010 1000 0000, FIFO and Reset Mode Command ........ f3 to f0, sp, ff, al, dr
#define SYNC_PATT_CMD_POR               (0xCED4)    // 1100 1110 1101 0100, Synchron pattern Command ........... b7 to b0
//#define RX_FIFO_READ_CMD_POR            (0xB000)    // 1011 0000 0000 0000, Receiver FIFO Read Command
#define RX_FIFO_READ_CMD                (0xB000)    // 1011 0000 0000 0000, Receiver FIFO Read Command
#define AFC_CMD_POR                     (0xC4F7)    // 1100 0100 1111 0111, AFC Command ........................ a1 to a0, rl1 to rl0, st, fi, oe, en
#define TX_CONF_CTRL_CMD_POR            (0x9800)    // 1001 1000 0000 0000, TX Configuration Control Command ... mp, m3 to m0, p2 to p0
#define PLL_SET_CMD_POR                 (0xCC77)    // 1100 1100 0111 0111, PLL Setting Command ................ ob1 to ob0, ddit, dly, bw0
#define TX_REG_WRITE_CMD_POR            (0xB8AA)    // 1011 1000 1010 1010, Transmitter Register Write Command . t7 to t0
#define WKUP_TIMER_CMD_POR              (0xE196)    // 1110 0001 1001 0110, Wake-Up Timer Command .............. r4 to r0, m7 to m0
#define LOW_DUTY_CYCLE_CMD_POR          (0xC80E)    // 1100 1000 0000 1110, Low Duty-Cycle Command ............. d6 to d0, en
#define LOW_BAT_AND_MCU_CLK_DIV_CMD_POR (0xC000)    // 1100 0000 0000 0000, Low Bat. Detect. and MCU Clock Div . d2 to d0, v3to v0
//#define STATUS_READ_CMD_POR             (0x0000)    // 0000 0000 0000 0000, Status Read Command
#define STATUS_READ_CMD                 (0x0000)    // 0000 0000 0000 0000, Status Read Command
//delay
#define SLEEP_AFTER_INIT (850000)
#define SEND_TIME_OUT (10) // pour l'instant on es a 10ms
/** ------------------------>> CFG_SET_CMD_POR  <<----------------------------*/

#define FQ_RESTRICTION_LOW              // Preprocessor macros definition --> 119 Fq possible
//#define FQ_RESTRICTION_MEDIUM           // Preprocessor macros definition --> 238 Fq possible
//#define FQ_RESTRICTION_HIGH             // Preprocessor macros definition --> 477 Fq possible

#define FQ_SET_CMD_CODE     (0xA000)    // Frequency Setting Command

typedef enum {
    FQ_NOT_DEFINE, // 000: Frequence non d?finit, lecture des microswitch == 0
    FQ_001, FQ_002, FQ_003, FQ_004, FQ_005, FQ_006, FQ_007, FQ_008, FQ_009, FQ_010,
    FQ_011, FQ_012, FQ_013, FQ_014, FQ_015, FQ_016, FQ_017, FQ_018, FQ_019, FQ_020,
    FQ_021, FQ_022, FQ_023, FQ_024, FQ_025, FQ_026, FQ_027, FQ_028, FQ_029, FQ_030,
    FQ_031, FQ_032, FQ_033, FQ_034, FQ_035, FQ_036, FQ_037, FQ_038, FQ_039, FQ_040,
    FQ_041, FQ_042, FQ_043, FQ_044, FQ_045, FQ_046, FQ_047, FQ_048, FQ_049, FQ_050,
    FQ_051, FQ_052, FQ_053, FQ_054, FQ_055, FQ_056, FQ_057, FQ_058, FQ_059, FQ_060,
    FQ_061, FQ_062, FQ_063, FQ_064, FQ_065, FQ_066, FQ_067, FQ_068, FQ_069, FQ_070,
    FQ_071, FQ_072, FQ_073, FQ_074, FQ_075, FQ_076, FQ_077, FQ_078, FQ_079, FQ_080,
    FQ_081, FQ_082, FQ_083, FQ_084, FQ_085, FQ_086, FQ_087, FQ_088, FQ_089, FQ_090,
    FQ_091, FQ_092, FQ_093, FQ_094, FQ_095, FQ_096, FQ_097, FQ_098, FQ_099, FQ_100,
    FQ_101, FQ_102, FQ_103, FQ_104, FQ_105, FQ_106, FQ_107, FQ_108, FQ_109, FQ_110,
    FQ_111, FQ_112, FQ_113, FQ_114, FQ_115, FQ_116, FQ_117, FQ_118, FQ_119,

    /* Ajout des frequences en restriction moyenne et haute */
#if defined(FQ_RESTRICTION_MEDIUM) || defined(FQ_RESTRICTION_HIGH)
    FQ_120,
    FQ_121, FQ_122, FQ_123, FQ_124, FQ_125, FQ_126, FQ_127, FQ_128, FQ_129, FQ_130,
    FQ_131, FQ_132, FQ_133, FQ_134, FQ_135, FQ_136, FQ_137, FQ_138, FQ_139, FQ_140,
    FQ_141, FQ_142, FQ_143, FQ_144, FQ_145, FQ_146, FQ_147, FQ_148, FQ_149, FQ_150,
    FQ_151, FQ_152, FQ_153, FQ_154, FQ_155, FQ_156, FQ_157, FQ_158, FQ_159, FQ_160,
    FQ_161, FQ_162, FQ_163, FQ_164, FQ_165, FQ_166, FQ_167, FQ_168, FQ_169, FQ_170,
    FQ_171, FQ_172, FQ_173, FQ_174, FQ_175, FQ_176, FQ_177, FQ_178, FQ_179, FQ_180,
    FQ_181, FQ_182, FQ_183, FQ_184, FQ_185, FQ_186, FQ_187, FQ_188, FQ_189, FQ_190,
    FQ_191, FQ_192, FQ_193, FQ_194, FQ_195, FQ_196, FQ_197, FQ_198, FQ_199, FQ_200,
    FQ_201, FQ_202, FQ_203, FQ_204, FQ_205, FQ_206, FQ_207, FQ_208, FQ_209, FQ_210,
    FQ_211, FQ_212, FQ_213, FQ_214, FQ_215, FQ_216, FQ_217, FQ_218, FQ_219, FQ_220,
    FQ_221, FQ_222, FQ_223, FQ_224, FQ_225, FQ_226, FQ_227, FQ_228, FQ_229, FQ_230,
    FQ_231, FQ_232, FQ_233, FQ_234, FQ_235, FQ_236, FQ_237, FQ_238,
#endif

    /* Ajout des frequences en restriction haute */
#if defined(FQ_RESTRICTION_HIGH)
    FQ_239, FQ_240,
    FQ_241, FQ_242, FQ_243, FQ_244, FQ_245, FQ_246, FQ_247, FQ_248, FQ_249, FQ_250,
    FQ_251, FQ_252, FQ_253, FQ_254, FQ_255, FQ_256, FQ_257, FQ_258, FQ_259, FQ_260,
    FQ_261, FQ_262, FQ_263, FQ_264, FQ_265, FQ_266, FQ_267, FQ_268, FQ_269, FQ_270,
    FQ_271, FQ_272, FQ_273, FQ_274, FQ_275, FQ_276, FQ_277, FQ_278, FQ_279, FQ_280,
    FQ_281, FQ_282, FQ_283, FQ_284, FQ_285, FQ_286, FQ_287, FQ_288, FQ_289, FQ_290,
    FQ_291, FQ_292, FQ_293, FQ_294, FQ_295, FQ_296, FQ_297, FQ_298, FQ_299, FQ_300,
    FQ_301, FQ_302, FQ_303, FQ_304, FQ_305, FQ_306, FQ_307, FQ_308, FQ_309, FQ_310,
    FQ_311, FQ_312, FQ_313, FQ_314, FQ_315, FQ_316, FQ_317, FQ_318, FQ_319, FQ_320,
    FQ_321, FQ_322, FQ_323, FQ_324, FQ_325, FQ_326, FQ_327, FQ_328, FQ_329, FQ_330,
    FQ_331, FQ_332, FQ_333, FQ_334, FQ_335, FQ_336, FQ_337, FQ_338, FQ_339, FQ_340,
    FQ_341, FQ_342, FQ_343, FQ_344, FQ_345, FQ_346, FQ_347, FQ_348, FQ_349, FQ_350,
    FQ_351, FQ_352, FQ_353, FQ_354, FQ_355, FQ_356, FQ_357, FQ_358, FQ_359, FQ_360,
    FQ_361, FQ_362, FQ_363, FQ_364, FQ_365, FQ_366, FQ_367, FQ_368, FQ_369, FQ_370,
    FQ_371, FQ_372, FQ_373, FQ_374, FQ_375, FQ_376, FQ_377, FQ_378, FQ_379, FQ_380,
    FQ_381, FQ_382, FQ_383, FQ_384, FQ_385, FQ_386, FQ_387, FQ_388, FQ_389, FQ_390,
    FQ_391, FQ_392, FQ_393, FQ_394, FQ_395, FQ_396, FQ_397, FQ_398, FQ_399, FQ_400,
    FQ_401, FQ_402, FQ_403, FQ_404, FQ_405, FQ_406, FQ_407, FQ_408, FQ_409, FQ_410,
    FQ_411, FQ_412, FQ_413, FQ_414, FQ_415, FQ_416, FQ_417, FQ_418, FQ_419, FQ_420,
    FQ_421, FQ_422, FQ_423, FQ_424, FQ_425, FQ_426, FQ_427, FQ_428, FQ_429, FQ_430,
    FQ_431, FQ_432, FQ_433, FQ_434, FQ_435, FQ_436, FQ_437, FQ_438, FQ_439, FQ_440,
    FQ_441, FQ_442, FQ_443, FQ_444, FQ_445, FQ_446, FQ_447, FQ_448, FQ_449, FQ_450,
    FQ_451, FQ_452, FQ_453, FQ_454, FQ_455, FQ_456, FQ_457, FQ_458, FQ_459, FQ_460,
    FQ_461, FQ_462, FQ_463, FQ_464, FQ_465, FQ_466, FQ_467, FQ_468, FQ_469, FQ_470,
    FQ_471, FQ_472, FQ_473, FQ_474, FQ_475, FQ_476,
#endif

    NUMBER_OF_FQ_AVAILABLE // lire cette valeur -1, permet de connaitre le nombre de frequence maximum disponible
} SELECT_FQ_VAL;

/** ------------------------>> CFG_SET_CMD_POR  <<----------------------------*/

/**
 * Configuration Setting Command
 * recommended : 
 *      mode RX FIFO : (0x8067)
 *      mode TX REG  : (0x80A7)
 */
typedef union {
    uint16_t Val;
    uint8_t v[2];

    struct {
        uint8_t LB;
        uint8_t HB;
    } byte;

    struct {
        unsigned b0_x0 : 1; // select crystal load capacitor
        unsigned b1_x1 : 1;
        unsigned b2_x2 : 1;
        unsigned b3_x3 : 1;
        unsigned b4_b0 : 1; // select band
        unsigned b5_b1 : 1;
        unsigned b6_ef : 1; // Enable RX FIFO buffer
        unsigned b7_el : 1; // Enable TX register
        unsigned b8to15_cmd_code : 8; // Command code
    } bits;

    struct {
        unsigned SelectCrystalCapacitor : 4; // select crystal load capacitor
        unsigned SelectBand : 2; // select band
        unsigned EnableRXFIFO : 1; // Enable RX FIFO buffer
        unsigned EnableTXREG : 1; // Enable TX register
        unsigned cmd_code : 8; // Command code
    } REGbits;
} CFG_SET_CMD_VAL;

/**
 * x3..x0: select crystal load capacitor
 * recommended : 
 *      LOAD_C_12_0pF ==> 0111
 */

typedef enum {
    LOAD_C_8_5pF, // 0000:  8.5 pF
    LOAD_C_9_0pF, // 0001:  9.0 pF
    LOAD_C_9_5pF, // 0010:  9.5 pF
    LOAD_C_10_0pF, // 0011: 10.0 pF
    LOAD_C_10_5pF, // 0100: 10.5 pF
    LOAD_C_11_0pF, // 0101: 11.0 pF
    LOAD_C_11_5pF, // 0110: 11.5 pF
    LOAD_C_12_0pF, // 0111: 12.0 pF
    LOAD_C_12_5pF, // 1000: 12.5 pF
    LOAD_C_13_0pF, // 1001: 13.0 pF
    LOAD_C_13_5pF, // 1010: 13.5 pF
    LOAD_C_14_0pF, // 1011: 14.0 pF
    LOAD_C_14_5pF, // 1100: 14.5 pF
    LOAD_C_15_0pF, // 1101: 15.0 pF
    LOAD_C_15_5pF, // 1110: 15.5 pF
    LOAD_C_16_0pF // 1111: 16.0 pF
} SELECT_CRYSTAL_CAPACITOR;

/**
 * b1..b0: select band
 * USE : 
 *      868 MHz
 */
typedef enum _SELECT_BAND_T {
    BAND_RESERVED = 0, // 00: Reserved
    BAND_433, // 01: 433 MHz
    BAND_868, // 10: 868 MHz
    BAND_915 // 11: 915 MHz
} SELECT_BAND;


/** ------------------------>> PWR_MGMT_CMD_POR <<----------------------------*/

/**
 * Power Management Command
 * recommended : 
 *      mode RX : (0x82C9)
 *      mode TX : (0x8239)
 */
typedef union {
    uint16_t Val;
    uint8_t v[2];

    struct {
        uint8_t LB;
        uint8_t HB;
    } byte;

    struct {
        unsigned b0_dc : 1; // Disable clock output of CLK pin
        unsigned b1_ew : 1; // Enable wake-up timer
        unsigned b2_eb : 1; // Enable low battery detector
        unsigned b3_ex : 1; // Enable crystal oscillator
        unsigned b4_es : 1; // Enable synthesizer
        unsigned b5_et : 1; // Enable transmitter
        unsigned b6_ebb : 1; // Enable base band block
        unsigned b7_er : 1; // Enable receiver
        unsigned b8to15_cmd_code : 8; // Command code
    } bits;
} PWR_MGMT_CMD_VAL;

/** ------------------------>> FQ_SET_CMD_POR <<------------------------------*/

/**
 * Frequency Setting Command
 * recommended : (0xA640)
 */
typedef union {
    uint16_t Val;
    uint8_t v[2];

    struct {
        uint8_t LB;
        uint8_t HB;
    } byte;

    struct {
        unsigned b0_f0 : 1; // Set operation frequency
        unsigned b1_f1 : 1;
        unsigned b2_f2 : 1;
        unsigned b3_f3 : 1;
        unsigned b4_f4 : 1;
        unsigned b5_f5 : 1;
        unsigned b6_f6 : 1;
        unsigned b7_f7 : 1;
        unsigned b8_f8 : 1;
        unsigned b9_f9 : 1;
        unsigned b10_f10 : 1;
        unsigned b11_f11 : 1;
        unsigned b12to15_cmd_code : 4; // Command code
    } bits;

    struct {
        unsigned SetOperationFrequency_L : 8; // Set operation frequency
        unsigned SetOperationFrequency_H : 4; // Set operation frequency
        unsigned cmd_code : 4; // Command code
    } REGbits;
} FQ_SET_CMD_VAL;

// > 10 MHz crystal used :
// f11..f0: Set operation frequency:
//          2x^y12= 4096
// 433band: Fc=430+F*0.0025 MHz
//             430+36*0.0025=   430.09   MHz
//             430+96*0.0025=   430.24   MHz (433 MHz band, 2.5 KHz step)
//             430+3903*0.0025= 439.7575 MHz
// POR_FQ = 0b011010000000= 1664
//             430+1664*0.0025= 434.16   MHz
// 868band: Fc=860+F*0.0050 MHz
// 915band: Fc=900+F*0.0075 MHz
// Fc is carrier frequency and F is the frequency parameter. 36 < F < 3903

/**
 * Data Rate Command:
 * (POR= 0x23 = 0b 0010 0011 = 35d)
 * BR= 10000000/29/(r+1)/(1+cs*7)
 * BR= 10000000/29/(35+1)/(1+0*7)= 9578.544 baud (avec 0xC623)
 * BR= 10000000/29/(71+1)/(1+0*7)= 4789.272 baud (avec 0xC647)
 */

/** ------------------------>> RX_CTRL_CMD_POR <<------------------------------*/

/**
 * Receiver Control Command
 * recommended : 
 */
typedef union {
    uint16_t Val;
    uint8_t v[2];

    struct {
        uint8_t LB;
        uint8_t HB;
    } byte;

    struct {
        unsigned b0_r0 : 1; // Set operation frequency
        unsigned b1_r1 : 1;
        unsigned b2_r2 : 1;
        unsigned b3_g0 : 1;
        unsigned b4_g1 : 1;
        unsigned b5_i0 : 1;
        unsigned b6_i1 : 1;
        unsigned b7_i2 : 1;
        unsigned b8_d0 : 1;
        unsigned b9_d1 : 1;
        unsigned b10_p16 : 1;
        unsigned b11to15_cmd_code : 5; // Command code
    } bits;

    struct {
        unsigned RSSIsetth : 3; // Bits 2-0 (r2 to r0): RSSI detector threshold
        unsigned GLNA : 2; // Bits 4-3 (g1 to g0): LNA gain select: "GLNA"
        unsigned RX_BW_Select : 3; // Bits 7-5 (i2 to i0): Receiver baseband bandwidth (BW) select
        unsigned VDI_RespSetting : 2; // Bits 9-8 (d1 to d0): VDI (Valid Data Indicator) signal response time setting
        unsigned Pin16_function : 1; // Bit 10 (p16): select function of pin 16
        unsigned cmd_code : 5; // Command code
    } REGbits;
} RX_CTRL_CMD_VAL;

/**
 * Bits 2-0 (r2 to r0): RSSI detector threshold:
 * The RSSI threshold depends on the LNA gain, the real RSSI threshold can be calculated:
 * RSSIth = RSSIsetth + GLNA
 */
typedef enum {
    RSSIsetth_n103, // 000: -103
    RSSIsetth_n97, // 001: -97
    RSSIsetth_n91, // 010: -91
    RSSIsetth_n85, // 011: -85
    RSSIsetth_n79, // 100: -79
    RSSIsetth_n73, // 101: -73
    RSSI_RESERVED_a, // 110: Reserved
    RSSI_RESERVED_b // 111: Reserved
} RSSI_THRESHOLD;

/**
 * Bits 4-3 (g1 to g0): LNA gain select: "GLNA"
 */
typedef enum {
    GAIN_0_dB, // 00: GLNA =   0 dB
    GAIN_n6_dB, // 01: GLNA =  -6 dB
    GAIN_n14_dB, // 10: GLNA = -14 dB
    GAIN_n20_dB // 11: GLNA = -20 dB
} LNA_GAIN_SELECT;

// Bits 7-5 (i2 to i0): Receiver baseband bandwidth (BW) select:

typedef enum _SELECT_BW_BASEBAND_T {
    BW_RESERVED_a, // 000: Reserved
    BW_400_KHz, // 001: 400 KHz
    BW_340_KHz, // 010: 340 KHz
    BW_270_KHz, // 011: 270 KHz
    BW_200_KHz, // 100: 200 KHz
    BW_134_KHz, // 101: 134 KHz
    BW_67_KHz, // 110:  67 KHz
    BW_RESERVED_b // 111: Reserved
} _SELECT_BW_BASEBAND_T;

// Bits 9-8 (d1 to d0): VDI (Valid Data Indicator) signal response time setting:

typedef enum _SELECT_VDI_RESPONSE_TIMING_T {
    FAST, // 00
    MEDIUM, // 01
    SLOW, // 10
    ALWAYS_ON // 11
} SELECT_VDI_RESPONSE_TIMING;

// Bit 10 (p16): select function of pin 16
// recommended : 0
#define INTERRUPT_INPUT     (0)     // Pin 16 function select is Interrupt input
#define VDI_OUTPUT          (1)     // Pin 16 function select is VDI output

/** 
 * Data Filter Command 
 * recommended : 
 */
// Bit 4 (s): Select the type of the data filter:
#define DIGITAL_FILTER      (0)     // Digital filter
#define ANALOG_RC_FILTER    (1)     // Analog RC filter

/** ------------------------>> FIFO_RST_MODE_CMD_POR <<------------------------*/

/** 
 * FIFO and Reset Mode Command 
 * recommended : 
 *      (1) : (0xCA81)
 *      (2) : (0xCA83)
 */
typedef union {
    uint16_t Val;
    uint8_t v[2];

    struct {
        uint8_t LB;
        uint8_t HB;
    } byte;

    struct {
        unsigned b0_dr : 1; // Disables the highly sensitive RESET mode
        unsigned b1_ff : 1; // FIFO fill will be enabled after synchron pattern reception. The FIFO fill stops when this bit is cleared.
        unsigned b2_al : 1; // Set the input of the FIFO fill start condition
        unsigned b3_sp : 1; // Select the length of the synchron pattern
        unsigned b4_f0 : 1; // FIFO IT level. The FIFO generates IT when the number of received data bits reaches this level
        unsigned b5_f1 : 1;
        unsigned b6_f2 : 1;
        unsigned b7_f3 : 1;
        unsigned b8to15_cmd_code : 8; // Command code
    } bits;

    struct {
        unsigned SetOperationFrequency_L : 8; // Set operation frequency
        unsigned SetOperationFrequency_H : 4; // Set operation frequency
        unsigned cmd_code : 4; // Command code
    } REGbits;
} FIFO_RST_MODE_CMD_VAL;

// Synchron pattern Command
// Bit 3 (sp):    Select the length of the synchron pattern:
#define SYNCHRON_PATTERN_BYTE_1_0  (0)     // Two bytes 0x2DD4, for Synchron Pattern (Byte1+Byte0)
#define SYNCHRON_PATTERN_BYTE_0    (1)     // One byte 0xD4, for Synchron Pattern (Byte0)


/* RX_FIFO_READ: Receiver FIFO Read */
// Receiver FIFO Read Command

typedef union {
    uint16_t Val;
    uint8_t v[2];

    struct {
        uint8_t LB;
        uint8_t HB;
    } byte;

    struct {
        uint8_t FIFO;
        uint8_t FFIT;
    } REGbyte;
} RX_FIFO_READ_CMD_VAL;

/** ------------------------>> STATUS_READ_POR <<------------------------------*/
//...
// AFC Command
// TX Configuration Control Command
// PLL Setting Command
// Transmitter Register Write Command
// Wake-Up Timer Command
// Low Duty-Cycle Command
// Low Battery Detector and Microcontroller Clock Divider Command

/* 
 * Status Read Command 
 * recommended : (0x0000)
 *      
 */
typedef union {
    uint16_t Val;
    uint8_t v[2];

    struct {
        uint8_t LB;
        uint8_t HB;
    } byte;

    struct {
        unsigned b0_OFFS_b0 : 1; // Measured Offset Frequency Value (3 bits)
        unsigned b1_OFFS_b1 : 1;
        unsigned b2_OFFS_b2 : 1;
        unsigned b3_OFFS_Sign : 1; // Measured Offset Frequency Sign Value 1='+', 0='-'
        unsigned b4_ATGL : 1; // Toggling in each AFC cycle
        unsigned b5_CRL : 1; // Clock Recovery Locked status
        unsigned b6_DQD : 1; // Data Quality Detector status
        unsigned b7_ATS_RSSI : 1; // er=1: Received signal strength indicator; er=0: Antenna tuning signal strength
        unsigned b8_ATSS : 1; // Antenna tuning signal strength
        unsigned b9_FFEM : 1; // FIFO empty status
        unsigned b10_LBD : 1; // Low battery detect status
        unsigned b11_EXT : 1; // Interrupt on external source status
        unsigned b12_WKUP : 1; // Wakeup timer overflow status
        unsigned b13_RGUR_FFOV : 1; // TX Register under run or RX FIFO Overflow status
        unsigned b14_POR : 1; // Power on reset status
        unsigned b15_RGIT_FFIT : 1; // TX ready for next byte or FIFO received data status
    } bits;

    struct {
        unsigned FreqOffset : 3; // Measured Offset Frequency Value
        unsigned FreqOffsetSign : 1; // Measured Offset Frequency Sign Value 1='+', 0='-'
        unsigned : 4; // none
        unsigned : 8; // none
    } REGbits;
} STATUS_READ_VAL;

/** ------------------------>> FIFO_RST_MODE_CMD_POR <<------------------------*/
//FIXME: Correction des testes de 2 bits vers 1 bits

typedef union {
    uint16_t word;

    struct {
        uint8_t low;
        uint8_t high;
    } byte;

} WORD_VAL_T;


/*------------------------> E X T E R N S <-------------------------------------*/

// Global Variables for internal register RF module:
extern CFG_SET_CMD_VAL RF_ConfigSet; // Configuration Setting Command
extern PWR_MGMT_CMD_VAL RF_PowerManagement; // Power Management Command
extern FQ_SET_CMD_VAL RF_FrequencySet; // Frequency Setting Command
extern RX_CTRL_CMD_VAL RF_ReceiverControl; // Receiver Control Command 
extern RX_FIFO_READ_CMD_VAL RF_FIFO_Read; // Receiver FIFO Read
extern FIFO_RST_MODE_CMD_VAL RF_FIFOandResetMode; // FIFO and Reset Mode Command
extern STATUS_READ_VAL RF_StatusRead; // Status Read Command

/*------------------------> P R I V A T E   P R O T O T Y P E S <--------------*/
// Global Function Prototypes:

/**
 * \brief Initialise le module radio
 */
void radioAlphaTRX_Init(void);

/**
 * Transmission d'une commande du MCU au modul Alpha TRX par liaison SPI
 * Cette fonction est utilisee, autant pour l'envoie que la reception 
 * 
 * @param cmd_write : commande a transmettre 
 * @return : l'etat du registre
 */
uint16_t radioAlphaTRX_Command(uint16_t cmdWrite);

/**
 * Initialiser les conditions d'attente d'un nouveau message
 * Le module doit etre en mode RX.
 */
void radioAlphaTRX_ReceivedMode(void);

/**
 * Initialiser les conditions d'envoie d'un nouveau message, le module passe en mode TX.
 * Avant d'appeler cette fonction, vous devez avoir initialiser la trame "Frame_RF_Send"
 * L'envoie des donnees de la trame se fait par la fonction RF12_nIRQ_Service()
 * 
 * @return : 0 init ko 1 si ok 
 */
int8_t radioAlphaTRX_SendMode(void);

/**
 * transmet un octet seulement si le nIRQ est a l'etat bas 
 * 
 * @param data_send : l'octet a transmettre 
 * @param timeout : delais apres quoi on gener une erreur de transmsission
 * @return : 0 si timeout 1 si RGIT OK
 */
int8_t radioAlphaTRX_SendByte(uint8_t dataToSend, int8_t timeout);

/**
 * transmission flot d'octets a la suite 
 * 
 * @param bytes : tableau d'octet
 * @param size : la taille du tableau 
 * @param timeout : delais apres quoi on gener une erreur de transmsission 
 * @return : nombre d'octets effectivement transmis 
 */
int8_t radioAlphaTRX_SendData(uint8_t* bytes, int8_t size);

/**
 * 
 * @param timeout : le nombre de fois qu'il faut attendre le nIRQ en cas de non reponse
 * @return : 
 *         1 : on n a pas depasse le delais imposer pour l'attente
 *         0 : sinon  
 */
int8_t radioAlphaTRX_WaitLownIRQ(int timeout);

/**
 * cette procedure est appele par le hundler d'interuption, 
 * a savoir lorsque le nIRQ est a l'etat bas et le bit 15 du regstre status est 
 * egale a 1, et qu'on ne se trouve pas en mode emission 
 * 
 */
void radioAlphaTRX_CaptureFrame();


/**
 * 
 * @param mode_rf
 */
void radioAlphaTRX_SetSendMode(int8_t modeRF);
/**
 * 
 * @return 
 */
int8_t radioAlphaTRX_IsSendMode();

/**
 * <pres condition : receive_msg == 1 > avant l'apelle de cette fonction 
 * @return : le contenue du buffer
 */
uint8_t * radioAlphaTRX_ReadBuf();

/**
 * 
 * @return : la taille du buffer indice
 */
uint8_t radioAlphaTRX_GetSizeBuf();


/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/

#endif	/* XC_HEADER_TEMPLATE_H */