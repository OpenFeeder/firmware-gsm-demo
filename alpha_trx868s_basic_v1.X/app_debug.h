/**
 * @file app_debug.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/07/2016
 */

#ifndef APP_DEBUG_H
#define APP_DEBUG_H

/**
 * Section: Global Variable Definitions
 */
#define UART1_BUFFER_SIZE  4


/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
 */

//typedef enum
//{
//    /* In this state, the application opens the driver */
//    SC_NONE,
//    SC_SEND_RF_DATA,
//    SC_READ_RF_STATUS
//
//} SERIAL_CONTROL; // serial control

//extern SERIAL_CONTROL received_order;
extern uint8_t received_order;


void displayBootMessage( void );
//SERIAL_CONTROL APP_SerialDebugTasks( void );
uint8_t APP_SerialDebugTasks( void );
void printUSBHostDeviceStatus( void );
uint16_t readIntFromUart1( void );


#endif /* APP_DEBUG_H */


/*******************************************************************************
 End of File
 */
