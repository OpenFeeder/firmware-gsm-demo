/*
 * File:   Timer.c
 * Author: mmadi
 *
 * Created on 5 avril 2019, 22:06
 */


#include "xc.h"
#include "timer.h"
#define NB_MAX_TIMER 10

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// ****************************************************************************
volatile uint16_t tmr_msg_recu; 

volatile uint16_t tmr_wait_rqst_timeout; //on s'en sert pour le poulling 

//TODO : penser rendre ça generique
volatile uint16_t tmr_horloge_timeout[4];  

volatile uint16_t tmr_nIRQ_low_timeou = 0;


void TMR_CallBack( void ) {
    
    //nIRQ timeout
    if (tmr_nIRQ_low_timeou) {
        --tmr_nIRQ_low_timeou;
    }
}
