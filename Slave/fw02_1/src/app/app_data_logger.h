/**
 * @file app_data_logger.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 08/09/2016
 */

#ifndef _APP_DATA_LOGGER_HEADER_H
#define _APP_DATA_LOGGER_HEADER_H


/* Nb de caracteres max sur une ligne du fichier LOG. */
#define MAX_CHAR_PER_LINE 90

/* Quantite de donnees a stocker dans le buffer avant ecriture dans le fichier LOG
 * --> donc nombre de passages d'oiseau
 */
#define MAX_NUM_DATA_TO_STORE 20

#define BATTERY_LOG_FILE "BATTERY.CSV"
#define RFID_LOG_FILE "RFIDFREQ.CSV"
#define FIRMWARE_LOG_FILE "FIRMWARE.CSV"
#define UDID_LOG_FILE "UDID.CSV"
#define EXT_TEMP_LOG_FILE "EXTTEMP.CSV"
#define ERRORS_LOG_FILE "ERRORS.CSV"
#define CALIBRATION_LOG_FILE "TIMCALIB.CSV"
//anzilane modif
#define TEST_SYNCHRO_FILE "SYNCHRO.CSV" // pense a changer 

#define FLUSH_DATA_ON_USB_DEVICE_FAIL -1
#define FLUSH_DATA_ON_USB_DEVICE_SUCCESS 0

#define NUM_BATTERY_LEVEL_TO_LOG 24
#define NUM_RFID_FREQ_TO_LOG 96
#define NUM_TIME_CALIBRATION_TO_LOG 96
#define NUM_DS3231_TEMP_TO_LOG 96

#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))

typedef struct
{
    /* Log file name - 8.3 convention - Upper case only */
    char filename[13];
    
    bool is_file_name_set;
    
    /* Data separator in log file */
    char separator[2];

    /* Character buffer to store data before export to the log file */
    char buffer[MAX_CHAR_PER_LINE * MAX_NUM_DATA_TO_STORE];
    /* Number of written characters in the buffer */
    unsigned int num_char_buffer;
    /* Number of line stored in the buffer */
    uint8_t num_data_stored;

    /* Bird Data */
    struct tm bird_arrived_time;
    struct tm bird_quit_time;
    char bird_pit_tag_str[11];
    bool is_pit_tag_denied;
    bool is_reward_taken;
    
    bool did_door_open;

    /* Attractive LEDs color*/
    uint8_t attractive_leds_current_color_index;

    uint8_t door_status_when_bird_arrived;

    int16_t battery_level[NUM_BATTERY_LEVEL_TO_LOG][2];
    uint8_t num_battery_level_stored;

    int16_t rfid_freq[NUM_RFID_FREQ_TO_LOG][3];
    uint8_t num_rfid_freq_stored;
    
    double  time_calibration[NUM_TIME_CALIBRATION_TO_LOG][3];
    uint8_t num_time_calib_stored;
    
    float ds3231_temp[NUM_DS3231_TEMP_TO_LOG][3];
    uint8_t num_ds3231_temp_stored;
    
    bool log_birds;
    bool log_data; // juste ajouter pour eviter l'erreur de compilation 
    bool log_udid;
    bool log_events;
    bool log_errors;
    bool log_battery;
    bool log_rfid;
    bool log_date;
    bool log_temp;
    bool log_calibration;

} APP_DATA_LOG;

bool dataLog( bool );
bool setLogFileName( void );
void GetTimestamp( FILEIO_TIMESTAMP * );

void clearLogBuffer( void );
void clearRfidFreqBuffer( void );
void clearExtTemperatureBuffer(void);
void clearBatteryBuffer( void );
void clearCalibrationBuffer( void );

int flushDataOnUsbDevice( void );

FILEIO_RESULT logFirmware( void );
FILEIO_RESULT logBatteryLevel( void );
FILEIO_RESULT logRfidFreq( void );
FILEIO_RESULT logDs3231Temp( void );
FILEIO_RESULT logCalibration( void );
FILEIO_RESULT logUdid(void);

#endif /* _APP_DATA_LOGGER_HEADER_H */


/*******************************************************************************
 End of File
 */
