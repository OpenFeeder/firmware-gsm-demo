/*
 * File:   pca9622.c
 * Author: Arnauld Biganzoli
 * Created 10/02/2017
 */

#include "app_master.h"
#include "pca9622.h"

I2C1_MESSAGE_STATUS I2C1_MasterWritePCA9622( const uint16_t addr7bits,
                                             uint8_t *p_data,
                                             const uint8_t data_len )
{
    I2C1_MESSAGE_STATUS status;

    I2C1_MasterWrite( p_data, data_len, addr7bits, &status );

    // wait for the message to be sent or status has changed.
    while ( I2C1_MESSAGE_PENDING == status )
    {
        Nop( ); // without pull-up resistor program will be blocked here
    }

    return status;
}

bool I2C1_PCA9622_SoftwareReset( void )
{
    I2C1_MESSAGE_STATUS i2c_status = I2C1_MESSAGE_COMPLETE; // the status of write data on I2C bus
    uint8_t write_buffer[] = { SWRST_DATA_1, SWRST_DATA_2 }; // data to transmit

    I2C1_MasterWrite( write_buffer, 2, PCA9622_SWRST_ADR, &i2c_status );

    // wait for the message to be sent or status has changed.
    while ( I2C1_MESSAGE_PENDING == i2c_status )
    {
        Nop( ); // without pull-up resistor program will be blocked here
    }

    return I2C1_MESSAGE_COMPLETE == i2c_status;
}



/*******************************************************************************
 End of File
 */
