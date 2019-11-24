/** 
 * File: app_master.c
 * Author: OpenFeeder Team <https://github.com/orgs/OpenFeeder/people> 
 * Comments:
 * Revision history:
 */

/* *****************************************************************************
 * 
 * _____________________________________________________________________________
 *
 *                            MASTER API  (.c)
 * _____________________________________________________________________________
 *
 * Titre            : Mise en oeuvre de l'api et la machie a etat du master   
 * Version          : v00
 * Date de creation : 26/05/2019
 * Auteur           : OpenFeeder Team <https://github.com/orgs/OpenFeeder/people> 
 * Contact          : anzilan@hotmail.fr
 * Web page         : 
 * Collaborateur    : ...
 * Processor        : PIC24
 * Tools used       : MPLAB X IDE v5.15 and MPLAB Code Configurator (MCC) Version: 3.36
 * Compiler         : Microchip XC16 v1.35
 * Programmateur    : PICkit 3
 *******************************************************************************
 *******************************************************************************/

/**------------------------>> I N C L U D E <<---------------------------------*/

#include <time.h>

#include "app_master.h"
#include "appGSM3_SIM800.h"
#include "../../fw02_1.X/mcc_generated_files/driver/driverGSM3_SIM800.h"
#define _DEBUG (1)

/******************************************************************************/
/******************************************************************************/
/********************* COUCHE APPLICATION DU MASTER  **************************/
/***************************                ***********************************/
/**************************                         ***************************/
/*****************                                         ********************/

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
 */

MASTER_APP_DATA appData; /* Global application data. */
APP_ERROR appError; /* Errors application data */
APP_DATA_USB appDataUsb; /* USB application data */
APP_DATA_ALARM appDataAlarmSleep; /* Alarm sleep application data */
APP_DATA_ALARM appDataAlarmWakeup; /* Alarm wake-up application data */
APP_DATA_ALARM appDataAlarmDefaultStopOF;
APP_DATA_ALARM appDataAlarmDefaultOnOF;
APP_DATA_LEDS appDataAttractiveLeds; /* Atractive LEDs application data */
APP_DATA_LOG appDataLog; /* Datalogger application data */
APP_DATA_RC appDataRc; /* Remote control application data */
APP_DATA_EVENT appDataEvent; /* Events application data */

//AlphaTRX modifcation 
bool print = false;



/**-------------------------->> M A C R O S <<--------------------------------*/
#define GETpREAD(prio) appData.ptr[prio][PTR_READ] // recupere le ponteur de lecture
#define GETpWRITE(prio) appData.ptr[prio][PTR_WRITE] // recupere le pointeur d'ecriture
#define GET_OVFF(prio)  appData.ptr[prio][PTR_OVFF] // recipere le pointeur d'overFlow

#define INCpREAD(prio) (appData.ptr[prio][PTR_READ] = (appData.ptr[prio][PTR_READ]+1) % NB_BEHAVIOR_PER_PRIO)
#define INCpWRITE(prio) (appData.ptr[prio][PTR_WRITE] = (appData.ptr[prio][PTR_WRITE]+1) % NB_BEHAVIOR_PER_PRIO)
#define SET_0VFF(prio, set) (appData.ptr[prio][PTR_OVFF] = set)

/**-------------------------->> D E B U G <<----------------------------------*/
void printPointeur(PRIORITY prio) {
    printf("prio %d : \npREAD %d\nWRITE %d\n", prio, GETpREAD(prio), GETpWRITE(prio));
}
//______________________________________________________________________________



/**-------------------------->> L O C A L -- F O N C T I O N S <<--------------*/

//_______________________________ IMPLEMENTATION________________________________

int8_t MASTER_SendMsgRF(uint8_t dest,
        uint8_t typeMsg,
        uint8_t idMsg,
        uint8_t nbR,
        uint8_t * data,
        uint8_t sizeData) {
    Frame frameToSend;
    memset(frameToSend.paquet, 0, FRAME_LENGTH);
    //_____________CREATE FRAME____________________________________________
    int8_t ret = 0;
    // en tete 
    frameToSend.Champ.dest = dest;
    frameToSend.Champ.crc ^= frameToSend.paquet[0];
    frameToSend.Champ.src = appData.masterId;
    frameToSend.Champ.crc ^= frameToSend.paquet[1];
    frameToSend.Champ.station = appData.station;
    frameToSend.Champ.crc ^= frameToSend.paquet[2];
    frameToSend.Champ.nbR = nbR;
    frameToSend.Champ.typeMsg = typeMsg;
    frameToSend.Champ.crc ^= frameToSend.paquet[3];
    frameToSend.Champ.idMsg = idMsg;
    frameToSend.Champ.crc ^= frameToSend.paquet[4];
    // data
    int8_t i;
    frameToSend.Champ.size = sizeData;
    frameToSend.Champ.crc ^= frameToSend.paquet[5];
    for (i = 0; i < frameToSend.Champ.size; i++) {
        frameToSend.Champ.data[i] = data[i];
        frameToSend.Champ.crc ^= frameToSend.paquet[i + HEADER]; // penser ? changer le 6 en generique 
    }
    //____________________________________________________________________
    //________________________TRANSMSISSION_______________________________
    if (radioAlphaTRX_SendMode()) {
        ret = radioAlphaTRX_SendData(frameToSend);
    } else {
#if defined(UART_DEBUG)
        printf("non Envoye\n");
#endif
    }
    radioAlphaTRX_ReceivedMode(); // je me remets en attente d'un msg
    return ret;
}

uint8_t MASTER_SendDateRF() {
    struct tm picDate;
    //____________________________
    //________UPDATE DATE_________
    // with GSM module 
    if (appData.masterSynchronizeTime) {
        if (!app_UpdateRtcTimeFromGSM()) {
#if defined(UART_DEBUG)
            printf("No update from GSM\n");
#endif
            return 0;
        }
        appData.masterSynchronizeTime = false;
    }
    //____________________________
    //________GET DATE____________
    if (RTCC_TimeGet(&picDate)) {
        Date d;
        memset(d.date, 0, 5);
        d.dateVal = 0;
        d.Format.yy = picDate.tm_year;
        d.Format.mon = picDate.tm_mon;
        d.Format.day = picDate.tm_mday;
        d.Format.h = picDate.tm_hour;
        d.Format.min = picDate.tm_min;
        d.Format.sec = picDate.tm_sec;
        //____________________________
        //________SEND DATE___________
#if defined(UART_DEBUG)
        printf("SEND DATE\n");
#endif
        return MASTER_SendMsgRF(appData.broadCastId, HORLOGE, 1, 1, d.date, 5); // visualiser cette valeur 
    }
    return 0;
}

//______________________________________________________________________________
//______________________________________________________________________________
//________________________________STATE MACHINE FUNCTION________________________

bool MASTER_StoreBehavior(MASTER_APP_STATES state, PRIORITY prio) {
    appData.behavior[prio][GETpWRITE(prio)] = state; // on ecrit le comportement 
    INCpWRITE(prio);
    if (GETpREAD(prio) == GETpWRITE(prio)) { // Je viens d'ecraser un comportement 
        //        SET_0VFF(prio, 1); // overflow
        INCpREAD(prio);
    }

    return true; // 
}

MASTER_APP_STATES MASTER_GetBehavior() {
    // on cmmence par chercher un comportement de prio eleve
    MASTER_APP_STATES state = MASTER_APP_STATE_IDLE;
    // l'ordre des condition est important car on respect la priorite
    if (GETpREAD(PRIO_EXEPTIONNEL) != GETpWRITE(PRIO_EXEPTIONNEL)) {
        // on ne peut avoir l'egalite et a voir un comportement present 
        state = appData.behavior[PRIO_EXEPTIONNEL][GETpREAD(PRIO_EXEPTIONNEL)];
        INCpREAD(PRIO_EXEPTIONNEL);
    } else if (GETpREAD(PRIO_HIGH) != GETpWRITE(PRIO_HIGH)) {
        // on ne peut avoir l'egalite et a voir un comportement present 
        state = appData.behavior[PRIO_HIGH][GETpREAD(PRIO_HIGH)];
        INCpREAD(PRIO_HIGH);
    } else if (GETpREAD(PRIO_MEDIUM) != GETpWRITE(PRIO_MEDIUM)) {
        state = appData.behavior[PRIO_MEDIUM][GETpREAD(PRIO_MEDIUM)];
        INCpREAD(PRIO_MEDIUM);
    } else if (GETpREAD(PRIO_LOW) != GETpWRITE(PRIO_LOW)) {
        state = appData.behavior[PRIO_LOW][GETpREAD(PRIO_LOW)];
        INCpREAD(PRIO_LOW);
    }
    return state;
}

/*********************************************************************
 * Function:        void MSTER_RetransGsmData(void)
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
void MSTER_RetransGsmData(void) {
    appData.ensSlave[appData.slaveSelected].nbRepet++;
    if (appData.ensSlave[appData.slaveSelected].nbRepet < MAX_REP_GSM_MSG)
        MASTER_StoreBehavior(MASTER_APP_STATE_SEND_FROM_GSM, PRIO_HIGH);
    else {
#if defined (USE_UART1_SERIAL_INTERFACE) 
        printf("ERROR TRANSMISSION DE LA DATA AU GSM\n");
#endif
        /* Log event if required */
        if (true == appDataLog.log_events) {
            store_event(OF_GSM_MSG_NOT_SEND_TO_SERVER);
        }
        sprintf(appError.message, "SERVER NO REQUEST");
        appError.current_line_number = __LINE__;
        sprintf(appError.current_file_name, "%s", __FILE__);
        appError.number = ERROR_SERVER_NO_REQUEST;
        MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_EXEPTIONNEL);
    }
}

void sendEndReportToServer(uint8_t * buf) {
    bool sendOk = false;
    int8_t k = 0;
    do {
        sendOk = app_TCPsend(buf, 5000);
        k++;
    } while (!sendOk && k < 10);

    if (true == appDataLog.log_events && sendOk) {
        store_event(OF_GSM_MSG_COLLECT_END);
    } else {
        store_event(OF_GSM_MSG_NOT_SEND_TO_SERVER);
    }
}

/******************************************************************************
  Function:
    void MASTER_APP_Tasks( void )

  Remarks:
    See prototype in app.h.
 */
