/* *****************************************************************************
 *  
 * _____________________________________________________________________________
 *
 *                         MASTER API (.h)
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


#ifndef _APP_HEADER_H
#define _APP_HEADER_H

// *****************************************************************************
// *****************************************************************************
// Section: Firmware Configuration
// *****************************************************************************
// *****************************************************************************

#define DELAY_BEFORE_RESET 5000

#define PRINT_DATE
#define USE_UART1_SERIAL_INTERFACE  // uncomment to display information dsent to UART
#define DISPLAY_CURRENT_STATE       // uncomment to display the current state of main state machine (app.c))

#define DISPLAY_INI_READ_DATA
//#define DISPLAY_USB_INFO          // uncomment to display USB information
//#define DISPLAY_LOG_INFO 
#define DISPLAY_CHECK_INFO 
//#define DISPLAY_EVENT_INFO
//#define DISPLAY_I2C_STATUS
//#define DISPLAY_I2C_SCANNER
//#define DISPLAY_LEDS_STATUS
//#define DISPLAY_PCA9622_STATUS
//#define DISPLAY_ISR_RTCC          // uncomment to display interruption event
//#define DISPLAY_ISR_I2C           // uncomment to display interruption event
//#define DISPLAY_USB_ISR_INFO      // in USB_HostInterruptHandler( )

#define DISPLAY_RESET_INFORMATION

//#define DISPLAY_REMOTE_CONTROL_INFO 

#define DISPLAY_RESET_REGISTERS

//#define TEST_RTCC_SLEEP_WAKEUP

#define ENABLE_ERROR_LEDS

#define ENABLE_DEEP_SLEEP

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

/**------------------------>> I N C L U D E <<---------------------------------*/
#include "mcc.h"
#include "stdio.h"

#include "delay.h"
#include "mcp23017.h"
#include "pca9622.h"
#include "digits.h"
#include "buttons.h"
#include "led_status.h"
#include "ir_sensor.h"

#include "app_alarm.h"
#include "app_attractive_leds.h"
#include "app_check.h"
#include "app_config.h"
#include "app_data_logger.h"
#include "app_datetime.h"
#if defined (USE_UART1_SERIAL_INTERFACE)
#include "app_debug.h"
#endif
#include "app_error.h"
#include "app_event.h"
#include "app_i2c.h"
#include "app_power.h"
#include "app_remote.h"
#include "app_timers_callback.h"
#include "app_usb.h"
#include "app_fileio.h"
#include "app_rtc_action.h"
#include "app_version.h"


#include "../framework/AlphaTRX/Services.h"
#include "../framework/AlphaTRX/radio_alpha_trx.h"
#include "oc5.h"

/******************************************************************************/
/******************************************************************************/
/******************     MASTER APPLICATION CONTROL         ********************/
/***************************                ***********************************/

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

#define MAX_NUM_RESET 5
#define OPENFEEDER_IS_AWAKEN    1
#define OPENFEEDER_IS_SLEEPING  0
#define MAX_NUM_REWARD_TIMEOUT 5

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/******************************************************************************/

/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
 */

typedef enum {
    /* In this state, the application opens the driver */
    MASTER_APP_STATE_INITIALIZE,

    MASTER_APP_STATE_CONFIGURE_SYSTEM,

    MASTER_APP_STATE_IDLE,

    MASTER_APP_STATE_BATTERY_LEVEL_CHECK,
    MASTER_APP_STATE_GET_EMPERATURE,
    MASTER_APP_STATE_RTC_CALIBRATION,

    MASTER_APP_STATE_SERIAL_COMMUNICATION,

    MASTER_APP_STATE_DATA_LOG,

    MASTER_APP_STATE_STANDBY,
    MASTER_APP_STATE_SLEEP,
    MASTER_APP_STATE_WAKE_UP,

    MASTER_APP_STATE_FLUSH_DATA_TO_USB,
    MASTER_APP_STATE_REMOVE_USB_DEVICE,

    MASTER_APP_STATE_FLUSH_DATA_BEFORE_ERROR,

    // GSM 
    MASTER_APP_STATE_GPRS_ATTACHED,
    MASTER_APP_STATE_SEND_STATUS_TO_SERVER,
    MASTER_APP_STATE_SEND_FROM_GSM,
    MASTER_APP_STATE_SEND_ERROR_TO_SERVER,
    MASTER_APP_STATE_MSG_GSM_RECEIVE,
    //alphaTRX states 
    MASTER_APP_STATE_SEND_DATE,

    MASTER_APP_STATE_SELECTE_SLAVE,
    MASTER_APP_STATE_MSG_RF_RECEIVE,
    MASTER_APP_STATE_SEND_REQUEST_INFOS,
    MASTER_APP_STATE_TIMEOUT,
    MASTER_APP_STATE_END,

    /* Application error state */
    MASTER_APP_STATE_ERROR

} MASTER_APP_STATES;

// *****************************************************************************

/* Application : 

  Summary:
   

  Description:
    This structure ...

  Remarks:
    ...
 */
