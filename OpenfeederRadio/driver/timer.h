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
#include "Services.h"

/******************************************************************************/
/******************************************************************************/
/******************** PARAMETRE GESTIONNAIRE DE TIMER *************************/
/***************************                ***********************************/
/*****************                                         ********************/

// est active des qu'un msg est recu est non encore lu ==> c'est un compteur 

/**
 * 
 * @return : 
 *         0 : transmettre l'horloge
 *         !0 : on fait rien   
 */
int16_t TMR_GetHorlogeTimeout();

/**
 * determine combirne de minute il faut attendre avant de transmettre la date
 * @param timeout : le temps qu'il faut attendre avant de transmettre l'horloge 
 */
void TMR_SetHorlogeTimeout(int16_t timeout_min);

/**
 * tant que cette fonction me retourne une valeure differente de 0 
 * le master est toujour en ecoute du slave questionne, 
 * 
 * @return : 
 *      0 : timeout : le slave n'a pas eu le temps de me repondre 
 *      si non > 0 
 */
int16_t TMR_GetWaitRqstTimeout(); //on s'en sert pour le poulling
/**
 * le temps d'attente d'une reponse suite ? une demande d'information
 * il est declanche a chaque fois que je demande une information a un slave 
 * 
 * @param timeout : le temps que le master s'autorise a attendre une reonse 
 */
void TMR_SetWaitRqstTimeout(int16_t timeout);

/**
 * permet de savoir combient de temps (en ms) le buffer i est remplie 
 * @return : le temps ecoule en (ms) depuis le dernier remplissage du buffer 
 */
int16_t TMR_GetMsgRecuTimeout();

/**
 * determine le temps max pour prendre en compte un msg de type horloge,
 * au dela le paquet n'est pas utilisable 
 * 
 * @param timeout : 
 */
void TMR_SetMsgRecuTimeout(int16_t timeout);

/**
 * 
 * @return : 
 *      0 : timeout ==> Le Nirq n'est pas passe a l'etat bas et le temps est fini
 *      1 : si non 
 */
int16_t TMR_GetnIRQLowTimeout();

/**
 * 
 * @param timout
 */
void TMR_SetnIRQLowTimeout(int16_t timeout);

/**
 * a utiliser librement 
 * 
 * @param timeout 
 */
void TMR_SetTimeout(int16_t timeout);
/**
 * 
 * @return :
 *      0 : timeout
 *      1 : si non  
 */
int16_t TMR_GetTimeout();

/**
 * 
 */
void TMR_Delay();

/**
 * fonction principale de gestion des timer 
 */
void __attribute__ ((weak)) TMR_CallBackTMR(void);

void __attribute__ ((weak)) TMR_CollBackRTC(void);


/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/

#endif	/* XC_HEADER_TEMPLATE_H */

