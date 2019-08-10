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

#include "app_master.h"

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
APP_DATA_LEDS appDataAttractiveLeds; /* Atractive LEDs application data */
APP_DATA_LOG appDataLog; /* Datalogger application data */
APP_DATA_RC appDataRc; /* Remote control application data */
APP_DATA_EVENT appDataEvent; /* Events application data */

//AlphaTRX modifcation 
struct tm t;
volatile int8_t msgReceive = 0;
int8_t noPrint = 0;



/**-------------------------->> M A C R O S <<--------------------------------*/
#define GETpREAD(prio) appData.ptr[prio][PTR_READ] // recupere le ponteur de lecture
#define GETpWRITE(prio) appData.ptr[prio][PTR_WRITE] // recupere le pointeur d'ecriture
#define GET_OVFF(prio)  appData.ptr[prio][PTR_OVFF] // recipere le pointeur d'overFlow

#define INCpREAD(prio) (appData.ptr[prio][PTR_READ] = (appData.ptr[prio][PTR_READ]+1) % NB_BEHAVIOR_PER_PRIO)
#define INCpWRITE(prio) (appData.ptr[prio][PTR_WRITE] = (appData.ptr[prio][PTR_WRITE]+1) % NB_BEHAVIOR_PER_PRIO)
#define SET_0VFF(prio, set) (appData.ptr[prio][PTR_OVFF] = set)

#define GET_PUBLIC_ID_SLAVE(localIdSlave) (publicSlaveAdress[localIdSlave])

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
    frameToSend.Champ.src = MASTER_ID;
    frameToSend.Champ.crc ^= frameToSend.paquet[0];
    frameToSend.Champ.nbR = nbR;
    frameToSend.Champ.typeMsg = typeMsg;
    frameToSend.Champ.crc ^= frameToSend.paquet[1];
    frameToSend.Champ.idMsg = idMsg;
    frameToSend.Champ.crc ^= frameToSend.paquet[2];
    // data
    int8_t i;
    frameToSend.Champ.size = sizeData;
    frameToSend.Champ.crc ^= frameToSend.paquet[3];
    for (i = 0; i < frameToSend.Champ.size; i++) {
        frameToSend.Champ.data[i] = data[i];
        frameToSend.Champ.crc ^= frameToSend.paquet[i + 5]; // penser ? changer le 5 en generique 
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
    //TODO : use GSM function to update date here
    //    if (!app_UpdateRtcTimeFromGSM()) {
    //#if defined(UART_DEBUG)
    //        printf("No update from GSM\n");
    //#endif
    //        return 0;
    //    }
    //____________________________
    //________GET DATE____________
    if (RTCC_TimeGet(&picDate)) {
        Date d;
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
        return MASTER_SendMsgRF(BROADCAST_ID, HORLOGE, 1, 1, d.date, 5); // visualiser cette valeur 
    }
    return 0;
}

//______________________________________________________________________________
//______________________________________________________________________________
//________________________________STATE MACHINE FUNCTION________________________

