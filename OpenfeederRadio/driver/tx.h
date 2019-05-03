/* 
 * File:   tx.h
 * Author: alex
 *
 * Created on 15 f�vrier 2019, 23:05
 */

#ifndef TX_H
#define	TX_H

#include "./trx.h"

/**
 * configure le module en mode emmision (TX)
 * elle fait appelle � la fonction ecrire_reg
 * en lui passant la commande pour l'ecriture 
 * du registre  
 */
void configure_tx();

/**
 * active l'envoie de donn�r : on ouvre le canal pour commencer la transmission
 */
int open_tx(int delais);

/**
 * stop l'envoie de donn�
 */
void close_tx();

/**
 * permet de mettre le registre en etat par defaut 
 * cette fonction sera utiliser pour permettre le 
 * changement de mode (tx ou rx)
 */
void clear_tx();

/**
 * prend une chaine de caractere et la transmet
 * 
 * @param buffer : la chaine de caractere � transmettre 
 * @param size : la taille de la chaine � transmettre 
 * 
 */
int send(uint8_t* buffer,int size, int delais);

/**
 * dans cette fonction on decoupe la chaine re�u en un ensemblde d'octe 
 * � transmetre car la transmission se fait octe par octe 
 * @param bytes : la chaine de caractere � transmettre en octe
 * @param size : la taille de la chaine � transmettre 
 */
int send_data(uint8_t* bytes, int size, int delais);

/**
 * envoie d'un octe 
 * @param byte : octe � transmettre 
 */
int send_byte(uint8_t byte, int delais);


#endif	/* TX_H */

