/**
  System Interrupts Generated Driver File 

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.c

  @Summary:
    This is the generated manager file for the MPLAB(c) Code Configurator device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for MPLAB(c) Code Configurator interrupts.
    Generation Information : 
        Product Revision  :  MPLAB(c) Code Configurator - 4.15.7
        Device            :  PIC24FJ256GB406
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.31
        MPLAB             :  MPLAB X 3.60

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

 */


/**
    Section: Includes
 */
#include <xc.h>
#include "pin_manager.h"
#include "../../src/framework/AlphaTRX/radio_alpha_trx.h"
#include "led_status.h"

/**
    void PIN_MANAGER_Initialize(void)
 */
void PIN_MANAGER_Initialize(void) {
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATB = 0x8120;
    LATC = 0x0000;
    LATD = 0x0008;
    LATE = 0x0002;
    LATF = 0x0003;
    LATG = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISB = 0x42CF;
    TRISC = 0x1000;
    TRISD = 0x0E37;
    TRISE = 0x0015;
    TRISF = 0x00B8;
    TRISG = 0x024C;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    IOCPDA = 0x0000;
    IOCPDB = 0x0000;
    IOCPDC = 0x0000;
    IOCPDD = 0x0000;
    IOCPDE = 0x0000;
    IOCPDF = 0x0000;
    IOCPDG = 0x0000;
    IOCPDH = 0x0000;
    IOCPDJ = 0x0000;
    IOCPUA = 0x0000;
    IOCPUB = 0x0000;
    IOCPUC = 0x0000;
    IOCPUD = 0x0000;
    IOCPUE = 0x0000;
    IOCPUF = 0x0000;
    IOCPUG = 0x0000;
    IOCPUH = 0x0000;
    IOCPUJ = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;
    ODCC = 0x0000;
    ODCD = 0x0000;
    ODCE = 0x0000;
    ODCF = 0x0000;
    ODCG = 0x0000;
    ODCH = 0x0000;
    ODCJ = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSA = 0xC6ED;
    ANSB = 0x0044;
    ANSC = 0x001E;
    ANSD = 0xF000;
    ANSE = 0x0300;
    ANSF = 0x3134;
    ANSG = 0xF203;
    ANSH = 0x001F;

    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    RPINR0bits.INT1R = 0x0019;   //RD4->EXT_INT:INT1;
    RPOR13bits.RP26R = 0x0007;   //RG7->SPI1:SDO1;
    RPINR18bits.U1RXR = 0x000E;   //RB14->UART1:U1RX;
    RPINR1bits.INT3R = 0x0018;   //RD1->EXT_INT:INT3;
    RPINR1bits.INT2R = 0x0010;   //RF3->EXT_INT:INT2;
    RPOR9bits.RP19R = 0x0008;   //RG8->SPI1:SCK1OUT;
    RPINR2bits.INT4R = 0x0017;   //RD2->EXT_INT:INT4;
    RPOR14bits.RP29R = 0x0003;   //RB15->UART1:U1TX;
    RPOR4bits.RP8R = 0x0005;   //RB8->UART2:U2TX;
    RPINR20bits.SDI1R = 0x0015;   //RG6->SPI1:SDI1;
    RPINR19bits.U2RXR = 0x0007;   //RB7->UART2:U2RX;

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    /****************************************************************************
     * Interrupt On Change for group IOCFE - flag
     ***************************************************************************/
    IOCFEbits.IOCFE2 = 0; // Pin : RE2

    /****************************************************************************
     * Interrupt On Change for group IOCNE - negative
     ***************************************************************************/
    IOCNEbits.IOCNE2 = 1; // Pin : RE2

    /****************************************************************************
     * Interrupt On Change for group IOCPE - positive
     ***************************************************************************/
    IOCPEbits.IOCPE2 = 0; // Pin : RE2

    /****************************************************************************
     * Interrupt On Change for group PADCON - config
     ***************************************************************************/
	PADCONbits.IOCON = 1; 

    IEC1bits.CNIE = 1; // Enable CNI interrupt 
}

/* Interrupt service routine for the CNI interrupt. */
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    if (IFS1bits.CNIF == 1) {
        // Clear the flag
        IFS1bits.CNIF = 0;
        // interrupt on change for group IOCFE
        if (IOCFEbits.IOCFE2 == 1) {
            IOCFEbits.IOCFE2 = 0;
            // Add handler code here for Pin - RE2
            STATUS_READ_VAL RF_StatusRead;
            RF_StatusRead.Val = radioAlphaTRX_Command(STATUS_READ_CMD); //lecture du registre status 
            if (RF_StatusRead.bits.b15_RGIT_FFIT && !radioAlphaTRX_IsSendMode()) { // on verifie si la fifo est remplie 
                radioAlphaTRX_CaptureFrame();
            }
        }
    }
}
