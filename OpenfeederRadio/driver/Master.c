/*
 * File:   Maitre.c
 * Author: mmadi
 *
 * Created on 16 mars 2019, 15:56
 */


#include "xc.h"
#include "Master.h"
#include "tx.h"
#include "rx.h"

//permet de savoir quelle esclave doit nous transmettre les donnee et 
//quelle est notre situation precedente 
typedef struct sContextEsclave {
    int16_t idEsclave;
    int8_t ptrLog; //quelle numero de log on attend au cas ou reprise sur erreur 
    int8_t logRecup; // 0 si on a pas tout recupérer 1 si oui et 2 si  
    int8_t nbErr; // parametre a partir du quel on estime que le slave à un probleme
}ContextEsclave;

ContextEsclave ensEsclave[3];
// determine la fin de collection == 3 on aura tout recuperer
int8_t finCollect;
int8_t stopErrCon;

void maitre_resetNbErr() {
    ensEsclave[0].nbErr = 5;
    ensEsclave[1].nbErr = 5;
    ensEsclave[2].nbErr = 5;
    stopErrCon = 0;
}
void maitre_initS(int s, int i) {
    ensEsclave[i].idEsclave = s;
    ensEsclave[i].ptrLog = 1;
    ensEsclave[i].logRecup = 0;
    ensEsclave[i].nbErr = 5; // à 10 demande d'affiler sans resultat on estime 
                              // qu'il y'a une erreur avec cette openfeeder 
}

void maitre_init_context(int16_t s0, int16_t s1, int16_t s2) {
    maitre_initS(s0, 0);
    maitre_initS(s1, 1);
    maitre_initS(s2, 2);
    finCollect = 0;
    stopErrCon = 0;
}



int8_t mster_send_cmd_rf(uint8_t* cmd, int8_t tpCMD, uint16_t dest, int delais, int8_t nbFois) {
    uint8_t paquetCmd[TAILE_MAX_PAQUET];
    int8_t size_cmd = srv_create_paket_rf(paquetCmd, cmd, dest, srv_getIDM(), tpCMD, '0'); // pour l'instant 
    return srv_send_rf(paquetCmd, size_cmd, delais, nbFois);
}


int8_t mster_send_date(int delais, int8_t nbFois) {
    struct heure_format hf;
    uint8_t date[14]; // cas particulier 
    //récupération de l'heure 
    int i = 0;
    int8_t ret = 0;
    for (; i < nbFois; i++) { // on a une date mise a jour
        get_time(&hf);
        //creation de du format pour l'envoie  ensEsclave[0].logRecup = 0;
        serial_buffer(date, hf);
        uint8_t paquetDate[TAILE_MAX_PAQUET];
        int8_t size_h = srv_create_paket_rf(paquetDate, date, srv_getBroadcast(), srv_getIDM(), 
                srv_horloge(), '0');  
        ret = srv_send_rf(paquetDate, size_h, delais, 1);
    }
    return ret;
}


int8_t mster_send_config_rf(uint8_t *config[], int *ptrConfig, int nbLines, uint16_t escalve) {
    uint8_t paquetEnvoi[TAILE_MAX_PAQUET];
    int8_t size = 0; int16_t curseur = 0;
    //on envoie 2 par 2;
    int8_t w = 1;
    int8_t pw = 0;
    // si on apelle cette fonction ce qu'on estime qu'on est syncronis?
//    int8_t fin = 0;
    
    while (*ptrConfig < nbLines) {
        if (pw < w && curseur < nbLines) {
            //construction du paquet
            uint8_t numPaquet = curseur+1;
            size = srv_create_paket_rf(paquetEnvoi, config[curseur++], 
                    escalve, srv_getIDM(), srv_data(), numPaquet);
            //attente 
            srv_wait(20);
            printf("Maitre : envoie du paquet num %d\n", curseur);
            srv_send_rf(paquetEnvoi, size, 300, 1); // 0 veut dire 1 fois
            if (curseur == nbLines+1) curseur == nbLines;  // on changera ça bien sur par le nombre de lignes
            pw++;
        }else {
            printf("Maitre : attente des ack ==> go back n\n");
            if (!srv_goBackN(config, ptrConfig, curseur, srv_getIDM(), escalve, &w)) { // penser à changer ça 
                return 0;
            }
            pw = 0; // pour pouvoir recommencer 
        }
        
        size = srv_create_paket_rf(paquetEnvoi, "FIN", 
                    srv_getIDM(), srv_getIDS1(), srv_fin_trans(), curseur);
        //attente 
        srv_wait(20);
        printf("Slave : envoie du paquet de fin de transmission \n");
        srv_send_rf(paquetEnvoi, size, 40, 2); // 0 veut dire 1 fois
    }
    return 0;
}

