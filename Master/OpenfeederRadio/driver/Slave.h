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
 * File:   Esclave.h 
 * Author: Anzilane
 * Comments: ensemble des fonctionnalité d'un escalve 
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SLAVE_H
#define	SLAVE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#include "Services.h"
/**
 * cette procedure permet de mettre a jour 
 *  
 * @param date : la date ? utiliser pour la mise a jour
 */
void slave_update_date(uint8_t* date);

/**
 * permet l'envoie de l'erreur au maitre
 * 
 * @param numErr : le numero de l'erreur a transmetre 
 * @param niveauErr : le niveau de gravite
 * @param idS : l'identifiant de l'esclave 
 * @return : 1 si l'erreur est transmise : 0 sinon 
 */
short slave_send_error(int numErr, short niveauErr, int16_t idS);

/**
 * demare la transmission du fichier log au maitre
 * 
 * @param log : ici c'est un tabelau de chaine de char ==> char = une ligne du log
 * @param pointeurLog : nous inddique a partir de quelle indice on commence la transmission
 * @param nbLines : nombre de lignes a transmetre 
 * @return : nombre de lignes transmises
 */
int8_t slave_send_log(const uint8_t* log[], int8_t * pointeurLog, int nbLines, int16_t idS);

/**
 * reception du fichier permetant le changement de comportement de l'OF slave
 * @param idS : l'identifiant de l'esclave 
 * @return : 1 la config est bien recu et sauvgardee 0 il y'a eu un probleme a la reception
 */
uint8_t slave_get_config(int16_t idS);


/**
 * attend un evenement : soit timeout soit reception d'une commande du master
 * ou une erreur est survenue  
 * 
 * @param delais : le temps pour attendre une commande 
 * @param cmdRecu : la commande recu, si recu 
 * @param idS : l'identifiant de l'esclave 
 * @return : > 0 une commande est recu 0 sinon 
 */
short slave_wait_event(int delais, Paquet *cmdRecu, int16_t idS);

/**
 * represente les differentes etats d'un esclave lors de la communication
 * 
 * @param idS : l'identifiant de l'esclave 
 * @return O si probleme de la machine a zetat 1 si non 
 */
int8_t slave_state_machine(int16_t idS);

#endif	/* SLAVE_H */

