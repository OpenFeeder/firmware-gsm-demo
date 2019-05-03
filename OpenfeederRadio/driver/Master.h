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
 * File: Maitre.h
 * Author: Anzilane Mmadi
 * Comments: decrit le comportement du maitre 
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MASTER_H
#define	MASTER_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "Services.h"

/**
 * prend une commande est l'envoie en nbFois exemplaire avec un interval de x10ms 
 * on peut reuire cet interval bien sur 
 * 
 * @param cmd : la commande a evoyer
 * @param tpCMD : le type de commande 
 * @param dest : a qui est destin?e la commande 
 * @param delais : le temps d'attente entre deux transmission de deux meme cmd
 * @param nbFois : nombre de redondance d'envoie en nbFois
 * @return : nb octet effectivement envoyer 0 sinon
 */
int8_t mster_send_cmd_rf(uint8_t* cmd, int8_t tpCMD, uint16_t dest, int delais, int8_t nbFois);

/**
 * envoie de la date a tout le monde tous les 10 minuite 
 * @param delais : le temps de transmission de l'heure
 * @param nbFois : le nombre de fois qu'il faut la transmetre
 * @return 0 si echec de transmission > 0 sinon 
 */
int8_t mster_send_date(int delais, int8_t nbFois);

/**
 * debute la collecte des donn?es d'un slave 
 * 
 * @param slave : l'indice de l'esclave
 * @return 1 si tout le log est recu entierement, 0 si interuption du a une erreur exemple 
 */
int8_t mster_get_log_rf(int8_t slave);

/**
 * permet d'envoyer fichier config a un slave 
 * 
 * @param config : le fichier ï¿½ envoyï¿½ 
 * @param ptrConfig : le pointeur ï¿½ partir du quel on commence l'envoie
 * @param nbLines : le nombre de lignes ï¿½ transmettre 
 * @param escalve : l'of ï¿½ qui l'on souhaite transmettre le fichier 
 * @return 0 si echec 1 sinon 
 */
int8_t mster_send_config_rf(uint8_t *config[], int *ptrConfig, int nbLines, uint16_t escalve);

/**
 * machine a etat du maitre, on qui cette machine par une interuption 
 * non implemente pour le moment 
 * 
 * @return 0 si un probleme avec la machine 1 si tout c'est bien passer 
 */
int8_t mster_state_machine();

/**
 * demare la collecte des log de tous les slave 
 * 
 * @return 1 si tout est recupere 0 sinon 
 */
int8_t mster_get_log_for_all();

/**
 * traitement de la journee
 * 
 * @return 0 si traitement echec 1 si non 
 */
int8_t mster_state_machine_day();

/**
 * ecoute le slave au cas ou il y'a une erreure
 * 
 * @param delais : timeout
 * @param cmdRecu : contendra l'erreur reçu
 * @return 0 si timeout 1 si erreur recu 
 */
int8_t mster_lesten_slave(int delais, Paquet *errRecu);

/**
 * transmet l'ack a l'esclave et envoie l'erreur au server 
 * 
 * @param delais : delais d'interval avant retransmission du meme paquet
 * @param nbFois : nb retransmission 
 * @param slave : dest
 */
void mster_send_error_ack(int delais, int8_t nbFois, int16_t slave);


/**
 * traite l'erreur recu
 *  
 * @param errRecu : les information de l'erreur recu 
 * @return 1 si traitement ok 0 sinon 
 */
int8_t mster_trait_error(Paquet *errRecu);

#endif	/* MASTER_H */

