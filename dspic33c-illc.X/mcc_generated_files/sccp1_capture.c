/**
  SCCP1 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    sccp1.c

  @Summary
    This is the generated driver implementation file for the SCCP1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for SCCP1. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.169.0
        Device            :  dsPIC33CK256MP506
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.50
        MPLAB             :  MPLAB X v5.40
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

/**
  Section: Included Files
*/

#include "sccp1_capture.h"


void SCCP1_CAPTURE_Initialize(void)
{
    // CCPON enabled; MOD Edge Detect; CCSEL enabled; CCPSIDL disabled; T32 16 Bit; CCPSLP disabled; TMRPS 1:1; CLKSEL FOSC/2; TMRSYNC disabled; 
    CCP1CON1L = (0x8010 & 0x7FFF); //Disabling CCPON bit
    //RTRGEN disabled; ALTSYNC disabled; ONESHOT disabled; TRIGEN disabled; OPS 4th IC Event; SYNC None; OPSSRC Timer Interrupt Event; 
    CCP1CON1H = 0x300;
    //ASDGM disabled; SSDG disabled; ASDG 0; PWMRSEN disabled; 
    CCP1CON2L = 0x00;
    //ICGSM Level-Sensitive mode; ICSEL IC1; AUXOUT Disabled; OCAEN disabled; OENSYNC disabled; 
    CCP1CON2H = 0x00;
    //OETRIG disabled; OSCNT None; POLACE disabled; PSSACE Tri-state; 
    CCP1CON3H = 0x00;
    //ICDIS disabled; SCEVT disabled; TRSET disabled; ICOV disabled; ASEVT disabled; ICGARM disabled; TRCLR disabled; 
    CCP1STATL = 0x00;
    //TMR 0; 
    CCP1TMRL = 0x00;
    //TMR 0; 
    CCP1TMRH = 0x00;
    //PR 0; 
    CCP1PRL = 0x00;
    //PR 0; 
    CCP1PRH = 0x00;
    //CMP 0; 
    CCP1RA = 0x00;
    //CMP 0; 
    CCP1RB = 0x00;
    //BUF 0; 
    CCP1BUFL = 0x00;
    //BUF 0; 
    CCP1BUFH = 0x00;

    CCP1CON1Lbits.CCPON = 0x1; //Enabling CCP

    // Clearing IF flag before enabling the interrupt.
    IFS0bits.CCP1IF = 0;
    // Enabling SCCP1 interrupt.
    IEC0bits.CCP1IE = 1;

    // Clearing IF flag before enabling the interrupt.
    IFS0bits.CCT1IF = 0;
    // Enabling SCCP1 interrupt.
    IEC0bits.CCT1IE = 1;
}

void SCCP1_CAPTURE_Start( void )
{
    /* Start the Timer */
    CCP1CON1Lbits.CCPON = true;
}

void SCCP1_CAPTURE_Stop( void )
{
    /* Stop the Timer */
    CCP1CON1Lbits.CCPON = false;
}

void __attribute__ ((weak)) SCCP1_CAPTURE_CallBack(void)
{
    // Add your custom callback code here
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _CCP1Interrupt ( void )
{
    /* Check if the Timer Interrupt/Status is set */
    if(IFS0bits.CCP1IF)
    {
		// SCCP1 CAPTURE callback function 
		SCCP1_CAPTURE_CallBack();
        
        IFS0bits.CCP1IF = 0;
    }
}

void __attribute__ ((weak)) SCCP1_CAPTURE_TimerCallBack(void)
{
    // Add your custom callback code here
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _CCT1Interrupt ( void )
{
    if(IFS0bits.CCT1IF)
    {
		// SCCP1 CAPTURE Timer callback function 
		SCCP1_CAPTURE_TimerCallBack();

        IFS0bits.CCT1IF = 0;
    }
}

uint16_t SCCP1_CAPTURE_Data16Read( void )
{
    /* Return the captured timer data */
    return(CCP1BUFL);

}


bool SCCP1_CAPTURE_HasBufferOverflowed( void )
{
    return( CCP1STATLbits.ICOV );
}

bool SCCP1_CAPTURE_IsBufferEmpty( void )
{
    return( ! CCP1STATLbits.ICBNE );
}

void SCCP1_CAPTURE_OverflowFlagReset( void )
{
    CCP1STATLbits.ICOV = 0;
}
/**
 End of File
*/