
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
    
//#if defined(UART_DEBUG)
//    printf("Module alpha trx Power on\n");
//#endif
//    radioAlphaTRX_Init();
//    radioAlphaTRX_ReceivedMode();
    
#if defined(UART_DEBUG)
    printf("Module GSM Power on\n");
#endif
//    int8_t ok = 0;
//    if (GMS3_ModulePower(true)) {
//        //        demo();
//        ok = 1;
//    } else {
//#if defined(UART_DEBUG)
//        printf("non conncter \n");
//#endif
//    }
    MASTER_AppInit();
    while (1) {
        MASTER_AppTask();
    }
    return -1;
}
