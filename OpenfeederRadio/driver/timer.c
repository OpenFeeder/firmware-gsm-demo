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
volatile int16_t TMR_HorlogeTimeout = 0; 

// il a besoin d'etre activer 
volatile int16_t TMR_WaitRqstTimeout = 0; //on s'en sert pour le poulling 

//TODO : penser rendre ?a generique
volatile int16_t TMR_MsgRecuTimeout = 0;  

volatile int16_t TMR_nIRQLowTimeout = 0;

volatile int16_t TMR_Timeout = 0;

volatile int16_t TMR_DelayMs = 0;
/*_____________________________________________________________________________*/

/**-------------------------->> D E F I N I T I O N <<-------------------------*/

int16_t TMR_GetHorlogeTimeout() { return TMR_HorlogeTimeout; } 
void TMR_SetHorlogeTimeout(int16_t timeout_min) { 
    TMR_HorlogeTimeout = timeout_min; 
} 


int16_t TMR_GetWaitRqstTimeout() { return TMR_WaitRqstTimeout; } //on s'en sert pour le poulling 
void TMR_SetWaitRqstTimeout(int16_t set) { TMR_WaitRqstTimeout = set; }

int16_t TMR_GetMsgRecuTimeout() { 
    int16_t temp = TMR_MsgRecuTimeout;
    TMR_MsgRecuTimeout = 0;
    return temp; 
} 

void TMR_SetMsgRecuTimeout(int16_t timeout) { TMR_MsgRecuTimeout = timeout; }

int16_t TMR_GetnIRQLowTimeout() { return TMR_nIRQLowTimeout; }
void TMR_SetnIRQLowTimeout(int16_t timeout) { TMR_nIRQLowTimeout = timeout; }

void TMR_SetTimeout(int16_t timeout) { TMR_Timeout = timeout; }
int16_t TMR_GetTimeout() { return TMR_Timeout; }

void TMR_Delay(int16_t delayMs) {
    TMR_DelayMs = delayMs;
    while (TMR_DelayMs > 0) { }
}

void __attribute__ ((weak)) TMR_CallBackRTC() {
    if (TMR_HorlogeTimeout) --TMR_HorlogeTimeout; // on ferra autrement 
}

void __attribute__ ((weak)) TMR_CallBackTMR( void ) {
    
    if (TMR_DelayMs > 0) --TMR_DelayMs;
    
    if (TMR_Timeout > 0) --TMR_Timeout;
    
    //nIRQ timeout
    if (TMR_nIRQLowTimeout > 0) --TMR_nIRQLowTimeout;

    //dis si une reponse est recu ou pas 
    if (TMR_WaitRqstTimeout > 0) --TMR_WaitRqstTimeout;
    
    //timer du buffer 
    if (TMR_MsgRecuTimeout > 0) --TMR_MsgRecuTimeout;
}

/*_____________________________________________________________________________*/

//END FILE