void MASTER_AppTask(void) {
    static bool button_user_state;
    static bool previous_button_user_state = BUTTON_NOT_PRESSED;
    APP_CHECK chk;
    int i;
    static bool enter_default_state = false;

    if (!appError.OfInCriticalError)
        appData.state = MASTER_GetBehavior();
    else {
        appData.state = MASTER_APP_STATE_ERROR;
    }

    /* Check the Application State. */
    switch (appData.state) {
        case MASTER_APP_STATE_INITIALIZE:
        {
            /*
             * Initializing the application.
             * 
             * Process:
             *  - Date and Time calibration
             * 
             * Next state: APP_STATE_CONFIGURE_SYSTEM
             */

            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined ( DISPLAY_CURRENT_STATE )
                printf("> APP_STATE_INITIALIZE\n");
#endif

                //______________________________________________________________
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_STATE_INITIALIZE);
                }

            }

            /* Date & time calibration using external RTC module */
            calibrateDateTime();

            /*
             * Power on and Init GSM module 
             * default sms send mode. 
             * we try to power and init 10 repetition 
             */
            uint8_t i = 0;
            while (i < 10 && !appData.gsmInit) {
                i++;
                if (CMD_VDD_APP_V_USB_GetValue()) {
                    appData.gsmInit = GMS3_ModulePower(true); // try to power on module 
                } else {
                    powerUsbGSMEnable();
                }
                TMR_Delay(1000);
            }

            if (i >= 10) { // module n
#if defined(USE_UART1_SERIAL_INTERFACE)
                printf("GSM module not power on \n");
#endif  
                sprintf(appError.message, "Error GSM No POwer On");
                appError.current_line_number = __LINE__;
                sprintf(appError.current_file_name, "%s", __FILE__);
                appError.number = ERROR_GSM_NO_POWER_ON;
                MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_EXEPTIONNEL);
                //______________________________________________________________
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_ALPHA_TRX_MODULE_FAIL);
                }
                break;
            } else {
                // test du timestamp
            }


            /*
             * Power on and Init radio Alpha TRX module 
             * default mode is receive mode. 
             * we try to power and init 10 repetition 
             */
            i = 0;
            while (i < 10 && !appData.RfModuleInit) {
                if (CMD_3V3_RF_GetValue() == false) {
                    if (radioAlphaTRX_Init()) {
#if defined  (USE_UART1_SERIAL_INTERFACE )
                        printf("RF Module Enable And INIT\n");
#endif
                        radioAlphaTRX_ReceivedMode(); // receive mode actived
                        appData.RfModuleInit = true;
                        //______________________________________________________________
                        /* Log event if required */
                        if (true == appDataLog.log_events) {
                            store_event(OF_ALPHA_TRX_MODULE_INIT_OK);
                        }
                    } else
                        i++;
                } else {
                    powerRFEnable();
                    TMR_Delay(1000);
                    i++;
#if defined  (USE_UART1_SERIAL_INTERFACE )
                    printf("RF Module disable.\n");
#endif
                }
            }

            if (i >= 10) { // module n
#if defined(USE_UART1_SERIAL_INTERFACE)
                printf("RF module not power on \n");
#endif  
                sprintf(appError.message, "Error Rf module No POwer On");
                appError.current_line_number = __LINE__;
                sprintf(appError.current_file_name, "%s", __FILE__);
                appError.number = ERROR_GSM_NO_POWER_ON;
                MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_EXEPTIONNEL);
                //______________________________________________________________
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_ALPHA_TRX_MODULE_FAIL);
                }
                break;
            }


            /* Go to device configuration state */
            MASTER_StoreBehavior(MASTER_APP_STATE_CONFIGURE_SYSTEM, PRIO_HIGH);
            break;
        }
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_CONFIGURE_SYSTEM:
            /*
             * System configuration.
             * 
             * Process:
             *  - Set log file name
             *  - Power USB device
             *  - Search for plugged USB device 
             *  - Check status LEDs 
             *  - Check important parameters:
             *      - battery level (mandatory)
             *      - VBat level (mandatory)
             *  - Log Unique Device ID (UDID)
             *  - Test servomotor
             *  - Test I2C interface
             *  - Initialize attractive LEDs (if needed)
             *  
             * Next state: 
             *  - if configuration succeed => APP_STATE_SELECTE_SLAVE
             *  - if configuration failed => APP_STATE_ERROR
             */

            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined ( DISPLAY_CURRENT_STATE )
                printf("> APP_STATE_CONFIGURE_SYSTEM\n");
#endif
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_STATE_CONFIGURE_SYSTEM);
                }

                /* Set log file name => 20yymmdd.CSV (one log file per day). */
                if (false == setLogFileName()) {
#if defined(_DEBUG)
                    printf("false == setLogFileName( )\n"
                            "==> ERROR STATE\n");
#endif
                    appDataUsb.is_device_needed = false;
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                    break;
                }

                /* Power USB device */
                powerUsbGSMEnable();
                /* Ask for the device */
                appDataUsb.is_device_needed = true;

                /* Set timeout if USB device is not found */
                setDelayMs(MAX_DELAY_TO_DETECT_USB_DEVICE);

#if defined (USE_UART1_SERIAL_INTERFACE)
                printf("\tSearching USB device.\n\tThe system will restart if not found after %ldms.\r\n", (long) MAX_DELAY_TO_DETECT_USB_DEVICE);
#endif
            }

            /* If USB device is plugged and a valid address is set */
            if (appDataUsb.is_device_address_available) {
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf("\tUSB device found.\n\t=========================================\n");
#endif   
                /* Mount drive on USB device */
                if (USB_DRIVE_NOT_MOUNTED == usbMountDrive()) {
#if defined(_DEBUG)
                    printf("USB_DRIVE_NOT_MOUNTED == usbMountDrive( )\n"
                            "==> ERROR STATE\n");
#endif
                    /* If mount failed => error */
                    appDataUsb.is_device_needed = false;
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                    break;
                }
            }/* Otherwise wait for USB device connection or for timeout to reach */
            else {
                /* If timeout reached => error */
                if (true == isDelayMsEnding()) {
#if defined(UART_DEBUG)
                    printf("true == isDelayMsEnding( )\n"
                            "==> ERROR STATE\n");
#endif
                    appDataUsb.is_device_needed = false;
                    sprintf(appError.message, "USB device detection took more than %ldms", (long) MAX_DELAY_TO_DETECT_USB_DEVICE);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    appError.number = ERROR_USB_DEVICE_NOT_FOUND;
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                    break;
                }

                MASTER_StoreBehavior(MASTER_APP_STATE_CONFIGURE_SYSTEM, PRIO_HIGH);
                /* Blue and yellow status LEDs blink as long USB key is required. */
                LedsStatusBlink(LED_USB_ACCESS, LED_YELLOW, 500, 500);
                break;
            }

            /* Turn on LEDs status while USB device is accessed */
            setLedsStatusColor(LED_USB_ACCESS);

            /* Set log events file name if required */
            if (true == appDataLog.log_events) {
                /* If set file name failed => error */
                if (false == setEventFileName()) {
#if defined(_DEBUG)
                    printf("false == setEventFileName( )\n"
                            "==> ERROR STATE\n");
#endif
                    appDataUsb.is_device_needed = false;
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                    break;
                }
            }
            /* Configure the system with the CONFIG.INI file */
            appData.flags.bit_value.system_init = config_set();

            /* If system configuration succeed */
            if (true == appData.flags.bit_value.system_init) {

                srand(appData.current_time.tm_mon + appData.current_time.tm_mday + appData.current_time.tm_min + appData.current_time.tm_sec);

                /* Check all status LEDs */
                checkLedsStatus();

#if defined (USE_UART1_SERIAL_INTERFACE)
                printf("\t=========================================\n");
#endif  
                /* Check mandatory parameters */
                chk = checkImportantParameters();
                switch (chk) {
                    case APP_CHECK_OK:

                        appData.openfeeder_state = OPENFEEDER_IS_AWAKEN;
                        appData.ptr[PRIO_HIGH][PTR_READ] = appData.ptr[PRIO_HIGH][PTR_WRITE];
                        MASTER_StoreBehavior(MASTER_APP_STATE_GPRS_ATTACHED, PRIO_HIGH);
                        //                        MASTER_StoreBehavior(MASTER_APP_STATE_CONFIGURE_SYSTEM, PRIO_HIGH);
                        //                        appData.state = MASTER_APP_STATE_CONFIGURE_SYSTEM;
                        break;
                    case APP_CHECK_BATTERY_PB:
                    case APP_CHECK_VBAT_PB:
                        MASTER_StoreBehavior(MASTER_APP_STATE_FLUSH_DATA_BEFORE_ERROR, PRIO_HIGH);
                        //                        appData.state = MASTER_APP_STATE_FLUSH_DATA_BEFORE_ERROR;
                        break;
                }

                /* If one of the mandatory parameter failed => error */
                if (appData.state != MASTER_APP_STATE_CONFIGURE_SYSTEM) {
                    break;
                }

#if defined (USE_UART1_SERIAL_INTERFACE)
                printf("\t=========================================\n");
#endif  

                /* Flush data on USB device */
                if (FLUSH_DATA_ON_USB_DEVICE_SUCCESS != flushDataOnUsbDevice()) {
#if defined(_DEBUG)
                    printf("FLUSH_DATA_ON_USB_DEVICE_SUCCESS != flushDataOnUsbDevice( )\n"
                            "==> ERROR STATE\n");
#endif
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                    break;
                }

                /* Log current firmware version */
                logFirmware();

                /* Log the Unique Devide ID is required */
                if (true == appDataLog.log_udid) {
                    logUdid();
                }

#if defined (USE_UART1_SERIAL_INTERFACE)
                printf("\t=========================================\n");
#endif  
                /* Test servomotor by opening and closing the door */
                //testServomotor();

                /* Manage attractive LEDs if needed */
                if (true == appData.flags.bit_value.attractive_leds_status) {

                    //                    /* Reset the attractive LEDs driver */                    
                    //                    if ( I2C1_PCA9622_SoftwareReset( ) )
                    //                    {
                    //#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_PCA9622_STATUS)
                    //                        printf( "\tAttractive LEDs driver OK\n" );
                    //#endif
                    //                    }
                    //                    /* If LEDs driver reset failed => error*/
                    //                    else
                    //                    {
                    //                        sprintf( appError.message, "Unable to reset the attractive LEDS driver" );
                    //                        appError.current_line_number = __LINE__;
                    //                        sprintf( appError.current_file_name, "%s", __FILE__ );
                    //                        appError.number = ERROR_ATTRACTIVE_LED_DRIVER_RESET;
                    //                        appData.state = APP_STATE_ERROR;
                    //                        break;
                    //                    }

                    /* If LEDs initialization OK => test LEDs */
                    if (initAttractiveLeds()) {
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined ( DISPLAY_LEDS_STATUS )
                        printf("\tAttractive LEDs: ok\n");
#endif
                        /* Turn on attractive LEDs one by one in red */
                        testAttractiveLeds();

                    }/* If LEDs initialization failed => error */
                    else {
                        sprintf(appError.message, "Unable to initialize attractive LEDS via I2C");
                        appError.current_line_number = __LINE__;
                        sprintf(appError.current_file_name, "%s", __FILE__);
                        appError.number = ERROR_ATTRACTIVE_LED_INIT;
                        MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                        //                        appData.state = MASTER_APP_STATE_ERROR;
                        break;
                    }

                    /* Set attractive LEDs order according to file UDIDLEDS.TXT */
                    if (true == setAttractiveLedsIndex()) {
                        /* Turn on attractive LEDs one by one in green */
                        testAttractiveLedsOrder();
                    }

                    /* Turn on attractive LEDs if required */
                    getDateTime();

                    if ((appData.current_time.tm_hour * 60 + appData.current_time.tm_min) >= (appDataAttractiveLeds.wake_up_time.tm_hour * 60 + appDataAttractiveLeds.wake_up_time.tm_min) &&
                            (appData.current_time.tm_hour * 60 + appData.current_time.tm_min)< (appDataAttractiveLeds.sleep_time.tm_hour * 60 + appDataAttractiveLeds.sleep_time.tm_min)) {
                        setAttractiveLedsOn();
                    }
                }

                /* Set RTC alarm to raise every second */
                rtcc_set_alarm(appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_SECOND);

                /* Go to state idle */
                //                appData.state = MASTER_APP_STATE_IDLE;
            }/* If system configuration failed => error */
            else {
#if defined  (USE_UART1_SERIAL_INTERFACE )
                printf("false == appData.flags.bit_value.system_init\n"
                        "==> ERROR STATE\n");
#endif
                appDataUsb.is_device_needed = false;
                MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                //                appData.state = MASTER_APP_STATE_ERROR;
                break;
            }

            /* Unmount drive on USB device */
            if (USB_DRIVE_MOUNTED == usbUnmountDrive()) {
#if defined(_DEBUG)
                printf("fUSB_DRIVE_MOUNTED == usbUnmountDrive( )\n"
                        "==> ERROR STATE\n");
#endif
                /* If unmount failed => error */
                MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                //                appData.state = MASTER_APP_STATE_ERROR;
            }

            appDataUsb.is_device_needed = false;
            break;
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_IDLE:

            /*
             * Idle state.
             * 
             * Process:
             *  - Wait for bird detection by PIR sensor  
             *  - Serial communication for debugging
             *  - Manage RTC action
             * 
             * Next state: 
             *  - if bird is detected: APP_STATE_RFID_READING_PIT_TAG
             */
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined ( DISPLAY_CURRENT_STATE )
                printf("> MASTER_APP_STATE_IDLE\n");
