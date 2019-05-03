/* 
 * File:   rx.h
 * Author: alex
 *
 * Created on 15 février 2019, 23:05
 */

#ifndef RX_H
#define	RX_H

#include "../driver/trx.h"

/**
 * ouvre (de la communication) de la reception d'un msg
 */
void open_rx();

/**
 * configuration du registre pour permettre la reception
 */
void configure_rx();

/**
 * fermeture de la reception
 */
void close_rx();

/**
 * permet de mettre le registre en etat par defaut 
 * cette fonction sera utiliser pour permettre le 
 * changement de mode (tx ou rx)
 */
void clear_rx();

/**
 * @param buffer : stocke l'information lu
 * @param size : on lit par bloc
 * @param delais : le nombre d'attente toléré pour une lecture d'info
 * @return le nombre d'octe effectivement lu 
 */
int receive(uint8_t* buffer, int size, int delais);

void wait_clock(int* compteur);

/**
 * 
 * @param buffer : stocke l'information lu
 * @param size : on lit par bloc
 * @param delais : le nombre d'attente toléré pour une lecture d'info
 * @return le nombre d'octe effectivement lu
 */
int read_data(uint8_t* buffer, int size, int delais);


#endif	/* RX_H */

