/**
  PWM Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    pwm.c

  @Summary
    This is the generated driver implementation file for the PWM driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for PWM.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  dsPIC33CK256MP506
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
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

#include "pwm.h"
#include "clock.h"

/**
 Section: Driver Interface Function Definitions
*/

void PWM_Initialize (void)
{
    // MCLKSEL AFPLLO - Auxiliary Clock with PLL Enabled; HRERR disabled; LOCK disabled; DIVSEL 1:2; 
    PCLKCON = 0x03;
    // FSCL 0; 
    FSCL = 0x00;
    // FSMINPER 0; 
    FSMINPER = 0x00;
    // MPHASE 0; 
    MPHASE = 0x00;
    // MDC 0; 
    MDC = 0x00;
    // MPER 16; 
    MPER = 0x10;
    // LFSR 0; 
    LFSR = 0x00;
    // CTA7EN disabled; CTA8EN disabled; CTA1EN disabled; CTA2EN disabled; CTA5EN disabled; CTA6EN disabled; CTA3EN disabled; CTA4EN disabled; 
    CMBTRIGL = 0x00;
    // CTB8EN disabled; CTB3EN disabled; CTB2EN disabled; CTB1EN enabled; CTB7EN disabled; CTB6EN disabled; CTB5EN disabled; CTB4EN disabled; 
    CMBTRIGH = 0x01;
    // PWMLFA PWMS1 or PWMS2;; S1APOL Positive logic; S2APOL Positive logic; PWMLFAD No Assignment; PWMS1A PWM1H; PWMS2A PWM1H; 
    LOGCONA = 0x00;
    // PWMLFB PWMS1 | PWMS2; S2BPOL Positive logic; PWMLFBD No Assignment; S1BPOL Positive logic; PWMS2B PWM1H; PWMS1B PWM1H; 
    LOGCONB = 0x00;
    // PWMLFC PWMS1 | PWMS2; PWMLFCD No Assignment; S2CPOL Positive logic; S1CPOL Positive logic; PWMS1C PWM1H; PWMS2C PWM1H; 
    LOGCONC = 0x00;
    // PWMS1D PWM1H; S1DPOL Positive logic; PWMLFD PWMS1 | PWMS2; PWMLFDD No Assignment; S2DPOL Positive logic; PWMS2D PWM1H; 
    LOGCOND = 0x00;
    // PWMS1E PWM1H; PWMS2E PWM1H; S1EPOL Positive logic; PWMLFE PWMS1 | PWMS2; S2EPOL Positive logic; PWMLFED No Assignment; 
    LOGCONE = 0x00;
    // S1FPOL Positive logic; PWMS2F PWM1H; PWMS1F PWM1H; S2FPOL Positive logic; PWMLFFD No Assignment; PWMLFF PWMS1 | PWMS2; 
    LOGCONF = 0x00;
    // EVTASEL ADC Trigger 1; EVTASYNC Not synchronized; EVTAPOL Active-high; EVTAPGS PG2; EVTASTRD Not stretched; EVTAOEN disabled; 
    PWMEVTA = 0x2081;
    // EVTBPGS PG1; EVTBSYNC Not synchronized; EVTBPOL Active-high; EVTBSEL PCI Sync Active Output; EVTBSTRD Stretched to 8 PWM clock cycles minimum; EVTBOEN disabled; 
    PWMEVTB = 0x20;
    // EVTCPGS PG1; EVTCPOL Active-high; EVTCSEL PGTRGSEL bits; EVTCSTRD Stretched to 8 PWM clock cycles minimum; EVTCSYNC Not synchronized; EVTCOEN disabled; 
    PWMEVTC = 0x00;
    // EVTDOEN disabled; EVTDSTRD Stretched to 8 PWM clock cycles minimum; EVTDPOL Active-high; EVTDPGS PG1; EVTDSEL PGTRGSEL bits; EVTDSYNC Not synchronized; 
    PWMEVTD = 0x00;
    // EVTEOEN disabled; EVTEPOL Active-high; EVTEPGS PG1; EVTESTRD Stretched to 8 PWM clock cycles minimum; EVTESEL PGTRGSEL bits; EVTESYNC Not synchronized; 
    PWMEVTE = 0x00;
    // EVTFPOL Active-high; EVTFPGS PG1; EVTFSTRD Stretched to 8 PWM clock cycles minimum; EVTFSEL PGTRGSEL bits; EVTFOEN disabled; EVTFSYNC Not synchronized; 
    PWMEVTF = 0x00;
    // MSTEN enabled; TRGMOD Single trigger mode; SOCS Self-trigger; UPDMOD SOC update; MPHSEL disabled; MPERSEL disabled; MDCSEL disabled; 
    PG1CONH = 0x800;
    // MSTEN disabled; TRGMOD Retriggerrable mode; SOCS Trigger output selected by PG1 or PG5; UPDMOD Slaved SOC update; MPHSEL disabled; MPERSEL disabled; MDCSEL disabled; 
    PG2CONH = 0x241;
    // MSTEN enabled; TRGMOD Retriggerrable mode; SOCS TRIG bit or PCI Sync function; UPDMOD SOC update; MPHSEL disabled; MPERSEL disabled; MDCSEL disabled; 
    PG3CONH = 0x84F;
    // MSTEN disabled; TRGMOD Retriggerrable mode; SOCS Trigger output selected by PG3 or PG7; UPDMOD Slaved SOC update; MPHSEL disabled; MPERSEL disabled; MDCSEL disabled; 
    PG4CONH = 0x243;
    // TRSET disabled; UPDREQ disabled; CLEVT disabled; TRCLR disabled; CAP disabled; SEVT disabled; FFEVT disabled; UPDATE disabled; FLTEVT disabled; 
    PG1STAT = 0x00;
    // TRSET disabled; UPDREQ disabled; CLEVT disabled; TRCLR disabled; CAP disabled; SEVT disabled; FFEVT disabled; UPDATE disabled; FLTEVT disabled; 
    PG2STAT = 0x00;
    // TRSET disabled; UPDREQ disabled; CLEVT disabled; TRCLR disabled; CAP disabled; SEVT disabled; FFEVT disabled; UPDATE disabled; FLTEVT disabled; 
    PG3STAT = 0x00;
    // TRSET disabled; UPDREQ disabled; CLEVT disabled; TRCLR disabled; CAP disabled; SEVT disabled; FFEVT disabled; UPDATE disabled; FLTEVT disabled; 
    PG4STAT = 0x00;
    // FLTDAT 0; DBDAT 0; SWAP disabled; OVRENH enabled; OVRENL enabled; OSYNC User output overrides are synchronized to the local PWM time base; CLMOD disabled; FFDAT 0; CLDAT 0; OVRDAT 0; 
    PG1IOCONL = 0x3000;
    // FLTDAT 0; DBDAT 0; SWAP enabled; OVRENH enabled; OVRENL enabled; OSYNC User output overrides are synchronized to the local PWM time base; CLMOD disabled; FFDAT 0; CLDAT 0; OVRDAT 0; 
    PG2IOCONL = 0x7000;
    // FLTDAT 0; DBDAT 0; SWAP disabled; OVRENH enabled; OVRENL enabled; OSYNC User output overrides are synchronized to the local PWM time base; CLMOD disabled; FFDAT 0; CLDAT 0; OVRDAT 0; 
    PG3IOCONL = 0x3000;
    // FLTDAT 0; DBDAT 0; SWAP enabled; OVRENH enabled; OVRENL enabled; OSYNC User output overrides are synchronized to the local PWM time base; CLMOD disabled; FFDAT 0; CLDAT 0; OVRDAT 0; 
    PG4IOCONL = 0x7000;
    // PENL enabled; DTCMPSEL PCI Sync Logic; PMOD Independent; POLL Active-high; PENH enabled; CAPSRC Software; POLH Active-high; 
    PG1IOCONH = 0x1C;
    // PENL enabled; DTCMPSEL PCI Sync Logic; PMOD Independent; POLL Active-high; PENH enabled; CAPSRC PCI Current-limit Signal; POLH Active-high; 
    PG2IOCONH = 0x301C;
    // PENL enabled; DTCMPSEL PCI Sync Logic; PMOD Independent; POLL Active-high; PENH enabled; CAPSRC Software; POLH Active-high; 
    PG3IOCONH = 0x1C;
    // PENL enabled; DTCMPSEL PCI Sync Logic; PMOD Independent; POLL Active-high; PENH enabled; CAPSRC Software; POLH Active-high; 
    PG4IOCONH = 0x1C;
    // UPDTRG Duty Cycle; ADTR1PS 1:6; PGTRGSEL EOC event; ADTR1EN3 enabled; ADTR1EN1 disabled; ADTR1EN2 disabled; 
    PG1EVTL = 0x2C08;
    // UPDTRG Manual; ADTR1PS 1:1; PGTRGSEL EOC event; ADTR1EN3 enabled; ADTR1EN1 disabled; ADTR1EN2 disabled; 
    PG2EVTL = 0x400;
    // UPDTRG Duty Cycle; ADTR1PS 1:6; PGTRGSEL EOC event; ADTR1EN3 enabled; ADTR1EN1 disabled; ADTR1EN2 disabled; 
    PG3EVTL = 0x2C08;
    // UPDTRG Manual; ADTR1PS 1:1; PGTRGSEL EOC event; ADTR1EN3 disabled; ADTR1EN1 disabled; ADTR1EN2 disabled; 
    PG4EVTL = 0x00;
    // ADTR2EN1 disabled; IEVTSEL EOC; SIEN disabled; FFIEN disabled; ADTR1OFS None; CLIEN disabled; FLTIEN disabled; ADTR2EN2 disabled; ADTR2EN3 disabled; 
    PG1EVTH = 0x00;
    // ADTR2EN1 disabled; IEVTSEL EOC; SIEN disabled; FFIEN disabled; ADTR1OFS None; CLIEN disabled; FLTIEN disabled; ADTR2EN2 disabled; ADTR2EN3 disabled; 
    PG2EVTH = 0x00;
    // ADTR2EN1 disabled; IEVTSEL EOC; SIEN disabled; FFIEN disabled; ADTR1OFS None; CLIEN disabled; FLTIEN disabled; ADTR2EN2 disabled; ADTR2EN3 disabled; 
    PG3EVTH = 0x00;
    // ADTR2EN1 disabled; IEVTSEL EOC; SIEN disabled; FFIEN disabled; ADTR1OFS None; CLIEN disabled; FLTIEN disabled; ADTR2EN2 disabled; ADTR2EN3 disabled; 
    PG4EVTH = 0x00;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG1FPCIL = 0x00;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG2FPCIL = 0x00;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG3FPCIL = 0x00;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG4FPCIL = 0x00;
    // TQPS Not inverted; LATMOD disabled; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG1FPCIH = 0x00;
    // TQPS Not inverted; LATMOD disabled; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG2FPCIH = 0x00;
    // TQPS Not inverted; LATMOD disabled; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG3FPCIH = 0x00;
    // TQPS Not inverted; LATMOD disabled; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG4FPCIH = 0x00;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG1CLPCIL = 0x00;
    // PSS Tied to 0; PPS Inverted; SWTERM disabled; PSYNC disabled; TERM Auto-Terminate; AQPS Inverted; AQSS None; TSYNCDIS Immediately; 
    PG2CLPCIL = 0x9820;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG3CLPCIL = 0x00;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG4CLPCIL = 0x00;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG1CLPCIH = 0x00;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Latched Rising edge; 
    PG2CLPCIH = 0x400;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG3CLPCIH = 0x00;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG4CLPCIH = 0x00;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG1FFPCIL = 0x00;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG2FFPCIL = 0x00;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG3FFPCIL = 0x00;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG4FFPCIL = 0x00;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG1FFPCIH = 0x00;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG2FFPCIH = 0x00;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG3FFPCIH = 0x00;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG4FFPCIH = 0x00;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG1SPCIL = 0x00;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG2SPCIL = 0x00;
    // PSS PWM Event A; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Auto-Terminate; AQPS Not inverted; AQSS None; TSYNCDIS Immediately; 
    PG3SPCIL = 0x9017;
    // PSS Tied to 0; PPS Not inverted; SWTERM disabled; PSYNC disabled; TERM Manual Terminate; AQPS Not inverted; AQSS None; TSYNCDIS PWM EOC; 
    PG4SPCIL = 0x00;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG1SPCIH = 0x00;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Rising edge; 
    PG2SPCIH = 0x100;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Rising edge; 
    PG3SPCIH = 0x100;
    // PCIGT disabled; TQPS Not inverted; SWPCI Drives '0'; BPEN disabled; TQSS None; SWPCIM PCI acceptance logic; BPSEL PWM Generator 1; ACP Level-sensitive; 
    PG4SPCIH = 0x00;
    // LEB 0; 
    PG1LEBL = 0x00;
    // LEB 0; 
    PG2LEBL = 0x00;
    // LEB 0; 
    PG3LEBL = 0x00;
    // LEB 0; 
    PG4LEBL = 0x00;
    // PWMPCI 1; PLR disabled; PLF disabled; PHR disabled; PHF disabled; 
    PG1LEBH = 0x00;
    // PWMPCI 1; PLR disabled; PLF disabled; PHR disabled; PHF disabled; 
    PG2LEBH = 0x00;
    // PWMPCI 1; PLR disabled; PLF disabled; PHR disabled; PHF disabled; 
    PG3LEBH = 0x00;
    // PWMPCI 1; PLR disabled; PLF disabled; PHR disabled; PHF disabled; 
    PG4LEBH = 0x00;
    // PHASE 0; 
    PG1PHASE = 0x00;
    // PHASE 592; 
    PG2PHASE = 0x250;
    // PHASE 0; 
    PG3PHASE = 0x00;
    // PHASE 592; 
    PG4PHASE = 0x250;
    // DC 0; 
    PG1DC = 0x00;
    // DC 0; 
    PG2DC = 0x00;
    // DC 0; 
    PG3DC = 0x00;
    // DC 0; 
    PG4DC = 0x00;
    // DCA 0; 
    PG1DCA = 0x00;
    // DCA 0; 
    PG2DCA = 0x00;
    // DCA 0; 
    PG3DCA = 0x00;
    // DCA 0; 
    PG4DCA = 0x00;
    // PER 3992; 
    PG1PER = 0xF98;
    // PER 3992; 
    PG2PER = 0xF98;
    // PER 3992; 
    PG3PER = 0xF98;
    // PER 3992; 
    PG4PER = 0xF98;
    // TRIGA 0; 
    PG1TRIGA = 0x00;
    // TRIGA 0; 
    PG2TRIGA = 0x00;
    // TRIGA 0; 
    PG3TRIGA = 0x00;
    // TRIGA 0; 
    PG4TRIGA = 0x00;
    // TRIGB 0; 
    PG1TRIGB = 0x00;
    // TRIGB 0; 
    PG2TRIGB = 0x00;
    // TRIGB 0; 
    PG3TRIGB = 0x00;
    // TRIGB 0; 
    PG4TRIGB = 0x00;
    // TRIGC 0; 
    PG1TRIGC = 0x00;
    // TRIGC 1000; 
    PG2TRIGC = 0x3E8;
    // TRIGC 0; 
    PG3TRIGC = 0x00;
    // TRIGC 0; 
    PG4TRIGC = 0x00;
    // DTL 200; 
    PG1DTL = 0xC8;
    // DTL 200; 
    PG2DTL = 0xC8;
    // DTL 200; 
    PG3DTL = 0xC8;
    // DTL 200; 
    PG4DTL = 0xC8;
    // DTH 200; 
    PG1DTH = 0xC8;
    // DTH 200; 
    PG2DTH = 0xC8;
    // DTH 200; 
    PG3DTH = 0xC8;
    // DTH 200; 
    PG4DTH = 0xC8;
    

    //Wait until AUX PLL clock is locked
    while(!CLOCK_AuxPllLockStatusGet());

    // HREN enabled; MODSEL Independent Edge, dual output; TRGCNT 1; CLKSEL Master clock; ON disabled; 
    PG1CONL = 0x8A;
    // HREN enabled; MODSEL Independent Edge, dual output; TRGCNT 1; CLKSEL Master clock; ON disabled; 
    PG2CONL = 0x8A;
    // HREN enabled; MODSEL Independent Edge, dual output; TRGCNT 1; CLKSEL Master clock; ON disabled; 
    PG3CONL = 0x8A;
    // HREN enabled; MODSEL Independent Edge, dual output; TRGCNT 1; CLKSEL Master clock; ON disabled; 
    PG4CONL = 0x8A;
}

