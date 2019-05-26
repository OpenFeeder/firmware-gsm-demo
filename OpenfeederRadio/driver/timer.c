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
/*_____________________________________________________________________________*/


/**-------------------------->> V A R I A B L E S <<---------------------------*/
int8_t cpt_trick_horloge = 1000;
volatile uint16_t tmr_horloge_timeout; 

volatile uint16_t tmr_wait_rqst_timeout; //on s'en sert pour le poulling 

//TODO : penser rendre ça generique
volatile uint16_t tmr_msg_recu_timeout[4];  

volatile uint16_t tmr_nIRQ_low_timeout;

volatile uint16_t tmr_timeout;
/*_____________________________________________________________________________*/

/**-------------------------->> D E F I N I T I O N <<-------------------------*/

uint16_t get_tmr_horloge_timeout() { return tmr_horloge_timeout; } 
void set_tmr_horloge_timeout_x1000_ms(uint16_t timeout_x1000_ms) { 
    tmr_horloge_timeout = timeout_x1000_ms; 
} 


uint16_t get_tmr_wait_rqst_timeout(); //on s'en sert pour le poulling 
void set_tmr_wait_rqst_timeout(uint16_t set);

//TODO : penser rendre ça generique
uint16_t get_tmr_msg_recu_timeout(int8_t i) {
    int16_t temps_mis = tmr_msg_recu_timeout[i];
    tmr_msg_recu_timeout[i] = 0;
    return temps_mis;
}  
void set_tmr_msg_recu_timeout(int8_t i, uint16_t set) {
    tmr_msg_recu_timeout[i] = set;
}

uint16_t get_tmr_nIRQ_low_timeout() { return tmr_nIRQ_low_timeout; }
void set_tmr_nIRQ_low_timeout(int16_t set) { tmr_nIRQ_low_timeout = set; }

void set_tmr_timeout(uint16_t timeout) { tmr_timeout = timeout; }
uint16_t get_tmr_timeout() { return tmr_timeout; }


void tmr_callBack( void ) {
    
    //nIRQ timeout
    if (tmr_nIRQ_low_timeout) --tmr_nIRQ_low_timeout;
    
    //dis si une reponse est recu ou pas 
    if (tmr_wait_rqst_timeout) --tmr_wait_rqst_timeout;
    
    if (tmr_horloge_timeout) --tmr_horloge_timeout; 
    
    //timer du buffer 
    if (tmr_msg_recu_timeout[0]) --tmr_msg_recu_timeout[0];
    if (tmr_msg_recu_timeout[1]) --tmr_msg_recu_timeout[1];
    if (tmr_msg_recu_timeout[2]) --tmr_msg_recu_timeout[3];
    if (tmr_msg_recu_timeout[3]) --tmr_msg_recu_timeout[3];
}

/*_____________________________________________________________________________*/

//END FILE