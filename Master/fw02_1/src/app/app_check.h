/**
 * @file app_check.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 */

#ifndef _APP_CHECK_HEADER_H
#define _APP_CHECK_HEADER_H

typedef enum
{
    APP_CHECK_OK,

    APP_CHECK_BATTERY_PB,
    APP_CHECK_VBAT_PB

} APP_CHECK;

APP_CHECK checkImportantParameters( void );

/**
 * check battery level
 * @return : in same time return true if battery is good, false is not
 */
bool isPowerBatteryGood( void );
bool isPowerVbatGood( void );

#endif /* _APP_CHECK_HEADER_H */


/*******************************************************************************
 End of File
 */
