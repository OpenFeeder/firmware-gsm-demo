// https://en.wikipedia.org/wiki/Javadoc
// TODO: Documentation of the microcontroller program to achieve with the documentation generation tool "Doxygen"

/**
 * @file name.ext
 * @brief Titre function
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date dd/mm/yyyy
 * Revision history:
 * @param name usage
 * @return usage
 */

/* Software */
// Docklight COM connections (RS232 terminal to communicate with USB Serial Port)
// Downloads: http://docklight.de/
// Lors de l'appuis dans Docklight sur la touche [Enter] le PC envoie 0D 0A soit 2 octets,
// dont 0A est interpréter comme la valeur suivante !
// --> pour résoudre cela dans Docklight, il faut appuyer sur la combinaison de touches
//     [Ctrl]+[Enter] pour n'envoyer que la valeur 0D (Carriage Return <CR>)
//     https://en.wikipedia.org/wiki/Control_character

// TODO: RE02/SPARE_4 => SP1 à relier sur nIRQ pin 2 du ALPHA-TRX433S


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"


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

APP_DATA appData; /* Global application data. */

/******************************************************************************
  Function:
    void APP_Tasks( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Tasks( void )
{
    /* Check the Application State. */
    switch ( appData.state )
    {
        case APP_STATE_INITIALIZE:
        {
            /**
             * Initializing the application.
             * (en) Application initialization when starting the main power.
             * (fr) Initialisation de l'application lors du démarrage de l'alimentation principale.
             */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                displayBootMessage( );
                printf( "> APP_STATE_INITIALIZE\n" );
                powerRFEnable( );

                // Check the power statut of the RF module
                if ( CMD_3v3_RF_GetValue( ) == false )
                {
                    printf( "RF Module enable.\n" );
                    FSK_Transceiver_Init( );
                }
                else
                {
                    printf( "RF Module disable.\n" );
                    printf( "Send 'T' to change power state of radio module.\n" );
                }
#endif
            }
            appData.state = APP_STATE_IDLE;
            break;
        }
            /* -------------------------------------------------------------- */

        case APP_STATE_IDLE:
            /**
             * Application idle state.
             *  - waiting for a event during a timeout period
             *  - if user detached USB key go to error state
             *  - event PIR sensor, detecting movement near the system
             *  - after the timeout period go into sleep mode
             */
            if ( appData.state != appData.previous_state )
            {
                appData.previous_state = appData.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE)
                printf( "> APP_STATE_IDLE\n" );
#endif
            }

            /* Green status LED blinks in idle mode. */
            LedsStatusBlink( LED_GREEN, 20, 1980 );

            //            // Check if we received data from RF module
            //            if ( 0 )
            //            {
            //                appData.state = APP_STATE_RADIO_RECEIVED;
            //                break;
            //            }



