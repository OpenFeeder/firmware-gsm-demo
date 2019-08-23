/**
 * @file app_check.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 12/20/2016
 * @revision
 */

#include "app_master.h"
#include "app_check.h"


APP_CHECK checkImportantParameters(void)
{
    /* Check battery level at startup. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf("\n\tBattery level: ");
#endif
    if (false == isPowerBatteryGood())
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
        printf("pb\n");
#endif
        return APP_CHECK_BATTERY_PB;
    }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf("ok\n");
#endif

    /* Check battery level at startup. */
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf("\tVBat level: ");
#endif
    if (false == isPowerVbatGood())
    {
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
        printf("pb\n");
#endif
        return APP_CHECK_VBAT_PB;
    }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CHECK_INFO)
    printf("ok\n");
#endif

    return APP_CHECK_OK;
}


bool isPowerBatteryGood( void )
{
    bool battery_level_ok;
    
    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
       store_event( OF_CHECK_BATTERY ); 
    }

    getBatteryLevel( );
    battery_level_ok = appData.battery_level > LOW_BATTERY_LEVEL;

    if (false == battery_level_ok)
    {
        strcpy( appError.message, "Low battery level" );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        appError.number = ERROR_MASTER_LOW_BATTERY;
    }

    return (battery_level_ok);
}


bool isPowerVbatGood(void)
{
    bool vbat_level_ok;
    
    /* Log event if required */
    if ( true == appDataLog.log_events )
    {
       store_event( OF_CHECK_VBAT ); 
    }

    getVBatLevel( );
    vbat_level_ok = appData.vbat_level > ( LOW_VBAT_LEVEL / 2 );

    if (false == vbat_level_ok)
    {
        strcpy( appError.message, "Low vbat level" );
        appError.current_line_number = __LINE__;
        sprintf( appError.current_file_name, "%s", __FILE__ );
        appError.number = ERROR_LOW_VBAT;
    }

    return (vbat_level_ok);
}


/*******************************************************************************
 End of File
 */
