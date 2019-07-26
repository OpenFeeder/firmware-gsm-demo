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
#include "radio_alpha_trx_slave_api.h"

#if defined (USE_UART1_SERIAL_INTERFACE)

/* Current date to a C string (page 244)) */
//const uint8_t HW_VERSION[] = "v2.0"; // Board version
const uint8_t HW_VERSION[] = "v3.0"; // Board version
const uint8_t FW_VERSION[] = "v1.00"; // Firmware version
const uint8_t BUILD_DATE[] = {__DATE__};
const uint8_t BUILD_TIME[] = {__TIME__};

//SERIAL_CONTROL received_order;
uint8_t received_order;

/* Display information on serial terminal. */
void displayBuildDateTime(void) {
    /* Displaying the build date and time. */
    printf("Build on %s, %s\n", BUILD_DATE, BUILD_TIME);
}

void displayBootMessage(void) {
    printf("\n\n================ OpenFeeder ================\n");
    printf("      Board: %s - Firmware: ALPHA-TRX868S\n", HW_VERSION);
    printf("      Code Version %s\n", FW_VERSION);
    printf("      Build on %s, %s\n", BUILD_DATE, BUILD_TIME);
    printf("============================================\n");
    printf("   Web page: https://github.com/OpenFeeder\n");
    printf("   Mail: contact.openfeeder@gmail.com\n");
    printf("============================================\n");
    printf("Type [?] key to display the Key mapping interface.\n\n");
}

bool rf_power_status = false;
int8_t i = 0;

//SERIAL_CONTROL APP_SerialDebugTasks( void )

//uint8_t APP_SerialDebugTasks(void) {

void APP_SerialDebugTasks(void) {
    //uint8_t new_serial_command = 0;
    //    uint16_t dc_pwm;
    //    int i, j;
    //    received_order = SC_NONE;

    if (UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet()) {
        /* If there is at least one byte of data has been received. */
        uint8_t data_from_uart1 = UART1_Read();

        switch (data_from_uart1) {
            case ',':
            case '?':
                /* Interface firmware terminal (Debug) */
                printf("Key mapping:\n");
                printf(" !: displaying the build date and time\n");

                printf(" b or B: Send byte to radio module\n");
                //                printf( " r or R: display received data from radio module\n" );
                printf(" r or R: Genere ERROR\n");
                //                printf( " s or S: send data to other radio module\n" );
                printf(" t or T: change power state of radio module\n");
                //printf( " t or T: display date and time from RTCC module\n" );
                //                return SC_NONE;
                break;
                /* -------------------------------------------------------------- */

            case '!':
                displayBuildDateTime();
                //                return SC_NONE;
                break;
                /* -------------------------------------------------------------- */

            case 'b':
            case 'B':
                radioAlphaTRX_Init();
                radioAlphaTRX_ReceivedMode();
                break;
                /* -------------------------------------------------------------- */
                
            case 'p':
            case 'P':
                display_STATUS_register_from_RF_module();
                break;
                /* -------------------------------------------------------------- */
                display_STATUS_register_from_RF_module();
            case 'r':
            case 'R':
#if defined(UART_DEBUG)
                printf("ERROR GENERATED\n");
#endif          
                i = i % 8 + 1;
                radioAlphaTRX_SlaveSaveError(i);
                break;
                /* -------------------------------------------------------------- */

            case 's':
            case 'S':
                //case 'S':
#if defined(UART_DEBUG)
                printf("envoie  %d\n", radioAlphaTRX_SlaveSendNothing());
#endif
                break;
                /* -------------------------------------------------------------- */

            case 't':
            case 'T':
                rf_power_status = !rf_power_status;
                if (rf_power_status == false) {
                    powerRFEnable();
                    printf("RF Module enable\n");
                    //                    radioAlphaTRX_Init();
                    //                    radioAlphaTRX_ReceivedMode(); // receive mode actived
                } else {
                    powerRFDisable();
                    printf("RF Module disable\n");
                }
                /* Display date and time from RTCC module. */
                //                printCurrentDate( );
                //                printf( "CMD_3v3_RF: %u\n", CMD_3v3_RF_GetValue( ) );
                //                putchar( '\n' );
                break;
                /* -------------------------------------------------------------- */

            default:
                putchar(data_from_uart1); /* echo RX data if doesn't match */
                break;
        }
        //        return SC_NONE;
        //        return new_serial_command;
    } /* end of if ( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet( ) ) */
}

uint16_t readIntFromUart1(void) {
    char rx_data_buffer[UART1_BUFFER_SIZE];
    uint8_t numBytes;

    numBytes = 0; /* initialized numBytes */
    do {
        if (UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet()) {
            rx_data_buffer[numBytes] = UART1_Read();

            /* if received <CR> the stop received */
            if ('\r' == rx_data_buffer[numBytes]) {
                break;
            }

            /* if received data is not a numerical value then return 0 */
            if (isdigit(rx_data_buffer[numBytes]) == false) {
                return 0;
            }

            ++numBytes;
        }
    } while (numBytes < UART1_BUFFER_SIZE);

    rx_data_buffer[numBytes + 1] = '\0'; /* add end of string */

    return (int) strtol(rx_data_buffer, NULL, 10);
} /* End of readIntFromUart1( ) */


#endif

void display_STATUS_register_from_RF_module(void) {
    if (true == appPower_get_RFPowerState()) {
        /* Read RF module STATUS register */
        RF_StatusRead.Val = radioAlphaTRX_Command(STATUS_READ_CMD);
        printf("Read RF STATUS register: 0x%04X\n", RF_StatusRead.Val); // 4.1. ?criture format?e de donn?es --> https://www.ltam.lu/cours-c/prg-c42.htm
        printf("Bit [ 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 ]\n");
        printf("    [  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u |  %u ]\n", \
                        RF_StatusRead.bits.b15_RGIT_FFIT, \
                        RF_StatusRead.bits.b14_POR, \
                        RF_StatusRead.bits.b13_RGUR_FFOV, \
                        RF_StatusRead.bits.b12_WKUP, \
                        RF_StatusRead.bits.b11_EXT, \
                        RF_StatusRead.bits.b10_LBD, \
                        RF_StatusRead.bits.b9_FFEM, \
                        RF_StatusRead.bits.b8_ATSS, \
                        RF_StatusRead.bits.b7_ATS_RSSI, \
                        RF_StatusRead.bits.b6_DQD, \
                        RF_StatusRead.bits.b5_CRL, \
                        RF_StatusRead.bits.b4_ATGL, \
                        RF_StatusRead.bits.b3_OFFS_Sign, \
                        RF_StatusRead.bits.b2_OFFS_b2, \
                        RF_StatusRead.bits.b1_OFFS_b1, \
                        RF_StatusRead.bits.b0_OFFS_b0 \
                        );
        printf("     FFIT| POR|FFOV|WKUP| EXT| LBD|FFEM|RSSI| DQD| CRL|ATGL|OFFS|OFFS|OFFS|OFFS|OFFS|\n"); // transceiver in receive (RX) mode, bit 'er' is set
        printf("     RGIT      RGUR                      ATS                 <6>  <3>  <2>  <1>  <0>\n"); // bit 'er' is cleared
    } else {
        printf("Can't read status register, RF module is power OFF!\n");
    }
}

/*******************************************************************************
 End of File
 */
