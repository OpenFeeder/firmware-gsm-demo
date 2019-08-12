/**
 * @file app_config.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#include "app_master.h"
#include "app_config.h"

#define _DEBUG (1) // à effacer 

bool config_set(void) {

    INI_READ_STATE read_ini_status;
    char buf[50];

    /* Search for the CONFIG.INI file. */
    if (FILEIO_RESULT_FAILURE == config_find_ini()) {
#if defined(_DEBUG)
        printf("ERRO ==> FILEIO_RESULT_FAILURE == config_find_ini( )\n");
#endif
        strcpy(appError.message, "CONFIG.INI not found");
        appError.current_line_number = __LINE__;
        sprintf(appError.current_file_name, "%s", __FILE__);
        appError.number = ERROR_INI_FILE_NOT_FOUND;
        return false;
    }

    /* Read the CONFIG.INI file. */
    read_ini_status = config_read_ini();

    if (INI_READ_OK != read_ini_status) {
#if defined(_DEBUG)
        printf("ERRO ==> INI_READ_OK %d != %d read_ini_status\n", INI_READ_OK, read_ini_status);
#endif
        getIniPbChar(read_ini_status, buf, sizearray(buf));

        sprintf(appError.message, "Wrong parameters in CONFIG.INI: %s (%d)", buf, read_ini_status);
        appError.current_line_number = __LINE__;
        sprintf(appError.current_file_name, "%s", __FILE__);
        appError.number = ERROR_INI_FILE_READ;
        return false;
    }

    return true;
}

FILEIO_RESULT config_find_ini(void) {

    FILEIO_SEARCH_RECORD searchRecord;

    /* Log event if required */
    if (true == appDataLog.log_events) {
        store_event(OF_FIND_INI);
    }

    return FILEIO_Find("CONFIG.INI", FILEIO_ATTRIBUTE_ARCHIVE, &searchRecord, true);
}

