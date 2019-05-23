
/**
  Section: Included Files
 */

#include "mcc_generated_files/tmr1.h"
#include <xc.h>
#include "alpha_trx_driver/radio_alpha_trx.h"
#include "driver/trx.h"
#include "driver/rx.h"


int main(void) {
    // initialize the device
    SYSTEM_Initialize();
    
    printf("Init ok\n");
    radioAlphaTRX_Init();
//    init_trx();
    
    test_rx();
    
    //test_tx();
    while(1) {
//        if (getSyncDate()) {
//            LED_Toggle();
//            reset_sync_date();
//        }
//
    } // on attend la 
    return -1;
}
