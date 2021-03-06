// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SERVICE_H
#define	SERVICE_H
/**------------------------>> I N C L U D E <<---------------------------------*/
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../mcc_generated_files/pin_manager.h"
#include "timer.h"
#include "temps.h"

/**------------------------>> D E F I N I T I O N S - G L O B A L <<-----------*/

/*******************************************************************************/
//______________________________D E B U G______________________________________*/
#define UART_DEBUG (1)
/*_____________________________________________________________________________*/

/*******************************************************************************/
//_________________________Radio Alpha TRX Infos_______________________________*/
#define TIME_OUT_WAIT_RQST        30000
#define TIME_OUT_COLLECT_LOG       1000
#define FRAME_LENGTH                 25 // Longueur total d'une trame en octet
#define ERROR_LENGTH                  8
#define HEADER                        5
#define SIZE_DATA                    20
#define TIME_OUT_nIRQ                10 // en ms
#define LAPS                         50 // on attend X ms avant de transmettre un nouveau msg 
#define SEND_HORLOG_TIMEOUT           4 // en +1 min
#define AFTER_SEND_HORLOGE           20 // 20 ms
#define TIME_OUT_GET_FRAME         1500 // temps max, pour que le msg recu soit encore exploitable
// au dela le mster ne m'ecoute pas donc cela ne sert ? rien 
#define NB_ERR_BUF                   10 // nombre d'errerur possible 
#define NB_DATA_BUF                  20 // pour l'instat on dit qye c'est 20 ==>
#define MAX_W                        10 // nombre MAX de paquet a transmettre avant d'attendre un ack 
#define NB_SLAVE                      3
#define MAX_TIMEOUT                  20 // nombre de timeout avant de decider que la liaison avec le slave est couper 
#define MAX_ERROR                    10 // nombre du quel on considere que la communication est interompue entre le slave est le master
#define MAX_TRY_TO_SYNC               5 // le nombre d'essaie avant de decider qu'on est pas connecte
#define MAX_LEVEL_PRIO                3 // 3 niveau de priorite, si l'on veut en ajouter il suiffit de modifier 
#define NB_BEHAVIOR_PER_PRIO          3
#define TIME_LIMIT_OF_CONFIG          6 // avant 6h
#define TIME_LIMIT_TO_GET_INFOS      19 // avant 19h
#define TIME_LIMIT_TO_COLLECT_LOG    00 // avant 00h et apres 19h

//_______________________________TYPE__PAQUET________________________________________*/
//max 16 type de paquet possible coder sur 4bits
#define ERROR       1
#define HORLOGE     2
#define DATA        3
#define INFOS       4
#define NOTHING     5
#define CONFIG      6
#define ACK         7


//_______________________________IF__OF________________________________________*/
// id public 
#define SLAVE_ID_PUB1 18
#define SLAVE_ID_PUB2 36
#define SLAVE_ID_PUB3 39
#define SLAVE_ID_PUB4 37
//------
#define SLAVE_ID_PUB5 36
#define SLAVE_ID_PUB6 35
#define SLAVE_ID_PUB7 39
#define SLAVE_ID_PUB8 40

// id locale 
#define SLAVE1_ID 1
#define SLAVE2_ID 2
#define SLAVE3_ID 3
#define SLAVE4_ID 4
#define BROADCAST_ID 15 // la plus grande valeur possible 

#define MASTER_ID 14
#define STATION 1
/*_____________________________________________________________________________*/



/**------------------------>> M A C R O S <<-----------------------------------*/
////CS => chip select
//#define RF_nSEL_SetLow( ) RF_nSEL_SetLow( )
//#define RF_nSEL_SetHigh( ) RF_nSEL_SetHigh( )
//#define RF_nSEL_Toggle( ) RF_nSEL_Toggle( )
//#define RF_nSEL_GetValue() RF_nSEL_GetValue()
//
//#define RF_nIRQ_SetLow( ) RF_nIRQ_SetLow( )
//#define RF_nIRQ_SetHigh( ) RF_nIRQ_SetHigh( )
//#define RF_nIRQ_Toggle( ) RF_nIRQ_Toggle( )
//#define RF_nIRQ_GetValue() RF_nIRQ_GetValue()

/*_____________________________________________________________________________*/


/**------------------------>> S T R U C T U R E - P A Q U E T <<---------------*/
/* Structure d'une trame d'un message RF:
 *      +------+------+---------+------+-------+-------+------+--------+
 *      | DEST | SRC  | TypeMsg | nbRI | idMsg | size  |  crc |  Data  |
 *      +------+------+---------+------+-------+-------+------+--------+
 *        4bits+4bits +    4bits+4bits + 8bits + 8bist + 8bits  80bits = 25 octets
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
        uint8_t head[5];
        uint8_t Infos[SIZE_DATA];
    }Section;
    
    struct {
        unsigned dest    : 4;   //-----
        unsigned src     : 4;   //    |
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
    uint8_t date [4]; // la date est compresse en 4 octe a la place de 12
    uint32_t dateVal;

    struct {
        unsigned yy : 6;
        unsigned mom : 4;
        unsigned day : 5;
        unsigned h : 5;
        unsigned min : 6;
        unsigned sec : 6;
    } Format;
} Date;

/*____________________________________________________________________________*/


/******************************************************************************/
/******************************************************************************/
/****************** FONCTIONNALITEES COMMUNES AU ALPHA TRX ********************/
/***************************                ***********************************/
/*****************                                 ****************************/

/**
 * genere une somme controle 
 * 
 * @param paquet : le paquet a calculer le checksum
 * @param size : la taille du paquet 
 * @return : somme controle 
 */
uint8_t srv_Checksum(uint8_t* paquet, int size);

/**
 * verifie le paquet recu en calculant la somme controle et en le comparant
 * la somme controle recu 
 * 
 * @param paquet : le paquet a verifier 
 * @param size : sa taille 
 * @param somme_ctrl : la valeure du somme controle recu 
 * @return : 
 *         true : test positif 
 *         false : test negatif 
 */
bool srv_TestCheksum(uint8_t* paquet, int size, uint8_t somme_ctrl);

/**
 * 
 * @param frame
 * @param packetToSend
 * @return 
 */
int8_t srv_CreatePaketRF(Frame frame, uint8_t *packetToSend);

/**
 * 
 * @param buffer
 * @param frameReceive
 * @return 
 */
int8_t srv_DecodePacketRF(uint8_t* buffer, Frame *frameReceive, uint8_t size);



/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/
#endif	/* SERVICE_H */