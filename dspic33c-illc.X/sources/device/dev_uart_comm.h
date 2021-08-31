/**
  @Company
    Microchip Technology Inc.

  @File Name
    dev_uart_comm.h

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs UART communication using SMPS protocol.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.167.0
        Device            :  dsPIC33CK256MP506      
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.50
        MPLAB 	          :  MPLAB X v5.35
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
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


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UART_FUNCTIONS_H
#define	UART_FUNCTIONS_H

#include <xc.h> // include processor files - each processor file is guarded.  

//======================================================================================================================
// @brief flags and variables for UART communication structure
//======================================================================================================================

typedef struct {

    struct {
        volatile unsigned RXFrameReady : 1;
        volatile unsigned TXFrameReady : 1;
        volatile unsigned TXSendDone : 1;
        volatile unsigned SOF : 1;
        volatile unsigned : 4;

        volatile uint8_t RXBytes[128];
        volatile uint8_t UartRecCounter;
        volatile uint16_t UartRecActionID;
        volatile uint8_t UartRecLength;
        volatile uint16_t RecCRC;

        volatile uint8_t TXBytes[128];
        volatile uint8_t UartSendCounter;
        volatile uint8_t UartSendLength;

        volatile uint16_t SendCRC;
        volatile uint16_t ReceiveCRC;
    };

} UART_T;

extern UART_T UART_RxTx;

//======================================================================================================================
// @brief function prototypes
//======================================================================================================================

void UARTsend(void);
void UART_load_for_send(void);
void UARTTxInit(void);
void UARTreceive(void);

#endif	/* XC_HEADER_TEMPLATE_H */

