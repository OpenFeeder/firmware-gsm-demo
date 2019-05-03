/* 
 * File:   temps.h
 * Author: alex
 *
 * Created on 15 février 2019, 21:10
 */

#ifndef TEMPS_H
#define	TEMPS_H


#include "../driver/trx.h"

inline uint8_t conv_dec(uint8_t g, uint8_t d);

void conv_date_readed(uint8_t* buff, struct heure_format* res);

void serial_buffer(uint8_t* buff, struct heure_format hf);

void deserial_buffer(uint8_t* buff, struct heure_format* hf);

void read_clock_uart(struct heure_format* hf);

void set_time(struct heure_format hf);

void get_time(struct heure_format* hf);


#endif	/* TEMPS_H */