void maitre_envoie_ack(int8_t s, int8_t paquetAttendu) {
    uint8_t paquetEnvoi[TAILE_MAX_PAQUET];
    int size = srv_create_paket_rf(paquetEnvoi, "NACK", ensEsclave[s].idEsclave, srv_getIDM(), 
                            srv_ack(), paquetAttendu);
    srv_send_rf(paquetEnvoi, size, 20, 2);
}

int8_t mster_get_log_rf(int8_t esclave) {
    int MAX_TIME_OUT = 10;
    int8_t MAX_ATTENTE = 5; // x fois le paquet de debut
    
    uint8_t SAVE_LOG[20][TAILE_MAX_PAQUET];
    
    Paquet paquetRecu;
    uint8_t paquetAttendu = ensEsclave[esclave].ptrLog;
    int8_t fin = 0; int8_t stopErr = 0;
    int8_t nbTimeOut = 0; int8_t atendConnect = 0;
    int delais = 20; // ==> x10ms
    while (!fin && !stopErr) {
        if (srv_listen_rf(delais, &paquetRecu, srv_getIDM()) > 0) {
            if (paquetRecu.typeDePaquet == srv_data()) {
                //data recu 
                atendConnect = 0;
                if (paquetRecu.numPaquet == paquetAttendu) {
                    nbTimeOut /= 2;
                    printf("Master : paquet attendu recu %d : %s\n", paquetAttendu, paquetRecu.data);
                    srv_cpy(SAVE_LOG[paquetAttendu-1], paquetRecu.data, TAILE_MAX_PAQUET);
                    paquetAttendu++;
                }else {
                    printf("Master : ce n'est pas le paquet attendu %d vs %d\n", paquetAttendu,
                            paquetRecu.numPaquet);
                    //traansmission de l'ack 
                    maitre_envoie_ack(esclave, paquetAttendu);
                }
            }else if (paquetRecu.typeDePaquet == srv_fin_trans()) {
                printf("Master : fin de reception de donnée %d\n", paquetRecu.numPaquet);
                fin = 1;
            }else {
                // ce n'est pas le type de paquet que j'attens 
                printf("Master : ce n'est pas le type de paquet que j'attends %d\n", paquetRecu.typeDePaquet);
                maitre_envoie_ack(esclave, paquetAttendu);
            }
            srv_dec_delais(&delais, 50, 5); //diminue de 50 ms et le seul de 500 ms
        }else { // timeout
            srv_inc_delais(&delais, 10); // j'augmente de 100 ms 
            printf("Master : timeOut j'attends tjr %d\n", paquetAttendu);
            nbTimeOut++;
            if (paquetAttendu == ensEsclave[esclave].ptrLog) // ce qu'on attend tjr le paquet de debut 
                atendConnect++;
            if (nbTimeOut >= MAX_TIME_OUT) fin = 1;
            else if (atendConnect >= MAX_ATTENTE) { 
                stopErr = 1;
                ensEsclave[esclave].nbErr--; // on a pas pu se connecter a ce slave 
                if (ensEsclave[esclave].nbErr == 0) {
                    printf("Maitre : une erreur sur le slave [%d]\n", ensEsclave[esclave].idEsclave);
                    stopErrCon++;
                }
            }else{
                maitre_envoie_ack(esclave, paquetAttendu);
            }
                
        }
    }
    
    if (nbTimeOut >= MAX_TIME_OUT || stopErr == 1) {
        ensEsclave[esclave].ptrLog = paquetAttendu;
        ensEsclave[esclave].logRecup = 2;
        return 0;
    }
    ensEsclave[esclave].logRecup = 1;
    finCollect++;
    int i; // debugg 
    for (i = 0; i < 20 ; i++) {
        printf("recu : %s \n", SAVE_LOG[i]);
    }
    return 1;
}


