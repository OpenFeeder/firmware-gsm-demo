/* 
 * File:   test.h
 * Author: alex
 *
 * Created on February 12, 2019, 3:33 PM
 */

#ifndef TEST_H
#define	TEST_H


#include "../driver/trx.h"

void test_state_register();

void test_clock_seal();

uint8_t test_serial_datetime(uint8_t* datetime);


#endif	/* TEST_H */

