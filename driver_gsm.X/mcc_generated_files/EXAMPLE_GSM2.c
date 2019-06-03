/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include <string.h>
#include "mcc.h"
#include "GSM2_driver.h"
#include "EXAMPLE_GSM2.h"
#include "drivers/uart.h"

//*********************************************************
//          Local Defines/Variables used for Example
//*********************************************************
#define responseBufferSize  48
static uint8_t gsm2ResponseIndex = 0;
static char gsm2ResponseBuffer [responseBufferSize] = {0};
static char exampleReadStorage [responseBufferSize] = {0};

volatile uint16_t timeout = 0;
volatile uint16_t delay = 0;
//*********************************************************
//          Local Prototype Functions
//*********************************************************
static void EXAMPLE_blockingWait(uint16_t);
static void EXAMPLE_ReadyReceiveBuffer(void);
static char* EXAMPLE_GetResponse(void);

void set_delay(uint16_t delay_ms) { 
    delay = delay_ms; 
    while (delay > 0) {}
}
int get_delay () { return delay; }

void tmr_collBack() {
    if (delay > 0){
        --delay;
    }
}
//*********************************************************
//          Accessible Example Functions
//*********************************************************
void EXAMPLE_useGSM2 (void)
{
    char* exampleRead;
    GSM2_SetHardwareReset(true);                     // Toggle Module HW Reset   
    EXAMPLE_blockingWait(400);
    GSM2_SetHardwareReset(false); 
    EXAMPLE_blockingWait(400);                      // Wait for Module Information Response             
    exampleRead = EXAMPLE_GetResponse();            // Read Response
    strcpy(exampleReadStorage, exampleRead);        // Store for use/reference
    EXAMPLE_ReadyReceiveBuffer();                   // Prepare for next message
    GSM2_SendString("AT");                          // Send "AT" for Baud Rate Negotiation            
    EXAMPLE_blockingWait(400);                      // Wait
    exampleRead = EXAMPLE_GetResponse();            // Read 'OK' String from Buffer
#if defined(_DEBUG)
    printf("GSM : %s\n", responseBufferSize);
#endif
    strcpy(exampleReadStorage, exampleRead);        // Store for use/reference
    EXAMPLE_ReadyReceiveBuffer();                   // Prepare for next message
#if defined(_DEBUG)
    printf("GSM : %s\n", responseBufferSize);
#endif
    GSM2_SendString("ATE0");                        // Send "ATEO" to disable command ECHOs 
    EXAMPLE_blockingWait(400);                      // Wait
    exampleRead = EXAMPLE_GetResponse();            // Read 'Ok' String from Buffer
    strcpy(exampleReadStorage, exampleRead);        // Store for use/reference
#if defined(_DEBUG)
    printf("GSM : %s\n", responseBufferSize);
#endif
    EXAMPLE_ReadyReceiveBuffer();                   // Prepare for next message
    GSM2_SendString("AT+GMI");                      // Send "AT+GMI" to request Module Info 
    EXAMPLE_blockingWait(400);                      // Wait
    exampleRead = EXAMPLE_GetResponse();            // Read "Info" response String from Buffer
#if defined(_DEBUG)
    printf("GSM : %s\n", exampleRead);
#endif
    strcpy(exampleReadStorage, exampleRead);        // Store for use/reference
}
void EXAMPLE_CaptureReceivedMessage(void)       // Processed from ISR
{
    uint8_t readByte = uart[GSM2].Read();
    if ( (readByte != '\0') && (gsm2ResponseIndex < responseBufferSize) )
        gsm2ResponseBuffer[gsm2ResponseIndex++] = readByte;
}
//*********************************************************
//          Local Used Example Functions
//*********************************************************
static void EXAMPLE_ReadyReceiveBuffer (void)
{
    gsm2ResponseIndex = 0;
    uint8_t position;
    for (position = 0; position < responseBufferSize; position++)
        gsm2ResponseBuffer[position] = 0;
}
static char* EXAMPLE_GetResponse(void)
{
    return gsm2ResponseBuffer;
}
static void EXAMPLE_blockingWait (uint16_t limit)
{
    uint16_t counter;
    for (counter = 0; counter < limit; counter++)
        set_delay(15);
//        __delay_ms(15);
}
/**
 End of File
 */