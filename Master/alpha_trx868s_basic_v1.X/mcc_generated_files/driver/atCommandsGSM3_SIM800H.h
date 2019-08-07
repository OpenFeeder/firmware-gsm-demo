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
 *                    AT COMMANDES M95
 * _____________________________________________________________________________
 *
 * Titre            : Mise en oeuvre du module GSM m95 quectel
 * Version          : v00
 * Date de creation : 05/06/2019
 * Auteur           : MMADI Anzilane
 * Contact          : anzilan@hotmail.fr
 * Web page         : 
 * Collaborateur    : ...
 * Processor        : PIC24
 * Tools used       : MPLAB X IDE v5.15 and MPLAB Code Configurator (MCC) Version: 3.36
 * Compiler         : Microchip XC16 v1.35
 * Programmateur    : PICkit 3
 *******************************************************************************
 *******************************************************************************
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef AT_COMMANDS_GSM3_SIM800_H
#define	AT_COMMANDS_GSM3_SIM800_H

/**------------------------->> I N C L U D E S <<------------------------------*/
#include <xc.h> // include processor files - each processor file is guarded.  


/**------------------------->> C M D . A T  <<---------------------------------*/
//Responses
#define POSITIVE "\r\nOK\r\n"
#define NEGATIVE "ERROR"
#define NETWORK_OK_1 "OK+CREG: 0,5" 
#define NETWORK_OK_2 "OK+CREG: 0,2"

#define AT "AT"


#define ENTER_PIN "AT+CPIN=\"0000\"" //Enter pin \"0000\"
#define SMS_MODE_TXT "AT+CMGF=1" // Set sms in text mode
#define POWER_OFF "AT+CPOWD=1 " //Power off TODO Investigate why it does error
#define QUALITY_TEST "AT+CSQ" //Signal quality test, value range is 0-31 , 31 is the best
#define SIM_TEST "AT+CCID" //Read SIM information to confirm whether the SIM is plugged
#define GET_NETWORK_INFO "AT+COPS?" //Check which network you are connected to, in this case 40466 (BSNL)
#define GET_OP "AT+COPS?" //Return network name
#define SMS_TXT_MODE "AT+CSMP=17,167,0,0"






/*        ======== ABREVIATION ===========
 * ME (Mobile Equipment)
 * MS (Mobile Station)
 * TA (Terminal Adapter)
 * DCE (Data Communication Equipment)
 * Facsimile DCE(FAX modem, FAX board)
 * TE (Terminal Equipment)
 * DTE (Data Terminal Equipment)
 * 
 * 
 *        ======== AT CMD SYNTAX ==========
 * La cast n'a pas d'importance  AT ou at
 * CMD entry : AT+CMD<CR>
 * Response format : <CR><LF><response><CR><LF>
 * 
 * いい Basic syntax
 * These AT Commands have the format of ?AT<x><n>?, or ?AT&<x><n>?, where ?<x>? is the
 * command, and ?<n>? is/are the argument(s) for that command. An example of this is ?ATE<n>?,
 * which tells the DCE whether received characters should be echoed back to the DTE according to
 * the value of ?<n>?. ?<n>? is optional and a default will be used if it is missing. 
 * 
 * 
 * いい S parameter syntax
 * These AT Commands have the format of ?ATS<n>=<m>?, where ?<n>? is the index of the S
 * register to set, and ?<m>? is the value to assign to it. ?<m>? is optional; if it is missing, then a 
 * default value is assigned. 
 * 
 * 
 * ==========================================================================================
 * = Test Command     | AT+<x>=?    | This command returns the list of parameters and value =
 * =                  |             | ranges set by the corresponding Write Command or      = 
 * =                  |             | internal processes.                                   =
 * =__________________|_____________|_______________________________________________________=
 * = Read Command     | AT+<x>?     | This command returns the currently set value of the   =
 * =                  |             | parameter or parameters.                              =
 * =__________________|_____________|_______________________________________________________=
 * = Write Command    | AT+<x>=<?>  |This command sets the user-definable parameter         =
 * =                  |             | values.                                               =
 * =__________________|_____________|_______________________________________________________=
 * = Execution        | AT+<x>      | This command reads non-variable parameters affected   =
 * = Command          |             | by internal processes in the GSM engine               =
 * ==========================================================================================
 */





/**---------------------> P O W E R - M O D U L E <----------------------------*/
#define NORMAL_POWER_OFF  "AT+QPOWD=1"  // 1 : indique la mise hors tension normal
//response : NORMAL POWER DOWN
#define URGENT_POWER_OFF  "AT+QPOWD=0"  // 0 : indique la mise hors tension urgente
//response : OK 

/**---------------------> U A R T - C O N F I G <------------------------------*/
// POUR L'UART c'est fait automatiquement dans notre cas 


/**---------------------> V E R S I O N & S T A T U S - I N F O S <------------*/

#define ATI  "ATI"    // informe sur la version 
/*response : 
 *         Quectel_Ltd                // fabrication
 *         Quectel_M95                // mode gsm
 *         Revision: M95FAR02A08      // version du firmware
 * 
 *         OK   
 */

#define ATV  "AT&V"     // infos sur la config actuelle 
/*response : 
 *         ACTIVE PROFILE 
 *         E: 0
 *         Q: 0
 *         V: 1 
 *         +QECHO(NORMAL_AUDIO): 221,1024,16388,849,0 (may be differente)
 *         +QECHO(Earphone_AUDIO): 221,1024,0,849,1 
 *         +QECHO(LoudSpk_AUDIO): 224,1024,5128,374,2 
 *         +QSIDET(NORMAL_AUDIO): 80
 *         +QSIDET(HEADSET_AUDIO): 144
 *         +QCLIP: 0
 *         +CSNS: 0 
 * 
 *         OK 
 */
