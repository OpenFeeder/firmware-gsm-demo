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
#ifndef MASTER_API_H
#define	MASTER_API_H

/**------------------------>> I N C L U D E <<---------------------------------*/
#include <xc.h> // include processor files - each processor file is guarded.  
#include "time.h"
#include "Services.h"
#include "../alpha_trx_driver/radio_alpha_trx.h"
/******************************************************************************/
/******************************************************************************/
/******************     MASTER APPLICATION CONTROL         ********************/
/***************************                ***********************************/
/*****************                                 ****************************/


//______________________________________________________________________________
//______________________________DEBUG___________________________________________
void modif(int val);
void display_STATUS_register_from_RF_module(void);
//______________________________________________________________________________

typedef enum { // if you want to add a new level, you must be increase MAX_LEVEL_PRIO
    PRIO_EXEPTIONNEL, //00 priorite exeptionnelle 
    PRIO_HIGH, //01
    PRIO_MEDIUM, //10
    PRIO_LOW //11
} PRIORITY;

typedef enum {
    PTR_READ, //00
    PTR_WRITE, //01
    PTR_OVFF //10
} PTR;

typedef enum { // you can add others states
    MASTER_STATE_INIT,
    MASTER_STATE_TIMEOUT,
    MASTER_STATE_SEND_DATE,
    MASTER_STATE_SEND_FROM_UART,
    MASTER_STATE_SEND_ERROR_TO_UART,
    MASTER_STATE_SELECTE_SLAVE,
    MASTER_STATE_MSG_RF_RECEIVE,
    MASTER_STATE_MSG_GSM_RECEIVE,
    MASTER_STATE_SEND_REQUEST_INFOS,
    MASTER_STATE_IDLE,
    MASTER_STATE_END,
    MASTER_STATE_ERROR,
    MASTER_STATE_NONE // no comportementent
} MASTER_STATES;

/**-------------------------->> D E B U G <<----------------------------------*/
void printPointeur(PRIORITY prio);

/**-------------------------->> P R O T O T Y P E S <<------------------------*/

/**-------------------------->> S T R U C T -O F- S L A V E - S T A T E <<----*/

typedef enum {
    SLAVE_SYNC, // phase de syncronisation 
    SLAVE_ERROR, // en etat d'erreur 
    SLAVE_NO_REQUEST,
    SLAVE_DAYTIME, // on est en journee 
    SLAVE_CONFIG, // etat de configiration 
    SLAVE_COLLECT, // Slave en etat de collecte de donnee
    SLAVE_SLECTED, // Slave selectionner 
    SLAVE_COLLECT_END, // fin de la collecte
    SLAVE_COLLECT_END_BLOCK, // fin de recuperation d'un bloc
    SLAVE_NONE // etat neutre 
} SLAVE_STATES;

typedef struct {
    uint8_t idSlave; // l'identifiant du slave
    uint8_t uidSlave; // l'identifiant reel du slave 
    int8_t tryToConnect; // nb d'essaie de connexion 
    int8_t nbTimeout; // le nombre consecutif de timeout lorsqu'on attend une reponse de ce slave
    int8_t nbError; // nombre d'erreurs, survenue pour ce slave
    uint8_t index; // le numero de paquet attendu, lors de la collecte des donnees 
    uint8_t nbBloc; // nombre de bloc re?u 
    SLAVE_STATES state; // etat du slave
    bool errorNotify; // si on a deja notifier l'erreur de ce slave 
} SlaveState;

typedef enum {
    GOOD_MORNING, //master wake up 
    GOOD_DAY, // the daytime  
    GOOD_NIGHT, // end day collect log 
    SEE_YOU_TOMORROW // master go to sleep
} STEP_OF_DAY;

//______________________________________________________________________________

typedef struct {
    /* Application current state */
    MASTER_STATES state; /* current state */
    MASTER_STATES previous_state; /* save previous state */

    /* DateTime structure */
    struct tm current_time;

    union {
        uint16_t reg;

        struct {
            unsigned num_software_reset : 3;
            unsigned : 13;
        } bit_value;
    } dsgpr0;

    union {
        uint16_t reg;

        struct {
            unsigned : 16;
        } bit_value;
    } dsgpr1;

    union {
        uint16_t reg;

        struct {
            unsigned por : 1;
            unsigned bor : 1;
            unsigned idle : 1;
            unsigned sleep : 1;
            unsigned wdto : 1;
            unsigned swdten : 1;
            unsigned swr : 1;
            unsigned extr : 1;
            unsigned pmslp : 1;
            unsigned cm : 1;
            unsigned dpslp : 1;
            unsigned : 1;
            unsigned reten : 1;
            unsigned : 1;
            unsigned iopuwr : 1;
            unsigned trapr : 1;
        } bit_value;
    } reset_1;

    /* Declaration of FLAGS type. */
    union {
        uint8_t reg;

        struct {
            unsigned system_init : 1; /* true if configuration success from "CONFIG.INI" */
            unsigned attractive_leds_status : 1; /* true if configuration success from "CONFIG.INI" */
            unsigned remote_control_connected : 1; /* true if Remote Control is connected */
            unsigned new_valid_pit_tag : 1; /* true if a new PIT Tag is validated. */
            unsigned : 4;
        } bit_value;
    } flags;

    bool openfeeder_state;
    //
    STEP_OF_DAY dayTime;

    Frame receive;
    //init rf module module 
    int8_t timeToSynchronizeHologe;
    bool synchronizeTime;
    bool RfModuleInit;

    volatile uint8_t behavior[MAX_LEVEL_PRIO][NB_BEHAVIOR_PER_PRIO];
    volatile uint8_t ptr[MAX_LEVEL_PRIO][3]; //READ - WRITE - OVFF (overflow)
    //    uint8_t BUFF_COLLECT[NB_BLOCK][SIZE_DATA];
    uint8_t BUFF_COLLECT[NB_BLOCK*SIZE_DATA];
    int8_t nbSlaveOnSite;
    SlaveState ensSlave[8]; // max of on site
    int8_t slaveSelected;
    int8_t station;
    int8_t masterId;
    int8_t broadCastId;

} MASTER_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************

extern MASTER_DATA appData; /* Main application data. */
extern MASTER_ERROR appError; /* Error application data. */

//______________________________________________________________________________

/**SIZE_DATA
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

#endif	/* MASTER_API */