#endif              
                //#if defined ( USE_UART1_SERIAL_INTERFACE )
                //                printf("\tUser button pressed briefly => flush data and eject USB device\n");
                //                printf("\tUser button pressed longer => activate serial communication mode\n");
                //#endif   
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_STATE_IDLE);
                }
            }

#if defined ( PRINT_DATE )
            if (getDateTime()) {
                if (appData.current_time.tm_sec % 30 == 0 && (print == true)) {
                    print = false;
                    printDateTime(appData.current_time);
                } else if (appData.current_time.tm_sec % 30 != 0 && (print == false)) {
                    print = true;
                }
            }
            APP_SerialDebugTasks();
#endif

            /* If the user ask for reconfiguration via serial communication */
            if (appData.need_to_reconfigure) {
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_RECONFIGURE_SYSTEM);
                }
                appData.need_to_reconfigure = false;
                MASTER_StoreBehavior(MASTER_APP_STATE_CONFIGURE_SYSTEM, PRIO_HIGH);
                //                appData.state = MASTER_APP_STATE_CONFIGURE_SYSTEM;
                break;
            }

            /* If event buffer is full => flush events to USB device */
            if (true == appDataLog.log_events && appDataEvent.num_events_stored >= MAX_NUM_EVENT_BEFORE_SAVE) {
                if (FILEIO_RESULT_FAILURE == logEvents()) {
                    appDataUsb.is_device_needed = false;
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                    break;
                }
            }

            /* Check USER BUTTON detected. */
            button_user_state = USER_BUTTON_GetValue();

            if (button_user_state != previous_button_user_state) {
                previous_button_user_state = button_user_state;
                if (BUTTON_PRESSED == button_user_state) {
                    setLedsStatusColor(LEDS_ON);
                    setDelayMs(2000);
                    while (0 == isDelayMsEnding());

                    button_user_state = USER_BUTTON_GetValue();

                    setLedsStatusColor(LEDS_OFF);

                    if (BUTTON_PRESSED == button_user_state) {
                        MASTER_StoreBehavior(MASTER_APP_STATE_SERIAL_COMMUNICATION, PRIO_LOW);
                        break;
                    } else {
                        MASTER_StoreBehavior(MASTER_APP_STATE_FLUSH_DATA_TO_USB, PRIO_MEDIUM);
                        break;
                    }
                }
            }

            /* Green status LED blinks in idle mode. */
            LedsStatusBlink(LED_GREEN, LEDS_OFF, 25, 4975);
            break;
            /* -------------------------------------------------------------- */
        case MASTER_APP_STATE_SERIAL_COMMUNICATION:

            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf("> APP_STATE_SERIAL_COMMUNICATION\n");
#endif

                /* Disable PIR interruption */
                EX_INT0_InterruptDisable();

                /* Disable RTC alarm */
                rtcc_stop_alarm();

                if (true == appData.flags.bit_value.attractive_leds_status) {
                    setAttractiveLedsNoColor();
                }

                displayKeyMapping();

#if defined (USE_UART1_SERIAL_INTERFACE)
                printf("\t=========================================\n");
#endif  
                getDateTime();
                printf("\t");
                printDateTime(appData.current_time);
                printf(" (PIC)\n\t");
                if (getExtDateTime()) {
                    printExtDateTime();
                    printf(" (EXT)\n");
                } else {
                    printf("XX/XX/XXXX XX:XX:XX (EXT)\n");
                }
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf("\t=========================================\n");
#endif     
                getBatteryLevel();
                printBatteryLevel();
                getVBatLevel();
                printVBatLevel();
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf("\t=========================================\n");
#endif    

#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf("\n\t/!\\ The system will reset after 60s of inactivity.\n");
#endif  
#if defined (USE_UART1_SERIAL_INTERFACE) && defined( DISPLAY_CURRENT_STATE )
                printf("\t=========================================\n");
#endif  
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_STATE_SERIAL_COMMUNICATION);
                }

                /* Empty UART RX buffer to avoid garbage value */
                while (UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet()) {
                    UART1_Read();
                }

                setDelayMsStandBy(60000);

            }

            /* Green status LED blinks in idle mode. */
            LedsStatusBlink(LED_SERIAL_COMMUNICATION, LEDS_OFF, 100, 4900);

            /* Get interaction with the serial terminal. */
            APP_SerialDebugTasks();

            /* If more than 60s of inactivity => reset the system */
            if (true == isDelayMsEndingStandBy()) {
#if defined (USE_UART1_SERIAL_INTERFACE) 
                printf("\t=========================================\n");
#endif 
#if defined ( USE_UART1_SERIAL_INTERFACE ) 
                printf("\t/!\\ More than 60s of inactivity.\n\tThe system will reset in 3s.\n");
#endif
#if defined (USE_UART1_SERIAL_INTERFACE)
                printf("\t=========================================\n");
#endif 
                for (i = 0; i < 3; i++) {
                    setLedsStatusColor(LEDS_ON);
                    setDelayMs(500);
                    while (0 == isDelayMsEnding());
                    setLedsStatusColor(LEDS_OFF);
                    setDelayMs(500);
                    while (0 == isDelayMsEnding());
                }

                /* Unmount drive on USB device before power it off. */
                if (USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status) {
                    usbUnmountDrive();
                }
                USBHostShutdown();

                __asm__ volatile ( "reset");

            }

            /* If the user press "q" to quit serial communication mode */
            if (MASTER_APP_STATE_SERIAL_COMMUNICATION != appData.state) {

            }
            MASTER_StoreBehavior(MASTER_APP_STATE_SERIAL_COMMUNICATION, PRIO_LOW);
            break;
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_REMOVE_USB_DEVICE:

            /*
             * Remove USB device.
             * 
             * Process:
             * 
             * Next state: 
             */

            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;

#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf("> APP_STATE_REMOVE_USB_DEVICE\n");
#endif
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf("\t/!\\ Don't remove the USB device yet.\n");
#endif
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_STATE_REMOVE_USB_DEVICE);
                }

                /* Unmount drive on USB device before power it off. */
                if (USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status) {
                    usbUnmountDrive();
                }

                USBHostShutdown();

                setDelayMs(2000);
                while (0 == isDelayMsEnding());

                powerUsbGSMDisable();

                setLedsStatusColor(LED_GREEN);
#if defined ( USE_UART1_SERIAL_INTERFACE )
                printf("\tUSB device can be safely removed\r\n");
                printf("\tThe system will automatically restart in 60s\r\n");
                printf("\tThe serial communication is off until the system reset\r\n");
#endif
                setDelayMsStandBy(60000);

            }

            if (true == isDelayMsEndingStandBy()) {
                appData.dsgpr0.bit_value.num_software_reset = 0;
                DSGPR0 = appData.dsgpr0.reg;
                DSGPR0 = appData.dsgpr0.reg;

                __asm__ volatile ( "reset");
            }

            break;
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_FLUSH_DATA_TO_USB:

            /*
             * Flush data to USB device.
             * 
             * Process:
             * 
             * Next state: 
             */

            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf("> APP_STATE_FLUSH_DATA_TO_USB\n");
