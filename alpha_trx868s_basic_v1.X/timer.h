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
 *                    GESTIONNAIRE TIMER (.h)
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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

/**------------------------>> I N C L U D E <<---------------------------------*/
#include <xc.h> // include processor files - each processor file is guarded.
#include "mcc_generated_files/pin_manager.h"
#include "Services.h"
 /******************************************************************************/
 /******************************************************************************/
 /******************** PARAMETRE GESTIONNAIRE DE TIMER *************************/
 /***************************                ***********************************/
 /*****************                                         ********************/

// est active des qu'un msg est recu est non encore lu ==> c'est un compteur 

/**
 * 
 * @return : 0 if faut envoyer l'horloge, si non c'est pas encore time out 
 */
uint16_t get_tmr_horloge_timeout(); 
/**
 * determine combirne de minute il faut attendre avant de transmettre la date
 * @param timeout_x1000_ms : ex 10 = 10x1000 == 10000 ms = 10s
 */
void set_tmr_horloge_timeout_x1000_ms(uint16_t timeout_x1000_ms); 

/**
 * ateste de l'arriver ou pas d'un msg 
 * @return : 
 *      0 : timeout 
 *      si non > 0
 */
uint16_t get_tmr_wait_rqst_timeout(); //on s'en sert pour le poulling
/**
 * le temps d'attente d'une reponse suite à une demande 
 * @param set
 */
void set_tmr_wait_rqst_timeout(uint16_t set);

/**
 * permet de savoir combient de temps (en ms) le buffer i est remplie 
 * @return : le temps ecoule en (ms) depuis le dernier remplissage du buffer 
 */
uint16_t get_tmr_msg_recu_timeout();  
/**
 * determine le temps maxe pour prendre en compte un msg de type horloge,
 * au dela le paquet n'est pas utilisable 
 * @param set : le temps en question 
 */
void set_tmr_msg_recu_timeout(uint16_t set);

/**
 * nombre de temps en (ms) que l'on s'autorise à attendre un octet en reception 
 * apres une interuption (evite l'attente active) 
 * @return : 
 *      0 : timeout
 *      1 : si non 
 */
uint16_t get_tmr_nIRQ_low_timeout();
/**
 * choisir le timout 
 * @param set
 */
void set_tmr_nIRQ_low_timeout(int16_t set);

/**
 * a utiliser librement 
 * @param timeout 
 */
void set_tmr_timeout(uint16_t timeout);
/**
 * 
 * @return :
 *      0 : timeout
 *      1 : si non  
 */
uint16_t get_tmr_timeout();

/**
 * fonction principale de gestion des timer 
 */
void tmr_callBack( void );

 /****************                                         *********************/
 /*************************                     ********************************/
 /******************************************************************************/
 /******************************************************************************/

#endif	/* XC_HEADER_TEMPLATE_H */

