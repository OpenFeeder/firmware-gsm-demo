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

/**------------------------->> I N C L U D E S <<------------------------------*/
#include <stdio.h>

#include "appGSM3_SIM800.h"

/******************************************************************************/
/******************************************************************************/
/*********************        APP SIM 800H            *************************/
/***************************                ***********************************/
/*****************                                         ********************/

/*------------------------------> L O C A L - P R O T O T Y P E <-------------*/

int8_t read_buf_uart(uint8_t buff[48]);
//______________________________________________________________________________

/*##############*
 * TE PARAMETER
 *##############*/


bool app_Echo() {
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("AT+CPIN=\"1234\"");
    TMR_Delay(1000);
    printf("%s", GSM3_GetResponse());
    return true;
}

void app_DisplayProductIdInfos() {
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("AT+GSV");
    TMR_Delay(1000);
    printf("%s", GSM3_GetResponse());
}

/*********************************************************************
 * Function:        bool app_SetEchoMode(bool mode)
 *
 * PreCondition:    module must be power on 
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
bool app_SetEchoMode(bool mode) {
    GSM3_ReadyReceiveBuffer();
    if (mode)
        GSM3_TransmitCommand(ECHO_ON);
    else
        GSM3_TransmitCommand(ECHO_OFF);
    TMR_Delay(1000);
    return GSM3_findStringInResponse("OK", GSM3_GetResponse());
}

bool app_RstDefaultConfig() {
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("ATZ");
    TMR_Delay(1000);
    return GSM3_findStringInResponse("OK", GSM3_GetResponse());
}

bool app_SaveLastConfig() {
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("AT&W");
    TMR_Delay(1000);
    return GSM3_findStringInResponse("OK", GSM3_GetResponse());
}

bool app_SelectTECharacterSet(uint8_t * shset) {
    uint8_t buf[20];
    sprintf(buf, "AT+CSCS=\"%s\"", shset);
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand(buf);
    TMR_Delay(1000);
    return GSM3_findStringInResponse("OK", GSM3_GetResponse());
}

bool app_NetWorkRegistration() {
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("AT+CREG?");
    TMR_Delay(1000);
    return GSM3_findStringInResponse("0,1", GSM3_GetResponse()) ||
        GSM3_findStringInResponse("0,5", GSM3_GetResponse());
}

bool app_NetWorkQuality() {
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("AT+CSQ");
    TMR_Delay(1000);
#if defined(_DEBUG)
    printf("resp : %s\n", GSM3_GetResponse());
#endif
}
bool app_SetPhoneFunctionality(int8_t cfun) {
    if (cfun != 1 || cfun != 0 || cfun != 4) return false;
    //a surveiller et tester 
    uint8_t buf[20];
    sprintf(buf, "AT+CFUN=%d", cfun);
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand(buf);
    TMR_Delay(1000);
    return GSM3_findStringInResponse("READY", GSM3_GetResponse());
}

bool app_SwitchOnOrOffEDGE(bool mode) { // on or off
    GSM3_ReadyReceiveBuffer();
    if (mode)
        GSM3_TransmitCommand("AT+CEGPRS=1");
    else
        GSM3_TransmitCommand("AT+CEGPRS=0");
    TMR_Delay(1000);
    return GSM3_findStringInResponse("OK", GSM3_GetResponse());
}

/*********************************************************************
 * Function:        bool app_SetLocalRate(int32_t iprBaudRate)
 *
 * PreCondition:    !(iprBaudRate < 1200 || iprBaudRate > 115200)
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note: de preference laisser le baudrate a 0 ==> automatique    
 *       aussi cette commande doit etre suivi d'une sauvgarde de
 *       configuration  AT&W
 ********************************************************************/
bool app_SetLocalRate(int32_t iprBaudRate) {
    if (iprBaudRate < 1200 || iprBaudRate > 115200) return false;
    uint8_t buf[20];
    sprintf(buf, "AT+IPR=%d", iprBaudRate);
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand(buf);
    TMR_Delay(1000);
    return GSM3_findStringInResponse("OK", GSM3_GetResponse());
}

