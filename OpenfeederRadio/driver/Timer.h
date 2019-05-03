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

/* 
 * File: Timer.h   
 * Author: Anzilane
 * Comments: gestionaire des timers ==> implementé à partir du timer physique 1
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

/**
 * demare le numero du timer indiqué avec le nombre de ms indiqué 
 * @param n : numéro du timer
 * @param ms : delais 
 * @return : 1 timer demarré 0 si non
 */
int8_t tmr_depart_num(int n, int ms);

/**
 * demare le temporisateur pour l'attente des paquet 
 * @param ms
 */
void tmr_depart_rdy_rf(int ms);

/**
 * demarre le timer pour le 'enoie des paquet 
 * @param ms
 */
void tmr_depart_enoie_paquet_rf(int ms);

/**
 * prends un numero de timer et l'arrete 
 * @param n : le numero du timer à arreter   0<n<10;
 */
void tmr_arreter_num(int n);

/**
 * arrete le timer du rdy 
 */
void tmr_arreter_rdy();

void tmr_arreter_enoie_paquet_rf();
#endif	/* XC_HEADER_TEMPLATE_H */

