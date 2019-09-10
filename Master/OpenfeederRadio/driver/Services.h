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
 *           ENSEMBLE DES SERVICES RENDU AU SYSTEME DE COM RADIO  (.h)
 * _____________________________________________________________________________
 *
 * Titre            : Mise en oeuvre d'un ensemble de fonctionnalitee et de variable commune  
 * Version          : v00
 * Date de creation : 26/05/2019
 * Auteur           : MMADI Anzilane 
 * Contact          : anzilan@hotmail.fr
 * Web page         : 
 * Collaborateur    : ...
 * Processor        : PIC24
 * Tools used       : MPLAB X IDE v5.15 and MPLAB Code Configurator (MCC) Version: 3.36
 * Compiler         : Microchip XC16 v1.35
 * Programmateur    : PICkit 3
 *******************************************************************************
 *******************************************************************************/



// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SERVICE_H
#define	SERVICE_H
/**------------------------>> I N C L U D E <<---------------------------------*/
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/**------------------------>> D E F I N I T I O N S - G L O B A L <<-----------*/

/*******************************************************************************/
//______________________________D E B U G______________________________________*/
#define UART_DEBUG (1)
/*_____________________________________________________________________________*/

/*******************************************************************************/
//_________________________Radio Alpha TRX Infos_______________________________*/
#define TIME_OUT_WAIT_RQST         2000
#define TIME_OUT_COLLECT_LOG        500
#define FRAME_LENGTH                 68 // Longueur total d'une trame en octet
#define HEADER                        6
#define ERROR_LENGTH                  8
#define SIZE_DATA                    61
#define TIME_OUT_nIRQ                10 // en ms
#define LAPS                         50 // on attend X ms avant de transmettre un nouveau msg 
#define SEND_HORLOG_TIMEOUT           4 // en +1 min
#define AFTER_SEND_HORLOGE           20 // 20 ms
#define TIME_OUT_GET_FRAME         1500 // temps max, pour que le msg recu soit encore exploitable
// au dela le mster ne m'ecoute pas donc cela ne sert ? rien 
#define NB_ERR_BUF                   10 // nombre d'errerur possible 
#define NB_BLOCK                     16 // pour l'instat on dit qye c'est 20 ==>
#define MAX_W                        10 // nombre MAX de paquet a transmettre avant d'attendre un ack 

#define NB_SLAVE                      2
#define MAX_TIMEOUT                   5 // nombre de timeout avant de decider que la liaison avec le slave est couper 
#define MAX_ERROR                     5 // nombre du quel on considere que la communication est interompue entre le slave est le master
#define MAX_TRY_TO_SYNC               5 // le nombre d'essaie avant de decider qu'on est pas connecte
#define MAX_LEVEL_PRIO                4 // 3 niveau de priorite, si l'on veut en ajouter il suiffit de modifier 

#define MAX_LEVEL_PRIO 4
#define NB_BEHAVIOR_PER_PRIO 5
#define TIME_LIMIT_OF_CONFIG          6 // avant 6h
#define TIME_LIMIT_TO_GET_INFOS      19 // avant 19h
#define TIME_LIMIT_TO_COLLECT_LOG    00 // avant 00h et apres 19h
#define MIN_ADMISSIBLE_YEAR          18
#define MAX_ADMISSIBLE_YEAR          25
//_______________________________TYPE__PAQUET________________________________________*/
//max 16 type de paquet possible coder sur 4bits
#define ERROR       1
#define HORLOGE     2
#define DATA        3
#define INFOS       4
#define NOTHING     5
#define CONFIG      6
#define ACK         7
#define SLEEP       8


//_______________________________IF__OF________________________________________*/
// id and station 
#define SLAVE1_ID 1
#define SLAVE2_ID 2
#define SLAVE3_ID 3
#define SLAVE4_ID 4
#define BROADCAST_ID 15 // la plus grande valeur possible 

#define MASTER_ID 14
#define STATION 1
/*_____________________________________________________________________________*/


/**------------------------>> M A C R O S <<-----------------------------------*/
//CS => chip select
#define RF_nSEL_SetLow( ) CS_SetLow()
#define RF_nSEL_SetHigh( ) CS_SetHigh()
#define RF_nSEL_Toggle( ) CS_Toggle()
#define RF_nSEL_GetValue() CS_GetValue()

