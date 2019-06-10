/* *****************************************************************************
 * 
 * _____________________________________________________________________________
 *
 *                    GESTIONNAIRE TIMER (.c)
 * _____________________________________________________________________________
 *
 * Titre            : Mise en oeuvre d'un TIMER local 
 * Version          : v00
 * Date de creation : 24/05/2019
 * Auteur           : MMADI Anzilane (A partir du code de Arnauld BIGANZOLI (AB))
 * Contact          : anzilan@hotmail.fr
 * Web page         : 
 * Collaborateur    : ...
 * Processor        : PIC24
 * Tools used       : MPLAB X IDE v5.15 and MPLAB Code Configurator (MCC) Version: 3.36
 * Compiler         : Microchip XC16 v1.35
 * Programmateur    : PICkit 3
 *******************************************************************************
 *******************************************************************************/

/**------------------------>> I N C L U D E <<---------------------------------*/
#include <time.h>
#include "xc.h"
#include "timer.h"


 /******************************************************************************/
 /******************************************************************************/
 /******************** PARAMETRE GESTIONNAIRE DE TIMER *************************/
 /***************************                ***********************************/
 /*****************                                         ********************/

/**-------------------------->> V A R I A B L E S <<---------------------------*/
int8_t TMR_CptTrickHorloge = 1000;
volatile uint16_t TMR_HorlogeTimeout = 0; 

volatile uint16_t TMR_WaitRqstTimeout = 0; //on s'en sert pour le poulling 

//TODO : penser rendre �a generique
volatile uint16_t TMR_MsgRecuTimeout = 0;  

volatile uint16_t TMR_nIRQLowTimeout = 0;

volatile uint16_t TMR_Timeout = 0;

volatile uint16_t TMR_DelayMs = 0;
/*_____________________________________________________________________________*/

/**-------------------------->> D E F I N I T I O N <<-------------------------*/

uint16_t TMR_GetHorlogeTimeout() { return TMR_HorlogeTimeout; } 
void TMR_SetHorlogeTimeout(uint16_t timeout) { 
    TMR_HorlogeTimeout = timeout; 
} 


uint16_t TMR_GetWaitRqstTimeout() { return TMR_WaitRqstTimeout; } //on s'en sert pour le poulling 
void TMR_SetWaitRqstTimeout(uint16_t set) { TMR_WaitRqstTimeout = set; }

uint16_t TMR_GetMsgRecuTimeout() { 
    int16_t temp = TMR_MsgRecuTimeout;
    TMR_MsgRecuTimeout = 0;
    return temp; 
} 
void TMR_SetMsgRecuTimeout(uint16_t timeout) { TMR_MsgRecuTimeout = timeout; }

uint16_t TMR_GetnIRQLowTimeout() { return TMR_nIRQLowTimeout; }
void TMR_SetnIRQLowTimeout(int16_t timeout) { TMR_nIRQLowTimeout = timeout; }

void TMR_SetTimeout(uint16_t timeout) { TMR_Timeout = timeout; }
uint16_t TMR_GetTimeout() { return TMR_Timeout; }

void TMR_Delay(uint16_t delayMs) {
    TMR_DelayMs = delayMs;
    while (TMR_DelayMs > 0) { }
}
void TMR_CallBack( void ) {
    
    if (TMR_DelayMs) --TMR_DelayMs;
    
    if (TMR_Timeout) --TMR_Timeout;
    
    //nIRQ timeout
    if (TMR_nIRQLowTimeout) --TMR_nIRQLowTimeout;

    //dis si une reponse est recu ou pas 
    if (TMR_WaitRqstTimeout) --TMR_WaitRqstTimeout;
    
    if (TMR_HorlogeTimeout) --TMR_HorlogeTimeout; // on ferra autrement 

    //timer du buffer 
    if (TMR_MsgRecuTimeout) --TMR_MsgRecuTimeout;
}

/*_____________________________________________________________________________*/

//END FILE