#if defined (USE_UART1_SERIAL_INTERFACE)
            /* Get interaction with the serial terminal. */
            received_order = APP_SerialDebugTasks( );
            STATUS_READ_VAL StatusRead;
            uint16_t i; // incrément de la boucle for
            switch ( received_order ) // for serial commande (SC)
            {
                    /* List of character user interface */
                    //                case ',':
                    //                case '?':
                    //                    /* Interface firmware terminal (Debug) */
                    //                    printf( "Key mapping:\n" );
                    //                    printf( " -      s: read STATUS register of the radio module\n" );
                    //                    break;
                    //                    /* -------------------------------------------------------------- */

                    //                case SC_READ_RF_STATUS:
                    //                    StatusRead.Val = FSK_Transceiver_Command( 0x0000 );
                    //                    printf( "StatusRead: %u, 0x%04X\n", StatusRead.Val, StatusRead.Val );
                    //                    //                    printf( "SC_READ_RF_STATUS\n" );
                    //                    break;

                    //                case SC_SEND_RF_DATA:
                case 'B':
                    // A complete transmit sequence should be performed as follows:
                    // a. Enable the TX register by setting the el bit to 1 (Configuration Setting Command, page 16)
                    // b. The TX register automatically filled out with 0xAAAA, which can be used to generate preamble.
                    // c. Enable the transmitter by setting the et bit (Power Management Command, page 16)
                    // d. The synthesizer and the PLL turns on, calibrates itself then the power amplifier automatically enabled
                    // e. The TX data transmission starts
                    // f. When the transmission of the byte completed, the nIRQ pin goes high, the SDO pin goes low at the same time. The nIRQ
                    // pulse shows that the first 8 bits (the first byte, by default 0xAA) has transmitted. There are still 8 bits in the transmit
                    // register.
                    // g. The microcontroller recognizes the interrupt and writes a data byte to the TX register
                    // h. Repeat f. - g. until the last data byte reached
                    // i. Using the same method, transmit a dummy byte. The value of this dummy byte can be anything.
                    // j. The next high to low transition on the nIRQ line (or low to high on the SDO pin) shows that the transmission of the data
                    // bytes ended. The dummy byte is still in the TX latch.
                    // k. Turn off the transmitter by setting the et bit to 0. This event will probably happen while the dummy byte is being
                    // transmitted. Since the dummy byte contains no useful information, this corruption will cause no problems.
                    // l. Clearing the el bit clears the Register Underrun interrupt; the nIRQ pin goes high, the SDO low.

                    /* Envoie des octets de synchronisation, Pre-amble: 1010 1010 */
                    printf( "Waitting before config TX mode...\n" );
                    FSK_Transceiver_Received_Pattern_Init( );

                    /* voir page 31 */
                    if ( RF_nIRQ_GetValue( ) == 0 )
                    {
                        display_STATUS_register_from_RF_module( );
                    }

                    printf( "Config TX mode...\n" );

                    FSK_Transceiver_Send_Init( );

                    while ( RF_nIRQ_GetValue( ) == 1 )
                    {
                        Nop( );
                    }

                    printf( "TX mode enable.\n" );

                    //                    RF_PowerManagement.bits.b7_er = 0;
                    //                    FSK_Transceiver_Command( RF_PowerManagement.Val );
                    //
                    //                    /** Passer en mode émission - configure transmitter "Configuration Setting Command" */
                    //                    RF_ConfigSet.bits.b7_el = 1;
                    //                    FSK_Transceiver_Command( RF_ConfigSet.Val );
                    //
                    //                    //                    RF_Status.bits.TX_Mode_Start = 1; /** Démarrage du Mode TX */
                    //                    // Power Management Command
                    //                    //RF_PowerManagement.bits.b4_es = 1; // 
                    //                    RF_PowerManagement.bits.b5_et = 1; // Enabling the Transmitter preloads the TX latch with 0xAAAA
                    //                    FSK_Transceiver_Command( RF_PowerManagement.Val );

                    //FSK_Transceiver_Send_Byte( 0xAA ); // Command Send: 0xB8AA
                    //                        ++NumberByteSend; // incrémenter le nombre d'octet envoyé
                    //
                    //                        if ( NumberByteSend == NB_PREAMBLE_TO_SEND )
                    //                        {
                    //                            NumberByteSend = 0; // RAZ du nombre d'octet à transmettre
                    //                            StateMachine_RF_Service = SM_TX_FRAME_SYNCHRON_PATTERN;
                    //                        }
                    // Transmitter turn-on time = 250 us
                    //                    for ( i = 0; i < 1000; i++ )
                    //                    {
                    //                        Nop( );
                    //                    }

                    /* Wait until ALPHA Module is out of power-up reset */
                    //                    do
                    //                    {
                    //                        RF_StatusRead.Val = FSK_Transceiver_Command( STATUS_READ_CMD ); // intitial SPI transfer added to avoid power-up problem
                    //#ifdef USE_UART
                    //                        printf( "A other Wait until RFM12B is out of power-up reset, status: 0x%04X\r\n", RF_StatusRead.Val );
                    //#endif
                    //                    }
                    //                    while ( RF_StatusRead.bits.b14_POR );
                    int i;
                    uint8_t send_byte = 0x31;
                    for ( i = 0; i < 5; i++ )
                    {
                        FSK_Transceiver_Send_Byte( send_byte ); // send 'B' for example
                        while ( RF_nIRQ_GetValue( ) == 0 )
                        {
                            Nop( );
                        }
                        //display_STATUS_register_from_RF_module( );
                        send_byte++;
                    }

                    FSK_Transceiver_Send_Byte( 0x42 ); // send 'B' for example

                    //                    while ( RF_nIRQ_GetValue( ) == 0 )
                    //                    {
                    //                        Nop( );
                    //                    }

                    display_STATUS_register_from_RF_module( );

                    while ( RF_nIRQ_GetValue( ) == 1 )
                    {
                        Nop( );
                    }

                    printf( "Transmit 1 byte.\n" );
                    display_STATUS_register_from_RF_module( );
                    //                    printf( "SC_SEND_RF_DATA\n" );
                    //                    printf( "send preamble and data\n" );
                    //                    RF_nSEL_SetLow( ); // Start communication with slave
                    //                    SPI1_Exchange8bit( 0xB8 ); // Transmit command WORD
                    //                    SPI1_Exchange8bit( 0xAA ); // Transmit data
                    //                    while ( RF_nIRQ_GetValue( ) == 1 ); // wait for the end of transmission
                    //                    SPI1_Exchange8bit( 0xAA ); // Transmit data
                    //                    while ( RF_nIRQ_GetValue( ) == 1 ); // wait for the end of transmission
                    //                    SPI1_Exchange8bit( 'B' ); // Transmit data
                    //                    RF_nSEL_SetHigh( ); // Stop communication with slave

                    FSK_Transceiver_Send_Byte( 0x00 ); // send 'B' for example

                    //                    while ( RF_nIRQ_GetValue( ) == 0 )
                    //                    {
                    //                        Nop( );
                    //                    }

                    display_STATUS_register_from_RF_module( );

                    while ( RF_nIRQ_GetValue( ) == 1 )
                    {
                        Nop( );
                    }

                    printf( "Transmit done.\n" );

                    printf( "Restauration RX mode\n" );
                    RF_PowerManagement.bits.b5_et = 0;
                    FSK_Transceiver_Command( RF_PowerManagement.Val );
                    RF_ConfigSet.bits.b7_el = 0;
                    FSK_Transceiver_Command( RF_ConfigSet.Val );

                    while ( RF_nIRQ_GetValue( ) == 0 )
                    {
                        Nop( );
                        //                        display_STATUS_register_from_RF_module( );
                    }

                    //                    RF_PowerManagement.bits.b7_er = 1;
                    //                    FSK_Transceiver_Command( RF_PowerManagement.Val );
                    FSK_Transceiver_Received_Pattern_Init( );
                    printf( "fin.\n" );
                    break;

                    //                case SC_NONE:
                default:
                    // if nothing else matches, do the default
                    // default is optional
                    break;
            }
#endif
            break;
            /* -------------------------------------------------------------- */

        default:
            //            Nop( );
#if defined (USE_UART1_SERIAL_INTERFACE) && defined(DISPLAY_CURRENT_STATE)
            printf( "> APP_STATE_DEFAULT\n" );
#endif
            setLedsStatusColor( LED_RED );
            break;
    }
}

void APP_Initialize( void )
{
    /* APP state task initialize */
    appData.state = APP_STATE_INITIALIZE;
    appData.previous_state = APP_STATE_ERROR;

}


/*******************************************************************************
End of File
 */
