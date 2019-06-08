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
 *                    COURCHE APPLICATION (OF) SLAVE  (.h)
 * _____________________________________________________________________________
 *
 * Titre            : Mise en oeuvre de la couche application du slave 
 * Version          : v00
 * Date de creation : 24/05/2019
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
#include "radio_alpha_trx.h"
#include "Services.h"

 /******************************************************************************/
 /******************************************************************************/
 /********************* COUCHE APPLICATION DU SLAVE  ***************************/
 /***************************                ***********************************/
 /*****************                                         ********************/

/**
 * ajoute l'erreur dans le buffer des erreurs et met à jour les pointeurs des erreur 
 * @param num_error : numero de l'erreur 
 */
void radioAlphaTRX_Slave_save_error(int8_t num_error);


/**
 * 
 * @return :
 *          0 : si aucune erreur n'est present 
 *          numero de l'erreur  
 */
int8_t radioAlphaTRX_slave_get_error();


/**
 * apres la reception d'un ack, si on avait transmsi une erreur on met a jour 
 * les pointeurs du buffer des erreurs 
 */
void radioAlphaTRX_slave_update_buf_err_ptr();


/**
 * lorsque l'on transmet une erreur, le num paquet devient le numero de l'erreur 
 * @param err_to_send : l'erreur à transmettre 
 */
void radioAlphaTRX_slave_send_err(int8_t err_to_send);


/**
 * si l'on recois une demande d'infos que l'on a rien a transmettre 
 * on notifie quand meme, au master de notre existance, pour lui eviter de nous 
 * considerer infonctionnel 
 */
void radioAlphaTRX_slave_send_nothing();


/**
 * met a jour l'horloge avec la date recu en prenant en compte le temps de 
 * latence avant le traitement du msg 
 *  
 * @param date : la date recu
 * @param derive : le temps ecouler avant le traitement du msg 
 */
void radioAlphaTRX_slave_update_date(uint8_t* date, int16_t derive);

/**
 * machine a etat du systeme de communicatio du slave pendant la journee 
 */
void radioAlphaTRX_slave_behaviour_of_daytime();


 /****************                                         *********************/
 /*************************                     ********************************/
 /******************************************************************************/
 /******************************************************************************/

#endif	/* XC_HEADER_TEMPLATE_H */

