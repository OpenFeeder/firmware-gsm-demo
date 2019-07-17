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
#include <stdbool.h>
#include "../mcc_generated_files/pin_manager.h"
#include "timer.h"
#include "../temps/temps.h"
/**------------------------>> D E F I N I T I O N S - G L O B A L <<-----------*/

/*******************************************************************************/
//______________________________D E B U G______________________________________*/
#define UART_DEBUG (1)
/*_____________________________________________________________________________*/

/*******************************************************************************/
//_________________________Radio Alpha TRX Infos_______________________________*/
#define TIME_OUT_WAIT_RQST         1000
#define FRAME_LENGTH                 50 // Longueur total d'une trame en octet
#define ERROR_LENGTH                  8
#define SIZE_DATA                    36
#define TIME_OUT_nIRQ                 2 // 2 ms
#define LAPS                         50 // on attend X ms avant de transmettre un nouveau msg 
#define SEND_HORLOG_TIMEOUT        5000 // 1 min
#define AFTER_SEND_HORLOGE           20 // 20 ms
#define TIME_OUT_GET_FRAME         1500 // temps max, pour que le msg recu soit encore exploitable
                                        // au dela le mster ne m'ecoute pas donc cela ne sert ? rien 
#define NB_ERR_BUF                   10 // nombre d'errerur possible 
#define NB_DATA_BUF                  20 // pour l'instat on dit qye c'est 20 ==>
#define MAX_W                        10 // nombre MAX de paquet a transmettre avant d'attendre un ack 
#define NB_SLAVE                      1
#define MAX_TIMEOUT                  20 // nombre de timeout avant de decider que la liaison avec le slave est couper 
#define MAX_ERROR                    10 // nombre du quel on considere que la communication est interompue entre le slave est le master
#define MAX_TRY_TO_SYNC              5 // le nombre d'essaie avant de decider qu'on est pas connecte


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
// id and station 
#define SLAVE1_ID 1
#define SLAVE2_ID 2
#define SLAVE3_ID 3
#define SLAVE4_ID 4
#define ID_BROADCAST 15

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
 *      +--------+--------+------------+----------+--------------+----------+
 *      |ID_DEST | ID_SRC | ID_Message | Typr_MSG | Data_Message | Checksum |
 *      +--------+--------+------------+----------+--------------+----------+
 * BYTE =   2    +    2   +     1      +    1     +     MAX = 40 +    1     
 * 
 * TAILLE EN TETE = 2+2+1+1+1 = 7
 *
 * ID_DEST / ID_SRC :
 *  . valeur min: 0x0001 (Hexa)
 *  . valeur max: 0xFFFF (Hexa)
 * ID_Message / Type_MSG / Checksum:
 *  . ex : msg num 1, num 2 .. ect 
 * Data_Message:
 *  . champ contenant les donnees a transmettre / ou recu 
 *    ex: la data : 200319172620 ==> 20/03/19 | 17h:26m:20s
 * Checksum:
 *  . champ de controle de la coherence : un XOR avec ID_XX^ID_MSG^Type_MSG^Data_Message
 *    --> detection d'erreur par checksum (somme controle) 
 *
 */
//sauvdarde des info
//typedef struct sFrame {
//    uint16_t ID_Dest;
//    uint16_t ID_Src;
//    uint8_t ID_Msg;
//    int8_t Type_Msg;
//    uint8_t nbRemaining;
//    uint8_t data[SIZE_DATA];
//}Frame;
/*_____________________________________________________________________________*/


/**------------------------>> T Y P E--M S G <<--------------------------------*/

typedef union {
    uint8_t code;

    struct {
        unsigned typePaquet : 4;
        unsigned nbRemaining : 4;
    } ret; //trouver un autre nom

} RF_Type_And_nbRemaining;

typedef union {
    uint8_t code;

    struct {
        unsigned src : 4;
        unsigned dest : 4;
    } id;

} idOF;

typedef struct {
    idOF id;
    RF_Type_And_nbRemaining rfTandNBR;
    uint8_t idMsg; // nume seq 
    uint8_t sumCtrl;
    uint8_t data[SIZE_DATA];
} Frame;
/*____________________________________________________________________________*/

/**------------------------>> I D-- O F <<-------------------------------------*/
uint16_t  srv_getID_Slave();
uint16_t  srv_getBroadcast();
uint16_t  srv_getID_Master();
/*_____________________________________________________________________________*/


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

