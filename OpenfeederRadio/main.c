
/**
  Section: Included Files
 */

#include "mcc_generated_files/tmr1.h"
#include <xc.h>
#include "alpha_trx_driver/radio_alpha_trx.h"
#include "driver/trx.h"
#include "driver/rx.h"
#include "test/test.h"
#include "driver/master_api.h"


int main(void) {
    // initialize the device
    SYSTEM_Initialize();
    
    printf("Init ok\n");
    radioAlphaTRX_Init();
    
    //test_rx();
    //test_timer();
    test_tx();
    
//    test_update_date_receive(); 
    //master_state_machine_of_daytime();
    return -1;
}
