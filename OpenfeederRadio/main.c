
/**
  Section: Included Files
 */

#include "driver/tx.h"
#include "driver/rx.h"
#include "driver/Slave.h"
#include "debug/debug.h"
#include "test/test.h"
#include "mcc_generated_files/tmr1.h"
#include "driver/Master.h"
#include "driver/Services.h"
#include <xc.h>
//1 : debug
#define DEBUG_CLOCK 0

//stop transmission

//0 : tx
//1 : rx
#define MODE 0

void run_tx() {
    struct tm t;
    maitre_init_context(srv_getIDS1(), srv_getIDS2(), srv_getIDS3());
    while(1) {
        mster_state_machine();
    }
      
//    printf("je rentre dans la boucle\n");
    RTCC_TimeGet(&t); 
    printf("Fin ==> %d h:%d min:%d s\n", t.tm_hour, t.tm_min, t.tm_sec);
}
//__delay32(SLEEP_AFTER_INIT);
void run_rx() {
    struct tm t;
    RTCC_TimeGet(&t); 
    printf("Debut ==> %d h:%d min:%d s\n", t.tm_hour, t.tm_min, t.tm_sec);
    slave_state_machine(srv_getIDS2());
    RTCC_TimeGet(&t); 
    printf("Fin ==> %d h:%d min:%d s\n", t.tm_hour, t.tm_min, t.tm_sec);
}

int main(void) {
    // initialize the device
    SYSTEM_Initialize();
    
    
    //int i = 0;
    // boucle principale

    init_trx();
    printf("Init ok\n");
    run_tx();
    //run_rx();
    printf("END\n");
//    while(1) {
//        if (getSyncDate()) {
//            LED_Toggle();
//            reset_sync_date();
//        }
//
//    } // on attend la 
    return -1;
}
