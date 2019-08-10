/**
 * @file app_timers_callback.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 12/19/2016
 * @revision
 */

#include "app_master.h"
#include "app_timers_callback.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

volatile uint16_t g_rdyclk_count_in_10ms; /* variable to display the number of positive edge counter on RDY/CLK in (10 ms ! --> 20 ms) */ // FIXME: add to appData ?
volatile uint16_t g_timeout_x20ms;
volatile uint16_t g_timeout_standby_x20ms;
volatile uint16_t g_timeout_em4095_x20ms;
volatile uint16_t g_timeout_leds_status_x20ms;
volatile uint8_t g_timeout_taking_reward;
volatile uint8_t g_timeout_punishment_x20ms;
volatile uint8_t g_timeout_read_from_uart_x20ms;
volatile uint32_t g_timeout_store_date_x20ms = 0; // attention possible error

//test 
void setLogDate(uint8_t timeout_ms) {
    g_timeout_store_date_x20ms = timeout_ms;
}

bool isTimeToStoreDate() {
    return g_timeout_store_date_x20ms == 0;
}

//******************************************************************************
//******************************************************************************
//  Function Implementation
//******************************************************************************
//******************************************************************************

void TMR2_CallBack( void )
{
    static volatile uint8_t count_cmd_multiplex = 0;

    /* Multiplexing LEDs, either every 3 ms, measuring = 3.744 ms */
    // ticker is 0.003/0.000031875= 94 -> Callback function gets called everytime this ISR executes
    if ( appData.flags.bit_value.remote_control_connected )
    {
        //        LED_STATUS_B_Toggle( ); // FIXME: Debug display
        //        if ( appData.mcp23017.status_bit.found )
        //        {
        if ( ++count_cmd_multiplex > 94 )
        {
            count_cmd_multiplex = 0; /* reset ticker counter */
            if ( APP_isRemoteControlConnected( ) )
            {
                APP_MultiplexingLEDsTasks( ); /* Multiplexing LEDs on I2C Control Device board. (duration: 474 us) */
            }
            else
            {
                appData.flags.bit_value.remote_control_connected = false;
                //                    appData.mcp23017.status_bit.found = false;
                appData.mcp23017.status_bit.initialized = false;
            }
        }
        //        }
    }
}

void TMR3_CallBack( void )
{
    static volatile uint8_t counter_100ms = 0; // variable for capture the number of positive edge counter in 10 ms
//    static volatile uint8_t count_call_scanning_push_button = 0; // variable for ScanningPushButton
    static volatile uint16_t capture_positive_edge_counter_in_10ms = 0; // variable for capture the number of positive edge counter in 10 ms
    static volatile uint16_t previous_positive_edge_counter_in_10ms = 0; // and this to check if there is a new value

    /* This is for delay routine in app.c file
     * if timeout x20ms counter == 0 do nothing.
     */
    if (g_timeout_store_date_x20ms > 0) --g_timeout_store_date_x20ms;
        
    if ( g_timeout_x20ms )
    {
        --g_timeout_x20ms;
    }
    
    if ( g_timeout_read_from_uart_x20ms )
    {
        --g_timeout_read_from_uart_x20ms;
    }

    if ( g_timeout_em4095_x20ms )
    {
        --g_timeout_em4095_x20ms;
    }
    
    if ( g_timeout_standby_x20ms )
    {
        --g_timeout_standby_x20ms;
    }

    if ( g_timeout_leds_status_x20ms )
    {
        --g_timeout_leds_status_x20ms;
    }
    
    if ( g_timeout_punishment_x20ms )
    {
        --g_timeout_punishment_x20ms;
    }

    /* Timeout x100 ms here = 5x 20 ms */
    if ( ++counter_100ms == 5 )
    {
        counter_100ms = 0; /* reset counter every 100 ms */
        if ( g_timeout_taking_reward != 0 )
        {
            --g_timeout_taking_reward;
        }
    }
}

void TMR4_CallBack( void )
{
    static volatile uint16_t counter_timeout_160ms = 0;

//#if defined (DEBUG_RFID_WORKING_ON_LED_STATUS)
//    LED_STATUS_G_SetHigh( );
//#endif

    // callback function - called every 15th pass 
    //    if ( ++CountCallBack >= TMR4_INTERRUPT_TICKER_FACTOR )
    // with 15 next interrupt occur at 442 us
    // old : with 17 next interrupt occur at 495 us
    // counter_delay_read_bit     
}


/*******************************************************************************
 End of File
 */