/*********************************************************************
 * Function:        int8_t app_GetBatteryLevel();
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
int8_t app_GetBatteryLevel() {
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("AT+CBC");
    TMR_Delay(1000);
    char *respons = GSM3_GetResponse();
    if (!GSM3_findStringInResponse("+CBC", respons)) return -1;
    uint8_t buf[4] = {0};
    int8_t i = 10;
    while ((respons[i] != ',') && (i - 10 < 4))
        buf[i - 10] = respons[i++];
    return atoi(buf); // je ne suis pas sur ici il faut peut etre mettre me \0
}


/*##############*
 * SIM CARD
 *##############*/

/*********************************************************************
 * Function:        bool app_SetPinCode(uint8_t*pincode);
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

bool app_SetPinCode(int16_t pincode) {
    uint8_t buf[20];
    sprintf(buf, "AT+CPIN=\"%d\"", pincode);
#if defined(_DEBUG)
    printf("%s\n", buf);
#endif
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand(buf);
    TMR_Delay(1000);
    return GSM3_findStringInResponse("READY", GSM3_GetResponse());
}

bool app_ChangePinCode(int16_t lastPinCode, int16_t newPinCode) {
    uint8_t buf[40];
    sprintf(buf, "AT+CPWD=\"SC\",\"%d\",\"%d\"", lastPinCode, newPinCode);
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand(buf);
    TMR_Delay(1000);
    return GSM3_findStringInResponse("READY", GSM3_GetResponse());
}

uint8_t * app_GetSimState() {
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("AT+CPIN?");
    TMR_Delay(100);
    return GSM3_GetResponse();
}

bool app_PowerDown(bool mode) {
    GSM3_ReadyReceiveBuffer();
    if (mode)
        GSM3_TransmitCommand("AT+CPOWD=1"); // normal mod
    else
        GSM3_TransmitCommand("AT+CPOWD=0"); // urgent mode 
    TMR_Delay(1000);
    return GSM3_findStringInResponse("NORMAL", GSM3_GetResponse());
}

/*##############*
 * GSM TIME
 *##############*/

bool app_ConfigureModuleToLocalTime() {
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("AT+CLTS=1;&w");
    TMR_Delay(100);
    if (!GSM3_findStringInResponse("OK", GSM3_GetResponse())) return false;
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("AT+COPS=2");
    TMR_Delay(1000);
    GSM3_TransmitCommand("AT+COPS=0");
    TMR_Delay(6000);
    return true;
}

//_____________local function________

inline uint8_t app_ConvDec(uint8_t g, uint8_t d) {
    return (uint8_t) ((g - 48) * 10 + (d - 48));
}

//void app_SetTimeToRtcc(uint8_t* buff) {
//    struct tm timeToSet;
//    int8_t i = 0;
//    for (; i < strlen(buff); i++) {
//        if (buff[i] == '"') break;
//    }
//    i++;
//    timeToSet.tm_year = app_ConvDec(buff[i], buff[i + 1]);
//    i += 3; //on saute le /
//    timeToSet.tm_mon = app_ConvDec(buff[i], buff[i + 1]);
//    i += 3;
//    timeToSet.tm_mday = app_ConvDec(buff[i], buff[i + 1]);
//    i += 3;
//    timeToSet.tm_hour = app_ConvDec(buff[i], buff[i + 1]);
//    i += 3;
//    timeToSet.tm_min = app_ConvDec(buff[i], buff[i + 1]);
//    i += 3;
//    timeToSet.tm_sec = app_ConvDec(buff[i], buff[i + 1]);
//    i += 3;
////    timeToSet.tm_wday = app_ConvDec(buff[i], buff[i + 1]);
////    i += 3;
//    RTCC_TimeSet(&timeToSet);
//}

