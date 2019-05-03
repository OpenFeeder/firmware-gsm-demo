/* 
 * File:   driver.h
 * Author: alex , (modifier par anzilane)
 *
 * Created on February 12, 2019, 4:02 PM
 */

#ifndef TRX_H
#define	TRX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <xc.h>
#include <time.h>

#include "../mcc_generated_files/system.h"
#include "./trx.h"
#include "./types.h"

// Lecture du SR
#define COM_READ_SR 0x0000

#define PITCH_TRANS 1000

#define SLEEP_AFTER_INIT 850000

#define BUFF_SIZE 7

//structure pour le temps
//extern struct tm time_pic;

void init_trx();

uint16_t ecrire_reg(uint16_t commande);

/**
 * 
 * @param delais : le nombre de fois qu'il faut attendre le nIRQ en cas de non reponse
 * @return : 1 on a pas depassé le delais : 0 sinon  
 */
int rdy(short delais);


#endif	/* DRIVER_H */