INI_READ_STATE config_read_ini(void) {

    int32_t read_parameter;
    int s, i;
    char str[20];
    bool slave_found = false;
    bool time_found = false;
    bool logs_found = false;
    bool siteid_found = false;
    bool attractiveleds_found = false;

    /* Log event if required */
    if (true == appDataLog.log_events) {
        store_event(OF_READ_INI);
    }

    /* Clear watch-dog timer because INI read take time */
    ClrWdt();

#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
    printf("\tReading CONFIG.INI.\n");
#endif 

    for (s = 0; ini_getsection(s, str, 20, "CONFIG.INI") > 0; s++) {
        /* Check if "slave" section is present in the INI file */
        if (0 == strcmp(str, "time")) {
            time_found = true;
        }
        /* Check if "slave" section is present in the INI file */
        if (0 == strcmp(str, "slave")) {
            slave_found = true;
        }
        /* Check if "logs" section is present in the INI file */
        if (0 == strcmp(str, "logs")) {
            logs_found = true;
        }
        /* Check if "check" section is present in the INI file */
        if (0 == strcmp(str, "siteid")) {
            siteid_found = true;
        }
        /* Check if "attractiveleds" is present in the INI file */
        if (0 == strcmp(str, "attractiveleds")) {
            attractiveleds_found = true;
        }


    }

    if (siteid_found) {
        /* Site identification. */
        s = ini_gets("siteid", "zone", "XXXX", appData.siteid, sizearray(appData.siteid), "CONFIG.INI");
        for (i = s; i < 4; i++) {
            appData.siteid[i] = 'X';
        }
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf("\tSite ID... read.\n");
#endif 
        /* local site */
        read_parameter = ini_getl("siteid", "local_site", -1, "CONFIG.INI");
        if (read_parameter == -1) {
            return INI_PB_SITEID_ZONE;
        } else {
            appData.station = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tStation id ... read.\n");
#endif     
        }

//        read_parameter = ini_getl("siteid", "master_id", -1, "CONFIG.INI");
//        if (read_parameter == -1) {
//            return INI_PB_SITEID_ZONE;
//        } else {
//            appData.masterId = (int) read_parameter;
//#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
//            printf("\tMaster id ... read.\n");
//#endif     
//        }

    }

    /* Clear watch-dog timer because INI read take time */
    ClrWdt();

    /* Wake up time. */
    read_parameter = ini_getl("time", "wakeup_hour", -1, "CONFIG.INI");
    if (-1 == read_parameter) {
        return INI_PB_TIME_WAKEUP_HOUR;
    } else {
        appDataAlarmWakeup.time.tm_hour = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf("\tTime Wakeup hour... read.\n");
#endif     
    }
    read_parameter = ini_getl("time", "wakeup_minute", -1, "CONFIG.INI");
    if (-1 == read_parameter) {
        return INI_PB_TIME_WAKEUP_MINUTE;
    } else {
        appDataAlarmWakeup.time.tm_min = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf("\tTime Wakeup minute... read.\n");
#endif
    }
    appDataAlarmWakeup.time.tm_sec = 0;

    /* Default Sleep time. */
    read_parameter = ini_getl("time", "sleep_hour", -1, "CONFIG.INI");
    if (-1 == read_parameter) {
        return INI_PB_TIME_SLEEP_HOUR;
    } else {
        appDataAlarmSleep.time.tm_hour = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf("\tTime sleep hour... read.\n");
#endif
    }
    read_parameter = ini_getl("time", "sleep_minute", -1, "CONFIG.INI");
    if (-1 == read_parameter) {
        return INI_PB_TIME_SLEEP_MINUTE;
    } else {
        appDataAlarmSleep.time.tm_min = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf("\tTime sleep minute... read.\n");
#endif
    }
    appDataAlarmSleep.time.tm_sec = 0;

    /* Clear watch-dog timer because INI read take time */
    ClrWdt();


    if (slave_found) {
        read_parameter = ini_getl("slave", "nb_slave", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_LOCAL_ID_SLAVE_NO_SET;
        } else {
            appData.nbSlaveOnSite = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tNumber of slave on site... read.\n");
#endif
        }

        int8_t i = 0;
        uint8_t buf[12];
        for (; i < appData.nbSlaveOnSite; i++) {
            sprintf(buf, "slave_id%d", i+1);
#if defined( USE_UART1_SERIAL_INTERFACE )
            printf("buf %s\n", buf);
#endif
            read_parameter = ini_getl("slave", buf, -1, "CONFIG.INI");
            if (-1 == read_parameter) {
                return INI_PB_LOCAL_ID_SLAVE_NO_SET;
            } else {
                appData.ensSlave[i].uidSlave = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
                printf("\tslave id%d  uid %d ... read.\n", i+1, appData.ensSlave[i].uidSlave);
#endif
            }
        }
    }

    /* Clear watch-dog timer because INI read take time */
    ClrWdt();
    if (true == attractiveleds_found) {
        appData.flags.bit_value.attractive_leds_status = true;

        read_parameter = ini_getl("attractiveleds", "red_a", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_ATTRACTIVE_LEDS_RED_A;
        } else {
            appDataAttractiveLeds.red[0] = (uint8_t) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs red A... read.\n");
#endif
        }
        read_parameter = ini_getl("attractiveleds", "green_a", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_ATTRACTIVE_LEDS_GREEN_A;
        } else {
            appDataAttractiveLeds.green[0] = (uint8_t) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs green A... read.\n");
#endif
        }
        read_parameter = ini_getl("attractiveleds", "blue_a", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_ATTRACTIVE_LEDS_BLUE_A;
        } else {
            appDataAttractiveLeds.blue[0] = (uint8_t) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs blue A... read.\n");
#endif
        }

        /* Attractive LEDs wake up time. */
        read_parameter = ini_getl("attractiveleds", "on_hour", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_ATTRACTIVE_LEDS_ON_HOUR;
        } else {
            appDataAttractiveLeds.wake_up_time.tm_hour = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs on hour... read.\n");
#endif
        }
        read_parameter = ini_getl("attractiveleds", "on_minute", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_ATTRACTIVE_LEDS_ON_MINUTE;
        } else {
            appDataAttractiveLeds.wake_up_time.tm_min = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs on minute... read.\n");
#endif
        }

        appDataAttractiveLeds.wake_up_time.tm_sec = 0;

        /* Attractive LEDs sleep time. */
        read_parameter = ini_getl("attractiveleds", "off_hour", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            return INI_PB_ATTRACTIVE_LEDS_OFF_HOUR;
        } else {
            appDataAttractiveLeds.sleep_time.tm_hour = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs off hour... read.\n");
#endif
        }
        read_parameter = ini_getl("attractiveleds", "off_minute", -1, "CONFIG.INI");
        if (read_parameter == -1) {
            return INI_PB_ATTRACTIVE_LEDS_OFF_MINUTE;
        } else {
            appDataAttractiveLeds.sleep_time.tm_min = (int) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tAttractive LEDs off minute... read.\n");
#endif
        }
        appDataAttractiveLeds.sleep_time.tm_sec = 0;
    }
    /* Logs */
    if (logs_found) {
        /* Data separator in the log file. */
        ini_gets("logs", "separator", DEFAULT_LOG_SEPARATOR, appDataLog.separator, sizearray(appDataLog.separator), "CONFIG.INI");
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
        printf("\tLogs separator... read.\n");
#endif
        //                read_parameter = ini_getl( "logs", "birds", -1, "CONFIG.INI" );
        //                if ( -1 == read_parameter )
        //                {
        //                    return INI_PB_LOGS_BIRDS;
        //                }
        //                else
        //                {
        //                    appDataLog.log_data = ( bool ) read_parameter;
        //                }
        read_parameter = ini_getl("logs", "udid", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            appDataLog.log_udid = true;
            //            return INI_PB_LOGS_UDID;
        } else {
            appDataLog.log_udid = (bool) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tLogs UDID... read.\n");
#endif
        }
        read_parameter = ini_getl("logs", "events", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            appDataLog.log_events = true;
            //            return INI_PB_LOGS_EVENTS;
        } else {
            appDataLog.log_events = (bool) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tLogs events... read.\n");
#endif
        }
        read_parameter = ini_getl("logs", "errors", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            appDataLog.log_errors = true;
            //            return INI_PB_LOGS_ERRORS;
        } else {
            appDataLog.log_errors = (bool) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tLogs errors... read.\n");
#endif
        }
        read_parameter = ini_getl("logs", "battery", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            appDataLog.log_battery = true;
            //            return INI_PB_LOGS_BATTERY;
        } else {
            appDataLog.log_battery = (bool) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tLogs battery... read.\n");
#endif
        }
        read_parameter = ini_getl("logs", "temperature", -1, "CONFIG.INI");
        if (-1 == read_parameter) {
            //            return INI_PB_LOGS_RFID;
            appDataLog.log_temp = true;
        } else {
            appDataLog.log_temp = (bool) read_parameter;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_INI_READ_DATA)
            printf("\tLogs temperature... read.\n");
#endif
        }

    } else {
        /* Data separator in the log file. */
        ini_gets("logfile", "separator", DEFAULT_LOG_SEPARATOR, appDataLog.separator, sizearray(appDataLog.separator), "CONFIG.INI");
        appDataLog.log_birds = true;
        appDataLog.log_udid = true;
        appDataLog.log_events = true;
        appDataLog.log_errors = true;
        appDataLog.log_battery = true;
        appDataLog.log_rfid = true;
        appDataLog.log_temp = true;
        appDataLog.log_calibration = true;
    }

    /* Clear watch-dog timer because INI read take time */
    ClrWdt();

    return INI_READ_OK;
}