#define RF_nIRQ_SetLow( ) nIRQ_SetLow()
#define RF_nIRQ_SetHigh( ) nIRQ_SetHigh()
#define RF_nIRQ_Toggle( ) nIRQ_Toggle()
#define RF_nIRQ_GetValue() nIRQ_GetValue()

/*_____________________________________________________________________________*/


/**------------------------>> S T R U C T U R E - P A Q U E T <<---------------*/
/* Structure d'une trame d'un message RF:
 *      +------+------+---------+---------+-------+-------+-------+-------+--------+
 *      | DEST | SRC  | N?Site  | TypeMsg |  nbRI | idMsg | size  |  crc  |  Data  |
 *      +------+------+---------+---------+-------+-------+-------+-------+--------+
 *        4bits+4bits +  8 bits +    4bits+4bits  + 8bits + 8bits + 8bits + ?? bits = 
 * 
 * TAILLE EN TETE = 1+1+1+1 = 4 oct
 *
 * ID_DEST / ID_SRC :
 *  . valeur min: 0b0001 (binary)
 *  . valeur max: 0b1111 (binary)
 *
 */
/*_____________________________________________________________________________*/

typedef union {
    uint8_t paquet[FRAME_LENGTH];
    
    struct {
        uint8_t head[HEADER];
        uint8_t Infos[SIZE_DATA];
    }Section;
    
    struct {
        unsigned dest    : 4;   //-----
        unsigned src     : 4;   //    |
        uint8_t station;        //    |
        unsigned typeMsg : 4;   //    |  
        unsigned nbR     : 4;   //     }==> l'en tete  
        uint8_t idMsg;          //    |
        uint8_t size;           //    | // taille de la data reelement envoye 
        uint8_t crc;            //-----
        uint8_t data[SIZE_DATA];
    } Champ;
} Frame;



/**------------------------>> D A T E  F O R M A T <<-------------------------*/
typedef union {
    uint8_t date [5]; // la date est compresse en 4 octe a la place de 12
    uint32_t dateVal;

    struct {
        unsigned yy : 6;
        unsigned mon : 4;
        unsigned day : 5;
        unsigned h : 5;
        unsigned min : 6;
        unsigned sec : 6;
    } Format;
} Date;

/*____________________________________________________________________________*/
/**------------------------>> E R R O R <<------------------------------------*/
typedef enum {
    ERROR_NONE,
    /* Critical errors: the system stops if errors below occurred */
    ERROR_LOW_BATTERY,
    ERROR_LOW_FOOD,
    ERROR_LOW_VBAT,
    ERROR_DOOR_CANT_CLOSE,
    ERROR_RF_MODULE,
    ERROR_GSM_NO_POWER_ON,
    ERROR_GPRS_NO_ATTACHED,
    ERROR_MASTER_LOW_BATTERY,
    ERROR_SLAVE_NO_REQUEST,
    ERROR_CRITICAL,
    ERROR_TOO_MANY_SOFTWARE_RESET = 100

} ERROR_NUMBER;

typedef struct {
    char message[200]; // Error message buffer
    char current_file_name[200];
    uint16_t current_line_number;

    ERROR_NUMBER number;

    struct tm time;

    bool is_data_flush_before_error;

    bool errorSend;
    bool OfInCriticalError;
    
    uint8_t nbErrorSendSms;
} MASTER_ERROR;


/******************************************************************************/
/******************************************************************************/
/****************** FONCTIONNALITEES COMMUNES AU ALPHA TRX ********************/
/***************************                ***********************************/
/*****************                                 ****************************/

/**
 * 
 * @param year
 * @param month
 * @param day
 * @param hour
 * @param minute
 * @param second
 * @return true  : 
 * @return false :
 */
bool setDateTime(int year, int month, int day, int hour, int minute, int second);

/**
 * 
 * @return true  :  
 * @return false :
 */
bool getDateTime(void);

/**
 * 
 * @param time
 */
void printDateTime(struct tm time);

//********
//ERROR
//********
/**
 * 
 */
void printError(void);

/**
 * 
 */
void clearError(void);
/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/
#endif	/* SERVICE_H */