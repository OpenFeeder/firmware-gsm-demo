/*******************************************************************************
  Sample Application Header

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
 ******************************************************************************/

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Firmware Configuration
// *****************************************************************************
// *****************************************************************************


#define USE_UART1_SERIAL_INTERFACE  // uncomment to display information dsent to UART
#define DISPLAY_CURRENT_STATE       // uncomment to display the current state of main state machine (app.c))

//#define DISPLAY_USB_INFO          // uncomment to display USB information
//#define DISPLAY_LOG_INFO 
#define DISPLAY_CHECK_INFO 

//#define DISPLAY_ISR_RTCC          // uncomment to display interruption event
//#define DISPLAY_ISR_IR              // uncomment to display interruption event
//#define DISPLAY_ISR_PIR           // uncomment to display interruption event
//#define DISPLAY_ISR_RFID          // uncomment to display interruption event
//#define DISPLAY_ISR_I2C           // uncomment to display interruption event
//#define DISPLAY_USB_ISR_INFO      // in USB_HostInterruptHandler( )

//#define DISPLAY_RFID_STATE        // uncomment to display the current state
#define DISPLAY_PIT_TAG_INFO 
//#define DEBUG_RFID_WORKING_ON_LED_STATUS // uncomment to display hardware trace in RFID decoding routine

//#define DISPLAY_REMOTE_CONTROL_INFO 

//#define PATH_HARDWARE_IR_SENSOR_DISABLE // FIXME: comment for normal use of OpenFeeder


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "mcc.h"
//#include <stdio.h>
//#include <stdint.h>
//#include <stdbool.h>

#include "delay.h"
#include "led_status.h"

#include "app_power.h"
#include "app_timers_callback.h"
#include "radio_alpha_trx.h"


#if defined (USE_UART1_SERIAL_INTERFACE)
#include "app_debug.h"
#endif



// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

#define OPENFEEDER_IS_AWAKEN    1
#define OPENFEEDER_IS_SLEEPING  0

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

typedef enum
{
    /* In this state, the application opens the driver */
    APP_STATE_INITIALIZE,
    APP_STATE_IDLE,

    APP_STATE_RADIO_SEND_DATA,
    APP_STATE_RADIO_SEND_END_BLOCK,
    APP_STATE_RADIO_SEND_END,
    APP_STATE_RADIO_RECEIVED,

    /* Application error state */
    APP_STATE_ERROR

} APP_STATES;



// *****************************************************************************

/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* Application current state */
    APP_STATES state; /* current state */
    APP_STATES previous_state; /* save previous state */

    /* DateTime structure */
//    struct tm current_time;

    /* Declaration of FLAGS type. */
    union
    {
        uint8_t reg;

        struct
        {
            unsigned systemInit : 1; /* true if configuration success from "CONFIG.INI" */
            unsigned  : 1; /* true if configuration success from "CONFIG.INI" */
            unsigned  : 1; /* true if Remote Control is connected */
            unsigned  : 1; /* true if a new PIT Tag is validated. */
            unsigned : 4;
        } bit_value;
    } flags;

} APP_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************

extern APP_DATA appData; /* Main application data. */



//******************************************************************************
//******************************************************************************
// Function prototypes
//******************************************************************************
//******************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
 */


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

void APP_Initialize( void );


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
void APP_Tasks( void );


#endif /* _APP_H */

/*******************************************************************************
 End of File
 */