void config_print(void) {
    int i, j;

    printf("Configuration parameters\n");

    printf("\tSite ID\n\t\tZone: %s\n",
           appData.siteid);

    printf("\tTime\n");
    printf("\t\tWake up: %02d:%02d\n",
           appDataAlarmWakeup.time.tm_hour,
           appDataAlarmWakeup.time.tm_min);
    printf("\t\tSleep: %02d:%02d\n",
           appDataAlarmSleep.time.tm_hour,
           appDataAlarmSleep.time.tm_min);

    printf("\tLoggers\n");
    printf("\t\tSeparator: %s\n", appDataLog.separator);

    if (true == appDataLog.log_battery) {
        printf("\t\tBattery: enable - %s\n", BATTERY_LOG_FILE);
    } else {
        printf("\t\tBattery: disable\n");
    }

    if (true == appDataLog.log_temp) {
        printf("\t\tTemperature: enable - %s\n", EXT_TEMP_LOG_FILE);
    } else {
        printf("\t\tTemperature: disable\n");
    }
    if (true == appDataLog.log_calibration) {
        printf("\t\tCalibration: enable - %s\n", CALIBRATION_LOG_FILE);
    } else {
        printf("\t\tTemperature: disable\n");
    }
    if (true == appDataLog.log_udid) {
        printf("\t\tUdid: enable - %s\n", UDID_LOG_FILE);
    } else {
        printf("\t\tUdid: disable\n");
    }
    if (true == appDataLog.log_errors) {
        printf("\t\tErrors: enable - %s\n", ERRORS_LOG_FILE);
    } else {
        printf("\t\tErrors: disable\n");
    }
    if (true == appDataLog.log_events) {
        if (EVENT_FILE_BINARY == appDataEvent.file_type) {
            printf("\t\tEvents: enable - %s\n", appDataEvent.bin_file_name);
        } else if (EVENT_FILE_TEXT == appDataEvent.file_type) {
            printf("\t\tEvents: enable - %s\n", appDataEvent.txt_file_name);
        } else {
            printf("\t\tEvents: enable - %s - %s\n", appDataEvent.txt_file_name, appDataEvent.bin_file_name);
        }
    } else {
        printf("\t\tEvents: disable\n");
    }

    if (true == appData.flags.bit_value.attractive_leds_status) {

        printf("\tAttractive LEDs\n");

        printf("\t\tLEDs order: %u %u %u %u\n",
               appDataAttractiveLeds.leds_order[0], appDataAttractiveLeds.leds_order[1],
               appDataAttractiveLeds.leds_order[2], appDataAttractiveLeds.leds_order[3]);

        printf("\t\tOn time: %02d:%02d\n",
               appDataAttractiveLeds.wake_up_time.tm_hour,
               appDataAttractiveLeds.wake_up_time.tm_min);
        printf("\t\tOff time: %02d:%02d\n",
               appDataAttractiveLeds.sleep_time.tm_hour,
               appDataAttractiveLeds.sleep_time.tm_min);
    }

    printf("\n");
}