#endif
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_STATE_FLUSH_DATA_TO_USB);
                }

                /* Disable PIR interruption */
                EX_INT0_InterruptDisable();
                /* Disable RTC alarm */
                rtcc_stop_alarm();

                setLedsStatusColor(LED_USB_ACCESS);

                if (appDataLog.num_data_stored > 0 ||
                        appDataLog.num_battery_level_stored > 0 ||
                        appDataEvent.num_events_stored > 0 ||
                        appDataLog.num_ds3231_temp_stored > 0) {
                    /* Log data on USB device */
                    appDataUsb.is_device_needed = true;
                    powerUsbGSMEnable();
                } else {
#if defined ( USE_UART1_SERIAL_INTERFACE )  
                    printf("\tNo data to flush.\n");
                    MASTER_StoreBehavior(MASTER_APP_STATE_REMOVE_USB_DEVICE, PRIO_HIGH);
                    break;
#endif
                }

                setDelayMs(MAX_DELAY_TO_DETECT_USB_DEVICE);

            }

            if (appDataUsb.is_device_address_available) {
                if (FLUSH_DATA_ON_USB_DEVICE_SUCCESS != flushDataOnUsbDevice()) {
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                    break;
                }
            } else {
                if (true == isDelayMsEnding()) {
                    appDataUsb.is_device_needed = false;
                    sprintf(appError.message, "USB device detection took more than %ld ms", (long) MAX_DELAY_TO_DETECT_USB_DEVICE);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    appError.number = ERROR_USB_DEVICE_NOT_FOUND;
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                    break;
                }
                /* Blue and yellow status LEDs blink as long USB key is required. */
                LedsStatusBlink(LED_USB_ACCESS, LED_YELLOW, 500, 500);
                break;
            }

            appDataUsb.is_device_needed = false;
            MASTER_StoreBehavior(MASTER_APP_STATE_REMOVE_USB_DEVICE, PRIO_HIGH);
            //            appData.state = MASTER_APP_STATE_REMOVE_USB_DEVICE;
            break;
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_SLEEP:
            /**
             * Application sleep state.
             *  - mise hors service de l'ensemble des fonctions hormis le capteur PIR
             *  - mise en sommeil simple du syst?me
             *    OPERATION DURING SLEEP MODES - PIR sensor and alarm deep sleep
             *  - lors de la sortie du mode SLEEP, remettre en service les fonctions de l'OpenFeeder
             *  - passer ? l'?tat APP_STATE_IDLE
             */
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined ( DISPLAY_CURRENT_STATE )
                printf("> APP_STATE_SLEEP\n");
#endif
                appData.dayTime = SEE_YOU_TOMORROW;
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_STATE_SLEEP);
                }

                /* Stop RTCC interuption during shutdown process */
                rtcc_stop_alarm();

                /* Log data on USB device */
                appDataUsb.is_device_needed = true;

            }

            //of state 
            appData.openfeeder_state = OPENFEEDER_IS_SLEEPING;

            if (true == appDataUsb.is_device_needed) {
                if (appDataUsb.is_device_address_available) {
                    if (FLUSH_DATA_ON_USB_DEVICE_SUCCESS != flushDataOnUsbDevice()) {
                        /* Unmount drive on USB device before power it off. */
                        if (USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status) {
                            usbUnmountDrive();
                        }
                    }

                    USBHostShutdown();
                    //                    powerUsbGSMDisable();
                } else {
                    break;
                }
            }

            /* stop gsm module */
            if (appData.gsmInit) {
#if defined( USE_UART1_SERIAL_INTERFACE )
                printf("power off GSM\n");
#endif
                appData.gsmInit = false;
                if (app_TCPconnected())
                    app_TCPclose();
                app_PowerDown(true);
                powerUsbGSMDisable();
            }

            /* power off radio module */
            if (appData.RfModuleInit) {
#if defined( USE_UART1_SERIAL_INTERFACE )
                printf("RF Power OFF\n");
#endif
                appData.RfModuleInit = false;
                powerRFDisable();
            }

            /* Turn status LED off */
            setLedsStatusColor(LEDS_OFF);

            //#if defined ( TEST_RTCC_SLEEP_WAKEUP )
            /* Next line for debugging sleep/wakeup only */
            /* Should be commented in normal mode */
            /* Modify time value according to wake up values in the CONFIG.INI file */
            //setDateTime(19, 8, 12, 4, 59, 40);
            //#endif
            /* Set alarm for wake up time */
            rtcc_set_alarm(appDataAlarmDefaultOnOF.time.tm_hour, appDataAlarmDefaultOnOF.time.tm_min, appDataAlarmDefaultOnOF.time.tm_sec, EVERY_DAY);

            rtcc_start_alarm();

            setDelayMs(500);
            while (false == isDelayMsEnding()) {
                Nop();
            }

            appData.dsgpr0.bit_value.num_software_reset += 1;

            /* The repeat sequence (repeating
            the instruction twice) is required to write to
            any of the Deep Sleep registers (DSCON,
            DSWAKE, DSGPR0, DSGPR1).
            PIC24FJ256GB406 datasheet page 200 */
            DSGPR0 = appData.dsgpr0.reg;
            DSGPR0 = appData.dsgpr0.reg;
            DSGPR1 = appData.dsgpr1.reg;
            DSGPR1 = appData.dsgpr1.reg;

#if defined ( ENABLE_DEEP_SLEEP )
            DSCONbits.DSEN = 1;
            DSCONbits.DSEN = 1;
#endif
            Sleep();
            MASTER_StoreBehavior(MASTER_APP_STATE_WAKE_UP, PRIO_EXEPTIONNEL);
            //            appData.state = MASTER_APP_STATE_WAKE_UP;
            break;
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_WAKE_UP:
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined ( DISPLAY_CURRENT_STATE )
                printf("> APP_STATE_WAKE_UP_FROM_SLEEP\n");
#endif
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_STATE_WAKE_UP);
                }
            }

            //#if defined ( TEST_RTCC_SLEEP_WAKEUP )
            /* Next line for debugging sleep/wakeup only */
            /* Should be commented in normal mode */
            /* Modify time value according to sleep values in the CONFIG.INI file */
            //            setDateTime(17, 9, 21, 22, 59, 50);
            //#endif
            appData.dayTime = GOOD_MORNING;
            //penser a bouger cet apel
            rtcc_set_alarm(appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_SECOND);

            MASTER_AppInit();
            break;
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_FLUSH_DATA_BEFORE_ERROR:
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf("> APP_STATE_FLUSH_DATA_BEFORE_ERROR\n");
#endif
                appError.is_data_flush_before_error = true;

                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_STATE_FLUSH_DATA_BEFORE_ERROR);
                }

                appDataUsb.is_device_needed = true;
                /* Log data on USB device */
                powerUsbGSMEnable();

                setDelayMs(MAX_DELAY_TO_DETECT_USB_DEVICE);
            }

            if (appDataUsb.is_device_address_available) {
                if (USB_DRIVE_NOT_MOUNTED == usbMountDrive()) {
                    appDataUsb.is_device_needed = false;
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                    break;
                }

                /* Battery level */
                if (appDataLog.num_battery_level_stored > 0) {
                    setLedsStatusColor(LED_USB_ACCESS);
                    if (FILEIO_RESULT_FAILURE == logBatteryLevel()) {
                        appDataUsb.is_device_needed = false;
                        MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                        break;
                    }
                }

                /* Error */
                if (ERROR_NONE < appError.number) {
                    setLedsStatusColor(LED_USB_ACCESS);
                    if (FILEIO_RESULT_FAILURE == logError()) {
                        appDataUsb.is_device_needed = false;
                        MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                        break;
                    }
                }

                /* Events */
                if (appDataEvent.num_events_stored > 0) {
                    if (true == appDataEvent.is_bin_file_name_set || true == appDataEvent.is_txt_file_name_set) {
                        setLedsStatusColor(LED_USB_ACCESS);
                        if (FILEIO_RESULT_FAILURE == logEvents()) {
                            appDataUsb.is_device_needed = false;
                            MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                            //                            appData.state = MASTER_APP_STATE_ERROR;
                            break;
                        }
                    }
                }

                /* Date and time calibration */
                if (appDataLog.num_time_calib_stored > 0) {
                    if (FILEIO_RESULT_FAILURE == logCalibration()) {
                        appDataUsb.is_device_needed = false;
                        MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                        //                        appData.state = MASTER_APP_STATE_ERROR;
                        break;
                    }
                }

                /* DS3231 temperature */
                if (appDataLog.num_ds3231_temp_stored > 0) {
                    if (FILEIO_RESULT_FAILURE == logDs3231Temp()) {
                        appDataUsb.is_device_needed = false;
                        MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                        //                        appData.state = MASTER_APP_STATE_ERROR;
                        break;
                    }
                }

                if (USB_DRIVE_MOUNTED == usbUnmountDrive()) {
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                }

            } else {
                if (true == isDelayMsEnding()) {
                    appDataUsb.is_device_needed = false;
                    sprintf(appError.message, "USB device detection took more than %ldms", (long) MAX_DELAY_TO_DETECT_USB_DEVICE);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    appError.number = ERROR_USB_DEVICE_NOT_FOUND;
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
                    break;
                }
                /* Blue and yellow status LEDs blink as long USB key is required. */
                LedsStatusBlink(LED_USB_ACCESS, LED_YELLOW, 500, 500);
                break;
            }

            setLedsStatusColor(LEDS_OFF);
            MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
            break;
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_ERROR:
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf("> APP_STATE_ERROR\n");
#endif
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_STATE_ERROR);
                }

                /* If the current error si not related to the USB device, try to log the error */
                if (appError.number < ERROR_USB || appError.number > ERROR_USB_SUSPEND_DEVICE) {
                    /* Flush data on USB device if necessary */
                    if (false == appError.is_data_flush_before_error) {
                        MASTER_StoreBehavior(MASTER_APP_STATE_FLUSH_DATA_BEFORE_ERROR, PRIO_HIGH);
                        //                        appData.state = MASTER_APP_STATE_FLUSH_DATA_BEFORE_ERROR;
                        //generation d'une erreure 
                        break;
                    }
                }

                //#if defined( USE_UART1_SERIAL_INTERFACE )
                //                printf("error number %d, errorSend %d\n", appError.number, appError.errorSend);
                //#endif
                if ((appError.number < ERROR_CRITICAL || appError.number > ERROR_TOO_MANY_SOFTWARE_RESET)
                        && !appError.errorSend && appError.number != ERROR_GSM_NO_POWER_ON) {
                    MASTER_StoreBehavior(MASTER_APP_STATE_SEND_ERROR_TO_SERVER, PRIO_EXEPTIONNEL);
                    break;
                }

                while (!RTCC_TimeGet(&appError.time)) {
                    Nop();
                }

                switch (appError.number) {
                    case ERROR_LOW_BATTERY:
                        appError.led_color_2 = LEDS_ERROR_CRITICAL_BATTERY;
                        break;
                    case ERROR_LOW_VBAT:
                        appError.led_color_2 = LEDS_ERROR_CRITICAL_VBAT;
                        break;
                    default:
                        appError.led_color_2 = LEDS_OFF;
                        break;
                }

                if (appError.number > ERROR_TOO_MANY_SOFTWARE_RESET) {
                    appError.led_color_2 = LEDS_TOO_MANY_SOFTWARE_RESET;
                }

