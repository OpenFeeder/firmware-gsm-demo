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
 *                         MASTER API (.h)
 * _____________________________________________________________________________
 *
 * Titre            : Mise en oeuvre de l'api et la machie a etat du master   
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
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

/**------------------------>> I N C L U D E <<---------------------------------*/
#include <xc.h> // include processor files - each processor file is guarded.  
#include "Services.h"

/******************************************************************************/
/******************************************************************************/
/******************     MASTER APPLICATION CONTROL         ********************/
/***************************                ***********************************/
/*****************                                 ****************************/

/**------------------------>> E N U M - S T A T E  <<--------------------------*/
typedef enum {
    MSTR_STATE_GENERAL_BEFOR_DAYTIME,
    MSTR_STATE_GENERAL_DAYTIME,
    MSTR_STATE_GENERAL_AFTER_DAYTIME,
    MSTR_STATE_GENERAL_ERROR
}MSTR_STATE_GENERAL;

typedef enum {
    MSTR_STATE_GET_LOG_IDLE,            // on attend une reponse de notre demande 
    MSTR_STATE_GET_LOG_SYNC,            // on demande un slave selectionner de transmettre des donnees
    MSTR_STATE_GET_LOG_COLLECT,         // on est en recuperation de donner 
    MSTR_STATE_GET_LOG_DESYNC,          // on est en fin de recuperation pour ce slave selectionne
    MSTR_STATE_GET_LOG_SEND_FROM_GSM,   // on transfere les donnees par gsm au srver       
    MSTR_STATE_GET_LOG_ERROR            // une erreur est survenue 
}MSTR_STATE_GET_LOG;

typedef enum {
    SLAVE_STATE_SELECTED,   // en cours d'interrogation 
    SLAVE_STATE_DESELCTED,  // n'est pas encours selectionne
    SLAVE_STATE_COLLECT_END,// si on a deja collecte ses donnees 
    SLAVE_STATE_ERROR       // si le slave est en error 
}SLAVE_STATE;

// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************
extern MSTR_STATE_GENERAL mstrState;
extern MSTR_STATE_GET_LOG mstrStateGetLog;
/**
 * envoie la date a tout les slaves present 
 * @return 
 *      0 : si non envoye
 *      1 : si oui 
 */
int8_t MASTER_SendDateRF();

/**
 * 
 */
void MASTER_HandlerMsgRF();

/**
 * 
 */
int8_t MASTER_SelectSlave();

/**
 * des qu'un msg est recu on declence un timer qui corespond au temps que master
 * va ecouter avant de decider qu'il y a timeout 
 *  
 * @return :
 *      0 : pas de msg ou n'est plus d'actalite 
 *      1 : un msg est presnt et il est toujours d'actualité 
 */
void MASTER_SetMsgReceiveRF(uint8_t set);


int8_t Master_SendMsgRF(uint16_t idSlave, uint8_t typeMsg, uint8_t * data, uint8_t idMsg);

/**
 * 
 */
void MASTER_StateMachineOfDaytime();

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/

#endif	/* XC_HEADER_TEMPLATE_H */

