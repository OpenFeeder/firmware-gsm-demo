/*
 * File:   Services.c
 * Author: mmadi
 *
 * Created on 16 mars 2019, 19:03
 */


#include "xc.h"
#include "Services.h"
#include "led_status.h"
#include <string.h>

/******************************************************************************/
/******************************************************************************/
/****************** FONCTIONNALITEES COMMUNES AU ALPHA TRX ********************/
/***************************                ***********************************/

/*****************                                 ****************************/

uint8_t srv_err() { return 1; }

uint8_t srv_data() { return 2; }

uint8_t srv_ack() { return 3; }

uint8_t srv_horloge() { return 4; }

uint8_t srv_infos() { return 5; }

uint8_t srv_end_trans() { return 6; }

uint8_t srv_end_block() { return 7; }

uint8_t srv_config() { return 8; }

uint8_t srv_nothing() { return 9; }
//notre identifiant @ 

uint16_t srv_getID_Slave() { return 37; } // a modifier en fonction du master 

uint16_t srv_getID_Master() { return 34; }

uint16_t service_getSRC() { return 37; }

uint16_t srv_getBroadcast() { return 1023; }


//dans la fenetre 

int8_t srv_in_windows(unsigned int inf, unsigned int pointeur, int taille) {

    unsigned int sup = (inf + taille - 1) % 256; // pour l'instant juste pour les tests 

    return
    /* inf <= pointeur <= sup */
    ( inf <= sup && pointeur >= inf && pointeur <= sup) ||
        /* sup < inf <= pointeur */
        (sup < inf && pointeur >= inf) ||
        /* pointeur <= sup < inf */
        (sup < inf && pointeur <= sup);
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
    return (srv_checksum(paquet, size) == somme_ctrl);
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
    } while (ok);
    return 1;
}

uint8_t srv_cpy(uint8_t *dest, uint8_t *src, uint8_t size) {
    uint8_t i = 0;
    uint8_t t = srv_len(src);
    if (size < t)
        return 0;
    while (i < t) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return 1;
}

int8_t srv_create_paket_rf(uint8_t paquet[], uint8_t data[],
                           uint16_t dest, uint16_t src, uint8_t typeDePaquet, uint8_t numPaquet) {
    uint8_t s = 10; // permet de diviser en octet l'entier sur 16 bit 
    int i;
    int j = 0;
    // dest, c'est pour le separer octet par octet 
    uint8_t d = dest / s;
    paquet[j++] = d;
    d = dest % s;
    paquet[j++] = d;
    //type de paquet cod? sur un oct?
    paquet[j++] = typeDePaquet;
    // numero du paquet cod� sur 2 octets
    paquet[j++] = numPaquet;
    //src
    paquet[j++] = src / s;
    paquet[j++] = src % s;

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
    return j; // j = taille du taquet g?n?r? 
}

void extractInfos(uint8_t* paquet, int *j, uint8_t* out, int count) {
    int i;
    for (i = 0; i < count; i++) { // 4 car l'adress est cod?e sur 4oct
        out[i] = paquet[*j];
        *j = *j + 1;
    }
    out[i] = '\0';
}

int8_t srv_decode_packet_rf(uint8_t* paquet, Frame *pPaquetRecu, uint8_t size,
                            uint16_t idOF) {
    uint8_t s = 10; // permet de concatener les adresses 
    int j = 0;
    //on recup?re id dest pour verifier si c'est egale au notre==> c'est le slave 1
    pPaquetRecu->ID_Dest = paquet[j] * s + paquet[j + 1];
    j += 2;
    //    printf("%d\n", pPaquetRecu->ID_Dest);
    //teste si le paquet m'est destin?
    if (pPaquetRecu->ID_Dest != idOF && pPaquetRecu->ID_Dest != ID_BROADCAST) // ?a veut dire pas les meme 
        return 0; // le paquet n'est pas ? moi
    
    //la premi?re chose qu'on recup?re c'est le crc
    uint8_t sum_ctl = paquet[size-1];
    if (srv_test_cheksum(paquet, size - 1, sum_ctl) == 0) {
        return 0; // on retourn 0 dans ce cas precis 
    }
    
    //on recup?re le type de paquet
    pPaquetRecu->Type_Msg = paquet[j++];

    pPaquetRecu->ID_Msg = paquet[j++]; // on recup?re le numero de paquet

    //on recup?re le src
    pPaquetRecu->ID_Src = paquet[j] * s + paquet[j + 1];
    j += 2;
    //on traitera ce cas plustard
    extractInfos(paquet, &j, pPaquetRecu->nonUtiliser, 2);
    //j = j + 3; //on saute les 2 oct non utiliser
    //data
    extractInfos(paquet, &j, pPaquetRecu->data, size - j - 1); //le -1 c'est pour ne pas recup?rer le crc

    return j;
}


/****************                                         *********************/
/*************************                     ********************************/
/******************************************************************************/
/******************************************************************************/