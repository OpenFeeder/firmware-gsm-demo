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
 *                    APP GSM3 SIM800H (.c)
 * _____________________________________________________________________________
 *
 * Titre            : Mise en oeuvre d'une appi gsm sim800H 
 * Version          : v00
 * Date de creation : 11/07/2019
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
#ifndef appGSM_SIM800_H
#define	appGSM_SIM800_H

/**------------------------->> I N C L U D E S <<------------------------------*/
#include <xc.h> // include processor files - each processor file is guarded.  
#include "../../fw02_1.X/mcc_generated_files/driver/atCommandsGSM3_SIM800H.h"
#include "../../fw02_1.X/mcc_generated_files/driver/driverGSM3_SIM800.h"


/******************************************************************************/
/******************************************************************************/
/*********************        APP SIM 800H            *************************/
/***************************                ***********************************/
/*****************                                         ********************/



/**-------------------------->> P R O T O T Y P E S <<-------------------------*/

//____________________________CONFIGURATION_____________________________________

/*##############*
 * TE PARAMETER
 *##############*/

/**
 * 
 * @return 
 */
bool app_Echo();

/**
 * 
 */
void app_DisplayProductIdInfos();


/**
 * active or desactive the echo response 
 * @param mode : 
 *              true  : active
 *              false : desactive 
 * @return confirmation in boolean mode 
 */
bool app_SetEchoMode(bool mode);

/**
 * 
 * @return 
 */
bool app_RstDefaultConfig();

/**
 * AT&W
 * @return 
 */
bool app_SaveLastConfig();

/**
 * CSCS=? 
 * @param shset :   
 *              "GSM"           : GSM 7 bit default alphabet (3GPP TS 23.038);
 *              "UCS2"          : 16-bit universal multiple-octet coded character set
 *                                (ISO/IEC10646); UCS2 character strings are converted to
 *                                hexadecimal numbers from 0000 to FFFF; e.g.
 *              "004100620063"  : equals three 16-bit characters with decimal
 *                                values 65, 98 and 99
 *              "IRA"           : International reference alphabet (ITU-T T.50)
 *              "HEX"           : Character strings consist only of hexadecimal
 *                                bers from 00 to FF;
 *              "PCCP"          : PC character set Code
 *              "PCDN"          : PC Danish/Norwegian character set
 *              "8859-1"        : ISO 8859 Latin 1 character set
 * @return 
 *      true  : ok 
 *      false : ko 
 */
bool app_SelectTECharacterSet(uint8_t * shset);

/**
 * 0,1 ==> OK
 * 0,5 ==> OK
 * Others ==> KO
 * @return 
 */
bool app_NetWorkRegistration();

/**
 * 
 * @return 
 */
bool app_NetWorkQuality();

/**
 * 
 * @param cfun
 * @return 
 */
bool app_SetPhoneFunctionality(int8_t cfun);

/**
 * 
 * @param mode
 * @return 
 */
bool app_SwitchOnOrOffEDGE(bool mode);

/**
 * note : la configuration par defaut c'est 0 auto-bauding
 * @param iprBaudRate : between 1200 - 115200 (uart communication)
 * @return 
 */
bool app_SetLocalRate(int32_t iprBaudRate);

/**
 * 
 * @return : le niveau de batterie en %  entre 0-100
 *          -1 error
 */
int8_t app_GetBatteryLevel();

/*##############*
 * SIM CARD
 *##############*/

/**
 * enter the pincode 
 * @param pincode : pincode integer de 4 chiffres 
 * @return :
 *          true  : pin code valide
 *          fasle : pin code error
 */
bool app_SetPinCode(uint8_t * pincode);

/**
 * modify the pin code : the sim must be ready
 * 
 * @param lastPinCode
 * @param newPinCode
 * @return 
 *        true  : pin code valide
 *        fasle : pin code error
 */
bool app_ChangePinCode(int16_t lastPinCode, int16_t newPinCode);

/**
 * 
 * @return : status of simCard
 *          READY       : MT is not pending for any password
 *          SIM PIN     : MT is waiting SIM PIN to be given
 *          SIM PUK     : MT is waiting for SIM PUK to be given
 *          PH_SIM PIN  : ME is waiting for phone to SIM card (antitheft)
 *          PH_SIM PUK  : ME is waiting for SIM PUK (antitheft)
 *          SIM PIN2    : PIN2, e.g. for editing the FDN book possible only 
 */
uint8_t * app_GetSimState();


/*##############*
 * GSM TIME
 *##############*/

/**
 * 
 * @return :
 *          true  : localtime config 
 *          false : error
 */
bool app_ConfigureModuleToLocalTime();

/**
 * 
 * @return :
 *          true  : rtc updated
 *          false : error
 */
bool app_UpdateRtcTimeFromGSM();

/*##############*
 * SMS SERVICE
 *##############*/

/**
 * 
 * @param mode : 
 *              1 = TXT mode 
 *              0 = PDU mode (default value)
 * @return :
 *          true  : OK 
 *          false : KO
 */
bool app_SetSmsFormat(bool mode);

/**
 * 
 * @param smsToSend
 * @return 
 */
bool app_SendSMS(uint8_t * smsToSend);

// juste to test
void demo();


/*##############*
 * GPRS SUPPORT
 *##############*/

/**
 * 
 * @param state :
 *              true  : attchement
 *              false : detachement
 * @return 
 */
bool app_AttachGPRS(bool state);

/**
 * 
 * @param cid       : PDP Context Identifier    < 4
 * @param PDP_type  : IP Internet Protocol (IETF STD 5) 
 * @param APN       : Access Point Name
 * @return :
 *          true  : OK
 *          false : error
 */
bool app_DefinePDPContext(int8_t cid, uint8_t* PDP_type, uint8_t * APN);

/**
 * 
 * @param cid   : specifie the context how want to active
 * @param state :
 *              true  : enable 
 *              false : desable
 * @return 
 */
bool app_ActivePDPContext(int8_t cid, bool state);

/**
 * 
 * @param sate : 
 *              true  : enable gprs
 *              false : desable gprs
 * @param APN  : if apn needed to enable gprs communication
 * @return :
 *          true  : process sucess 
 *          false : process error 
 */
bool app_EnableModuleInGPRSmode(bool sate, uint8_t * APN);

/*#################*
 * TCP APPLICATION
 *#################*/

/**
 * 
 * @return :
 *          true  : connected
 *          false : no connected
 */
bool app_TCPconnected();

/**
 * 
 * @param ipAdrr : server adress, or domain name 
 * @param port : server port 
 * @return : 
 *          true  : module connected to server  
 *          false : module no connected to server 
 */
bool app_StartTCPconnection(uint8_t * ipAdrr, uint8_t* port);

/**
 * 
 * @return :
 *          true  : module disconnected to server  
 *          false : error
 */
bool app_TCPclose(void);


bool app_TCPsend(uint8_t * dataToSend);

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/
#endif	/* appGSM_SIM800_H */