bool app_UpdateRtcTimeFromGSM() {
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("AT+CCLK?");
    TMR_Delay(100);
    // il faudrait un test icii d'abord mais bon
    char * buff = GSM3_GetResponse();
    if (!GSM3_findStringInResponse("+CCLK", buff)) return false;
    ;
    //
    struct tm timeToSet;
    int8_t i = 0;
    for (; i < strlen(buff); i++) {
        if (buff[i] == '"') break;
    }
    i++;
    timeToSet.tm_year = app_ConvDec(buff[i], buff[i + 1]);
    i += 3; //on saute le /
    timeToSet.tm_mon = app_ConvDec(buff[i], buff[i + 1]);
    i += 3;
    timeToSet.tm_mday = app_ConvDec(buff[i], buff[i + 1]);
    i += 3;
    timeToSet.tm_hour = app_ConvDec(buff[i], buff[i + 1]);
    i += 3;
    timeToSet.tm_min = app_ConvDec(buff[i], buff[i + 1]);
    i += 3;
    timeToSet.tm_sec = app_ConvDec(buff[i], buff[i + 1]);
    i += 3;
    //    timeToSet.tm_wday = app_ConvDec(buff[i], buff[i + 1]);
    //    i += 3;
    RTCC_TimeSet(&timeToSet);
    return true;
}


/*##############*
 * SMS SERVICE
 *##############*/

/*********************************************************************
 * Function:        bool app_SetSmsFormat(bool mode)
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
bool app_SetSmsFormat(bool mode) {
    GSM3_ReadyReceiveBuffer();
    if (mode)
        GSM3_TransmitCommand("AT+CGMF=1");
    else
        GSM3_TransmitCommand("AT+CGMF=0");
    TMR_Delay(1000);
    return GSM3_findStringInResponse("OK", GSM3_GetResponse());
}

/*********************************************************************
 * Function:        bool app_SendSMS(uint8_t * smsToSend)
 *
 * PreCondition:    strlen(smsToSend) < SMS_MAX_SIZE && ModulePower(true) == true 
 *
 * Input:           smsToSend : text msg to send 
 *
 * Output:          bool : true ==> send OK
 *                       : false ==> send KO
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            we send sms in txt mode 
 ********************************************************************/
bool app_SendSMS(uint8_t * smsToSend) {
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand(SMS_SEND_NUM);
    TMR_Delay(1000);
    char * response = GSM3_GetResponse();
    if (!GSM3_findStringInResponse(">", response)) {
        return false;
    }
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitString(smsToSend, TERMINATION_CHAR_ADD);
    TMR_Delay(1000);
#if defined(_DEBUG)
    printf("SEND ok\n");
#endif
    // un test doit se faire ici pour etre sur que le sms est transmis
    return true; //for the moment we do that 
}

/*##############*
 * DEBUG DEMO
 *##############*/
int8_t read_buf_uart(uint8_t buff[48]) {
    int i = 0;
    uint8_t get;
    uint8_t stop = 0;
    do {
        get = UART1_Read();
        if (i < 47) {
            switch (get) {
                case '$': //power module si c'etait deja eteit
                    GMS3_ModulePower(true);
                    return 0;
                case (char) 26:
                    buff[i++] = get;
                    stop = 1;
                    break;
                case (char) 27:
                    return 0;
                case '!':
                    buff[i++] = (char) 13;
                    stop = 1;
                    break;
                case '*': // test fonction 
                    app_UpdateRtcTimeFromGSM();

                    struct tm * timeToSet;
                    RTCC_TimeGet(timeToSet);
#if defined(_DEBUG)
                    printf("\nheur : %d/%d/%d, %d:%d:%d\n",
                           timeToSet->tm_year,
                           timeToSet->tm_mon,
                           timeToSet->tm_mday,
                           timeToSet->tm_hour,
                           timeToSet->tm_min,
                           timeToSet->tm_sec);
#endif

                    printf("Battery level %d\n", app_GetBatteryLevel());

                    return 0;
                default:
                    buff[i++] = get;
                    break;
            }
        }
    } while (!stop);
    buff[i] = '\0';
    return i;
}

