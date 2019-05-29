/**
 * @file app_debug.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 06/07/2016
 * @revision history 3
 */

#include <ctype.h>
#include <stdio.h>
#include "app.h"
#include "app_debug.h"

#if defined (USE_UART1_SERIAL_INTERFACE)

/* Current date to a C string (page 244)) */
//const uint8_t HW_VERSION[] = "v2.0"; // Board version
const uint8_t HW_VERSION[] = "v3.0"; // Board version
const uint8_t FW_VERSION[] = "v1.00"; // Firmware version
const uint8_t BUILD_DATE[] = { __DATE__ };
const uint8_t BUILD_TIME[] = { __TIME__ };

//SERIAL_CONTROL received_order;
uint8_t received_order;

/* Display information on serial terminal. */
void displayBuildDateTime( void )
{
    /* Displaying the build date and time. */
    printf( "Build on %s, %s\n", BUILD_DATE, BUILD_TIME );
}

void displayBootMessage( void )
{
    printf( "\n\n================ OpenFeeder ================\n" );
    printf( "      Board: %s - Firmware: ALPHA-TRX868S\n", HW_VERSION );
    printf( "      Code Version %s\n", FW_VERSION );
    printf( "      Build on %s, %s\n", BUILD_DATE, BUILD_TIME );
    printf( "============================================\n" );
    printf( "   Web page: https://github.com/OpenFeeder\n" );
    printf( "   Mail: contact.openfeeder@gmail.com\n" );
    printf( "============================================\n" );
    printf( "Type [?] key to display the Key mapping interface.\n\n" );
}

bool rf_power_status = false;


//SERIAL_CONTROL APP_SerialDebugTasks( void )

uint8_t APP_SerialDebugTasks( void )
{
    uint8_t new_serial_command = 0;
    //    uint16_t dc_pwm;
    //    int i, j;
    //    received_order = SC_NONE;

    if ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) )
    {
        /* If there is at least one byte of data has been received. */
        uint8_t data_from_uart1 = UART1_Read( );

        switch ( data_from_uart1 )
        {
            case ',':
            case '?':
                /* Interface firmware terminal (Debug) */
                printf( "Key mapping:\n" );
                printf( " !: displaying the build date and time\n" );

                printf( " b or B: Send byte to radio module\n" );
                //                printf( " r or R: display received data from radio module\n" );
                printf( " r or R: read the nIRQ value from the radio module\n" );
                //                printf( " s or S: send data to other radio module\n" );
                printf( " t or T: change power state of radio module\n" );
                //printf( " t or T: display date and time from RTCC module\n" );
                //                return SC_NONE;
                break;
                /* -------------------------------------------------------------- */

            case '!':
                displayBuildDateTime( );
                //                return SC_NONE;
                break;
                /* -------------------------------------------------------------- */

            case 'b':
            case 'B':
                printf( "Send byte to RF Module\n" );
                //                FSK_Transceiver_Send_Byte( 0x42 ); // send 'B' for example
                //while ( RF_nIRQ_GetValue( ) == 1 ); // wait for the end of transmission
                //                return SC_SEND_RF_DATA;
                new_serial_command = 'B';
                break;
                /* -------------------------------------------------------------- */

            case 'r':
            case 'R':
                //                printf( "Reception from the RF module\n" );
                if ( RF_nIRQ_GetValue( ) == true )
                {
                    printf( "nIRQ high.\n" );
                }
                else
                {
                    printf( "nIRQ low.\n" );
                }
                break;
                /* -------------------------------------------------------------- */

            case 's':
            case 'S':
                //case 'S':
                display_STATUS_register_from_RF_module( );
                //                    display_RF_Task( );
                break;
                /* -------------------------------------------------------------- */

                //{
                //                STATUS_READ_VAL StatusRead; // retour de la fonction write SPI

                //                printf( "Send to RF Module\n" );
                //printf( "Read status of RF module:\n" );
                /** Lecture du registre STATUS */
                //                StatusRead.Val = FSK_Transceiver_Command( 0x0000 );
                //                printf( "StatusRead: %u, ", StatusRead.Val );
                //                printf( "0x%04X\n", StatusRead.Val );
                //                printf( "StatusRead: %u, 0x%04X\n", StatusRead.Val, StatusRead.Val );
                //                return SC_READ_RF_STATUS;
                //return 's';
                //new_serial_command = 's';
                break;
                //}
                /* -------------------------------------------------------------- */

            case 't':
            case 'T':
                rf_power_status = !rf_power_status;
                if ( rf_power_status == true )
                {
                    CMD_3v3_RF_SetLow( );
                    printf( "RF Module enable\n" );
                }
                else
                {
                    CMD_3v3_RF_SetHigh( );
                    printf( "RF Module disable\n" );
                }
                /* Display date and time from RTCC module. */
                //                printCurrentDate( );
                //                printf( "CMD_3v3_RF: %u\n", CMD_3v3_RF_GetValue( ) );
                //                putchar( '\n' );
                break;
                /* -------------------------------------------------------------- */

            default:
                putchar( data_from_uart1 ); /* echo RX data if doesn't match */
                break;
        }
        //        return SC_NONE;
        return new_serial_command;
    } /* end of if ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) */
}

uint16_t readIntFromUart1( void )
{
    char rx_data_buffer[UART1_BUFFER_SIZE];
    uint8_t numBytes;

    numBytes = 0; /* initialized numBytes */
    do
    {
        if ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) )
        {
            rx_data_buffer[numBytes] = UART1_Read( );

            /* if received <CR> the stop received */
            if ( '\r' == rx_data_buffer[numBytes] )
            {
                break;
            }

            /* if received data is not a numerical value then return 0 */
            if ( isdigit( rx_data_buffer[numBytes] ) == false )
            {
                return 0;
            }

            ++numBytes;
        }
    }
    while ( numBytes < UART1_BUFFER_SIZE );

    rx_data_buffer[numBytes + 1] = '\0'; /* add end of string */

    return ( int ) strtol( rx_data_buffer, NULL, 10 );
} /* End of readIntFromUart1( ) */


#endif


/*******************************************************************************
 End of File
 */