typedef enum { // if you want to add a new level, you must be increase MAX_LEVEL_PRIO
    PRIO_EXEPTIONNEL, //00 priorite exeptionnelle 
    PRIO_HIGH, //01
    PRIO_MEDIUM, //10
    PRIO_LOW //11
} PRIORITY;

typedef enum {
    PTR_READ, //00
    PTR_WRITE, //01
    PTR_OVFF //10
} PTR;

/**-------------------------->> S T R U C T -O F- T I M E O U T <<------------*/
typedef enum {
    NONE_TIMEOUT,
    RF_TIMEOUT,
    GSM_TIMEOUT
} TIMEOUT_Type;

/**-------------------------->> S T R U C T -O F- S L A V E - S T A T E <<----*/
typedef enum {
    SLAVE_SYNC, // phase de syncronisation 
    SLAVE_ERROR, // en etat d'erreur 
    SLAVE_NO_REQUEST,
    SLAVE_DAYTIME, // on est en journee 
    SLAVE_CONFIG, // etat de configiration 
    SLAVE_COLLECT, // Slave en etat de collecte de donnee
    SLAVE_SLECTED, // Slave selectionner 
    SLAVE_COLLECT_END, // fin de la collecte
    SLAVE_COLLECT_END_BLOCK, // fin de recuperation d'un bloc
    SLAVE_NONE // etat neutre 
} SLAVE_STATES;

typedef struct {
    uint8_t idSlave; // l'identifiant du slave
    uint8_t uidSlave; // l'identifiant reel du slave 
    int8_t tryToConnect; // nb d'essaie de connexion 
    int8_t nbTimeout; // le nombre consecutif de timeout lorsqu'on attend une reponse de ce slave
    int8_t nbError; // nombre d'erreurs, survenue pour ce slave
    uint8_t index; // le numero de paquet attendu, lors de la collecte des donnees 
    uint8_t nbBloc; // nombre de bloc re?u 
    SLAVE_STATES state; // etat du slave
    bool errorNotify; // si on a deja notifier l'erreur de ce slave
    int8_t nbRepet; //
} SlaveState;

typedef enum {
    GOOD_MORNING, //master wake up 
    GOOD_DAY, // the daytime  
    GOOD_NIGHT, // end day collect log 
    SEE_YOU_TOMORROW // master go to sleep
} STEP_OF_DAY;

/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct {
    /*gsm parameter*/
    char gsm_num [11]; // save phone number 
    char gsm_ip_server[13]; // get the adress of server 
    char gsm_apn[20];
    char gsm_pin[5];
    char gsm_port[5];

    char siteid[5];

    /* Application current state */
    MASTER_APP_STATES state; /* current state */
    MASTER_APP_STATES previous_state; /* save previous state */

    /* DateTime structure */
    struct tm current_time;
    struct ts i2c_current_time;

    union {
        uint16_t reg;

        struct {
            unsigned num_software_reset : 3;
            unsigned : 13;
        } bit_value;
    } dsgpr0;

    union {
        uint16_t reg;

        struct {
            unsigned : 16;
        } bit_value;
    } dsgpr1;

    union {
        uint16_t reg;

        struct {
            unsigned por : 1;
            unsigned bor : 1;
            unsigned idle : 1;
            unsigned sleep : 1;
            unsigned wdto : 1;
            unsigned swdten : 1;
            unsigned swr : 1;
            unsigned extr : 1;
            unsigned pmslp : 1;
            unsigned cm : 1;
            unsigned dpslp : 1;
            unsigned : 1;
            unsigned reten : 1;
            unsigned : 1;
            unsigned iopuwr : 1;
            unsigned trapr : 1;
        } bit_value;
    } reset_1;

    union {
        uint16_t reg;

        struct {
            unsigned vbat : 1;
            unsigned vbpor : 1;
            unsigned vddpor : 1;
            unsigned vddbor : 1;
            unsigned : 12;
        } bit_value;
    } reset_2;

    /* Declaration of FLAGS type. */
    union {
        uint8_t reg;

        struct {
            unsigned system_init : 1; /* true if configuration success from "CONFIG.INI" */
            unsigned attractive_leds_status : 1; /* true if configuration success from "CONFIG.INI" */
            unsigned remote_control_connected : 1; /* true if Remote Control is connected */
            unsigned new_valid_pit_tag : 1; /* true if a new PIT Tag is validated. */
            unsigned : 4;
        } bit_value;
    } flags;

    /* I2C - Slave Device found */
    uint8_t i2c_add_found[MAX_OF_UNKNOWN_I2C_8_BIT_SLAVE_ADD]; // FIXME: Do not place this here !

    /* I2C - Status of MCP23017 */
    union {
        uint8_t status_reg;

        struct {
            unsigned cmd_digits : 4; /* cmd individual 7 segments digits 1 to 4: 0b0001; 0b0010; 0b0100; 0b1000; */
            unsigned : 2;
            unsigned found : 1; /* true if MCP23017 is detected when USER BUTTON PRESSED. */
            unsigned initialized : 1; /* FALSE if MCP23017 is not connected, TRUE means that PORTs have been initialized. */
        } status_bit;
    } mcp23017; // FIXME: Do not place this here !

    /* I2C - 4x 7 segments digits LEDs on MCP23017 GPIO */
    uint8_t digit[4]; /* binary value to display on each digit */

    /* I2C - Status of Buttons */
    BUTTON button_pressed;

    bool openfeeder_state;
    RTCC_ALARM_ACTION rtcc_alarm_action;

    /* Battery level*/
    uint16_t battery_level;
    uint16_t vbat_level;
    uint16_t light_level;

    MASTER_APP_STATES rc_previous_state;

    bool need_to_reconfigure;

    struct {
        uint8_t family;
        uint8_t device;
        uint8_t revision;
    } id;

    struct {
        uint32_t words[5];
    } udid;

    float ext_temperature;

    //
    STEP_OF_DAY dayTime;

    Frame receive;

    /* communication information */
    //gsm module 
    bool gsmInit;

    //init rf module module 
    int8_t timeToSynchronizeHologe;
    bool slaveSynchronizeTime;
    bool masterSynchronizeTime;
    bool RfModuleInit;
    bool connectToServer;

    volatile uint8_t behavior[MAX_LEVEL_PRIO][NB_BEHAVIOR_PER_PRIO];
    volatile uint8_t ptr[MAX_LEVEL_PRIO][3]; //READ - WRITE - OVFF (overflow)
    //    uint8_t BUFF_COLLECT[NB_BLOCK][SIZE_DATA];
    uint8_t BUFF_COLLECT[NB_BLOCK*SIZE_DATA];
    int8_t nbSlaveOnSite;
    SlaveState ensSlave[MAX_SLAVE]; // max of on site
    uint8_t slaveSelected;
    uint8_t station;
    uint8_t masterId;
    uint8_t broadCastId;
    uint8_t nbCharPerLine;
    bool gsmMsgSend;
    TIMEOUT_Type typeTimeout;

} MASTER_APP_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************

