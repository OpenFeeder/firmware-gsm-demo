/**
 * @file servomotor.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 * @revision history 1
 */

#ifndef _SERVOMOTOR_HEADER_H
#define	_SERVOMOTOR_HEADER_H

/* Change servomotor position, Ton time (in us) corresponds to an angular angle.
 * Absolute range: 500 us < servo_position < 2500 us
 * Ton value depends on the actuator type.
 * HS-322HD: 600 us < servo_position < 2400 us, flexible nylon noise --> Ok
 * PARRALAX: 600 us < servo_position < 2400 us (Product ID: 900-00005), sound gear 
 */
#define SERVO_DEFAULT_MIN_POSITION      600     /* min:  600 us */
#define SERVO_DEFAULT_MAX_POSITION      2400    /* max: 2400 us */
#define SERVO_DEFAULT_START_POSITION    1500    /* initial servomotor position at the middle */
#define SERVO_DEFAULT_SPEED_INC         10      /* servomotor position increment every 20 ms */

#define MAX_NUM_DOOR_REOPEN_ATTEMPT 2

typedef enum
{
    DOOR_IDLE, /* Not in use. */
    DOOR_OPENED,
    DOOR_CLOSED,
    DOOR_CLOSED_AT_NIGHT,
    DOOR_MOVED,
    DOOR_MOVING

} DOOR_STATUS;

typedef enum
{
    CLOSE_DOOR,
    OPEN_DOOR

} MOVEMENT_DIRECTION;

typedef struct
{
    uint16_t open_delay;
    uint16_t close_delay;

    struct tm open_time;
    struct tm close_time;

    uint8_t remain_open;

    DOOR_STATUS reward_door_status;
    
    uint8_t reward_probability;
    
    uint8_t habituation_percent;
    
    uint8_t num_reopen_attempt;
    
    uint16_t max_pos_offset;
    
    bool door_close_bird_sleep;
} APP_DATA_DOOR;

/* Servomotor components */
typedef struct
{
    bool cmd_vcc_servo_state;
    uint16_t ton_cmd; /* command servomotor position */
    uint16_t ton_min; /* minimum servomotor position */
    uint16_t ton_max; /* maximum servomotor position */
    uint16_t ton_goal; /* maximum servomotor position */
    uint16_t ton_min_night; /* minimum servomotor position to ensure door close at night */
    uint8_t speed; /* servomotor speed increment to move from position A to B(ex: every 20 ms) */
    uint8_t opening_speed;
    uint8_t closing_speed;
    uint16_t measure_position; /* measured servomotor position */

    uint8_t num_step;
    uint8_t num_empty_step;
    
    double reward_probability;
        
} APP_DATA_SERVO;

// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************

void SERVO_Initialize( void );
bool servomotorMoveTheDoor(void);
void servomotorPowerEnable( void );
void servomotorPowerDisable( void );
uint16_t servomotorGetDoorPosition( void );
void testServomotor( void );

#endif	/* _SERVOMOTOR_HEADER_H */


/*******************************************************************************
 End of File
 */
