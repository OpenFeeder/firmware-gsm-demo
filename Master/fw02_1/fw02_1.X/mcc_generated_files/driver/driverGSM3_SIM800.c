/* *****************************************************************************
 * 
 * _____________________________________________________________________________
 *
 *                    DRIVER GSM3 SIM800H (.C)
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
 ******************************************************************************/

/*------------------------------> I N C L U D E S  <--------------------------*/


#include "atCommandsGSM3_SIM800H.h"
#include "driverGSM3_SIM800.h"

/******************************************************************************/
/******************************************************************************/
/*********************     DRIVER GSM3 SIM800H        *************************/
/***************************                ***********************************/
/*****************                                         ********************/

/*------------------------------> V A R I A B L E S / D E F <-----------------*/

////DEBUG
//volatile int16_t nbSend = 0;


#define gsm3_response_bufferSize  255
static uint8_t gsm3_response_index = 0;
static char gsm3_response_buffer[gsm3_response_bufferSize] = {0};

/*------------------------------> L O C A L - P R O T O T Y P E <-------------*/

void GSM3_CaptureReceiveMsg(void);

int8_t read_buf_uart(uint8_t buff[48]);

void gsm_flush_buffer();

/*------------------------------> P U B L I C - F U C T I O N S <-------------*/

void GSM3_SetPWK(bool state) {
    PWK(state);
}

/*********************************************************************
 * Function:        void GMS3_SetCTS(bool state)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
//void GMS3_SetCTS(bool state) {
//    CTS(state);
//}

/*********************************************************************
 * Function:        bool GSM3_GetRTS(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
//bool GSM3_GetRTS(void) {
//    return RTS;
//}

/*********************************************************************
 * Function:        bool GMS3_GetSTA(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
bool GMS3_GetSTA(void) {
    return STAT;
}

/*********************************************************************
 * Function:        void transmitCommand(uint8_t * inToSend)
 *
 * PreCondition:    None
 *
 * Input:           inToSend : commande to send at GSM module
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
void GSM3_TransmitCommand(uint8_t * inToSend) {
    GSM3_UART_WriteBuffer(inToSend, strlen(inToSend));
    GSM3_Write(TERMINATION_CHAR);
}

/*********************************************************************
 * Function:        void GSM3_TransmitString(uint8_t * string, uint8_t delimiter)
 *
 * PreCondition:    None
 *
 * Input:           string : message to send 
 *                  delimiter : 
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
void GSM3_TransmitString(uint8_t * string, uint8_t delimiter) {
    int8_t size = strlen(string);
    uint8_t buf[size + 2];
    sprintf(buf, "%s%c", string, delimiter);
    GSM3_UART_WriteBuffer(string, size + 1);
    GSM3_Write(delimiter);
}

/*********************************************************************
 * Function:        bool GMS3_ModulePower(bool powerState)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
bool GMS3_ModulePower(bool powerState) {
    if (powerState) {
#if defined(_DEBUG)
        printf("GMS MODULE POWER ON...\n");
#endif
        GSM3_SetRxInterruptHandler(GSM3_CaptureReceiveMsg);
        GSM3_SetPWK(true);
        TMR_Delay(1000);
        GSM3_SetPWK(false);
        TMR_Delay(6000); //6 s
        uint8_t tryToConnect = 0;
        char * read;
        do {
            GSM3_ReadyReceiveBuffer(); // Prepare for next message
            //            sprintf(buf, "AT%c",TERMINATION_CHAR);
            GSM3_TransmitCommand((uint8_t*) "AT");
            TMR_Delay(1000);
            read = GSM3_GetResponse(); // Read 'OK' String from Buffer
            if (tryToConnect++ > GSM_TRY_TO_CON_MAX) {
                return false;
            }
        } while (!GSM3_findStringInResponse("OK", read));
        //code pin 
        read = app_GetSimState();
        if (!GSM3_findStringInResponse("READY", read)) {
            if (app_SetPinCode(PIN_INPUT) == false) {
#if defined(_DEBUG)
                printf("PIN CODE ERROR !!!!\n");
#endif
                return false;
            }
        }
#if defined(_DEBUG)
        printf("PIN OK ==> REDY \n");
#endif
        
        TMR_Delay(12000); // 12 seconde 
        if (!app_UpdateRtcTimeFromGSM()) {
#if defined(_DEBUG)
            printf("TIME NO UPDATE \n");
#endif
        }
#if defined(_DEBUG)
            printf("TIME UPDATED \n");
#endif
        GSM3_ReadyReceiveBuffer(); // Prepare for next message
        //            sprintf(buf, "AT%c",TERMINATION_CHAR);
        GSM3_TransmitCommand("AT+CMGF=1");
        TMR_Delay(1000);
        read = GSM3_GetResponse();
        if (!GSM3_findStringInResponse("OK", read)) {
            return false;
        }
    }
    return true;
}

void GSM3_TransmitCommandTest(uint8_t * inToSend) {
    GSM3_UART_WriteBuffer(inToSend, strlen(inToSend));
}

/*********************************************************************
 * Function:        bool GSM3_findStringInResponse(uint8_t* strToSearch, uint8_t * response)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
bool GSM3_findStringInResponse(uint8_t* strToSearch, uint8_t * response) {
    int8_t siezeStr = strlen(strToSearch);
    int8_t i = 0;
    int8_t j = 0;
    while (response[i] != '\0') {
        if (strToSearch[j] == response[i]) {
            j++;
            if (j == siezeStr) {
                return true;
            }
        } else
            j = 0;
        i++;
    }
    return false;
}

/*********************************************************************
 * Function:        static char* GSM3_GetResponse(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
char* GSM3_GetResponse(void) {
    return gsm3_response_buffer;
}

//______________________________________________________________________________
/*------------------------------> L O C A L - F U C T I O N S <-------------*/

/*********************************************************************
 * Function:        void GSM3_CaptureReceiveMsg(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
void GSM3_CaptureReceiveMsg(void) {
    GSM3_Receive_ISR();
    uint8_t readByte = uart[GSM3].Read();
    if ((readByte != '\0') && (gsm3_response_index < gsm3_response_bufferSize - 1)) {
        gsm3_response_buffer[gsm3_response_index++] = readByte;
    } else {
        gsm3_response_buffer[gsm3_response_index] = '\0';
    }
}

/*********************************************************************
 * Function:        static void GSM3_ReadyReceiveBuffer(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
void GSM3_ReadyReceiveBuffer(void) {
    gsm3_response_index = 0;
    int i;
    for (i = 0; i < gsm3_response_bufferSize; i++) {
        gsm3_response_buffer[i] = 0;
    }
}

void gsm_flush_buffer() {
    uint8_t c;
    while (!GSM3_ReceiveBufferIsEmpty()) {
        c = GSM3_Read();
    }
}


/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/