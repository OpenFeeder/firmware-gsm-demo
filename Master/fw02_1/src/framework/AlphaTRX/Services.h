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
#include <string.h>
#include <stdbool.h>
#include "../../../fw02_1.X/mcc_generated_files/pin_manager.h"
#include "timer.h"
/**------------------------>> D E F I N I T I O N S - G L O B A L <<-----------*/

/*******************************************************************************/
//______________________________D E B U G______________________________________*/
#define UART_DEBUG (1)
//#define ACTIVE_LOG_DATE (1)
/*_____________________________________________________________________________*/

/*******************************************************************************/
//_________________________Radio Alpha TRX Infos_______________________________*/
#define TIME_OUT_WAIT_RQST        30000
#define TIME_OUT_COLLECT_LOG        500
#define FRAME_LENGTH                 40 // Longueur total d'une trame en octet
#define ERROR_LENGTH                  8
#define SIZE_DATA                    35
#define TIME_OUT_nIRQ                10 // en ms
#define LAPS                         50 // on attend X ms avant de transmettre un nouveau msg 
#define SEND_HORLOG_TIMEOUT           4 // en +1 min
#define AFTER_SEND_HORLOGE           20 // 20 ms
#define TIME_OUT_GET_FRAME         1500 // temps max, pour que le msg recu soit encore exploitable
// au dela le mster ne m'ecoute pas donc cela ne sert ? rien 
#define NB_ERR_BUF                   10 // nombre d'errerur possible 
#define NB_BLOCK                     20 // pour l'instat on dit qye c'est 20 ==>
#define MAX_W                        10 // nombre MAX de paquet a transmettre avant d'attendre un ack 

#define NB_SLAVE                      2
#define MAX_TIMEOUT                  20 // nombre de timeout avant de decider que la liaison avec le slave est couper 
#define MAX_ERROR                    10 // nombre du quel on considere que la communication est interompue entre le slave est le master
#define MAX_TRY_TO_SYNC               5 // le nombre d'essaie avant de decider qu'on est pas connecte
#define MAX_LEVEL_PRIO                4 // 3 niveau de priorite, si l'on veut en ajouter il suiffit de modifier 

#define MAX_LEVEL_PRIO 4
#define NB_BEHAVIOR_PER_PRIO 5
#define TIME_LIMIT_OF_CONFIG          6 // avant 6h
#define TIME_LIMIT_TO_GET_INFOS      19 // avant 19h
#define TIME_LIMIT_TO_COLLECT_LOG    00 // avant 00h et apres 19h

/*******************************************************************************/

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
#define SLAVE_ID 1
#define MASTER_ID 14
#define BROADCAST_ID 15
/*_____________________________________________________________________________*/


/**------------------------>> M A C R O S <<-----------------------------------*/
//CS => chip select
//#define RF_nSEL_SetLow( ) CS_SetLow()
//#define RF_nSEL_SetHigh( ) CS_SetHigh()
//#define RF_nSEL_Toggle( ) CS_Toggle()
//#define RF_nSEL_GetValue() CS_GetValue()

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

