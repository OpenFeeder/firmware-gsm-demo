/*
 * File:   Services.c
 * Author: mmadi
 *
 * Created on 16 mars 2019, 19:03
 */


#include "xc.h"
#include "Services.h"

#define MAX_T_OUT 500 // 5 secondes
#define MIN_T_OUT 5 // 50 ms
const int8_t W_MAX = 20;
// pour les teste je le change manuellement mais apres ce sera automatique
int8_t moment; // 0 avant le reveille des oiseaux, 1 la journee, 2 le soir

uint8_t srv_err( ) { return 1; }
uint8_t srv_data() { return 2; }
uint8_t srv_ack() { return 3; }
uint8_t srv_horloge() { return 4; }
uint8_t srv_cmd() { return 5; }
uint8_t srv_fin_trans() { return 6; }
uint8_t srv_config() { return 8; }
//notre identifiant @ 
uint16_t  srv_getIDS1() { return 37;}
uint16_t  srv_getIDS2() { return 36;}
uint16_t  srv_getIDS3() { return 35;}
uint16_t  srv_getIDM() { return 34;}
uint16_t  service_getSRC() { return 37;}
uint16_t  srv_getBroadcast() { return 1023; }

srv_get_moment() { 
    srv_update_moment(); // met à jour le momet
    return moment;
}

void srv_update_moment() {
    struct heure_format hf;
    get_time(hf);
    if (hf.h >= 5 && hf.h < 6) // on est au demarage des OFs 
        moment = 0;
    else if (hf.h >= 6 && hf.h < 18) // on est dans la journee
        moment = 1;
    else // on est le soir
        moment = 2;
}

//dans la fenetre 
int8_t srv_in_windows(unsigned int inf, unsigned int pointeur, int taille) {

    unsigned int sup = (inf+taille-1) % 256; // pour l'instant juste pour les tests 

    return
    /* inf <= pointeur <= sup */
    ( inf <= sup && pointeur >= inf && pointeur <= sup ) ||
    /* sup < inf <= pointeur */
    ( sup < inf && pointeur >= inf) ||
    /* pointeur <= sup < inf */
    ( sup < inf && pointeur <= sup);
}

//on calcule la somme de controle
uint8_t srv_checksum(uint8_t* paquet, int size) {
    uint8_t somme = 0;
    int i;
    for (i = 0; i < size; ++i) {
        somme ^= paquet[i];
    }
    return somme;
}

int8_t srv_test_cheksum(uint8_t* paquet, int size, uint8_t somme_ctrl) {
    return (srv_checksum(paquet, size)==somme_ctrl);
}

int srv_len(const uint8_t *chaine) {
    int i = 0;
    while (chaine[i] != '\0') {
        i++;
    }
    return i;
}

int8_t service_cmp(const uint8_t *ch1, const uint8_t *ch2) {
    int8_t i = 0;
    int8_t ok = 1;
    do {
        if (ch1[i] != ch2[i]) return 0;
        else if (ch1[i] != '\0' || ch2[i] != '\0') ok = 0;
        i++;
    }while(ok);
    return 1;
}

int8_t srv_cpy(uint8_t *dest, uint8_t *src, int size) {
    int i = 0;
    int t = srv_len(src);
    if (size < t)
        return 0;
    while (i < t) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return 1;
}

void srv_wait(int delais) {
    int i = 0;
    TMR1_Counter16BitSet(0);
    while (i < delais) {
        if (TMR1_Counter16BitGet() == TMR1_Period16BitGet()) {
            TMR1_Counter16BitSet(0);
            i++;
        }
    }
}


/**
 * on lui passe un paquet et le transmet 
 * cette fonction est modifiee
 * 
 * @param paquet
 * @param size
 * @param delais
 * @return 
 */
