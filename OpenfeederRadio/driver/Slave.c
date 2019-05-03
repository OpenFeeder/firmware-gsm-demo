/*
 * File:   Esclave.c
 * Author: mmadi
 *
 * Created on 16 mars 2019, 14:03
 */


#include <stdio.h>

#include "xc.h"
#include "Slave.h"


void slave_update_date(uint8_t* date) {
    struct heure_format hf;
    deserial_buffer(date, &hf);
    set_time(hf);
}


short slave_wait_event(int delais, Paquet *cmdRecu, int16_t idS) {  
    int size = 0;
    printf("Slave [%d] : J'attends une cmd...\n", idS);
    if ((size = srv_listen_rf(delais, cmdRecu, idS)) > 0) { 
        return 1;
    }
    // ici ==> qu'on est sur un time out 
    return size;
}


short slave_send_error(int numErr, short niveauErr, int16_t idS);


int8_t slave_send_log(const uint8_t*log[], int8_t * pointeurLog, int nbLines, int16_t idS) {
    uint8_t paquetEnvoi[TAILE_MAX_PAQUET];
    int8_t size = 0; 
    int8_t curseur = *pointeurLog; 
    int8_t w = 1; // on commence par une taille de fenetre = 1
    int8_t pw = 0;
    int delais = 300;
    printf("Slave : transmission des log\n");
    while (*pointeurLog < nbLines) { // pour l'instant on envoie 5 paquet
        if (pw < w && curseur < nbLines) { // juste pour les tests
            //construction du paquet
            uint16_t numPaquet = curseur+1;
            size = srv_create_paket_rf(paquetEnvoi, log[curseur++], 
                    srv_getIDM(), srv_getIDS1(), srv_data(), numPaquet);
            //attente 
            srv_wait(20);
            printf("Slave : envoie du paquet num %d\n", numPaquet);
            srv_send_rf(paquetEnvoi, size, delais, 1); // 0 veut dire 1 fois
            if (curseur == nbLines+1) curseur == nbLines;  // on changera ca bien sur par le nombre de lignes
            pw++;
        }else { 
            printf("Slave : attente des ack ==> go back n  ptr == %d\n", *pointeurLog);
            if (!srv_goBackN(log, pointeurLog, curseur, idS, srv_getIDM(), &w)) { // penser a changer ca 
                return 0;
            }
            pw = 0; // pour pouvoir recommencer 
        }
    }
    
    size = srv_create_paket_rf(paquetEnvoi, "FIN", 
                    srv_getIDM(), idS, srv_fin_trans(), curseur);
    //attente 
    srv_wait(20);
    printf("Slave : envoie du paquet de fin de transmission \n");
    srv_send_rf(paquetEnvoi, size, 10, 5); // 0 veut dire 1 fois
            
    return 1;
}

uint8_t slave_get_config(int16_t idS) {
    //TODO : la reception du fichier config depuis le slave
    return 1;
}

int8_t slave_state_machine(int16_t idS) {
    uint8_t *log[40];
    log[0] = "23/01/1908:19:06M4OF3730011016BF5B1000100000"; log[10] = "23/01/1908:19:06M4OF3730011016BF5B1000100000";
    log[1] = "23/01/1908:19:10M4OF3730??????????0000100000"; log[11] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[2] = "23/01/1908:19:33M4OF3730011016BF5B1000100000"; log[12] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[3] = "23/01/1908:19:35M4OF3730011016BF5B1000100000"; log[13] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[4] = "23/01/1908:21:56M4OF3730011016BF5B1000100000"; log[14] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[6] = "23/01/1908:19:06M4OF3730011016BF5B1000100000"; log[15] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[7] = "23/01/1908:19:10M4OF3730??????????0000100000"; log[16] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[8] = "23/01/1908:19:33M4OF3730011016BF5B1000100000"; log[17] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[9] = "23/01/1908:19:35M4OF3730011016BF5B1000100000"; log[18] = "23/01/1908:19:10M4OF3730??????????0000100000"; 
    log[5] = "23/01/1908:21:56M4OF3730011016BF5B1000100000"; log[19] = "23/01/1908:19:10M4OF3730??????????0000100000";
    
    log[20] = "23/01/1908:19:06M4OF3730011016BF5B1000100000"; log[30] = "23/01/1908:19:06M4OF3730011016BF5B1000100000";
    log[21] = "23/01/1908:19:10M4OF3730??????????0000100000"; log[31] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[22] = "23/01/1908:19:33M4OF3730011016BF5B1000100000"; log[32] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[23] = "23/01/1908:19:35M4OF3730011016BF5B1000100000"; log[33] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[24] = "23/01/1908:21:56M4OF3730011016BF5B1000100000"; log[34] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[26] = "23/01/1908:19:06M4OF3730011016BF5B1000100000"; log[35] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[27] = "23/01/1908:19:10M4OF3730??????????0000100000"; log[36] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[28] = "23/01/1908:19:33M4OF3730011016BF5B1000100000"; log[37] = "23/01/1908:19:10M4OF3730??????????0000100000";
    log[29] = "23/01/1908:19:35M4OF3730011016BF5B1000100000"; log[38] = "23/01/1908:19:10M4OF3730??????????0000100000"; 
    log[25] = "23/01/1908:21:56M4OF3730011016BF5B1000100000"; log[39] = "23/01/1908:19:10M4OF3730??????????0000100000";
    
    
    
    int nbLigne = 0;
    int ptr = 0; //c'est le pointeur des logs mise a jours 
    struct tm t;
    int8_t fin = 0; 
    Paquet paquetRecu;
    int delais = 200; // ==> x10 ms
    while(!fin){
        // ecoute le maitre 
        
        RTCC_TimeGet(&t); 
        printf("Debut ==> %d h:%dmin:%ds\n", t.tm_hour, t.tm_min, t.tm_sec);
        if(slave_wait_event(delais, &paquetRecu, idS)) {
            if (paquetRecu.typeDePaquet == srv_horloge()) {
                    slave_update_date(paquetRecu.data);
                    RTCC_TimeGet(&t); 
                    printf("esclave : date mise à jour : %d/%d/%d -- %dh:%hmin:%ds\n",
                            t.tm_yday,t.tm_mon,t.tm_year,t.tm_hour,t.tm_min,t.tm_sec);
            }else if (paquetRecu.typeDePaquet == srv_data()) {
                    printf("esclave : j'envoie les logs\n");
                    if (slave_send_log(log, &ptr, 40, idS)) {
                        printf("esclave : tous les paquet ont ete transmsis \n");
                    }else {
                        printf("esclave : je n'ai pu envoye que %d sur %d\n", ptr, nbLigne);
                    }
            }else if (paquetRecu.typeDePaquet == srv_config()) {
                    slave_get_config(idS);
            }
        }else {
            printf("esclave : timeout aucune commande n'est recu\n");
        }
        
    }
    return 1;
}