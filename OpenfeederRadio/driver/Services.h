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
#define TIME_OUT_WAIT_RQST         3000
#define FRAME_LENGTH                128 // Longueur total d'une trame en octet
#define ERROR_LENGTH                  8
#define SIZE_DATA                    40
#define TIME_OUT_nIRQ                 2 // 2 ms
#define LAPS                          5 // on attend X ms avant de transmettre un nouveau msg 
#define SEND_HORLOG_TIMEOUT           1 // 1 min
#define AFTER_SEND_HORLOGE           20 // 20 ms
#define TIME_OUT_GET_FRAME         1500 // temps max, pour que le msg recu soit encore exploitable
                                        // au dela le mster ne m'ecoute pas donc cela ne sert ? rien 
#define NB_ERR_BUF                   10 // nombre d'errerur possible 
#define NB_DATA_BUF                  20 // pour l'instat on dit qye c'est 20 ==>
#define MAX_W                        10 // nombre MAX de paquet a transmettre avant d'attendre un ack 
#define NB_SLAVE                      1
#define MAX_TIMEOUT                  10 // nombre de timeout avant de decider que la liaison avec le slave est couper 
#define MAX_TRY_TO_SYNC               5 // le nombre d'essaie avant de decider qu'on est pas connecte

// id and station 
#define SLAVE1_ID 37
#define SLAVE2_ID 36
#define SLAVE3_ID 35
#define SLAVE4_ID 33
#define BROADCAST 1023

#define MASTER_ID 34
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
typedef struct sFrame {
    uint16_t ID_Dest;
    uint16_t ID_Src;
    uint8_t ID_Msg;
    int8_t Type_Msg;
    uint8_t nonUtiliser [3];
    uint8_t data[SIZE_DATA];
}Frame;
/*_____________________________________________________________________________*/


/**------------------------>> T Y P E--M S G <<--------------------------------*/
// type de paquet
uint8_t srv_err();
uint8_t srv_data();
uint8_t srv_ack();
uint8_t srv_horloge();
uint8_t srv_infos();
uint8_t srv_end_trans();
uint8_t srv_end_block();
uint8_t srv_config();
uint8_t srv_nothing();
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
 * dis si l'ack recu est dans la fenetre 
 * @param inf
 * @param pointeur
 * @param size 
 * @return 
 */
int8_t srv_in_windows(unsigned int inf, unsigned int pointeur, int size);

/**
 * genere une somme controle 
 * 
 * @param paquet : le paquet a calculer le checksum
 * @param size : la taille du paquet 
 * @return : somme controle 
 */
uint8_t srv_checksum(uint8_t* paquet, int size);

/**
 * verifie le paquet recu en calculant la somme controle et en le comparant
 * la somme controle recu 
 * 
 * @param paquet : le paquet a verifier 
 * @param size : sa taille 
 * @param somme_ctrl : la valeure du somme controle recu 
 * @return : 
 *         1 : test positif 
 *         0 : test negatif 
 */
int8_t srv_test_cheksum(uint8_t* paquet, int size, uint8_t somme_ctrl);

/**
 * donne la taille d'une chaine de caractere 
 * 
 * @param chaine 
 * @return : la taille de la chaine paissee en parametre 
 */
int srv_len(const uint8_t *chaine);

/**
 * compare deux chaines passees en parametre
 *  
 * @param ch1 : chaine de caractere 1
 * @param ch2 : chaine de caractere 2
 * @return 1 si ch1 == ch2, 0 sinon
 */
int8_t srv_cmp(const uint8_t *ch1, const uint8_t *ch2);

/**
 * copy d'une chaine source a une chaine dest
 * @param dest : ou on copie 
 * @param src : ce qu'on copie
 * @param size : la taille de la chaie 
 * @return : 1 ok : 0 ko
 */
int8_t srv_cpy(uint8_t *dest, uint8_t *src, int size);

/**
 * genere un paquet a partir des infos fournis
 * 
 * @param paquet : conteneur du paquet cree
 * @param donnee : la donnee a encapsuler 
 * @param src : qui l'envoie ==> 
 * @param dest : a qui on l'envoie ==> 
 * @param typePaquet : le type de paquet a transmettre (err, ack, data, horloge ...)
 * @param numPaquet : numero du paquet 
 * @return la taille totale du paquet creee
 */
int8_t srv_create_paket_rf(uint8_t paquet[], uint8_t data[], 
        uint16_t dest, uint16_t src, uint8_t typeDePaquet, uint8_t numPaquet);

/**
 * prends un paquet et de le decoder
 * 
 * @param paquet : le paquet a deballer
 * @param size : la taille du paquet
 * @param pPaquetRecu : contient les element qui compose le paquet 
 * @param idOF : l'identifiant de l'of qui vient de recevoir le paquet
 * @return la taille du paquet c'est un bon paquet, 0 si non 
 */
int8_t srv_decode_packet_rf(uint8_t* paquet, Frame *pPaquetRecu, int size, 
        uint16_t idOF);

/**
 * permet d'attends un temps donnee
 * 
 * @param delais : le temps d'attente en x10ms
 */
void srv_wait(int delais);

/**
 * permet d'implementer la methode du goBackN
 * 
 * @param data : la data qu'il faut envoyer les acks ne sont pas re?u 
 * @param offset : a partir du quel on doit debuter la retransmission 
 * @param curseur : cursseur la borne superieure 
 * @param idOfSrc : l'of qui recois le paquet 
 * @param idOfDest: l'of a qui on envoie le paquet 
 * @param taille de la fenetre 
 * @return 
 */
int8_t srv_goBackN(const uint8_t** data, int * offset, int curseur, 
        uint16_t idOfsrc, uint16_t idOfDest, int8_t *w);

/**
 * decrementer le delais de quelque x10ms
 * 
 * @param delais : le delais a modifier
 * @param seuil : le delais min a ne pas depasser
 * @param ms : la reduction a apporter x10 ms
 */
void srv_dec_delais(int *delais, int seuil, int ms);

/**
 * incremente le delais de quelque ms
 * @param delais : le delais a modifier
 * @param ms : l'ogmentation a apporter x10 ms
 */
void srv_inc_delais(int *delais, int ms);

/**
 * 
 * @param nbligne : le nombre de ligne qu'on recup?re sur le fichier 
 * @return : les log recup?rer depuis le fichier de sauvgarder 
 */
uint8_t **service_recup_data_sur_disque(int *nbligne);

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/
#endif	/* SERVICE_H */