//---> for more infos please viste section 4:  
//https://www.quectel.com/UploadImage/Downlad/Quectel_GSM_ATC_Application_V1.0.pdf

/**---------------------> C A R T E - S I M - S E C U R T E <------------------*/

#define LOCK_PIN   "AT+CLCK=\"SC\",0,\"1234\""      // <mode>=0 , cancel lock function of PIN code 
/*response : if ? 
 *          "SC",1,"1234"       // <mode>=1 means Open lock function of PIN code
 *                              // Open PIN lock successfully 
 *           
 *          "SC",2,             // <mode>=2 means Query state of PIN lock
 *          +CLCK:0             // <mode>=0 means the state of PIN lock is off
 * 
 */

#define PIN_STATUS "AT+CPIN?"  // le atatus du pin
/*response :
 *         +CPIN: SIM PIN    if pin needed
 *         
 *         OK 
 */

#define PIN_INPUT 1234     // saisir le code pin : remplacer 1234 par le votre 5983
/*response :
 *         +CPIN: READY    authentification succcessful
 *         
 *         OK 
 */

#define PUK_INPUT "AT+CPIN=\"26601934\",\"1234\"" // to unlock sim when, we have 3 errors 
/*response :
 *         +CPIN: READY    new pin code 
 *         
 *         OK 
 */

#define CHANGE_PIN_CODE = "AT+CPWD=\"SC\",\"1234\",\"4321\""   // remplacer 1234 par 4321
/*response : OK 
 */

/**---------------------> N E T W K - A N D - S E T T I N G <------------------*/
#define QUALITY_TEST "AT+CSQ"      // questionne la qualite du signal (comprise entre 0 et 30)
/*response : 
 *         +CSQ:xx
 * 
 *         OK   
 */

#define NETWK_GSM_STATUS "AT+CREG?"    // infos sur l'etat du reseau GSM
/*response : 
 *         +CREG:0,1               // le reseau est enregistre <stat> = 1
 * 
 *         OK   
 */

#define NETWK_GPRS_STATUS "AT+CGREG?"   // infos sur l'etat du reseau GPRS
/*response : 
 *         +CGREG:0,1               // le reseau est enregistre <stat> = 1
 * 
 *         OK   
 */

#define NETWK_GSM_STATUS "AT+COPS?"    // infos sur le reseau mobile
/*response : 
 *         +COPS:0,0,"SFR"          // retour l'opperateur utiliser ==> SFR dans mon exemple 
 * 
 *         OK   
 */

#define URC_EN_RP_STAT_GSM "AT+CREG=2"   //status du reseau gsm 
/*response : 
 *         +CGREG:0,1               // le reseau est enregistre <stat> = 1
 * 
 *         OK   
 */

#define GSM_BAND "AT+QBAND?"
/*response : OK   
 */


#define INFOS_NUM  "AT+CNUM"
/*response : 
 *           [<alpha1> = ""],
 *            <number1> = "+33XXXXXXXXX" 
 *            <type1> 
 *            [,<speed>,
 *            <service>
 *  
 *          OK      
 */

#define GET_TIME  "AT+CCLK?"  // recupere la date 
/*response : 
 *         +CCLK: "04/01/01,02:14:47+00"
 * 
 *         OK
 */


#define ECHO_OFF "ATE0"
/** response : ATE0
 *           OK
 */
#define ECHO_ON "ATE1"
/** response : 
 *           OK
 */



/**--------------------->  S M S - S E N D - R E C I E V E <------------------*/
/**
    AT command Description
    AT+CPMS Preferred SMS message storage
    AT+CSMP Set SMS text mode parameters
    AT+CMGF Select SMS message format
    AT+CSCS Select TE character set
    AT+CMGW Write SMS message to memory
    AT+CMGR Read SMS message
    AT+CMGL List SMS messages from preferred store
    AT+CMGS Send SMS message
    AT+CMGD Delete SMS message
    AT+QMGDA Delete all SMS
    AT+CSDH Show SMS text mode parameters
    AT+CSCA SMS service center address
    AT+CNMI New SMS message indications
    AT+CSAS Save SMS settings
    AT+CRES Restore SMS settings
    AT+CSCB Select cell broadcast SMS messages
 */

#define SMS_SUP_STORAGE "AT+CPMS=?"
/*response :  
 *          +CPMS: ("SM","ME","MT"),("SM","ME","MT"),("SM","ME","MT")
 *                        "SM" indicates that SMS is stored in SIM card storage, "ME" indicates
 *                         module storage, and "MT" indicates SIM card storage and module
 *                         storage(prior to store in SIM card storage?
 * 
 *          OK
 */

#define SMS_SUP_SETTING "AT+CPMS?"
/*response :  
 *          +CPMS: "SM",4,30,"SM",4,30,"SM",4,30
 *                         <mem1>="SM" indicates SMS to be read and deleted from SIM card 
 *                         storage, <used1>=4 indicates there are 4 SMS to be read and 
 *                         deleted,<total1>=30 indicates the SMS capacity of SIM card is 30 
 * 
 *          OK
 */

#define SMS_SET_TXT_MODE_GSM "AT+CMGF=1"   //config en mode texte 
/*response : OK 
 */

#define SMS_SET_TXT_MODE_PDU "AT+CMGF=0"   //config en mode texte PDU
/*response : OK 
 */

#define SMS_SET_CHAR_TO_GSM "AT+CSCS=GSM"   // parametrer les caractere sur gsm
/*response : OK 
 */

#define SMS_WRITE "AT+CMGW"  //ecrire un sms

#define SMS_SEND_NUM "AT+CMGS=\"0761925644\""

#endif	/* AT_COMMANDS_GSM3_SIM800_H */