void __attribute__ ((weak)) PWM_Generator1_CallBack(void)
{
    // Add Application code here
}

void PWM_Generator1_Tasks(void)
{
    if(IFS4bits.PWM1IF)
    {
        // PWM Generator1 callback function 
        PWM_Generator1_CallBack();

        // clear the PWM Generator1 interrupt flag
        IFS4bits.PWM1IF = 0;
    }
}

void __attribute__ ((weak)) PWM_Generator2_CallBack(void)
{
    // Add Application code here
}

void PWM_Generator2_Tasks(void)
{
    if(IFS4bits.PWM2IF)
    {
        // PWM Generator2 callback function 
        PWM_Generator2_CallBack();

        // clear the PWM Generator2 interrupt flag
        IFS4bits.PWM2IF = 0;
    }
}

void __attribute__ ((weak)) PWM_Generator3_CallBack(void)
{
    // Add Application code here
}

void PWM_Generator3_Tasks(void)
{
    if(IFS4bits.PWM3IF)
    {
        // PWM Generator3 callback function 
        PWM_Generator3_CallBack();

        // clear the PWM Generator3 interrupt flag
        IFS4bits.PWM3IF = 0;
    }
}

void __attribute__ ((weak)) PWM_Generator4_CallBack(void)
{
    // Add Application code here
}

