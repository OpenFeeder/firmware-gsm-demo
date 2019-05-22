
/**
  Section: Included Files
 */

#include "mcc_generated_files/tmr1.h"
#include <xc.h>
#include "alpha_trx_driver/radio_alpha_trx.h"
//1 : debug
#define DEBUG_CLOCK 0

//stop transmission

//0 : tx
//1 : rx
#define MODE 0


int main(void) {
    // initialize the device
    SYSTEM_Initialize();
    
    radioAlphaTRX_Init();
    radioAlphaTRX_Send_Init();
    printf("Init ok\n");
    printf("END\n");
    while(1) {
//        if (getSyncDate()) {
//            LED_Toggle();
//            reset_sync_date();
//        }
//
    } // on attend la 
    return -1;
}
