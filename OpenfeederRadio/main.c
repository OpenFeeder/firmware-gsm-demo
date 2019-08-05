
/**
  Section: Included Files
 */

#include "mcc_generated_files/tmr1.h"
#include <xc.h>
#include "alpha_trx_driver/radio_alpha_trx.h"
#include "test/test.h"
#include "driver/master_api.h"
#include "driver/Services.h"
#include "appGSM/appGSM3_SIM800.h"

int main(void) {
    // initialize the device
    SYSTEM_Initialize();

    printf("Init ok\n");

#if defined(UART_DEBUG)
    printf("Module GSM Power on\n");
#endif
    bool ok = false;
    ok = GMS3_ModulePower(true);

    MASTER_AppInit();
    while (1) {
        if (ok) {
//            demo();
            MASTER_AppTask();
        } else {
#if defined(UART_DEBUG)
            printf("non conncter \n");
#endif
            ok = GMS3_ModulePower(true);
        }
    }
    return -1;
}