uint8_t MASTER_GetSlaveSelected() {
    return (appData.ensSlave[appData.slaveSelected].idSlave);
}

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
    if (GETpREAD(PRIO_HIGH) != GETpWRITE(PRIO_HIGH)) {
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

    appData.state = MASTER_GetBehavior();
    /* Check the Application State. */
    switch (appData.state) {
        case MASTER_APP_STATE_INITIALIZE:
        {
            /*
             * Initializing the application.
             * 
             * Process:
             *  - Date and Time calibration
             *  - Enable PIR sensor    
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

            /* Power PIR sensor early in the code because of starting delay before usable */
            powerPIREnable();

            //alphaTRX modification : juste pour les test, il faut mieux structurer 
            powerRFEnable();
            // Check the power statut of the RF module
            if (CMD_3V3_RF_GetValue() == false) {
#if defined  (USE_UART1_SERIAL_INTERFACE )
                printf("RF Module Enable.\n");
#endif
                radioAlphaTRX_Init();
                radioAlphaTRX_ReceivedMode(); // receive mode actived
#if defined  (USE_UART1_SERIAL_INTERFACE ) 
                printf("RF Module INIT.\n");
#endif
            } else {
#if defined  (USE_UART1_SERIAL_INTERFACE )
                printf("RF Module disable.\n");
                printf("Send 'T' to change power state of radio module.\n");
#endif
            }

            /* Go to device configuration state */
            MASTER_StoreBehavior(MASTER_APP_STATE_CONFIGURE_SYSTEM, PRIO_HIGH);
            //            appData.state = MASTER_APP_STATE_CONFIGURE_SYSTEM;
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
             *      - RFID frequency (mandatory)
             *      - Food level 
             *  - Log Unique Device ID (UDID)
             *  - Test servomotor
             *  - Test I2C interface
             *  - Initialize attractive LEDs (if needed)
             *  
             * Next state: 
             *  - if configuration succeed => APP_STATE_IDLE
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
//                    appData.state = MASTER_APP_STATE_ERROR;
                    break;
                }

                /* Power USB device */
                powerUsbRfidEnable();
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
//                    appData.state = MASTER_APP_STATE_ERROR;
                    break;
                }
            }/* Otherwise wait for USB device connection or for timeout to reach */
            else {
                /* If timeout reached => error */
                if (true == isDelayMsEnding()) {
#if defined(_DEBUG)
                    printf("true == isDelayMsEnding( )\n"
                           "==> ERROR STATE\n");
#endif
                    appDataUsb.is_device_needed = false;
                    sprintf(appError.message, "USB device detection took more than %ldms", (long) MAX_DELAY_TO_DETECT_USB_DEVICE);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    appError.number = ERROR_USB_DEVICE_NOT_FOUND;
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
//                    appData.state = MASTER_APP_STATE_ERROR;
                    break;
                }

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
//                    appData.state = MASTER_APP_STATE_ERROR;
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
                        MASTER_StoreBehavior(MASTER_APP_STATE_CONFIGURE_SYSTEM, PRIO_HIGH);
//                        appData.state = MASTER_APP_STATE_CONFIGURE_SYSTEM;
                        break;
                    case APP_CHECK_BATTERY_PB:
                    case APP_CHECK_VBAT_PB:
                    case APP_CHECK_FOOD_LEVEL_PB:
                    case APP_CHECK_RFID_FREQ_PB:
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
//                    appData.state = MASTER_APP_STATE_ERROR;
                    break;
                }

                /* Log current firmware version */
                logFirmware();

                /* Log the Unique Devide ID is required */
                if (true == appDataLog.log_udid) {
                    logUdid();
                }

                /* Turn on LEDs status while servo is moved */
                setLedsStatusColor(LED_SERVO);

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
                printf("> APP_STATE_IDLE\n");