void getIniPbChar(INI_READ_STATE state, char *buf, uint8_t n) {

    switch (state) {
        case INI_PB_LOCAL_ID_SLAVE_NO_SET : 
            sprintf(buf, n, "slave local id");
        case INI_PB_SCENARIO_NUM:
            snprintf(buf, n, "Scenario: number");
            break;
        case INI_PB_SITEID_ZONE:
            snprintf(buf, n, "Site ID: zone");
            break;
        case INI_PB_TIME_WAKEUP_HOUR:
            snprintf(buf, n, "Wake-up: hour");
            break;
        case INI_PB_TIME_WAKEUP_MINUTE:
            snprintf(buf, n, "Wake-up: minute");
            break;
        case INI_PB_TIME_SLEEP_HOUR:
            snprintf(buf, n, "Sleep: hour");
            break;
        case INI_PB_TIME_SLEEP_MINUTE:
            snprintf(buf, n, "Sleep: minute");
            break;
        case INI_PB_LOGS_BIRDS:
            snprintf(buf, n, "Logs: birds");
            break;
        case INI_PB_LOGS_UDID:
            snprintf(buf, n, "Logs: udid");
            break;
        case INI_PB_LOGS_EVENTS:
            snprintf(buf, n, "Logs: events");
            break;
        case INI_PB_LOGS_ERRORS:
            snprintf(buf, n, "Logs: errors");
            break;
        case INI_PB_LOGS_BATTERY:
            snprintf(buf, n, "Logs: battery");
            break;
        case INI_PB_LOGS_RFID:
            snprintf(buf, n, "Logs: rfid");
            break;
        case INI_PB_ATTRACTIVE_LEDS_RED_A:
            snprintf(buf, n, "Attractive LEDs: red A");
            break;
        case INI_PB_ATTRACTIVE_LEDS_GREEN_A:
            snprintf(buf, n, "Attractive LEDs: green A");
            break;
        case INI_PB_ATTRACTIVE_LEDS_BLUE_A:
            snprintf(buf, n, "Attractive LEDs: blue A");
            break;
        case INI_PB_ATTRACTIVE_LEDS_RED_B:
            snprintf(buf, n, "Attractive LEDs: red B");
            break;
        case INI_PB_ATTRACTIVE_LEDS_GREEN_B:
            snprintf(buf, n, "Attractive LEDs: green B");
            break;
        case INI_PB_ATTRACTIVE_LEDS_BLUE_B:
            snprintf(buf, n, "Attractive LEDs: blue B");
            break;
        case INI_PB_ATTRACTIVE_LEDS_ALT_DELAY:
            snprintf(buf, n, "Attractive LEDs: alternate delay");
            break;
        case INI_PB_ATTRACTIVE_LEDS_ON_HOUR:
            snprintf(buf, n, "Attractive LEDs: off minute");
            break;
        case INI_PB_ATTRACTIVE_LEDS_ON_MINUTE:
            snprintf(buf, n, "Attractive LEDs: on minute");
            break;
        case INI_PB_ATTRACTIVE_LEDS_OFF_HOUR:
            snprintf(buf, n, "Attractive LEDs: off hour");
            break;
        case INI_PB_ATTRACTIVE_LEDS_OFF_MINUTE:
            snprintf(buf, n, "Attractive LEDs: off minute");
            break;
        case INI_PB_ATTRACTIVE_LEDS_PATTERN:
            snprintf(buf, n, "Attractive LEDs: pattern");
            break;
        case INI_PB_ATTRACTIVE_LEDS_PATTERN_PERCENT:
            snprintf(buf, n, "Attractive LEDs: pattern percent");
            break;
        default:
            snprintf(buf, n, "Error not listed");
            break;
    }


}

/*******************************************************************************
 End of File
 */