#if defined ( USE_UART1_SERIAL_INTERFACE )
                printError();
#endif
                rtcc_stop_alarm();


                /* Unmount drive on USB device before power it off. */
                if (USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status) {
                    usbUnmountDrive();
                }

                if (appError.number != ERROR_GSM_NO_POWER_ON &&
                        !appError.OfInCriticalError) { // indique que c'est la premiere fois qu'on entre dedans 
                    TMR_Delay(30000); //permet d'attendre la fin des tache du gsm avant de couper l'alime  
                    if (app_TCPconnected())
                        app_TCPclose();
                    app_PowerDown(true);
                }
                USBHostShutdown();
                powerUsbGSMDisable();

                /* Reset the system after DELAY_BEFORE_RESET milli-seconds if non critical error occurred */
                if (appError.number > ERROR_CRITICAL && appError.number < ERROR_TOO_MANY_SOFTWARE_RESET) {
                    if (MAX_NUM_RESET <= appData.dsgpr0.bit_value.num_software_reset) {
#if defined ( USE_UART1_SERIAL_INTERFACE ) 
                        printf("\t/!\\ The system reset %u times => Critical error\n", MAX_NUM_RESET);
#endif                       
                        appError.number = appError.number + ERROR_TOO_MANY_SOFTWARE_RESET;
                        appData.previous_state = MASTER_APP_STATE_IDLE;
                        break;
                    }

                    TMR3_Start();
                    setDelayMs(DELAY_BEFORE_RESET);
#if defined ( USE_UART1_SERIAL_INTERFACE ) 
                    printf("\t/!\\ The system will reset in %ums\n", DELAY_BEFORE_RESET);
#endif
                } else {
                    rtcc_set_alarm(0, 0, 0, EVERY_10_SECONDS);
                    rtcc_start_alarm();
                }
            }

            if (appError.number > ERROR_CRITICAL && appError.number < ERROR_TOO_MANY_SOFTWARE_RESET) {
                /* A non critical error occurred                      */
                /* Wait DELAY_BEFORE_RESET milli-seconds and reset    */
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined (ENABLE_ERROR_LEDS)
                LedsStatusBlink(appError.led_color_1, appError.led_color_2, 150, 150);
#endif
                if (isDelayMsEnding()) {

                    appData.dsgpr0.bit_value.num_software_reset += 1;
                    DSGPR0 = appData.dsgpr0.reg;
                    DSGPR0 = appData.dsgpr0.reg;

                    TMR3_Stop();

                    __asm__ volatile ( "reset");

                }
            } else {
                /* A critical error occured */
                /* Stop the openfeeder to save battery */
                /* Make status LEDs blink at long interval */

                if (!appError.OfInCriticalError) {
#if defined( USE_UART1_SERIAL_INTERFACE )
                    printf("RF MODULE POWER OFF\n");
#endif 
                    // on a un bug ici, que je ne parviens pas a expliquer encore 
                    // donc pour palier a cela on eteint pas le module radio
                    // on l'empeche juste de ne plus recevoir ni d'emettre 
                    //                    powerRFDisable();
                    RF_PowerManagement.Val = PWR_MGMT_CMD_POR;
                    radioAlphaTRX_Command(RF_PowerManagement.Val);
                    appError.OfInCriticalError = true;
                }
                Sleep();

#if defined (ENABLE_ERROR_LEDS)
                /* Status LED blinks */
                TMR3_Start();
                LED_STATUS_R_Toggle();
                setDelayMs(150);
                while (0 == isDelayMsEnding());
                LED_STATUS_R_Toggle();
                setDelayMs(150);
                while (0 == isDelayMsEnding());
                setLedsStatusColor(LEDS_OFF);
                TMR3_Stop();
#endif
            }
            break;
            /* -------------------------------------------------------------- */
        case MASTER_APP_STATE_BATTERY_LEVEL_CHECK:
        {
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf("> MASTER_APP_STATE_BATTERY_LEVEL_CHECK\n");
#endif
                bool flag = isPowerBatteryGood();
                if (appDataLog.num_battery_level_stored < NUM_BATTERY_LEVEL_TO_LOG) {
                    appDataLog.battery_level[appDataLog.num_battery_level_stored][0] = appData.current_time.tm_hour;
                    appDataLog.battery_level[appDataLog.num_battery_level_stored][1] = appData.battery_level;
                    ++appDataLog.num_battery_level_stored;
                } else {
                    /* Log event if required */
                    if (true == appDataLog.log_events) {
                        store_event(OF_BATTERY_LEVEL_OVERFLOW);
                    }
                }

                if (false == flag) {
                    MASTER_StoreBehavior(MASTER_APP_STATE_FLUSH_DATA_BEFORE_ERROR, PRIO_HIGH);
                    //                appData.state = MASTER_APP_STATE_FLUSH_DATA_BEFORE_ERROR;
                    return;
                }
            }
        }
            break;
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_GET_EMPERATURE:
        {
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf("> MASTER_APP_STATE_GET_EMPERATURE\n");
#endif
                if (0 < APP_I2CMasterSeeksSlaveDevice(DS3231_I2C_ADDR, DS3231_I2C_ADDR)) {
                    /* Log event if required */
                    if (true == appDataLog.log_events) {
                        store_event(OF_DS3231_GET_TEMPERATURE);
                    }

                    getDateTime();
                    getDS3231Temperature();

                    if (appDataLog.num_ds3231_temp_stored < NUM_DS3231_TEMP_TO_LOG) {
                        appDataLog.ds3231_temp[appDataLog.num_ds3231_temp_stored][0] = (float) appData.current_time.tm_hour;
                        appDataLog.ds3231_temp[appDataLog.num_ds3231_temp_stored][1] = (float) appData.current_time.tm_min;
                        appDataLog.ds3231_temp[appDataLog.num_ds3231_temp_stored][2] = appData.ext_temperature;
                        ++appDataLog.num_ds3231_temp_stored;
                    } else {
                        /* Log event if required */
                        if (true == appDataLog.log_events) {
                            store_event(OF_DS3231_TEMP_OVERFLOW);
                        }
                    }
                }
            }
        }
            break;
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_RTC_CALIBRATION:
        {
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf("> MASTER_APP_STATE_RTC_CALIBRATION\n");
#endif
                calibrateDateTime();
            }
        }
            break;
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_SEND_DATE:
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf("> MASTER_APP_STATE_SEND_DATE\n");
#endif
                MASTER_SendDateRF();
            }
            break;
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_SEND_REQUEST_INFOS:
            if (appData.state != appData.previous_state) {
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf(">MASTER STATE SEND REQUEST INFOS\n");
#endif
            }
            //pour tout transmission RF hors la date, on passe par cet etat: niveau medium
            MASTER_SendMsgRF(appData.ensSlave[appData.slaveSelected].idSlave,
                    INFOS,
                    1, 1, (uint8_t *) ("INFOS"), 5); // demande du paquet attendu 
            TMR_SetWaitRqstTimeout(TIME_OUT_WAIT_RQST); // on demare le timer 
            appData.typeTimeout = RF_TIMEOUT;
            break;
            /* -------------------------------------------------------------- */
        case MASTER_APP_STATE_SELECTE_SLAVE:
        {
            if (appData.state != appData.previous_state) {
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf(">MASTER STATE SELECT SLAVE\n");
                printf("day time : %d\n", appData.dayTime);
#endif
                appData.previous_state = appData.state;
            }

            //test si tous les slaves sont sans reponse 
            uint8_t i = 0;
            bool b = false;
            for (i = 0; i < appData.nbSlaveOnSite; i++) {
                if (appData.ensSlave[i].state != SLAVE_NO_REQUEST &&
                        appData.ensSlave[i].state != SLAVE_ERROR) {
                    b = true;
                    break;
                }
            }

            if (b) {
                i = (appData.slaveSelected + 1) % appData.nbSlaveOnSite;
                b = false;
                bool stop = false;
                do {
                    if (appData.ensSlave[i].state == SLAVE_ERROR ||
                            appData.ensSlave[i].state == SLAVE_COLLECT_END) {
                        i = (i + 1) % appData.nbSlaveOnSite;
                        if (i == appData.slaveSelected)
                            if (appData.ensSlave[i].state == SLAVE_COLLECT_END ||
                                    appData.ensSlave[i].state == SLAVE_ERROR) {
                                stop = true; // pas de slave operationel  
                            }
                    } else {
                        stop = true; // on a trouve un slave
                        appData.slaveSelected = i;
                        b = true;
                    }
                } while (!stop);

                if (b) {
                    //en foction de l'heur de la journee on change d'etat 
                    switch (appData.dayTime) {
                        case GOOD_MORNING:
                        {
                            appData.ensSlave[appData.slaveSelected].state = SLAVE_CONFIG;
                            TMR_SetWaitRqstTimeout(-1);
                            appData.typeTimeout = NONE_TIMEOUT;
                        }
                            break;
                        case GOOD_DAY:
                        {
                            if (appData.ensSlave[appData.slaveSelected].state != SLAVE_NO_REQUEST)
                                appData.ensSlave[appData.slaveSelected].state = SLAVE_DAYTIME;
                            MASTER_StoreBehavior(MASTER_APP_STATE_SEND_REQUEST_INFOS, PRIO_MEDIUM);
                        }
                            break;
                        case GOOD_NIGHT:
                        {
                            appData.ensSlave[appData.slaveSelected].tryToConnect = MAX_TRY_TO_SYNC;
                            appData.ensSlave[appData.slaveSelected].state = SLAVE_SYNC;
                            TMR_SetWaitRqstTimeout(0); // declenche une interuption logiciel 
                            appData.typeTimeout = RF_TIMEOUT;
                        }
                            break;
                        default:
                            break;
                    }
#if defined (USE_UART1_SERIAL_INTERFACE) 
                    printf("slave Selected: local id %d, global id  %d  num in tab %d state %d\n",
                            appData.ensSlave[appData.slaveSelected].idSlave,
                            appData.ensSlave[appData.slaveSelected].uidSlave,
                            appData.slaveSelected,
                            appData.ensSlave[appData.slaveSelected].state);
#endif
                    break; // fin de la selection du slave 
                }
            }
#if defined(UART_DEBUG)
            printf("Aucun slave en selection\n");
#endif
            MASTER_StoreBehavior(MASTER_APP_STATE_END, PRIO_LOW);

        }
            break;
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_MSG_RF_RECEIVE:
        {
            appData.receive = radioAlphaTRX_GetFrame();
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf(">MASTER_STATE_MSG_RECEIVE\n");
#endif
            }
            appData.ensSlave[appData.slaveSelected].nbError = MAX_ERROR;
            appData.ensSlave[appData.slaveSelected].nbTimeout = MAX_TIMEOUT;

            switch (appData.receive.Champ.typeMsg) {
                case DATA:
                    if (appData.dayTime == GOOD_NIGHT) { // si on est le soir ==> phase de collecte 
                        if (appData.ensSlave[appData.slaveSelected].state == SLAVE_SYNC) {
#if defined(UART_DEBUG)
                            printf("Slave %d connect\n", appData.ensSlave[appData.slaveSelected].idSlave);
#endif
                            appData.ensSlave[appData.slaveSelected].tryToConnect = MAX_TRY_TO_SYNC; // on reset le nombre de demande de connxion 
                            appData.ensSlave[appData.slaveSelected].state = SLAVE_COLLECT; //changement d'etat
                        }

                        if (appData.receive.Champ.idMsg == appData.ensSlave[appData.slaveSelected].index) {
                            appData.ensSlave[appData.slaveSelected].state = SLAVE_COLLECT;
                            strncpy(appData.BUFF_COLLECT +
                                    (SIZE_DATA - 1) * (appData.ensSlave[appData.slaveSelected].index - 1),
                                    appData.receive.Champ.data, SIZE_DATA); // save data
                            if (appData.receive.Champ.nbR == MAX_W) { // fin de block
                                TMR_SetWaitRqstTimeout(-1); // desactive le timer
                                appData.typeTimeout = NONE_TIMEOUT;
                                MASTER_StoreBehavior(MASTER_APP_STATE_SEND_FROM_GSM, PRIO_HIGH);
                                appData.ensSlave[appData.slaveSelected].state = SLAVE_COLLECT_END_BLOCK;
#if defined(UART_DEBUG)
                                printf("END BLOC\n");
#endif  
                            } else if (appData.receive.Champ.nbR == MAX_W + 1) { // fin de trans
                                TMR_SetWaitRqstTimeout(-1);
                                appData.typeTimeout = NONE_TIMEOUT;
                                MASTER_StoreBehavior(MASTER_APP_STATE_SEND_FROM_GSM, PRIO_HIGH);
                                appData.ensSlave[appData.slaveSelected].state = SLAVE_COLLECT_END;
#if defined(UART_DEBUG)
                                printf("END BLOC Collect End\n");
#endif
                            } else {
                                appData.ensSlave[appData.slaveSelected].index += 1;
                                // on demarre le timeout
                                if (appData.receive.Champ.nbR > 1) { // je suis plus en attente d'un paquet
                                    TMR_SetWaitRqstTimeout(TIME_OUT_COLLECT_LOG);
                                } else {
                                    TMR_SetWaitRqstTimeout(0); // deactive timer up to (until) request 
                                }
                                appData.typeTimeout = RF_TIMEOUT;
                            }
                        } else {
#if defined(UART_DEBUG)
                            printf("numSeq attendu %d vs %d recu \n",
                                    appData.ensSlave[appData.slaveSelected].index, appData.receive.Champ.idMsg);
#endif
                            TMR_SetWaitRqstTimeout(0);
                            appData.typeTimeout = RF_TIMEOUT;
                        }

                        break;
                        /* -------------------------------------------------------*/
                        case ERROR:
                        {
#if defined( USE_UART1_SERIAL_INTERFACE )
                            printf("ERROR RECEVE OF %d num Error %d\n", appData.ensSlave[appData.slaveSelected].uidSlave,
                                    appData.receive.Champ.idMsg);
#endif
                            //                        appError.errorSend = false;
                            appData.ensSlave[appData.slaveSelected].state = SLAVE_ERROR;
                            sprintf(appError.message, "Error Receive from Alpha TRX module");
                            appError.current_line_number = __LINE__;
                            sprintf(appError.current_file_name, "%s", __FILE__);
                            appError.number = appData.receive.Champ.idMsg;
                            MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_EXEPTIONNEL);
                            TMR_SetWaitRqstTimeout(-1); // desactive le timer 
                            appData.typeTimeout = NONE_TIMEOUT;
                        }
                        break;
                        /* -------------------------------------------------------*/
                        case NOTHING:
                        if (appData.ensSlave[appData.slaveSelected].state == SLAVE_SYNC) {
                            appData.ensSlave[appData.slaveSelected].state = SLAVE_COLLECT_END;
                            MASTER_StoreBehavior(MASTER_APP_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
                            // on n'a pas pu recuperer un block car c'est fini 
                        } else {
#if defined( USE_UART1_SERIAL_INTERFACE )
                            printf("Slave %d Nothing to send\n", appData.ensSlave[appData.slaveSelected].idSlave);
#endif
                        }
                        break;
                        /* ----------------------------------------------------*/
                        /* -------------------------------------------------------*/
                    }
                    break;
            }
        }
            break;
            /*-----------------------------------------------------------------*/
        case MASTER_APP_STATE_TIMEOUT: //if an event has occurred with the timer 
        {
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf(">MASTER STATE TIMEOUT\n");
#endif
            }
            // verifier le type de time out 
            if (GSM_TIMEOUT == appData.typeTimeout) {
                // pour l'instant je fais ça 
                MSTER_RetransGsmData();
            } else if (RF_TIMEOUT == appData.typeTimeout) {
                switch (appData.ensSlave[appData.slaveSelected].state) {
                    case SLAVE_CONFIG:
#if defined( USE_UART1_SERIAL_INTERFACE )
                        printf("Config slave with rf and gsm communication\n");
#endif
                        break;
                    case SLAVE_SYNC:
#if defined(UART_DEBUG)
                        printf("synchro try to connect %d \n",
                                appData.ensSlave[appData.slaveSelected].tryToConnect);
#endif              
                        appData.ensSlave[appData.slaveSelected].tryToConnect--;
                        if (appData.ensSlave[appData.slaveSelected].tryToConnect > 0) {
#if defined( USE_UART1_SERIAL_INTERFACE )
                            printf("[demande bloc %d]\n", appData.ensSlave[appData.slaveSelected].nbBloc - 1);
#endif  
                            memset(appData.BUFF_COLLECT, '\0', NB_BLOCK * SIZE_DATA);
                            MASTER_SendMsgRF(appData.ensSlave[appData.slaveSelected].idSlave,
                                    DATA,
                                    appData.ensSlave[appData.slaveSelected].nbBloc, 1,
                                    (uint8_t *) "",
                                    0); // attention a changer
                            TMR_SetWaitRqstTimeout(TIME_OUT_COLLECT_LOG); // active timer 
                            appData.typeTimeout = RF_TIMEOUT;
                        } else { // on ne peut pas se connecter ? ce slave 
                            appData.ensSlave[appData.slaveSelected].tryToConnect = MAX_TRY_TO_SYNC;
                            if (appData.ensSlave[appData.slaveSelected].nbError == 0) {
#if defined(UART_DEBUG)
                                printf("hundler slave %d communication corompu\n ERROR ==> SELECT SLAVE\n",
                                        appData.ensSlave[appData.slaveSelected].idSlave);
#endif
                                appData.ensSlave[appData.slaveSelected].state = SLAVE_ERROR;
                            } else {
                                appData.ensSlave[appData.slaveSelected].nbError--;
                            }
                            MASTER_StoreBehavior(MASTER_APP_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
                        }
                        break;
                        /*-----------------------------------------------------------------*/
                    case SLAVE_COLLECT:
                    {
#if defined(UART_DEBUG)
                        printf("==>COLLECT LOG\n");
#endif  
                        --appData.ensSlave[appData.slaveSelected].nbTimeout;
                        if (appData.ensSlave[appData.slaveSelected].nbTimeout <= 0) {
                            appData.ensSlave[appData.slaveSelected].nbError--;
                            if (appData.ensSlave[appData.slaveSelected].nbError <= 0) {
                                appData.ensSlave[appData.slaveSelected].state = SLAVE_ERROR; // informer le master 
                                MASTER_StoreBehavior(MASTER_APP_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
                                break;
                            } else {
                                appData.ensSlave[appData.slaveSelected].nbTimeout = MAX_TIMEOUT;
                            }
                        }
                        MASTER_SendMsgRF(appData.ensSlave[appData.slaveSelected].idSlave,
                                ACK,
                                appData.ensSlave[appData.slaveSelected].index, 1,
                                (uint8_t *) (""), 0); // demande du paquet attendu 
                        TMR_SetWaitRqstTimeout(TIME_OUT_COLLECT_LOG);
                        appData.typeTimeout = RF_TIMEOUT;
                    }
                        break;
                        /*-----------------------------------------------------------------*/

                    case SLAVE_DAYTIME:
                    case SLAVE_NO_REQUEST:
                    {
#if defined( USE_UART1_SERIAL_INTERFACE )
                        printf("==>DAYTIME OR SLAVE NO REQUEST\n");
#endif
                        if (appData.ensSlave[appData.slaveSelected].state != SLAVE_NO_REQUEST) {
                            --appData.ensSlave[appData.slaveSelected].nbTimeout;
                            if (appData.ensSlave[appData.slaveSelected].nbTimeout <= 0) {
                                if (appData.ensSlave[appData.slaveSelected].nbError <= 0) {
                                    //peut etre c'est le master qui a un probleme 
                                    //ou c'est effectivement le slave 
                                    //si les 4 slave ne repondent pas c'est probablement le master 
#if defined( USE_UART1_SERIAL_INTERFACE )
                                    printf("SLAVE NO REQUEST %d\n", appData.ensSlave[appData.slaveSelected].uidSlave);
#endif
                                    appData.ensSlave[appData.slaveSelected].state = SLAVE_NO_REQUEST;
                                    sprintf(appError.message, "Error Slave No Request");
                                    appError.current_line_number = __LINE__;
                                    sprintf(appError.current_file_name, "%s", __FILE__);
                                    appError.number = ERROR_SLAVE_NO_REQUEST;
                                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_EXEPTIONNEL);
                                    break;
                                } else {
                                    appData.ensSlave[appData.slaveSelected].nbTimeout = MAX_TIMEOUT;
                                    --appData.ensSlave[appData.slaveSelected].nbError;
                                }
                            }
                        }
                        MASTER_StoreBehavior(MASTER_APP_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
                    }
                        break;
                    default:
                        break;
                }
            }
        }
            break;
            /* -------------------------------------------------------------- */
        case MASTER_APP_STATE_GPRS_ATTACHED:
        {
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf("> MASTER_APP_STATE_GPRS_ATTACHED\n");
#endif
            }

            int8_t i = 0;
            bool stop = false;
            if (app_TCPconnected()) {
                stop = true;
                appData.connectToServer = true;
            } else {
                bool gprsEnable = app_StatusGPRS();
                do {
                    if (!gprsEnable) {
                        gprsEnable = app_EnableModuleInGPRSmode(true, appData.gsm_apn);
                    }
                    if (gprsEnable || app_StatusGPRS()) {
                        if (app_StartTCPconnection(appData.gsm_ip_server, appData.gsm_port)) {
#if defined( USE_UART1_SERIAL_INTERFACE )
                            printf("TCP CONNECTED\n");
#endif
                            appData.connectToServer = true;
                            stop = true;
                            break;
                        } else
                            i++;
                    } else {
                        i++;
                    }
                    TMR_Delay(1000);
                } while (!stop && i < 40);
            }
            if (!stop) {
#if defined( USE_UART1_SERIAL_INTERFACE )
                printf("ERROR GPRS CONNECTION \n");
#endif
                sprintf(appError.message, "Error GSPRS no Attached");
                appError.current_line_number = __LINE__;
                sprintf(appError.current_file_name, "%s", __FILE__);
                appError.number = ERROR_GPRS_NO_ATTACHED;
                MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_EXEPTIONNEL);
            } else {
                if (appData.dayTime == GOOD_NIGHT) {
                    MASTER_StoreBehavior(MASTER_APP_STATE_SELECTE_SLAVE, PRIO_LOW);
                } else {
                    MASTER_StoreBehavior(MASTER_APP_STATE_SEND_STATUS_TO_SERVER, PRIO_LOW);
                }
            }
        }
            break;
            /* -------------------------------------------------------------- */
        case MASTER_APP_STATE_SEND_STATUS_TO_SERVER:
        {
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf(">MASTER_APP_STATE_SEND_STATUS_TO_SERVER\n");
#endif
            }
            if (!app_TCPconnected()) { // on teste une fois 
                appData.connectToServer = app_StartTCPconnection(appData.gsm_ip_server, appData.gsm_port);
            }
            if (appData.connectToServer) {
                uint8_t * buf[50]; // apres il faudrais reflechir a transmettre d'autre information
                sprintf(buf, "alive#%d#%d#%s#%d",
                        appData.masterId, INFOS, appData.siteid, 0);
                app_TCPsend(buf, 2000); // pour l'instant c'est bloquant
            }
        }
            break;
        case MASTER_APP_STATE_SEND_ERROR_TO_SERVER:
        {
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf(">MASTER_APP_STATE_SEND_ERROR_TO_SERVER\n");
#endif
            }
            bool b = true;
            bool r = true; //error receve 
            uint8_t bufErrorMSG [150];
            //                    TMR_SetWaitRqstTimeout(0); // declencher le l'interuption logiciel  
            //TODO : traitement d'erreur send ack
            switch (appError.number) {
                    //different type d'erreur 
                case ERROR_NONE:
#if defined(UART_DEBUG)
                    printf("transmission d'une erreur inconnue\n");
#endif
                    sprintf(bufErrorMSG, "OF %d du site %s ERREUR CRITIQUE INCONNUE",
                            appData.ensSlave[appData.slaveSelected].uidSlave, appData.siteid);
                    break;
                case ERROR_LOW_BATTERY:
#if defined(UART_DEBUG)
                    printf("transmission de niveau de batterie faible\n");
#endif
                    sprintf(bufErrorMSG, "OF %d du site %s niveau de batterie faible",
                            appData.ensSlave[appData.slaveSelected].uidSlave, appData.siteid);
                    break;
                case ERROR_LOW_FOOD:
#if defined(UART_DEBUG)
                    printf("transmission de niveau de nourriture faible\n");
#endif
                    sprintf(bufErrorMSG, "OF %d du site %s niveau de nourriture faible",
                            appData.ensSlave[appData.slaveSelected].uidSlave, appData.siteid);
                    break;
                case ERROR_LOW_VBAT:
#if defined(UART_DEBUG)
                    printf("transmission de ERROR_LOW_VBAT\n");
#endif
                    sprintf(bufErrorMSG, "OF %d du site %s niveau de batterie faible",
                            appData.ensSlave[appData.slaveSelected].uidSlave, appData.siteid);
                    break;
                case ERROR_DOOR_CANT_CLOSE:
#if defined(UART_DEBUG)
                    printf("transmission de PORTE OUVERTE\n");
#endif      

                    sprintf(bufErrorMSG, "OF %d du site %s PORTE OUVERTE",
                            appData.ensSlave[appData.slaveSelected].uidSlave, appData.siteid);
                    break;
                case ERROR_RF_MODULE:
#if defined(UART_DEBUG)
                    printf("transmission Module radio ne demarre pas.\n");
#endif      

                    sprintf(bufErrorMSG, "OF %s du site %s module rf ne demmare pas",
                            appData.udid, appData.siteid);
                    r = false;
                    break;
                case ERROR_SLAVE_NO_REQUEST:
#if defined(UART_DEBUG)
                    printf("transmission Slave no request.\n");
#endif      

                    sprintf(bufErrorMSG, "OF %d du site %s ne repond pas",
                            appData.ensSlave[appData.slaveSelected].uidSlave, appData.siteid);
                    r = false;
                    break;
                case ERROR_SERVER_NO_REQUEST:
#if defined(UART_DEBUG)
                    printf("transmission Server no request.\n");
#endif      

                    sprintf(bufErrorMSG, "Master %s du site %s le server ne repond pas",
                            appData.udid, appData.siteid);
                    r = false;
                    break;
                case ERROR_MASTER_LOW_BATTERY:
#if defined(UART_DEBUG)
                    printf("transmission error niveau de battery master.\n");
#endif      

                    sprintf(bufErrorMSG, "master %s du site %s niveau de batterrie faible",
                            appData.udid, appData.siteid);
                    r = false;
                    break;
                case ERROR_GPRS_NO_ATTACHED:
                {
#if defined(UART_DEBUG)
                    printf("transmission error GSM No ATTACHED\n");
#endif      

                    sprintf(bufErrorMSG, "master %s du site %s echeque de transmission de donnee",
                            appData.udid, appData.siteid);
                    r = false;
                }
                    break;
                default:
#if defined(UART_DEBUG)
                    b = false;
                    printf("Erreur non connue\n");
#endif
                    break;
            }
            if (b) {
                bool resp = false;
                if (!app_TCPconnected()) { // on teste une fois 
                    appData.connectToServer = app_StartTCPconnection(appData.gsm_ip_server, appData.gsm_port);
                }
                if (!appData.connectToServer) {
                    // try to send error with sms
                    resp = app_SendSMS(bufErrorMSG);
                } else {
                    // try to send with mail from server 
                    uint8_t errorToSend[200];
                    sprintf(errorToSend, "%s#%d#%d#%%s#%d", bufErrorMSG,
                            appData.masterId,
                            ERROR, appData.siteid, 0);
                    resp = app_TCPsend(errorToSend, 5000);
                }

                if (resp) {
                    if (r) {
                        MASTER_SendMsgRF(appData.ensSlave[appData.slaveSelected].idSlave,
                                ACK, appData.receive.Champ.idMsg, 1, "", 0);
                        MASTER_StoreBehavior(MASTER_APP_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
                    } else {
                        if (appError.number == ERROR_SLAVE_NO_REQUEST) {
                            if (appData.dayTime == GOOD_NIGHT)
                                MASTER_StoreBehavior(MASTER_APP_STATE_END, PRIO_LOW);
                            else {

                                MASTER_StoreBehavior(MASTER_APP_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
                            }
                            appData.ensSlave[appData.slaveSelected].errorNotify = true;
                        } else {
                            // error suysteme envoyer
                            MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_EXEPTIONNEL);
                            appError.errorSend = true;
                        }
                    }
                } else {
#if defined( USE_UART1_SERIAL_INTERFACE )
                    printf("Error No SEND To User\n");
#endif  
                    /* Log event if required */
                    if (true == appDataLog.log_events) {
                        store_event(OF_GSM_MSG_NOT_SEND_TO_SERVER);
                    }
                    appError.nbErrorSendSms--;
                    if (appError.nbErrorSendSms > 0)
                        MASTER_StoreBehavior(MASTER_APP_STATE_SEND_ERROR_TO_SERVER, PRIO_EXEPTIONNEL);
                    else
                        MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_EXEPTIONNEL);
                }
            } else {
#if defined(UART_DEBUG)
                printf("ERROR non reconuue \n");
#endif
                MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_EXEPTIONNEL);
            }

        }
            break;
            /* -------------------------------------------------------------- */
        case MASTER_APP_STATE_SEND_FROM_GSM:
        {
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf(">MASTER_STATE_SEND_FROM_GSM\n");
#endif
            }

            uint8_t buf[40];
            sprintf(buf, "#%d#%d#%s#%d#%d",
                    appData.ensSlave[appData.slaveSelected].idSlave,
                    DATA, appData.siteid,
                    appData.ensSlave[appData.slaveSelected].nbBloc,
                    appData.ensSlave[appData.slaveSelected].uidSlave);
            strncpy(appData.BUFF_COLLECT + (SIZE_DATA - 1) * (appData.ensSlave[appData.slaveSelected].index - 1), buf, 40);
