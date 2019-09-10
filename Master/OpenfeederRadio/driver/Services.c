/*
 * File:   Services.c
 * Author: mmadi
 *
 * Created on 16 mars 2019, 19:03
 */

#include "Services.h"
#include "master_api.h"
#include "../mcc_generated_files/rtcc.h"
#include <string.h>
/******************************************************************************/
/******************************************************************************/
/****************** FONCTIONNALITEES COMMUNES AU ALPHA TRX ********************/
/***************************                ***********************************/

/*****************                                 ****************************/

bool setDateTime(int year, int month, int day, int hour, int minute, int second) {
    struct tm date_time;

    if (day < 1 || day > 31 ||
            month < 1 || month > 12 ||
            year < MIN_ADMISSIBLE_YEAR || year > MAX_ADMISSIBLE_YEAR ||
            hour < 0 || hour > 23 ||
            minute < 0 || minute > 59 ||
            second < 0 || second > 59) {
        return false;
    }

    date_time.tm_mday = day;
    date_time.tm_mon = month;
    date_time.tm_year = year;

    date_time.tm_hour = hour;
    date_time.tm_min = minute;
    date_time.tm_sec = second;

    RTCC_TimeSet(&date_time);

    return true;
}

bool getDateTime(void) {
    while (!RTCC_TimeGet(&appData.current_time)) {
        Nop();
    }
    if (appData.current_time.tm_mday < 1 || appData.current_time.tm_mday > 31 ||
            appData.current_time.tm_mon < 1 || appData.current_time.tm_mon > 12 ||
            appData.current_time.tm_year < MIN_ADMISSIBLE_YEAR || appData.current_time.tm_year > MAX_ADMISSIBLE_YEAR ||
            appData.current_time.tm_hour < 0 || appData.current_time.tm_hour > 23 ||
            appData.current_time.tm_min < 0 || appData.current_time.tm_min > 59 ||
            appData.current_time.tm_sec < 0 || appData.current_time.tm_sec > 59) {
        return false;
    } else {
        return true;
    }
}

void printDateTime(struct tm time) {
    // Print date and time on serial terminal (PC)

    printf("%02u/%02u/%04u %02u:%02u:%02u\n",
            time.tm_mday,
            time.tm_mon,
            2000 + time.tm_year,
            time.tm_hour,
            time.tm_min,
            time.tm_sec);

}


//ERROR

void printError(void) {
    printf("\t%02u/%02u/20%02u %02u:%02u:%02u ",
            appError.time.tm_mday,
            appError.time.tm_mon,
            appError.time.tm_year,
            appError.time.tm_hour,
            appError.time.tm_min,
            appError.time.tm_sec);

    if (appError.current_line_number > 0) {
        printf("\tERROR %03d\n\t%s\n\tIn %s at line %d\n", appError.number, appError.message, appError.current_file_name, appError.current_line_number);
    } else {
        printf("\tERROR %03d\n%s\n", appError.number, appError.message);
    }
}

void clearError(void) {
    memset(appError.message, '\0', sizeof ( appError.message));
    memset(appError.current_file_name, '\0', sizeof ( appError.current_file_name));
    appError.current_line_number = 0;
}

//************
// ALARM
//************
//unsigned char ConvertHexToBCD_A( unsigned char hexvalue ) {
//    unsigned char bcdvalue;
//    bcdvalue = ( hexvalue / 10 ) << 4;
//    bcdvalue = bcdvalue | ( hexvalue % 10 );
//    return ( bcdvalue );
//}

//void rtcc_set_alarm( int hour, int minute, int second, int alarmMask ) {
//
//    // Disable RTCC alarm
//    RTCCON1Hbits.ALRMEN = 0;
//
//    // set RTCC initial time
//    //    ALMDATEH = ( ConvertHexToBCD( initialTime->tm_year ) << 8 ) | ConvertHexToBCD( initialTime->tm_mon ); // YEAR/MONTH-1
//    //    ALMDATEL = ( ConvertHexToBCD( initialTime->tm_mday ) << 8 ) | ConvertHexToBCD( initialTime->tm_wday ); // /DAY-1/WEEKDAY
//    ALMTIMEH = ( ConvertHexToBCD_A( hour ) << 8 ) | ConvertHexToBCD_A( minute ); // /HOURS/MINUTES
//    ALMTIMEL = ( ConvertHexToBCD_A( second ) << 8 ); // SECOND
//
//    RTCCON1Hbits.AMASK = alarmMask;
//    
//    /* Indefinite repetition of the alarm  */       
//    RTCCON1Hbits.CHIME = 1;
//    
//    // Enable RTCC alarm
//    RTCCON1Hbits.ALRMEN = 1;
//
//}
//
//void rtcc_start_alarm( void )
//{
//    // Enable RTCC alarm
//    RTCCON1Hbits.ALRMEN = 1;
//}
//
//void rtcc_stop_alarm( void )
//{
//    // Disable RTCC alarm
//    RTCCON1Hbits.ALRMEN = 0;
//}

///****************                                         *********************/
///*************************                     ********************************/
///******************************************************************************/
///******************************************************************************/