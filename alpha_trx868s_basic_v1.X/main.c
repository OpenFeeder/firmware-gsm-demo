














/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : v1.35
        Device            :  PIC24FJ256GB406
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.31
        MPLAB             :  MPLAB X 3.65
*/

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

/*
    Transceiver ALPHA-TRX-868
    https://www.rfsolutions.co.uk/radio-modules-c10/alpha-trx-low-cost-high-performance-transceiver-module-p384
    Modules de t�l�m�trie RF ALPHA 868 MHz
    Module de donn�es radio transceiver (�metteur-r�cepteur) fonctionnant dans la bande de fr�quence ISM 868.
 * voir :
 * - Project:Alpha transceivers
 *   https://wiki.london.hackspace.org.uk/view/Project:Alpha_transceivers
 * - http://jeelabs.net/projects/cafe/wiki
 *   http://jeelabs.net/projects/cafe/wiki/RF12demo
 *   http://jeelabs.net/projects/hardware/wiki/JeeNode
 *   antenna (Ant): for 868 MHz, use a 82 mm wire
 */

#include "mcc_generated_files/mcc.h"
#include "app.h"

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    /* Initialize the application. */
    APP_Initialize( );
    while (1)
    {
        /* Maintain the application's state machine. */
        APP_Tasks( ); /* application specific tasks */
    }

        /* Execution should not come here during normal operation. */
    setLedsStatusColor( LED_RED );

    return ( EXIT_FAILURE );
}
/**
 End of File
*/