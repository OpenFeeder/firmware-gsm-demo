
/**
  Section: Included Files
 */

#include "mcc_generated_files/tmr1.h"
#include <xc.h>
#include "alpha_trx_driver/radio_alpha_trx.h"
#include "test/test.h"
#include "driver/master_api.h"
#include "driver/Services.h"


int main(void) {
    // initialize the device
    SYSTEM_Initialize();
    
    printf("Init ok\n");
    radioAlphaTRX_Init();
    radioAlphaTRX_ReceivedMode();
    while (1) {
        MASTER_Task();
    }
    return -1;
}