void demo() { // AT+CSDT? important
    //    LED_BLUE_SetHigh();
#if defined(_DEBUG)
    printf("Wait Commande : \n");
#endif
    char *response;
    uint8_t buf[48];
    memset(buf, 0, 48);
    if (read_buf_uart(buf)) {
#if defined(_DEBUG)
        printf("\nCMD : %s\n", buf);
#endif
        gsm_flush_buffer();
        GSM3_ReadyReceiveBuffer();
        GSM3_TransmitCommandTest(buf);
        TMR_Delay(1000); // 1s
        response = GSM3_GetResponse();
#if defined(_DEBUG)
        printf("\nresponse : %s\n", response);
#endif 
    } else {
        printf("\nBAD CMD\n");
    }
    //    LED_BLUE_SetLow();
}

/*##############*
 * GPRS SUPPORT
 *##############*/

bool app_AttachGPRS(bool state) {
    GSM3_ReadyReceiveBuffer();
    if (state)
        GSM3_TransmitCommand("AT+CGATT=1");
    else
        GSM3_TransmitCommand("AT+CGATT=0");
    TMR_Delay(1000);
    return GSM3_findStringInResponse("OK", GSM3_GetResponse());
}

bool app_DefinePDPContext(int8_t cid, uint8_t* PDP_type, uint8_t * APN) {
    if (cid > 3) return false;
    uint8_t buf[100];
    sprintf(buf, "AT+CGDCONT=%d,\"%s\",\"%s\"", cid, PDP_type, APN);
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand(buf);
    TMR_Delay(1000);
    return GSM3_findStringInResponse("OK", GSM3_GetResponse());
}

bool app_ActivePDPContext(int8_t cid, bool state) {
    uint8_t buf[100];
    sprintf(buf, "AT+CGACT=%d,%d", (int8_t) state, cid);
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand(buf);
    TMR_Delay(1000);
    return GSM3_findStringInResponse("OK", GSM3_GetResponse());
}

/*********************************************************************
 * Function:        bool app_EnableModuleInGPRSmode(bool sate, uint8_t * APN)
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
bool app_EnableModuleInGPRSmode(bool sate, uint8_t * APN) {
    if (sate) {
        GSM3_ReadyReceiveBuffer();
        GSM3_TransmitCommand("AT+CIPSHUT"); // necessary, before etablish TCP or UDP connection
        TMR_Delay(2000);
        if (!GSM3_findStringInResponse("SHUT OK", GSM3_GetResponse())) return false;
#if defined(_DEBUG)
        printf("Disconnect all socket ok \n");
#endif

        GSM3_ReadyReceiveBuffer();
        GSM3_TransmitCommand("AT+CGATT=1"); // attache mode gprs 
        TMR_Delay(1000);
        if (!GSM3_findStringInResponse("OK", GSM3_GetResponse())) return false;
#if defined(_DEBUG)
        printf("mode GPRS attached\n");
#endif

        GSM3_ReadyReceiveBuffer();
        GSM3_TransmitCommand("AT+SAPBR=3,1, \"CONTYPE\",\"GPRS\"");
        TMR_Delay(1000);
        if (!GSM3_findStringInResponse("OK", GSM3_GetResponse())) return false;
#if defined(_DEBUG)
        printf("Connection type GPRS ok\n");
#endif

        if (strlen(APN)) {
            uint8_t buf[100];
            sprintf(buf, "AT+SAPBR=3,1,\"APN\",\"%s\"", APN);
            GSM3_ReadyReceiveBuffer();
            GSM3_TransmitCommand(buf);
            TMR_Delay(1000);
            if (!GSM3_findStringInResponse("OK", GSM3_GetResponse())) return false;

            memset(buf, 0, 100); // surveiller cette ligne source d'erreur 
            sprintf("AT+CSTT=\"%s\"", APN);
            GSM3_ReadyReceiveBuffer();
            GSM3_TransmitCommand(buf);
            TMR_Delay(1000);
            if (!GSM3_findStringInResponse("OK", GSM3_GetResponse())) return false;
            //no passWord
            //no userName
            // else if, I must be configure this state 
        }
#if defined(_DEBUG)
        printf("APN set correctly\n");
#endif

        GSM3_ReadyReceiveBuffer();
        GSM3_TransmitCommand("AT+SAPBR=1,1");
        TMR_Delay(1000);
        if (!GSM3_findStringInResponse("OK", GSM3_GetResponse())) return false;
#if defined(_DEBUG)
        printf("GPRS Context open\n");
#endif

        GSM3_ReadyReceiveBuffer();
        GSM3_TransmitCommand("AT+CIICR");
        TMR_Delay(1000);
        if (!GSM3_findStringInResponse("OK", GSM3_GetResponse())) return false;
#if defined(_DEBUG)
        printf("Wireless connection is bring up\n");
        printf("GPRS ENABLED------------------->\n");
#endif
    } else {
        GSM3_ReadyReceiveBuffer();
        GSM3_TransmitCommand("AT+CIPSHUT"); // necessary, before etablish TCP or UDP connection
        TMR_Delay(2000);
        if (!GSM3_findStringInResponse("SHUT OK", GSM3_GetResponse())) return false;
#if defined(_DEBUG)
        printf("Disconnect all socket ok \n");
#endif

        GSM3_ReadyReceiveBuffer();
        GSM3_TransmitCommand("AT+SAPBR=0,1");
        TMR_Delay(1000);
        if (!GSM3_findStringInResponse("OK", GSM3_GetResponse())) return false;
#if defined(_DEBUG)
        printf("close GPRS context ok\n");
#endif

        GSM3_ReadyReceiveBuffer();
        GSM3_TransmitCommand("AT+CGATT=0"); // attache mode gprs 
        TMR_Delay(1000);
        if (!GSM3_findStringInResponse("OK", GSM3_GetResponse())) return false;
#if defined(_DEBUG)
        printf("mode GPRS deattached\n");
#endif
    }
    return true;
}

/*#################*
 * TCP APPLICATION
 *#################*/