int8_t srv_send_rf(uint8_t* paquet, int size, int delais, int nbFois) {
    //clear_reg(); // on met tout par defaut d'abord ==> il faut voir si on a besoin de ?a 
    configure_tx(); // a voir 
    int j = 0; 
    int ecr = 0;
    do {
        ecr = send(paquet, size, delais);
        srv_wait(delais);
        j++;
    }while (j < nbFois);
    return ecr;
}

int8_t srv_receive_rf(uint8_t *paquet, int size, int delais) {
    //je me mets en mode reception 
    //configure_rx(); // configuration en mode reception 
    return receive(paquet, size, delais);
}

int8_t srv_create_paket_rf(uint8_t paquet[], uint8_t data[], 
        uint16_t dest, uint16_t src, uint8_t typeDePaquet, uint8_t numPaquet) {
    uint8_t s = 10; // permet de diviser en octet l'entier sur 16 bit 
    int i;
    int j = 0;
     // dest, c'est pour le separer octet par octet 
   uint8_t d = dest/s;
   paquet[j++] = d;
   d = dest%s;
   paquet[j++] = d;
   //type de paquet cod? sur un oct?
   paquet[j++] = typeDePaquet;
   // numero du paquet codï¿½ sur 2 octets
   paquet[j++] = numPaquet;
   //src
   paquet[j++] = src/s;
   paquet[j++] = src%s;
   
   paquet[j++] = 'A'; // non utilis?
   paquet[j++] = 'A'; // non utilis?
    for (i = 0; i < srv_len(data); i++) { //donn?es
        paquet[j++] = data[i];
    }
    i = j;
    uint8_t sum_ctl = srv_checksum(paquet, i);
    paquet[j++] = sum_ctl;
    paquet[j] = '\0'; // fin de chaine 
//    printf("paquet == %d\n", paquet[0]);
    return j;// j = taille du taquet g?n?r? 
}


void extractInfos(uint8_t* paquet, int *j, uint8_t* out, int count) {
    int i;
    for (i = 0; i < count; i++) { // 4 car l'adress est cod?e sur 4oct
        out[i] = paquet[*j];
        *j = *j+1;
    }
    out[i] = '\0';
}

int8_t srv_decode_packet_rf(uint8_t* paquet, Paquet *pPaquetRecu, int size, 
        uint16_t idOF) {
    uint8_t s = 10; // permet de concatener les adresses 
    int j = 0;
    //on recup?re id dest pour verifier si c'est egale au notre==> c'est le slave 1
    pPaquetRecu->pDest = paquet[j]*s+paquet[j+1];
    j+=2;
    //teste si le paquet m'est destin?
    if (pPaquetRecu->pDest != idOF && pPaquetRecu->pDest != srv_getBroadcast()) // ?a veut dire pas les meme 
        return 0; // le paquet n'est pas ? moi
    
    
    //la premi?re chose qu'on recup?re c'est le crc
    uint8_t sum_ctl = paquet[size-1];
    if (srv_test_cheksum(paquet, size-1, sum_ctl) == 0){ 
        return 0; // on retourn 0 dans ce cas precis 
    }
    
    //on recup?re le type de paquet
    pPaquetRecu->typeDePaquet = paquet[j++];
    
    pPaquetRecu->numPaquet = paquet[j++]; // on recup?re le numero de paquet
    
    //on recup?re le src
    pPaquetRecu->pSrc = paquet[j]*s+paquet[j+1];
    j+=2;
    //on traitera ce cas plustard
    extractInfos(paquet, &j, pPaquetRecu->nonUtiliser, 2);
    //j = j + 3; //on saute les 2 oct non utiliser
    //data
    extractInfos(paquet, &j, pPaquetRecu->data, size-j-1); //le -1 c'est pour ne pas recup?rer le crc
    
    return j;
}

int8_t srv_listen_rf(int delais, Paquet *paquetRecu, uint16_t idOf) {
    uint8_t recu[TAILE_MAX_PAQUET];
    //ecoute d'un paquet // on attend une commende du Master
    int size;
    if ((size = srv_receive_rf(recu, TAILE_MAX_PAQUET, delais)) > 0) {
        //un paquet est re?u on le debale 
        return srv_decode_packet_rf(recu, paquetRecu, size, idOf);
    }
    return 0;
}

