/* *****************************************************************************
 * 
 * _____________________________________________________________________________
 *
 *                    TIMER GESTION 
 * _____________________________________________________________________________
 *
 * Titre            : Mise en oeuvre d'un gestionnaire de timer
 * Version          : v00
 * Date de creation : 30/06/2019
 * Auteur           : MMADI Anzilane 
 * Contact          : anzilan@hotmail.fr
 * Web page         : 
 * Collaborateur    : ...
 * Processor        : PIC24F
 * Tools used       : MPLAB X IDE v5.15 and MPLAB Code Configurator (MCC) Version: 3.36
 * Compiler         : Microchip XC16 v1.35
 * Programmateur    : PICkit 3
 *******************************************************************************
 *******************************************************************************/


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdlib.h>
/******************************************************************************/
/******************************************************************************/
/*********************     GESTIONNAIRE DE TIMER        ***********************/
/***************************                ***********************************/
/*****************                                         ********************/

/**
 * 
 * @param delay_ms
 */
void tmr_delay(int delay_ms);

/**
 * 
 */
void tmr_callBack();

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/
#endif	/* XC_HEADER_TEMPLATE_H */