extern MASTER_APP_DATA appData; /* Main application data. */
extern APP_ERROR appError; /* Error application data. */
extern APP_DATA_USB appDataUsb; /* USB host application data. */
extern APP_DATA_ALARM appDataAlarmSleep;
extern APP_DATA_ALARM appDataAlarmWakeup;
extern APP_DATA_LEDS appDataAttractiveLeds; /* Error application data. */
extern APP_DATA_ALARM appDataAlarmDefaultStopOF;
extern APP_DATA_ALARM appDataAlarmDefaultOnOF;
extern APP_DATA_LOG appDataLog;
extern APP_DATA_RC appDataRc; /* Remote control application data. */
extern APP_DATA_EVENT appDataEvent;

extern volatile bool is_bird_detected;

//******************************************************************************
//******************************************************************************
// Function prototypes
//******************************************************************************
//******************************************************************************

//#define is_bird_sensor_detected( ) appDataLog.bird_pir_sensor_status /* Return the value of global variable. */
//#define clear_bird_sensor_detected( ) {appDataLog.bird_pir_sensor_status = 0;} /* Clear the bird detection sensor. */

/**-------------------------->> P R O T O T Y P E S <<------------------------*/
//______________________________________________________________________________
/**
 * 
 * @return 
 */
uint8_t MASTER_GetSlaveSelected();

/**SIZE_DATA
 * tramet un ensemble de octets
 * 
 * @param dest : le destinataire 0 < dest < 16 
 * @param typeMsg : ex : DATA, ACK, HORLOGE, ... etc
 *                  rensegner le bloc de donnee attendu 
 * @param idMsg : l'identifiant du msg ==> ack, peut etre utiliser pour
 * @param nbR : nombre de paquet que je souhaite transmettre 
 * @param data : la donner a transmettre ==> taille < 20
 * @param sizeData : la taille de la donnee a transmettre 
 * @return : le nombre d'octe transmis
 */
int8_t MASTER_SendMsgRF(uint8_t dest,
        uint8_t typeMsg,
        uint8_t idMsg,
        uint8_t nbR,
        uint8_t * data,
        uint8_t sizeData);


/**
 * 
 * @return : le nombre d'octe transmis
 */
uint8_t MASTER_SendDateRF();

/**
 * enregistre le comportement dans le tableau des comportements,
 * en fonction sa priorite
 *  
 * @param behavior : le comportement a traiter 
 * @param prio : la priorite du comportement a traiter 
 * @return :
 *          true  : le comportement est enregistre avec succes 
 *          false : on a ecrase un comportement  
 */
bool MASTER_StoreBehavior(MASTER_APP_STATES behavior, PRIORITY prio);

/**
 * 
 * @return : le prochain comportement a adopter 
 */
MASTER_APP_STATES MASTER_GetBehavior();


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     Sample application's initialization routine

  Description:
    This routine initializes sample application's state machine.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
 */

void MASTER_AppInit(void);


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    Sample application tasks function

  Description:
    This routine is the sample application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

// (en) State machine of the application.
// (fr) Machine à états de l'application.
void MASTER_AppTask(void);

/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/
#endif /* _APP_HEADER_H */


/*******************************************************************************
 End of File
 */
