/*
 * File:   Timer.c
 * Author: mmadi
 *
 * Created on 5 avril 2019, 22:06
 */


#include "xc.h"
#include "Timer.h"
#define NB_MAX_TIMER 10

// structure d'un timer 
typedef struct sTimer_t {
	int num_timer;
	int exp;
} Timer;

//permet de gerer les timers 
Timer gestTimer[NB_MAX_TIMER+1];
int nbTimers = 0;

/**
 * demare le numero du timer indiqué avec le nombre de ms indiqué 
 * @param n : numéro du timer
 * @param ms : delais 
 * @return : 1 timer demarré 0 si non 
 */
int8_t tmr_depart_num(int n, int ms) {
    int i = 0; 
    if (n < 1 || n > NB_MAX_TIMER) {
        return 0; // timer impossible 
    }
    //verifie si le timer n'est pas deja demarer 
    while (i < nbTimers) {
        if(gestTimer[i].num_timer == n) {
            return 0; // timer deja demarrer 
        }
        i++;
    }
    
}

/**
 * demare le temporisateur pour l'attente des paquet 
 * @param ms
 */
void tmr_depart_rdy_rf(int ms);

/**
 * demarre le timer pour le 'enoie des paquet 
 * @param ms
 */
void tmr_depart_enoie_paquet_rf(int ms);

/**
 * prends un numero de timer et l'arrete 
 * @param n : le numero du timer à arreter   0<n<10;
 */
void tmr_arreter_num(int n);

/**
 * arrete le timer du rdy 
 */
void tmr_arreter_rdy();

void tmr_arreter_enoie_paquet_rf();
