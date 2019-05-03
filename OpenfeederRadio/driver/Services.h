/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SERVICE_H
#define	SERVICE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdlib.h>
#include "rx.h"
#include "tx.h"
//taille max d'un taquet 
#define TAILE_MAX_PAQUET 150 // ? voir avec les collegue
#define SIZE_DATE 100

//sauvdarde des info
typedef struct sPaquet {
    uint8_t numPaquet;
    uint16_t pDest;
    uint16_t pSrc;
    int8_t typeDePaquet;
    uint8_t nonUtiliser [3];
    uint8_t data[SIZE_DATE];
}Paquet;


// type de paquet
uint8_t srv_err();
uint8_t srv_data();
uint8_t srv_ack();
uint8_t srv_horloge();
uint8_t srv_cmd();
uint8_t srv_fin_trans();
uint8_t srv_fin_block();
uint8_t srv_config();

/**
 * les identifiants de of 
 * @return 
 */
uint16_t  srv_getIDS1();
uint16_t  srv_getIDS2();
uint16_t  srv_getIDS3();
uint16_t  srv_getBroadcast();
uint16_t  srv_getIDM();

/**
 * determine a quel moment de la journee on est 
 * 
 */
void srv_update_moment();

/**
 * @return le momeent de la journee 
 */
srv_get_moment();

/**
 * dis si l'ack re?u est dans la fenetre 
 * @param inf
 * @param pointeur
 * @param size 
 * @return 
 */
int8_t srv_in_windows(unsigned int inf, unsigned int pointeur, int size);

/**
 * 
 * @param paquet
 * @param size
 * @return 
 */
uint8_t srv_checksum(uint8_t* paquet, int size);

/**
 * 
 * @param paquet
 * @param size
 * @param somme_ctrl
 * @return 
 */
int8_t srv_test_cheksum(uint8_t* paquet, int size, uint8_t somme_ctrl);

/**
 * 
 * @param chaine
 * @return : la taille de la chaine paissee en parametre 
 */
int srv_len(const uint8_t *chaine);

/**
 * compare deux chaines passees en parametre
 *  
 * @param ch1 : chaine de caractere 1
 * @param ch2 : chaine de caractere 2
 * @return 1 si ch1 == ch2, 0 sinon
 */
int8_t srv_cmp(const uint8_t *ch1, const uint8_t *ch2);

/**
 * copy d'une chaine source a une chaine dest
 * @param dest : ou on copie 
 * @param src : ce qu'on copie
 * @param size : la taille de la chaie 
 * @return : 1 ok : 0 ko
 */
int8_t srv_cpy(uint8_t *dest, uint8_t *src, int size);

/**
 * permet l'envoie d'un paquet donnee : cmd, ack, ligne log, err, ...
 * 
 * @param paquet : le paquet a transmettre 
 * @param size : la taille 
 * @param delais : l'intervalle de temps qu'il faut avant de renvoyer le paquet ==> x10ms
 * @param nbFois : le nombre de paquet identique a envoyer : 0=1x, 1=2x, 2=3x ... etc
 * @return : la taille reelement ecris sinon 0 sinon
 */
int8_t srv_send_rf(uint8_t* paquet, int size, int delais, int nbFois);

/**
 * petmet la reception d'un paquet : cmd, err, ligne log, ack, ...
 * @param paquet : contiendera le paquet recu si valide 
 * @param size : la taille maximal d'un bloc
 * @param delais : le temps d'attente avant de declarer un timeout 
 * @return 0 si rien n'est recu, > 0 si quelque chose est recu
 */
int8_t srv_receive_rf(uint8_t* paquet, int size, int delais);

/**
 * genere un paquet a partir des infos fournis
 * 
 * @param paquet : conteneur du paquet cree
 * @param donnee : la donnee a encapsuler 
 * @param src : qui l'envoie ==> 
 * @param dest : a qui on l'envoie ==> 
 * @param typePaquet : le type de paquet a transmettre (err, ack, data, horloge ...)
 * @param numPaquet : numero du paquet 
 * @return la taille totale du paquet creee
 */
int8_t srv_create_paket_rf(uint8_t paquet[], uint8_t data[], 
        uint16_t dest, uint16_t src, uint8_t typeDePaquet, uint8_t numPaquet);

/**
 * prends un paquet et de le decoder
 * 
 * @param paquet : le paquet a deballer
 * @param size : la taille du paquet
 * @param pPaquetRecu : contient les element qui compose le paquet 
 * @param idOF : l'identifiant de l'of qui vient de recevoir le paquet
 * @return la taille du paquet c'est un bon paquet, 0 si non 
 */
int8_t srv_decode_packet_rf(uint8_t* paquet, Paquet *pPaquetRecu, int size, 
        uint16_t idOF);

/**
 * attend l'arriver d'un evenement, paquet recu ou timeout 
 * 
 * @param delais : delais d'coute en seconde (pour l'instant l'instant 1 periode dure 1 seconde)
 * @param paquetRecu : la ou on sauvgarde le paquet recu si tel est le cas 
 * @param idOF : l'identifiant de l'of qui vient de recevoir le paquet
 * @return 1 si paquet recu 0 sinon 
 */
int8_t srv_listen_rf(int delais, Paquet *paquetRecu, uint16_t idOf);

/**
 * permet d'attends un temps donnee
 * 
 * @param delais : le temps d'attente en x10ms
 */
void srv_wait(int delais);

/**
 * permet d'implementer la methode du goBackN
 * 
 * @param data : la data qu'il faut envoyer les acks ne sont pas re�u 
 * @param offset : a partir du quel on doit debuter la retransmission 
 * @param curseur : cursseur la borne superieure 
 * @param idOfSrc : l'of qui recois le paquet 
 * @param idOfDest: l'of a qui on envoie le paquet 
 * @param taille de la fenetre 
 * @return 
 */
int8_t srv_goBackN(const uint8_t** data, int * offset, int curseur, 
        uint16_t idOfsrc, uint16_t idOfDest, int8_t *w);

/**
 * decrementer le delais de quelque x10ms
 * 
 * @param delais : le delais a modifier
 * @param seuil : le delais min a ne pas depasser
 * @param ms : la reduction a apporter x10 ms
 */
void srv_dec_delais(int *delais, int seuil, int ms);

/**
 * incremente le delais de quelque ms
 * @param delais : le delais a modifier
 * @param ms : l'ogmentation a apporter x10 ms
 */
void srv_inc_delais(int *delais, int ms);

/**
 * 
 * @param nbligne : le nombre de ligne qu'on recup�re sur le fichier 
 * @return : les log recup�rer depuis le fichier de sauvgarder 
 */
uint8_t **service_recup_data_sur_disque(int *nbligne);
#endif	/* SERVICE_H */