#if defined (USE_UART1_SERIAL_INTERFACE) 
            int k = 0;
            while (appData.BUFF_COLLECT[k] != '\0' && k < SIZE_DATA * NB_BLOCK) {
                printf("%c", appData.BUFF_COLLECT[k++]);
            }
            printf("\n");
#endif
            //TODO : go to select slave 
            app_TCPsendToServer(appData.BUFF_COLLECT); // 
            // starrt timer 
            TMR_SetWaitRqstTimeout(TIME_OUT_WAIT_GSM_RQST);
            appData.typeTimeout = GSM_TIMEOUT;
        }
            break;
            /* -------------------------------------------------------------- */

        case MASTER_APP_STATE_MSG_GSM_RECEIVE:
        {
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf(">MASTER_APP_STATE_MSG_GSM_RECEIVE\n");
#endif
            }
            uint8_t * read = GSM3_GetResponse(); // je recupere le numero de bloc envoye
#if defined (USE_UART1_SERIAL_INTERFACE) 
            printf("==> %s\n", read);
#endif
            if (GSM3_findStringInResponse((uint8_t*) "MSG", read)) {
                //                if (appData.ensSlave[appData.slaveSelected].nbBloc == atoi(read)) {
                appData.ensSlave[appData.slaveSelected].nbRepet = 0;
                MASTER_StoreBehavior(MASTER_APP_STATE_SELECTE_SLAVE, PRIO_MEDIUM);
                if (appData.ensSlave[appData.slaveSelected].state == SLAVE_COLLECT_END_BLOCK) {
#if defined (USE_UART1_SERIAL_INTERFACE) 
                    printf("Slave end bloc\n");
#endif
                    appData.ensSlave[appData.slaveSelected].index = 1;
                    appData.ensSlave[appData.slaveSelected].nbBloc++;
                } else if (appData.ensSlave[appData.slaveSelected].state == SLAVE_COLLECT_END) {
                    //ACK permettant au slave de sleep
#if defined (USE_UART1_SERIAL_INTERFACE) 
                    printf("Slave end trans\n");
#endif
                    MASTER_SendMsgRF(appData.ensSlave[appData.slaveSelected].idSlave,
                            SLEEP, 1, 1, "", 0);
                }
            } else {
                MSTER_RetransGsmData();
            }
        }
            break;
        case MASTER_APP_STATE_END:
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf(">MASTER STATE END\n");
#endif
            }
            //TODO : gestion de fin
            // si on est a la collecte de donnees
            //     je verifie si j'ai collecte les donneer de tou le monde ==> SLLEEP
            // si on est pendant la journee
            //
            // si on est le soir
            if (appData.dayTime == GOOD_NIGHT) {
                int8_t i;
                int8_t j = 0;
                for (i = 0; i < appData.nbSlaveOnSite; i++) {
                    if (appData.ensSlave[i].state == SLAVE_ERROR) {

                        if (!appData.ensSlave[i].errorNotify) {
#if defined( USE_UART1_SERIAL_INTERFACE )
                            printf("ERROR Slave no request %d \n", appData.ensSlave[i].uidSlave);
#endif
                            sprintf(appError.message, "Error Slave No Request");
                            appError.current_line_number = __LINE__;
                            sprintf(appError.current_file_name, "%s", __FILE__);
                            appError.number = ERROR_SLAVE_NO_REQUEST;
                            MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_EXEPTIONNEL);
                            appData.slaveSelected = i;
                            appData.nbSlaveDataCollected--;
                            return; // stop loop
                        }
                    }
                    j++;
                }

                MASTER_StoreBehavior(MASTER_APP_STATE_SLEEP, PRIO_LOW);
                if (j >= appData.nbSlaveOnSite) {
#if defined( USE_UART1_SERIAL_INTERFACE )
                    printf("Sleep, rapport of collect %d\n", appData.nbSlaveOnSite);
#endif
                    if (appData.connectToServer) {
                        uint8_t buf[90];
                        // nothing to say end 
                        sprintf(buf, "Le site %s a fini la collecte pour %d/%d des OFs#%d#%d#%s#%d",
                                appData.siteid, appData.nbSlaveDataCollected, appData.nbSlaveOnSite,
                                appData.masterId, NOTHING, appData.siteid, 0);
                        sendEndReportToServer((uint8_t *) buf);
                    }
                }
            } else {
                if (appData.connectToServer) {
                    uint8_t buf[90];
                    sprintf(buf, "Le site %s aucuns OF Ne repond#%d#%d#%s#%d",
                            appData.siteid, appData.masterId, 2, appData.siteid, 0);
                    sendEndReportToServer((uint8_t *) buf);
                }

                if (true == appDataLog.log_events) {
                    store_event(OF_ALPHA_TRX_SALAVE_NO_REQUEST);
                }
            }
            break;
            /* -------------------------------------------------------------- */
        default:
            if (false == enter_default_state) {
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined( DISPLAY_CURRENT_STATE )
                printf("> DEFAULT_STATE\n");
#endif
#if defined ( USE_UART1_SERIAL_INTERFACE ) 
                getDateTime();
                printDateTime(appData.current_time);
#endif
                enter_default_state = true;
            }
            setLedsStatusColor(LED_RED);

            break;
    }
}