void srv_inc_delais(int *delais, int ms) {  //ms = x10 ms
    if (*delais < MAX_T_OUT) {
       *delais = *delais+ms; //on augmente de 100 ms au timeout
       printf("j'augmente le delais %d\n", *delais);
    }
}

void srv_dec_delais(int *delais, int seuil, int ms) {  //ms = x10 ms
    if (*delais > seuil) {
       *delais = *delais-ms; //on augmente de 100 ms au timeout
       printf("je diminue le delais %d\n", *delais);
    }
}

void srv_increase(int *w) {
    if (*w < W_MAX)
        *w = *w+1;
}

void srv_decrease(int *w) {
    if (*w > 1) //on divise par 2 la fenêtre de retransmission 
        *w = *w/2;
    printf("j'augmente le w %d\n", *w);
}

int8_t srv_goBackN(const uint8_t** data, int * offset, int curseur,
        uint16_t idOfSrc, uint16_t idOfDest, int8_t *w) {
    int nbTransmission = 0;
    int delais = 300; // => x10ms
    Paquet paquetRecu;
    int8_t retrasmission = 0;
    uint8_t paquetEnvoi[TAILE_MAX_PAQUET];
    while (*offset != curseur && nbTransmission < 10) { // 10 c'est provisoire
        if (srv_listen_rf(delais, &paquetRecu, idOfSrc) > 0) { // > 0 ==> un paquet est re?u 
            if (paquetRecu.typeDePaquet == srv_ack()){ // c'est un ack qui vient d'arriver
                //faire un test si l'ack est dans la fenetre ici important
                if (paquetRecu.numPaquet-1 >= *offset && paquetRecu.numPaquet-1 < 256) {
                    *offset = paquetRecu.numPaquet-1;
                    printf("[%d] : ack dans la fenetre %d %s\n", idOfSrc, *offset, paquetRecu.data); 
                    retrasmission = 1;
                    printf("[%d] offset %d vs %d cursseur\n", idOfSrc, *offset, curseur);
                    if (*offset == curseur) {
                        retrasmission = 0;
                        printf("[%d] tous les paquets attendus sont recuperes %d\n", idOfSrc, curseur);
                        srv_increase(w);
                    }else {
                        srv_decrease(w);
                    }
                }else {
                    retrasmission = 1;
                    printf("[%d] ack en dehors de la fenetre   %d \n", idOfSrc, paquetRecu.numPaquet);
                }
            }else {
                srv_inc_delais(&delais, 5);
                retrasmission = 1; // pour l'instant on fais ca ici
                printf("[%d] : ce n'est pas un ack %d\n", idOfSrc, paquetRecu.numPaquet);
            }
        }else {
            printf("[%d] timeOut \n", idOfSrc);
            srv_decrease(w);
            retrasmission = 1; // timeout ou ce n'est pas un paquet qui m'est destin?
            srv_inc_delais(&delais, 10);
        }
        if (retrasmission) {
            int i = *offset; 
            int8_t pw = 0;
            int size = 0;
            printf("[%d] retransmission depuis inf %d %d %d\n", idOfSrc, i, *offset, *w);
            while (i != curseur && pw < *w) {
                printf("[%d] : retransmission num [%d]\n",idOfSrc, i);
                size = srv_create_paket_rf(paquetEnvoi, data[i], 
                    idOfDest, idOfSrc, srv_data(), i+1); 
                srv_wait(20); //==> x10ms on attend un petit delais  
                srv_send_rf(paquetEnvoi, size, 100, 1); // 0 veut dire 1 fois
                i++;
                pw++;
            }
            nbTransmission++;
        }
    }
    if(nbTransmission < 10)
       return 1;
    return 0;        
}

uint8_t **service_recup_data_sur_disque(int *nbligne) {
    *nbligne = 40; 
    return NULL;
}
