
/**
  Section: Included Files
 */

#include "mcc_generated_files/system.h"
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

    MASTER_AppInit();
    while (1) {
        MASTER_AppTask();
    }
    return -1;
}
