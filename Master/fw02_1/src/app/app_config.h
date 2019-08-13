/**
 * @file app_config.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 */

#ifndef _APP_CONFIG_HEADER_H
#define _APP_CONFIG_HEADER_H

#include "min_ini.h"

#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))

#define DEFAULT_LOG_SEPARATOR ","

#define DEFAULT_GUILLOTINE_TIME_OFFSET 5000

typedef enum {
    INI_READ_OK = 0,

    INI_PB_SITEID_ZONE,

    INI_PB_TIME_WAKEUP_HOUR,
    INI_PB_TIME_WAKEUP_MINUTE,

    INI_PB_TIME_SLEEP_HOUR,
    INI_PB_TIME_SLEEP_MINUTE,

    INI_PB_ATTRACTIVE_LEDS_RED_A,
    INI_PB_ATTRACTIVE_LEDS_GREEN_A,
    INI_PB_ATTRACTIVE_LEDS_BLUE_A,

    INI_PB_ATTRACTIVE_LEDS_RED_B,
    INI_PB_ATTRACTIVE_LEDS_GREEN_B,
    INI_PB_ATTRACTIVE_LEDS_BLUE_B,

    INI_PB_ATTRACTIVE_LEDS_ALT_DELAY,

    INI_PB_ATTRACTIVE_LEDS_ON_HOUR,
    INI_PB_ATTRACTIVE_LEDS_ON_MINUTE,

    INI_PB_ATTRACTIVE_LEDS_OFF_HOUR,
    INI_PB_ATTRACTIVE_LEDS_OFF_MINUTE,

    INI_PB_ATTRACTIVE_LEDS_PATTERN,

    INI_PB_ATTRACTIVE_LEDS_PATTERN_PERCENT,

    INI_PB_LOGS_UDID,
    INI_PB_LOGS_EVENTS,
    INI_PB_LOGS_ERRORS,
    INI_PB_LOGS_BATTERY,


    //anzilane modif
    INI_PB_LOCAL_ID_SLAVE_NO_SET
            

} INI_READ_STATE;

bool config_set(void);
FILEIO_RESULT config_find_ini(void);
INI_READ_STATE config_read_ini(void);
void config_print(void);
void getIniPbChar(INI_READ_STATE, char *, uint8_t);

#endif /* _APP_CONFIG_HEADER_H */


/*******************************************************************************
 End of File
 */