void PWM_Generator4_Tasks(void)
{
    if(IFS4bits.PWM4IF)
    {
        // PWM Generator4 callback function 
        PWM_Generator4_CallBack();

        // clear the PWM Generator4 interrupt flag
        IFS4bits.PWM4IF = 0;
    }
}


void __attribute__ ((weak)) PWM_EventA_CallBack(void)
{
    // Add Application code here
}

void PWM_EventA_Tasks(void)
{
    if(IFS10bits.PEVTAIF)
    {
     
        // PWM EventA callback function 
        PWM_EventA_CallBack();
	
        // clear the PWM EventA interrupt flag
        IFS10bits.PEVTAIF = 0;
    }
}
void __attribute__ ((weak)) PWM_EventB_CallBack(void)
{
    // Add Application code here
}

void PWM_EventB_Tasks(void)
{
    if(IFS10bits.PEVTBIF)
    {
     
        // PWM EventB callback function 
        PWM_EventB_CallBack();
	
        // clear the PWM EventB interrupt flag
        IFS10bits.PEVTBIF = 0;
    }
}
void __attribute__ ((weak)) PWM_EventC_CallBack(void)
{
    // Add Application code here
}

void PWM_EventC_Tasks(void)
{
    if(IFS10bits.PEVTCIF)
    {
     
        // PWM EventC callback function 
        PWM_EventC_CallBack();
	
        // clear the PWM EventC interrupt flag
        IFS10bits.PEVTCIF = 0;
    }
}
void __attribute__ ((weak)) PWM_EventD_CallBack(void)
{
    // Add Application code here
}

void PWM_EventD_Tasks(void)
{
    if(IFS10bits.PEVTDIF)
    {
     
        // PWM EventD callback function 
        PWM_EventD_CallBack();
	
        // clear the PWM EventD interrupt flag
        IFS10bits.PEVTDIF = 0;
    }
}
void __attribute__ ((weak)) PWM_EventE_CallBack(void)
{
    // Add Application code here
}

void PWM_EventE_Tasks(void)
{
    if(IFS10bits.PEVTEIF)
    {
     
        // PWM EventE callback function 
        PWM_EventE_CallBack();
	
        // clear the PWM EventE interrupt flag
        IFS10bits.PEVTEIF = 0;
    }
}
void __attribute__ ((weak)) PWM_EventF_CallBack(void)
{
    // Add Application code here
}

void PWM_EventF_Tasks(void)
{
    if(IFS10bits.PEVTFIF)
    {
     
        // PWM EventF callback function 
        PWM_EventF_CallBack();
	
        // clear the PWM EventF interrupt flag
        IFS10bits.PEVTFIF = 0;
    }
}
