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

typedef enum { // if you want to add a new level, you must be increase MAX_LEVEL_PRIO
    HIGH, //00
    MEDIUM, //01
    LOW //02
} PRIORITY;

typedef enum {
    READ, //00
    WRITE, //01
    OVFF //10
}PTR;

typedef enum { // you can add others states
    MASTER_STATE_INIT,
    MASTER_STATE_SEND_DATE,
    MASTER_STATE_MSG_RF_RECEIVE,
    MASTER_STATE_SEND_REQUEST_INFOS,
    MASTER_STATE_IDLE,
    MASTER_STATE_ERROR,
    MASTER_STATE_NONE // no comportementent
} MASTER_STATES;



/**-------------------------->> P R O T O T Y P E S <<------------------------*/


/**
 * tramet un ensemble de octets
 * 
 * @param dest : le destinataire 0 < dest < 16 
 * @param typeMsg : ex : DATA, ACK, HORLOGE, ... etc
 *                  rensegner le bloc de donnee attendu 
 * @param idMsg : l'identifiant du msg ==> ack, peut etre utiliser pour
 * @param nbR : nombre de paquet que je souhaite transmettre 
 * @param data : la donner a transmettre ==> taille < 20
 * @param sizeData : la taille de la donnee a transmettre 
 * @return : le nombre d'octe transmis
 */
int8_t MASTER_SendMsgRF(uint8_t dest,
                        uint8_t typeMsg,
                        uint8_t idMsg,
                        uint8_t nbR,
                        uint8_t * data,
                        uint8_t sizeData);


/**
 * 
 * @return : le nombre d'octe transmis
 */
uint8_t MASTER_SendDateRF();

/**
 * enregistre le comportement dans le tableau des comportements,
 * en fonction sa priorite
 *  
 * @param behavior : le comportement a traiter 
 * @param prio : la priorite du comportement a traiter 
 * @return :
 *          true  : le comportement est enregistre avec succes 
 *          false : on a ecrase un comportement  
 */
bool MASTER_StoreBehavior(MASTER_STATES behavior, PRIORITY prio);

/**
 * 
 * @return : le prochain comportement a adopter 
 */
MASTER_STATES MASTER_GetBehavior();

/**
 * machine a etat general ==> noyau du systeme master 
 */
void MASTER_AppTask();

/**
 * initialisation des la machine a etat 
 */
void MASTER_AppInit();

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/

#endif	/* XC_HEADER_TEMPLATE_H */