#endif              
                //#if defined ( USE_UART1_SERIAL_INTERFACE )
                //                printf("\tUser button pressed briefly => flush data and eject USB device\n");
                //                printf("\tUser button pressed longer => activate serial communication mode\n");
                //#endif   
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_STATE_IDLE);
                }

                /* Enable RTC alarm */
                rtcc_start_alarm();
            }

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
//                    appData.state = MASTER_APP_STATE_ERROR;
                    break;
                }
            }

            if (RTCC_ALARM_IDLE != appData.rtcc_alarm_action) {
                manageRtcAction();

                if (MASTER_APP_STATE_IDLE != appData.state) {
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
                        //                        appData.state = APP_STATE_TEST_RFID;
                        MASTER_StoreBehavior(MASTER_APP_STATE_SERIAL_COMMUNICATION, PRIO_LOW);
//                        appData.state = MASTER_APP_STATE_SERIAL_COMMUNICATION;
                        break;
                    } else {
                        MASTER_StoreBehavior(MASTER_APP_STATE_FLUSH_DATA_TO_USB, PRIO_MEDIUM);
//                        appData.state = MASTER_APP_STATE_FLUSH_DATA_TO_USB;
                        //                        appData.state = APP_STATE_SERIAL_COMMUNICATION;
                        break;
                    }
                }
            }

            /* Green status LED blinks in idle mode. */
            LedsStatusBlink(LED_GREEN, LEDS_OFF, 25, 4975);

            /* If a new bird is detected by the PIR sensor */
            if (true == is_bird_detected) {

                /* Disable PIR sensor interruption */
                EX_INT0_InterruptFlagClear();
                EX_INT0_InterruptDisable();

                /* Get landing time */
                while (!RTCC_TimeGet(&appDataLog.bird_arrived_time)) {
                    Nop();
                }
                break;
            }

            //#if defined (TEST_RTCC_SLEEP_WAKEUP)
            //            /* Next line for debugging sleep/wakeup only */
            //            /* Should be commented in normal mode */
            //            /* Modify time value according to sleep values in the CONFIG.INI file */
            //            setDateTime( 17, 9, 21, 22, 59, 55 );
            //#endif
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

                //                /* Display battery level using 4 LEDs bar graph */
                //                I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
                //                uint8_t writeBuffer[2]; // data to transmit
                //
                //                getBatteryLevel( );
                //                
                //                uint16_t step = (uint16_t)(HIGH_BATTERY_LEVEL-LOW_BATTERY_LEVEL)/4;
                //
                //                if ( 1 == PCA9622_OE_GetValue() )
                //                {
                //                    /* Enable PCA9622 device in Normal mode */
                //                    PCA9622_OE_SetLow( ); // output enable pin is active LOW
                //                }                
                //    
                //                writeBuffer[0] = CTRLREG_LEDOUT3;
                //                writeBuffer[1] = 0b10101010; // CTRLREG PWM on all output for LEDOUT3
                //                i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
                //
                //                writeBuffer[0] = CTRLREG_PWM12;
                //                writeBuffer[1] = 0; // PWM0 Individual Duty Cycle for LED_RGB1_R
                //                i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);
                //                writeBuffer[0] = CTRLREG_PWM13;
                //                writeBuffer[1] = 0; // PWM0 Individual Duty Cycle for LED_RGB1_R
                //                i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);
                //                writeBuffer[0] = CTRLREG_PWM14;
                //                writeBuffer[1] = 0; // PWM0 Individual Duty Cycle for LED_RGB1_R
                //                i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);
                //                writeBuffer[0] = CTRLREG_PWM15;
                //                writeBuffer[1] = 0; // PWM0 Individual Duty Cycle for LED_RGB1_R
                //                i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);
                //                    
                //                if ( appData.battery_level > LOW_BATTERY_LEVEL )
                //                {
                //                    writeBuffer[0] = CTRLREG_PWM12;
                //                    writeBuffer[1] = 25; // PWM0 Individual Duty Cycle for LED_RGB1_R
                //                    i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);
                //                }
                //                if ( appData.battery_level > ( LOW_BATTERY_LEVEL + step ) )
                //                {
                //                    writeBuffer[0] = CTRLREG_PWM13;
                //                    writeBuffer[1] = 25; // PWM0 Individual Duty Cycle for LED_RGB1_R
                //                    i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);
                //                }
                //                if ( appData.battery_level > ( LOW_BATTERY_LEVEL + 2*step ) )
                //                {
                //                    writeBuffer[0] = CTRLREG_PWM14;
                //                    writeBuffer[1] = 25; // PWM0 Individual Duty Cycle for LED_RGB1_R
                //                    i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);
                //                }
                //                if ( appData.battery_level > ( LOW_BATTERY_LEVEL + 3*step ) )
                //                {
                //                    writeBuffer[0] = CTRLREG_PWM15;
                //                    writeBuffer[1] = 25; // PWM0 Individual Duty Cycle for LED_RGB1_R
                //                    i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);
                //                }
                //                
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
                //                /* Turn off battery level bar graph */
                //                I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
                //                uint8_t writeBuffer[2]; // data to transmit
                //                
                //                writeBuffer[0] = CTRLREG_LEDOUT3;
                //                writeBuffer[1] = 0b00000000; // CTRLREG PWM on all output for LEDOUT3
                //                i2c_status = I2C1_MasterWritePCA9622( PCA9622_ADDRESS, writeBuffer, 2 );
                //                
                //                writeBuffer[0] = CTRLREG_PWM12;
                //                writeBuffer[1] = 0; // PWM0 Individual Duty Cycle for LED_RGB1_R
                //                i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);
                //                writeBuffer[0] = CTRLREG_PWM13;
                //                writeBuffer[1] = 0; // PWM0 Individual Duty Cycle for LED_RGB1_R
                //                i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);
                //                writeBuffer[0] = CTRLREG_PWM14;
                //                writeBuffer[1] = 0; // PWM0 Individual Duty Cycle for LED_RGB1_R
                //                i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);
                //                writeBuffer[0] = CTRLREG_PWM15;
                //                writeBuffer[1] = 0; // PWM0 Individual Duty Cycle for LED_RGB1_R
                //                i2c_status = I2C1_MasterWritePCA9622(PCA9622_ADDRESS, writeBuffer, 2);
                //                
                //                /* Deactivate PCA9622 if attractive LEDs not used */
                //                if ( false == appData.flags.bit_value.attractive_leds_status )
                //                {
                //                    /* Disable PCA9622 device */
                //                    PCA9622_OE_SetHigh( );
                //                }
            }

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

                powerUsbRfidDisable();

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
                    appDataLog.num_rfid_freq_stored > 0 ||
                    appDataEvent.num_events_stored > 0 ||
                    appDataLog.num_ds3231_temp_stored > 0) {
                    /* Log data on USB device */
                    appDataUsb.is_device_needed = true;
                    powerUsbRfidEnable();
                } else {
#if defined ( USE_UART1_SERIAL_INTERFACE )  
                    printf("\tNo data to flush.\n");
                    MASTER_StoreBehavior(MASTER_APP_STATE_REMOVE_USB_DEVICE, PRIO_HIGH);
//                    appData.state = MASTER_APP_STATE_REMOVE_USB_DEVICE;
                    break;
#endif
                }

                setDelayMs(MAX_DELAY_TO_DETECT_USB_DEVICE);

            }

            if (appDataUsb.is_device_address_available) {
                if (FLUSH_DATA_ON_USB_DEVICE_SUCCESS != flushDataOnUsbDevice()) {
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
//                    appData.state = MASTER_APP_STATE_ERROR;
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
//                    appData.state = MASTER_APP_STATE_ERROR;
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
             *  - mise en sommeil simple du système
             *    OPERATION DURING SLEEP MODES - PIR sensor and alarm deep sleep
             *  - lors de la sortie du mode SLEEP, remettre en service les fonctions de l'OpenFeeder
             *  - passer à l'état APP_STATE_IDLE
             */
            if (appData.state != appData.previous_state) {
                appData.previous_state = appData.state;
#if defined ( USE_UART1_SERIAL_INTERFACE ) && defined ( DISPLAY_CURRENT_STATE )
                printf("> APP_STATE_SLEEP\n");
#endif
                /* Log event if required */
                if (true == appDataLog.log_events) {
                    store_event(OF_STATE_SLEEP);
                }

                /* Stop RTCC interuption during shutdown process */
                rtcc_stop_alarm();

                /* Log data on USB device */
                appDataUsb.is_device_needed = true;

            }

            if (true == appDataUsb.is_device_needed) {
                if (appDataUsb.is_device_address_available) {
                    if (FLUSH_DATA_ON_USB_DEVICE_SUCCESS != flushDataOnUsbDevice()) {
                        /* Unmount drive on USB device before power it off. */
                        if (USB_DRIVE_MOUNTED == appDataUsb.usb_drive_status) {
                            usbUnmountDrive();
                        }
                    }

                    USBHostShutdown();
                    powerUsbRfidDisable();
                } else {
                    break;
                }
            }

            /* Turn status LED off */
            setLedsStatusColor(LEDS_OFF);

#if defined ( TEST_RTCC_SLEEP_WAKEUP )
            /* Next line for debugging sleep/wakeup only */
            /* Should be commented in normal mode */
            /* Modify time value according to wake up values in the CONFIG.INI file */
            setDateTime(17, 9, 21, 5, 59, 50);
#endif
            /* Set alarm for wake up time */
            rtcc_set_alarm(appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min, appDataAlarmWakeup.time.tm_sec, EVERY_DAY);

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
            MASTER_StoreBehavior(MASTER_APP_STATE_WAKE_UP, PRIO_HIGH);
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

#if defined ( TEST_RTCC_SLEEP_WAKEUP )
            /* Next line for debugging sleep/wakeup only */
            /* Should be commented in normal mode */
            /* Modify time value according to sleep values in the CONFIG.INI file */
            setDateTime(17, 9, 21, 22, 59, 50);
#endif

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
                powerUsbRfidEnable();

                setDelayMs(MAX_DELAY_TO_DETECT_USB_DEVICE);
            }

            if (appDataUsb.is_device_address_available) {
                if (USB_DRIVE_NOT_MOUNTED == usbMountDrive()) {
                    appDataUsb.is_device_needed = false;
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
//                    appData.state = MASTER_APP_STATE_ERROR;
                    break;
                }

                /* Battery level */
                if (appDataLog.num_battery_level_stored > 0) {
                    setLedsStatusColor(LED_USB_ACCESS);
                    if (FILEIO_RESULT_FAILURE == logBatteryLevel()) {
                        appDataUsb.is_device_needed = false;
                        MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
//                        appData.state = MASTER_APP_STATE_ERROR;
                        break;
                    }
                }

                /* Error */
                if (ERROR_NONE < appError.number) {
                    setLedsStatusColor(LED_USB_ACCESS);
                    if (FILEIO_RESULT_FAILURE == logError()) {
                        appDataUsb.is_device_needed = false;
                        MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
//                        appData.state = MASTER_APP_STATE_ERROR;
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
//                    appData.state = MASTER_APP_STATE_ERROR;
                }

            } else {
                if (true == isDelayMsEnding()) {
                    appDataUsb.is_device_needed = false;
                    sprintf(appError.message, "USB device detection took more than %ldms", (long) MAX_DELAY_TO_DETECT_USB_DEVICE);
                    appError.current_line_number = __LINE__;
                    sprintf(appError.current_file_name, "%s", __FILE__);
                    appError.number = ERROR_USB_DEVICE_NOT_FOUND;
                    MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
//                    appData.state = MASTER_APP_STATE_ERROR;
                    break;
                }
                /* Blue and yellow status LEDs blink as long USB key is required. */
                LedsStatusBlink(LED_USB_ACCESS, LED_YELLOW, 500, 500);
                break;
            }

            setLedsStatusColor(LEDS_OFF);
            MASTER_StoreBehavior(MASTER_APP_STATE_ERROR, PRIO_HIGH);
//            appData.state = MASTER_APP_STATE_ERROR;
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
                USBHostShutdown();
                powerUsbRfidDisable();

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

                Sleep();

#if defined ( USE_UART1_SERIAL_INTERFACE )
                printError();
#endif

#if defined (ENABLE_ERROR_LEDS)
                /* Status LED blinks */
                TMR3_Start();
                setLedsStatusColor(appError.led_color_1);
                setDelayMs(150);
                while (0 == isDelayMsEnding());
                setLedsStatusColor(appError.led_color_2);
                setDelayMs(150);
                while (0 == isDelayMsEnding());
                setLedsStatusColor(LEDS_OFF);
                TMR3_Stop();
#endif
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
    int i;

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
    MASTER_StoreBehavior(MASTER_APP_STATE_INITIALIZE, PRIO_HIGH);
//    appData.state = MASTER_APP_STATE_INITIALIZE;
    appData.previous_state = MASTER_APP_STATE_ERROR;

    appData.need_to_reconfigure = false;

    appData.openfeeder_state = OPENFEEDER_IS_AWAKEN;
    appData.rtcc_alarm_action = RTCC_ALARM_WAKEUP_OPENFEEDER;

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

    /* Data logger */
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

    CMD_3V3_RF_SetLow();


    for (i = 0; i < 4; i++) {
        appData.siteid[i] = 'X';
    }
    appData.siteid[4] = '\0';

    is_bird_detected = false;

    appDataEvent.file_type = EVENT_FILE_BINARY;

    appData.ext_temperature = 0.0;


    appError.led_color_1 = LEDS_OFF;
    appError.led_color_2 = LEDS_OFF;
    appError.number = ERROR_NONE;
    appError.current_line_number = 0;
    appError.is_data_flush_before_error = false;

    appDataEvent.is_txt_file_name_set = false;
    appDataEvent.is_bin_file_name_set = false;

}


/*******************************************************************************
End of File
 */