void MASTER_AppInit(void) {
    int8_t i;

    OC4_Stop();
    OC5_Stop();
    TMR4_Stop();
    TMR2_Stop();

    /* Attractive LEDs */
    setAttractiveLedsOff();
    appDataAttractiveLeds.current_color_index = 0;
    appDataAttractiveLeds.alt_sec_elapsed = 0;
    appDataAttractiveLeds.red[0] = 0;
    appDataAttractiveLeds.green[0] = 0;
    appDataAttractiveLeds.blue[0] = 0;
    appDataAttractiveLeds.red[1] = 0;
    appDataAttractiveLeds.green[1] = 0;
    appDataAttractiveLeds.blue[1] = 0;

    /* APP state task */
    for (i = 0; i < NB_BEHAVIOR_PER_PRIO; i++) {
        memset(appData.ptr[i], 0, 3);
    }

    MASTER_StoreBehavior(MASTER_APP_STATE_INITIALIZE, PRIO_HIGH);
    //    appData.state = MASTER_APP_STATE_INITIALIZE;
    appData.previous_state = MASTER_APP_STATE_ERROR;

    appData.need_to_reconfigure = false;

    /* Initialize all flags */
    appData.flags.reg = 0;

    /* I2C */
    memset(appData.i2c_add_found, 0, MAX_OF_UNKNOWN_I2C_8_BIT_SLAVE_ADD); // clear tab i2c_add_found
    appData.mcp23017.status_reg = 0;
    /* Set all digit Off. */
    appData.digit[0] = 0xFF;
    appData.digit[1] = 0xFF;
    appData.digit[2] = 0xFF;
    appData.digit[3] = 0xFF;

    appData.button_pressed = BUTTON_READ; /* initialized button status */

    /* communication */
    // gsm module
    appData.gsmInit = false;
    appData.connectToServer = false;

    appData.dayTime = GOOD_MORNING; // a voir 
    appData.masterSynchronizeTime = false;
    appData.slaveSynchronizeTime = true;
    appData.timeToSynchronizeHologe = 5; // a lire depuis le fichier ini
    /* communication */
    appData.masterId = 254;
    appData.broadCastId = 255;

    memset(appData.BUFF_COLLECT, '\0', NB_BLOCK * SIZE_DATA);
    for (i = 0; i < MAX_SLAVE; i++) {
        appData.ensSlave[i].idSlave = i + 1;
        appData.ensSlave[i].index = 1;
        appData.ensSlave[i].nbBloc = 1;
        appData.ensSlave[i].state = SLAVE_NONE;
        appData.ensSlave[i].nbError = MAX_ERROR;
        appData.ensSlave[i].nbTimeout = MAX_TIMEOUT;
        appData.ensSlave[i].tryToConnect = MAX_TRY_TO_SYNC;
        appData.ensSlave[i].errorNotify = false;
        appData.ensSlave[i].nbRepet = 0;
    }

    /* GSM Parameter */
    memset(appData.gsm_ip_server, 0, 13);
    memset(appData.gsm_num, 0, 11);
    memset(appData.gsm_pin, 0, 5);
    memset(appData.gsm_ip_server, 0, 20);
    memset(appData.gsm_port, 0, 5);
    /* Data logger */
    appData.nbCharPerLine = 66;
    appDataLog.is_file_name_set = false;
    appDataLog.num_char_buffer = 0;
    appDataLog.num_data_stored = 0;
    appDataLog.attractive_leds_current_color_index = 0;

    appDataLog.log_udid = false;
    appDataLog.log_events = true;
    appDataLog.log_errors = false;
    appDataLog.log_battery = false;
    appDataLog.log_temp = false;

    /* USB host */
    appDataUsb.usb_drive_status = USB_DRIVE_NOT_MOUNTED;
    appDataUsb.is_device_address_available = false;
    appDataUsb.is_device_needed = false;

    //power on and init raio module 
    //    powerRFEnable();


    for (i = 0; i < 4; i++) {
        appData.siteid[i] = 'X';
    }
    appData.siteid[4] = '\0';

    is_bird_detected = false;

    appDataEvent.file_type = EVENT_FILE_BINARY;

    appData.ext_temperature = 0.0;

    // patch my issues

    appError.led_color_1 = LEDS_OFF;
    appError.led_color_2 = LEDS_OFF;
    appError.number = ERROR_NONE;
    appError.current_line_number = 0;
    appError.is_data_flush_before_error = false;
    appError.errorSend = false;
    appError.OfInCriticalError = false;
    appError.nbErrorSendSms = 10; // on esssaie 10 fois 
    // if we send msg we wait interuption else delay
    appData.gsmMsgSend = false;
    appData.typeTimeout = NONE_TIMEOUT;

    appDataEvent.is_txt_file_name_set = false;
    appDataEvent.is_bin_file_name_set = false;

    //Stop OF 
    appDataAlarmDefaultStopOF.time.tm_hour = 23;
    appDataAlarmDefaultStopOF.time.tm_min = 55;
    //ON OF
    appDataAlarmDefaultOnOF.time.tm_hour = 5;
    appDataAlarmDefaultOnOF.time.tm_min = 59;
}


/*******************************************************************************
End of File
 */