/*********************************************************************
 * Function:        bool app_TCPconnected()
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
bool app_TCPconnected() {
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("AT+CIPSTATUS");
    TMR_Delay(100);
    return GSM3_findStringInResponse("CONNECT OK", GSM3_GetResponse());
}

/*********************************************************************
 * Function:        bool app_StartTCPconnection(uint8_t * ipAdrr, int8_t* port)
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
bool app_StartTCPconnection(uint8_t * ipAdrr, int8_t* port) {
    uint8_t buf[100];
    sprintf(buf, "AT+CIPSTART=\"TCP\",\"%s\",\"%s\"", (int8_t) ipAdrr, port);
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand(buf);
    TMR_Delay(1000);
    if (!GSM3_findStringInResponse("OK", GSM3_GetResponse())) return false;
    return app_TCPconnected();
}

/*********************************************************************
 * Function:        bool app_TCPclose(void)
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
bool app_TCPclose(void) {
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("AT+CIPCLOSE"); // may be, we must be add equal 0 : AT+CIPCLOSE=0
    TMR_Delay(100);
    return GSM3_findStringInResponse("OK", GSM3_GetResponse());
}

/*********************************************************************
 * Function:        bool app_TCPsend(uint8_t * dataToSend)
 *
 * PreCondition:    app_TCPconnected() == true
 *
 * Input:           dataTosand : informations to send 
 *
 * Output:          boolean to confirme the processus 
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
bool app_TCPsend(uint8_t * dataToSend) {
#if defined(_DEBUG)
    printf("SEND %s \n", dataToSend);
#endif
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitCommand("AT+CIPSEND"); // may be we can add : AT+CIPSEND=length(dataToSend)
    TMR_Delay(50);
    if (!GSM3_findStringInResponse(">", GSM3_GetResponse())) return false;
    GSM3_ReadyReceiveBuffer();
    GSM3_TransmitString(dataToSend, TERMINATION_CHAR_ADD);
    TMR_Delay(3000);
    return GSM3_findStringInResponse("SEND OK", GSM3_GetResponse());
}

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/