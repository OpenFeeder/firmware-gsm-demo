/*
 * File:   app_rtc_action.c
 * Author: jerome
 *
 * Created on 17 novembre 2018, 07:16
 */


#include "app_master.h"

void manageRtcAction(  )
{
    bool flag;
    int i;
    
    /* If the OF is awaken => go to sleep mode */
    if ( OPENFEEDER_IS_AWAKEN == appData.openfeeder_state && RTCC_ALARM_SLEEP_OPENFEEDER == appData.rtcc_alarm_action )
    {
        MASTER_StoreBehavior(MASTER_APP_STATE_SLEEP, PRIO_EXEPTIONNEL);
//        appData.state = MASTER_APP_STATE_SLEEP;
        return;
    }

    /* If the OF is sleeping => wake up */
    if ( OPENFEEDER_IS_SLEEPING == appData.openfeeder_state && RTCC_ALARM_WAKEUP_OPENFEEDER == appData.rtcc_alarm_action )
    {
        MASTER_StoreBehavior(MASTER_APP_STATE_WAKE_UP, PRIO_EXEPTIONNEL);
//        appData.state = MASTER_APP_STATE_WAKE_UP;
        return;
    }

    /* Calibrate date and time using the external RTC module */
    if ( RTCC_RTC_CALIBRATION == appData.rtcc_alarm_action )
    {
        calibrateDateTime( );
    }

    /* Get temperature from the external RTC module */
    if ( RTCC_DS3231_TEMPERATURE == appData.rtcc_alarm_action )
    {
        if (0 < APP_I2CMasterSeeksSlaveDevice(DS3231_I2C_ADDR, DS3231_I2C_ADDR))
        {
            /* Log event if required */
            if ( true == appDataLog.log_events )
            {
               store_event(OF_DS3231_GET_TEMPERATURE); 
            }

            getDateTime( );
            getDS3231Temperature( );
            
            if ( appDataLog.num_ds3231_temp_stored < NUM_DS3231_TEMP_TO_LOG )
            {
                appDataLog.ds3231_temp[appDataLog.num_ds3231_temp_stored][0] = (float)appData.current_time.tm_hour;
                appDataLog.ds3231_temp[appDataLog.num_ds3231_temp_stored][1] = (float)appData.current_time.tm_min;
                appDataLog.ds3231_temp[appDataLog.num_ds3231_temp_stored][2] = appData.ext_temperature;
                ++appDataLog.num_ds3231_temp_stored;
            }
            else
            {
                /* Log event if required */
                if ( true == appDataLog.log_events )
                {
                    store_event( OF_DS3231_TEMP_OVERFLOW );
                }
            }
        }
    }

    /* Turn off the attractive LEDs */
    if ( RTCC_ALARM_SET_ATTRACTIVE_LEDS_OFF == appData.rtcc_alarm_action )
    {
        if ( ATTRACTIVE_LEDS_ON == appDataAttractiveLeds.status )
        {
            setAttractiveLedsOff( );
        }
    }

    /* Turn on the attractive LEDs */
    if ( RTCC_ALARM_SET_ATTRACTIVE_LEDS_ON == appData.rtcc_alarm_action )
    {
        if ( ATTRACTIVE_LEDS_OFF == appDataAttractiveLeds.status )
        {
            setAttractiveLedsOn( );
        }
    }
    
    //important
    if ( RTCC_BATTERY_LEVEL_CHECK == appData.rtcc_alarm_action )
    {
        flag = isPowerBatteryGood( );
        
        if ( appDataLog.num_battery_level_stored < NUM_BATTERY_LEVEL_TO_LOG )
        {
            appDataLog.battery_level[appDataLog.num_battery_level_stored][0] = appData.current_time.tm_hour;
            appDataLog.battery_level[appDataLog.num_battery_level_stored][1] = appData.battery_level;
            ++appDataLog.num_battery_level_stored;
        }
        else
        {
            /* Log event if required */
            if ( true == appDataLog.log_events )
            {
                store_event( OF_BATTERY_LEVEL_OVERFLOW );
            }
        }

        if ( false == flag )
        {
            MASTER_StoreBehavior(MASTER_APP_STATE_FLUSH_DATA_BEFORE_ERROR, PRIO_HIGH);
//            appData.state = MASTER_APP_STATE_FLUSH_DATA_BEFORE_ERROR;
            return;
        }
    }
    
    appData.rtcc_alarm_action = RTCC_ALARM_IDLE;
}


