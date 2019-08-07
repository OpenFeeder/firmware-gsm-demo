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
 *                    DRIVER GSM3 SIM800H (.h)
 * _____________________________________________________________________________
 *
 * Titre            : Mise en oeuvre du driver gsm sim800H 
 * Version          : v01
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
#ifndef DRIVER_GSM3_SIM800_H
#define	DRIVER_GSM3_SIM800_H

/*------------------------------> I N C L U D E S  <---------------------------*/
#include <xc.h> // include processor files - each processor file is guarded.  
#include <string.h>
#include <stdbool.h>
#include "../../timer.h"
#include "../uart1.h"
#include "../pin_manager.h"
#include "../driver_uart/uart.h"
#include "../rtcc.h"

/******************************************************************************/
/******************************************************************************/
/*********************     DRIVER GSM3 SIM800H        *************************/
/***************************                ***********************************/
/*****************                                         ********************/

/**------------------------->> D E F I N I T I O N S <<------------------------*/
#define _DEBUG (1)
//
#define GSM_TRY_TO_CON_MAX 5    // maxi 20 tentatives 

#define POSITIVE "\r\nOK\r\n"

//DELIMITER
/*
 * Termintation characters 
 */
#define TERMINATION_CHAR        0x0D  // CR
#define TERMINATION_CHAR_ADD    0x1A  // ctrl-z

/**------------------------->> M A C R O S <<----------------------------------*/
//UART MAPING
#define GSM3_Write(c) UART2_Write(c)
#define GSM3_UART_WriteBuffer(inToSend, size) UART2_WriteBuffer(inToSend, size)
#define GSM3_Read() UART2_Read()
#define GSM3_is_tx_ready() UART2_is_tx_ready()
#define GSM3_is_rx_ready() UART2_is_rx_ready()
#define GSM3_is_tx_done() UART2_is_tx_done()
#define GSM3_StatusGet() UART2_StatusGet()
#define GSM3_Receive_ISR() UART2_Receive_ISR()
#define GSM3_ReceiveBufferIsEmpty() UART2_ReceiveBufferIsEmpty()
#define GSM3_SetRxInterruptHandler(handler) UART2_SetRxInterruptHandler(handler)

//PIN MAPPING
#define STAT STAT_GetValue();

#define PWK(state) { PORTBbits.RB4 = state; }

/**-------------------------->> P R O T O T Y P E S <<-------------------------*/

/**
 * active ou desactive le module en fonction du parametre  
 * @param : 
 *      true : module desactive
 *      false: module active 
 */
void GSM3_SetPWK(bool);

/**
 * 
 * @return :  
 */
bool GSM3_GetStatus(void);

/**
 * 
 * @return : RTS state 
 */
bool GSM3_GetRTS(void);

/**
 * 
 * @param state : 
 *              <\br> true : set hight
 *              <\br> false: set low
 */
void GSM3_SetCTS(bool);

/**
 * 
 * @param powerState
 * @return 
 */
bool GMS3_ModulePower(bool powerState);

/**
 * 
 * @param inToSend
 */
void GSM3_TransmitCommand(uint8_t * inToSend);

/**
 * 
 * @param string
 * @param delimiter
 */
void GSM3_TransmitString(uint8_t * string, uint8_t delimiter);


/**
 * recherche un mot cle dans la reponse recu
 * 
 * cette fonction sera utilise pour confirner les reponse recu
 * 
 * @param strToSearch : le mot a chercher
 * @param response : la reponse recu 
 * @return :
 *          true  : le mot est trouve
 *          false : le mot n'existe pas dans la reponse 
 */
bool GSM3_findStringInResponse(uint8_t* strToSearch, uint8_t * response);

/**
 * 
 * @param smsToSend
 * @return 
 */
char* GSM3_GetResponse(void);

/**
 * 
 * @param smsToSend
 * @return 
 */
void GSM3_ReadyReceiveBuffer(void);


/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/
#endif	/* DRIVER_GSM3_SIM800_H */