int8_t mster_get_log_for_all() {
    int8_t fin = 0;
    int8_t s = 0; 
    while (!fin) {
        //choisir un slave 
        if(finCollect+stopErrCon < 3) { // il reste encore au moins un salave qui n'a pas transmis 
            if(ensEsclave[s].logRecup != 1 && ensEsclave[s].nbErr > 0) {
                printf("Maitre : choix du slave [%d]\n", ensEsclave[s].idEsclave);
                mster_send_cmd_rf("DATA", srv_data(), ensEsclave[s].idEsclave, 9, 5);
                srv_wait(10); //ensEsclave[s].nbErr optiennelle ==> avoir
                mster_get_log_rf(s);
            }
            s = (s+1)%3;
        }else{
            fin = 1;
        }
    }
    if (finCollect == 3) {
        printf("Maitre : tous les log on etait recup\n");
        return 1; // pour l'instant 
    }else {
        printf("Maitre : tous les log on etait recup\n");
        return 0;
    }
}



int8_t mster_lesten_slave(int delais, Paquet *errRecu) {
    return srv_listen_rf(delais, errRecu, srv_getIDM());
}

void mster_send_error_ack(int delais, int8_t nbFois, int16_t slave) {
    uint8_t ack[TAILE_MAX_PAQUET];
    int8_t size = srv_create_paket_rf(ack, "ACK", slave, srv_getIDM(), srv_ack(), '0');
    srv_send_rf(ack, size, delais, nbFois);
}

int8_t mster_trait_error(Paquet *errRecu) {
    mster_send_error_ack(5, 5, errRecu->pSrc); // on reviendra changer si c'est trop rapide 
    //transmission de l'erreur aux serveur 
    printf("Master : transmettre l'erreur au serveur\n");
    //TODO
    return 1;
}

int8_t mster_state_machine_day() {
//    Paquet errRecu;
//    int delais = 300;
//    printf("Master : on transmet l'horloge et on attend les erreures possible\n");
//        // machine a etat pour la journee 
//    if (getSyncDate()){
//        printf("Master : mise a jour de l'horloge\n");
//        LED_Toggle();
//        reset_sync_date();
//        //mettre a jour mon horloge en recuperant l'horloge depuis le GSM
//        //et apres transmettre mon horloge aux autres OFs
//        mster_send_date(5, 5); // arbitraire 
//    }else { // on ecoute le slave 
//        if(mster_lesten_slave(delais, &errRecu)) {
//            printf("Master : erreur est recu de [%d]\n", errRecu.pSrc);
//            // traitement de l'erreur avec ack au slave qui vien d'envoyer l'erreur 
//            mster_trait_error(&errRecu);
//        }
//    }
    return 1;
}

int8_t mster_state_machine() {
    if (srv_get_moment() == 0) { //avant le reveille des oiseaux,
        printf("Master : initialisation du system de comunication \n");
    }else if (srv_get_moment() == 1) { //debut de la journee
        return mster_state_machine_day();
    }else if (srv_get_moment() == 2) { //le soir
        printf("Master : on est le soir, recuperation des logs \n");
        return mster_get_log_for_all();
    }
    // pout l'instant on retourne 1
    return 1;
}

