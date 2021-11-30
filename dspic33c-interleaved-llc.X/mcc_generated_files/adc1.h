/**
  ADC1 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    adc1.h

  @Summary
    This is the generated header file for the ADC1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for ADC1.
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

#ifndef _ADC1_H
#define _ADC1_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: Data Types
*/

/** Scan Selected Macro Definition
 
 @Summary 
   Defines the scan option selection done for the shared channels.
 
 @Description
   This macro defines the scan option selection done for the shared channels.
 
 Remarks:
   None
 */
#define ADC1_SCAN_MODE_SELECTED true

/** ADC1 Channel Definition
 
 @Summary 
   Defines the channels selected.
 
 @Description
   This routine defines the channels that are available for the module to use.
 
 Remarks:
   None
 */
typedef enum 
{
    FB_VAUX,//Channel Name:AN6   Assigned to:Shared Channel
    FB_VOUT,//Channel Name:AN12   Assigned to:Shared Channel
    FB_TEMP,//Channel Name:AN14   Assigned to:Shared Channel
    channel_AN15,//Channel Name:AN15   Assigned to:Shared Channel
    FB_P_CT2_FLT,//Channel Name:AN17   Assigned to:Shared Channel
    FB_P_CT1_FLT,//Channel Name:AN19   Assigned to:Shared Channel
    channel_AN24,//Channel Name:AN24   Assigned to:Shared Channel
    channel_AN25,//Channel Name:AN25   Assigned to:Shared Channel
    FB_S_CT1_FLT,//Channel Name:AN0   Assigned to:Dedicated Core0
    FB_S_CT2_FLT,//Channel Name:AN1   Assigned to:Dedicated Core1
} ADC1_CHANNEL;

/**
  Section: Interface Routines
*/

/**
  @Summary
    Initializes ADC1 module.

  @Description
    This routine initializes ADC1 module, using the given initialization data. 
    This routine must be called before any other ADC routine is called. 

  @Preconditions
    None.

  @Param
    None.

  @Returns
    None

  @Example
    <code>
        int conversion,i=0;
        ADC1_Initialize();

        ADC1_Enable();
        ADC1_ChannelSelect(channel);
        ADC1_SoftwareTriggerEnable();
        //Provide Delay
        for(i=0;i <1000;i++)
        {
        }
        ADC1_SoftwareTriggerDisable();
        while(!ADC1_IsConversionComplete(channel));
        conversion = ADC1_ConversionResultGet(channel);
        ADC1_Disable(); 
    </code>
*/
void ADC1_Initialize (void);

/**
  @Summary
    Enables the ADC1 module.

  @Description
    This routine is used to enable the ADC1 module.
 
  @Preconditions
    ADC1_Initialize function should have been called 
    before calling this function.

  @Param
    None.

  @Returns
    None.

  @Example
    <code>
        int conversion,i=0;
        ADC1_Initialize();

        ADC1_Enable();
        ADC1_ChannelSelect(channel);
        ADC1_SoftwareTriggerEnable();
        //Provide Delay
        for(i=0;i <1000;i++)
        {
        }
        ADC1_SoftwareTriggerDisable();
        while(!ADC1_IsConversionComplete(channel));
        conversion = ADC1_ConversionResultGet(channel);
        ADC1_Disable(); 
    </code>
*/
inline static void ADC1_Enable(void)
{
   ADCON1Lbits.ADON = 1;
}

/**
  @Summary
    Disables the ADC1 module.

  @Description
    This routine is used to disable the ADC1 module.
 
  @Preconditions
    ADC1_Initialize function should have been called 
    before calling this function.

  @Param
    None.

  @Returns
    None.

  @Example
    <code>
        int conversion,i=0;
        ADC1_Initialize();

        ADC1_Enable();
        ADC1_ChannelSelect(channel);
        ADC1_SoftwareTriggerEnable();
        //Provide Delay
        for(i=0;i <1000;i++)
        {
        }
        ADC1_SoftwareTriggerDisable();
        while(!ADC1_IsConversionComplete(channel));
        conversion = ADC1_ConversionResultGet(channel);
        ADC1_Disable(); 
    </code>
*/
inline static void ADC1_Disable(void)
{
   ADCON1Lbits.ADON = 0;
}

/**
  @Summary
    Enables software common trigger.

  @Description
    This routine is used to enable the ADC1 software common trigger.
 
  @Preconditions
    ADC1_Initialize function should have been called 
    before calling this function.

  @Param
    None.

  @Returns
    None.

  @Example
    <code>
        int conversion,i=0;
        ADC1_Initialize();

        ADC1_Enable();
        ADC1_ChannelSelect(channel);
        ADC1_SoftwareTriggerEnable();
        //Provide Delay
        for(i=0;i <1000;i++)
        {
        }
        ADC1_SoftwareTriggerDisable();
        while(!ADC1_IsConversionComplete(channel));
        conversion = ADC1_ConversionResultGet(channel);
        ADC1_Disable(); 
    </code>
*/

inline static void ADC1_SoftwareTriggerEnable(void)
{
   ADCON3Lbits.SWCTRG = 1;
}

/**
  @Summary
    Disables software common trigger.

  @Description
    This routine is used to disable the ADC1 software common trigger.
 
  @Preconditions
    ADC1_Initialize function should have been called 
    before calling this function.

  @Param
    None.

  @Returns
    None.

  @Example
    <code>
        int conversion,i=0;
        ADC1_Initialize();

        ADC1_Enable();
        ADC1_ChannelSelect(channel);
        ADC1_SoftwareTriggerEnable();
        //Provide Delay
        for(i=0;i <1000;i++)
        {
        }
        ADC1_SoftwareTriggerDisable();
        while(!ADC1_IsConversionComplete(channel));
        conversion = ADC1_ConversionResultGet(channel);
        ADC1_Disable(); 
    </code>
*/
inline static void ADC1_SoftwareTriggerDisable(void)
{
   ADCON3Lbits.SWCTRG = 0;
}

/**
  @Summary
    Allows selection of a channel for conversion.

  @Description
    This routine is used to select desired channel for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.

  @Param
    channel - Channel for conversion

  @Returns
    None
  
  @Example
    <code>
        int conversion,i=0;
        ADC1_Initialize();

        ADC1_Enable();
        ADC1_ChannelSelect(channel);
        ADC1_SoftwareTriggerEnable();
        //Provide Delay
        for(i=0;i <1000;i++)
        {
        }
        ADC1_SoftwareTriggerDisable();
        while(!ADC1_IsConversionComplete(channel));
        conversion = ADC1_ConversionResultGet(channel);
        ADC1_Disable(); 
    </code>
*/
inline static void ADC1_ChannelSelect( ADC1_CHANNEL channel )
{
    /*This routine does not have any implementation since 
            *Shared channels are selected from UI.
            *Dedicated channels are selected from UI.
    */
}

/**
  @Summary
    Returns the conversion value for the channel selected.

  @Description
    This routine is used to get the analog to digital converted value for a 
    specific channel.
 
  @Preconditions
    This routine returns the conversion value only after the conversion is complete. 
    Conversion completion status can be checked using ADC1_IsConversionComplete(channel)
    routine.

  @Param
    channel - Selected channel
   
  @Returns
   Returns the analog to digital converted value
  
  @Example
    <code>
        int conversion,i=0;
        ADC1_Initialize();

        ADC1_Enable();
        ADC1_ChannelSelect(channel);
        ADC1_SoftwareTriggerEnable();
        //Provide Delay
        for(i=0;i <1000;i++)
        {
        }
        ADC1_SoftwareTriggerDisable();
        while(!ADC1_IsConversionComplete(channel));
        conversion = ADC1_ConversionResultGet(channel);
        ADC1_Disable(); 
    </code>
 */
inline static uint16_t ADC1_ConversionResultGet( ADC1_CHANNEL channel )
{
    uint16_t result;

    switch(channel)
    {
        case FB_VAUX:
                result = ADCBUF6;
                break;
        case FB_VOUT:
                result = ADCBUF12;
                break;
        case FB_TEMP:
                result = ADCBUF14;
                break;
        case channel_AN15:
                result = ADCBUF15;
                break;
        case FB_P_CT2_FLT:
                result = ADCBUF17;
                break;
        case FB_P_CT1_FLT:
                result = ADCBUF19;
                break;
        case channel_AN24:
                result = ADCBUF24;
                break;
        case channel_AN25:
                result = ADCBUF25;
                break;
        case FB_S_CT1_FLT:
                result = ADCBUF0;
                break;
        case FB_S_CT2_FLT:
                result = ADCBUF1;
                break;
        default:
                break;
    }
    return result;
}

/**
  @Summary
    Returns the status of conversion.

  @Description
    This routine is used to determine if conversion is completed. When conversion
    is complete the routine returns true otherwise false.
 
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.

  @Param
    channel - Selected channel
 
  @Returns
    true - Conversion is complete.
    false - Conversion is not complete.
  
  @Example
    <code>
        int conversion,i=0;
        ADC1_Initialize();

        ADC1_Enable();
        ADC1_ChannelSelect(channel);
        ADC1_SoftwareTriggerEnable();
        //Provide Delay
        for(i=0;i <1000;i++)
        {
        }
        ADC1_SoftwareTriggerDisable();
        while(!ADC1_IsConversionComplete(channel));
        conversion = ADC1_ConversionResultGet(channel);
        ADC1_Disable(); 
    </code>
 */
inline static bool ADC1_IsConversionComplete(ADC1_CHANNEL channel)
{
    bool status;

    switch(channel)
    {
        case FB_VAUX:
                status = ADSTATLbits.AN6RDY;
                break;
        case FB_VOUT:
                status = ADSTATLbits.AN12RDY;
                break;
        case FB_TEMP:
                status = ADSTATLbits.AN14RDY;
                break;
        case channel_AN15:
                status = ADSTATLbits.AN15RDY;
                break;
        case FB_P_CT2_FLT:
                status = ADSTATHbits.AN17RDY;
                break;
        case FB_P_CT1_FLT:
                status = ADSTATHbits.AN19RDY;
                break;
        case channel_AN24:
                status = ADSTATHbits.AN24RDY;
                break;
        case channel_AN25:
                status = ADSTATHbits.AN25RDY;
                break;
        case FB_S_CT1_FLT:
                status = ADSTATLbits.AN0RDY;
                break;
        case FB_S_CT2_FLT:
                status = ADSTATLbits.AN1RDY;
                break;
        default:
                break;
    }

    return status;
}

/**
  @Summary
    Enables interrupts.

  @Description
    This routine is used to enable the ADC1 interrupt.
 
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None.

  @Example
    <code>
        ADC1_InterruptEnable(); 
    </code>
*/
inline static void ADC1_InterruptEnable(void)
{
    IEC5bits.ADCIE = 1;
}

/**
  @Summary
    Disables interrupts.

  @Description
    This routine is used to disable the ADC1 interrupt.
 
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None.

  @Example
    <code>
        ADC1_InterruptDisable(); 
    </code>
*/
inline static void ADC1_InterruptDisable(void)
{
    IEC5bits.ADCIE = 0;
}

/**
  @Summary
    Clears interrupt flag

  @Description
    This routine is used to clear the interrupt flag manually.
 
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None.

  @Example
    <code>
        ADC1_InterruptFlagClear(); 
    </code>
*/
inline static void ADC1_InterruptFlagClear(void)
{
    IFS5bits.ADCIF = 0;
}

/**
  @Summary
    Allows selection of priority for interrupt.

  @Description
    This routine is used to select desired priority for interrupt.
  
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None.

  @Example
    <code>
        uint16_t priorityValue;
        priorityValue = 0x002;
 
        ADC1_InterruptPrioritySet(priorityValue); 
    </code>
*/
inline static void ADC1_InterruptPrioritySet( uint16_t priorityValue )
{
    IPC22bits.ADCIP = 0x7 & priorityValue;
}

/**
  @Summary
    ADC1 Common callback routine.

  @Description
    This routine is a ADC1 Common callback function.
  
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetCommonInterruptHandler(&ADC1_CallBack);
    </code>
*/
void ADC1_CallBack(void);

/**
  @Summary
    Assigns a function pointer with a ADC1 Common callback address.

  @Description
    This routine assigns a function pointer with a ADC1 Common callback address.
  
  @Preconditions
    None.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetCommonInterruptHandler(&ADC1_CallBack);
    </code>
*/
void ADC1_SetCommonInterruptHandler(void* handler);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Param
    None

  @Returns 
    None
 
  @Example
    <code>    
        ADC1_Tasks();
    </code>
*/
void ADC1_Tasks(void);

/**
  @Summary
    Enables individual channel interrupt.

  @Description
    This routine is used to enable the ADC1 individual channel
    interrupt.
 
  @Preconditions
    None.

  @Param
    channel - Selected channel

  @Returns
    None.

  @Example
    <code>
        ADC1_IndividualChannelInterruptEnable(channel); 
    </code>
*/
inline static void ADC1_IndividualChannelInterruptEnable(ADC1_CHANNEL channel)
{
    switch(channel)
    {
        case FB_VAUX:
                IEC6bits.ADCAN6IE = 1;
                break;
        case FB_VOUT:
                IEC6bits.ADCAN12IE = 1;
                break;
        case FB_TEMP:
                IEC6bits.ADCAN14IE = 1;
                break;
        case channel_AN15:
                IEC6bits.ADCAN15IE = 1;
                break;
        case FB_P_CT2_FLT:
                IEC6bits.ADCAN17IE = 1;
                break;
        case FB_P_CT1_FLT:
                IEC6bits.ADCAN19IE = 1;
                break;
        case channel_AN24:
                IEC12bits.ADCAN24IE = 1;
                break;
        case channel_AN25:
                IEC12bits.ADCAN25IE = 1;
                break;
        case FB_S_CT1_FLT:
                IEC5bits.ADCAN0IE = 1;
                break;
        case FB_S_CT2_FLT:
                IEC5bits.ADCAN1IE = 1;
                break;
        default:
                break;
    }
}

/**
  @Summary
    Disables individual channel interrupt.

  @Description
    This routine is used to disable the ADC1 individual channel
    interrupt.
 
  @Preconditions
    None.

  @Param
    channel - Selected channel

  @Returns
    None.

  @Example
    <code>
        ADC1_IndividualChannelInterruptDisable(channel); 
    </code>
*/
inline static void ADC1_IndividualChannelInterruptDisable(ADC1_CHANNEL channel)
{
    switch(channel)
    {
        case FB_VAUX:
                IEC6bits.ADCAN6IE = 0;
                break;
        case FB_VOUT:
                IEC6bits.ADCAN12IE = 0;
                break;
        case FB_TEMP:
                IEC6bits.ADCAN14IE = 0;
                break;
        case channel_AN15:
                IEC6bits.ADCAN15IE = 0;
                break;
        case FB_P_CT2_FLT:
                IEC6bits.ADCAN17IE = 0;
                break;
        case FB_P_CT1_FLT:
                IEC6bits.ADCAN19IE = 0;
                break;
        case channel_AN24:
                IEC12bits.ADCAN24IE = 0;
                break;
        case channel_AN25:
                IEC12bits.ADCAN25IE = 0;
                break;
        case FB_S_CT1_FLT:
                IEC5bits.ADCAN0IE = 0;
                break;
        case FB_S_CT2_FLT:
                IEC5bits.ADCAN1IE = 0;
                break;
        default:
                break;
    }
}

/**
  @Summary
    Clears individual channel interrupt flag

  @Description
    This routine is used to clear the individual channel interrupt flag manually.
 
  @Preconditions
    None.

  @Param
    channel - Selected channel

  @Returns
    None.

  @Example
    <code>
        ADC1_IndividualChannelInterruptFlagClear(channel); 
    </code>
*/
inline static void ADC1_IndividualChannelInterruptFlagClear(ADC1_CHANNEL channel)
{
    switch(channel)
    {
        case FB_VAUX:
                IFS6bits.ADCAN6IF = 0;
                break;
        case FB_VOUT:
                IFS6bits.ADCAN12IF = 0;
                break;
        case FB_TEMP:
                IFS6bits.ADCAN14IF = 0;
                break;
        case channel_AN15:
                IFS6bits.ADCAN15IF = 0;
                break;
        case FB_P_CT2_FLT:
                IFS6bits.ADCAN17IF = 0;
                break;
        case FB_P_CT1_FLT:
                IFS6bits.ADCAN19IF = 0;
                break;
        case channel_AN24:
                IFS12bits.ADCAN24IF = 0;
                break;
        case channel_AN25:
                IFS12bits.ADCAN25IF = 0;
                break;
        case FB_S_CT1_FLT:
                IFS5bits.ADCAN0IF = 0;
                break;
        case FB_S_CT2_FLT:
                IFS5bits.ADCAN1IF = 0;
                break;
        default:
                break;
    }
}

/**
  @Summary
    ADC1 FB_VAUX callback routine.

  @Description
    This routine is a ADC1 FB_VAUX callback function.
  
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_VAUXInterruptHandler(&ADC1_FB_VAUX_CallBack);
    </code>
*/
void ADC1_FB_VAUX_CallBack(uint16_t adcVal);

/**
  @Summary
    Assigns a function pointer with a ADC1 FB_VAUX callback address.

  @Description
    This routine assigns a function pointer with a ADC1 FB_VAUX callback address.
  
  @Preconditions
    None.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_VAUXInterruptHandler(&ADC1_FB_VAUX_CallBack);
    </code>
*/
void ADC1_SetFB_VAUXInterruptHandler(void* handler);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for ADC1 FB_VAUX polled implementations.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Param
    None

  @Returns 
    None
 
  @Example
    <code>    
        ADC1_FB_VAUX_Tasks();
    </code>
*/
void ADC1_FB_VAUX_Tasks(void);

/**
  @Summary
    ADC1 FB_VOUT callback routine.

  @Description
    This routine is a ADC1 FB_VOUT callback function.
  
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_VOUTInterruptHandler(&ADC1_FB_VOUT_CallBack);
    </code>
*/
void ADC1_FB_VOUT_CallBack(uint16_t adcVal);

/**
  @Summary
    Assigns a function pointer with a ADC1 FB_VOUT callback address.

  @Description
    This routine assigns a function pointer with a ADC1 FB_VOUT callback address.
  
  @Preconditions
    None.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_VOUTInterruptHandler(&ADC1_FB_VOUT_CallBack);
    </code>
*/
void ADC1_SetFB_VOUTInterruptHandler(void* handler);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for ADC1 FB_VOUT polled implementations.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Param
    None

  @Returns 
    None
 
  @Example
    <code>    
        ADC1_FB_VOUT_Tasks();
    </code>
*/
void ADC1_FB_VOUT_Tasks(void);

/**
  @Summary
    ADC1 FB_TEMP callback routine.

  @Description
    This routine is a ADC1 FB_TEMP callback function.
  
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_TEMPInterruptHandler(&ADC1_FB_TEMP_CallBack);
    </code>
*/
void ADC1_FB_TEMP_CallBack(uint16_t adcVal);

/**
  @Summary
    Assigns a function pointer with a ADC1 FB_TEMP callback address.

  @Description
    This routine assigns a function pointer with a ADC1 FB_TEMP callback address.
  
  @Preconditions
    None.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_TEMPInterruptHandler(&ADC1_FB_TEMP_CallBack);
    </code>
*/
void ADC1_SetFB_TEMPInterruptHandler(void* handler);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for ADC1 FB_TEMP polled implementations.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Param
    None

  @Returns 
    None
 
  @Example
    <code>    
        ADC1_FB_TEMP_Tasks();
    </code>
*/
void ADC1_FB_TEMP_Tasks(void);

/**
  @Summary
    ADC1 channel_AN15 callback routine.

  @Description
    This routine is a ADC1 channel_AN15 callback function.
  
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_Setchannel_AN15InterruptHandler(&ADC1_channel_AN15_CallBack);
    </code>
*/
void ADC1_channel_AN15_CallBack(uint16_t adcVal);

/**
  @Summary
    Assigns a function pointer with a ADC1 channel_AN15 callback address.

  @Description
    This routine assigns a function pointer with a ADC1 channel_AN15 callback address.
  
  @Preconditions
    None.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_Setchannel_AN15InterruptHandler(&ADC1_channel_AN15_CallBack);
    </code>
*/
void ADC1_Setchannel_AN15InterruptHandler(void* handler);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for ADC1 channel_AN15 polled implementations.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Param
    None

  @Returns 
    None
 
  @Example
    <code>    
        ADC1_channel_AN15_Tasks();
    </code>
*/
void ADC1_channel_AN15_Tasks(void);

/**
  @Summary
    ADC1 FB_P_CT2_FLT callback routine.

  @Description
    This routine is a ADC1 FB_P_CT2_FLT callback function.
  
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_P_CT2_FLTInterruptHandler(&ADC1_FB_P_CT2_FLT_CallBack);
    </code>
*/
void ADC1_FB_P_CT2_FLT_CallBack(uint16_t adcVal);

/**
  @Summary
    Assigns a function pointer with a ADC1 FB_P_CT2_FLT callback address.

  @Description
    This routine assigns a function pointer with a ADC1 FB_P_CT2_FLT callback address.
  
  @Preconditions
    None.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_P_CT2_FLTInterruptHandler(&ADC1_FB_P_CT2_FLT_CallBack);
    </code>
*/
void ADC1_SetFB_P_CT2_FLTInterruptHandler(void* handler);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for ADC1 FB_P_CT2_FLT polled implementations.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Param
    None

  @Returns 
    None
 
  @Example
    <code>    
        ADC1_FB_P_CT2_FLT_Tasks();
    </code>
*/
void ADC1_FB_P_CT2_FLT_Tasks(void);

/**
  @Summary
    ADC1 FB_P_CT1_FLT callback routine.

  @Description
    This routine is a ADC1 FB_P_CT1_FLT callback function.
  
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_P_CT1_FLTInterruptHandler(&ADC1_FB_P_CT1_FLT_CallBack);
    </code>
*/
void ADC1_FB_P_CT1_FLT_CallBack(uint16_t adcVal);

/**
  @Summary
    Assigns a function pointer with a ADC1 FB_P_CT1_FLT callback address.

  @Description
    This routine assigns a function pointer with a ADC1 FB_P_CT1_FLT callback address.
  
  @Preconditions
    None.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_P_CT1_FLTInterruptHandler(&ADC1_FB_P_CT1_FLT_CallBack);
    </code>
*/
void ADC1_SetFB_P_CT1_FLTInterruptHandler(void* handler);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for ADC1 FB_P_CT1_FLT polled implementations.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Param
    None

  @Returns 
    None
 
  @Example
    <code>    
        ADC1_FB_P_CT1_FLT_Tasks();
    </code>
*/
void ADC1_FB_P_CT1_FLT_Tasks(void);

/**
  @Summary
    ADC1 channel_AN24 callback routine.

  @Description
    This routine is a ADC1 channel_AN24 callback function.
  
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_Setchannel_AN24InterruptHandler(&ADC1_channel_AN24_CallBack);
    </code>
*/
void ADC1_channel_AN24_CallBack(uint16_t adcVal);

/**
  @Summary
    Assigns a function pointer with a ADC1 channel_AN24 callback address.

  @Description
    This routine assigns a function pointer with a ADC1 channel_AN24 callback address.
  
  @Preconditions
    None.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_Setchannel_AN24InterruptHandler(&ADC1_channel_AN24_CallBack);
    </code>
*/
void ADC1_Setchannel_AN24InterruptHandler(void* handler);

/**
  @Summary
    ADC1 channel_AN25 callback routine.

  @Description
    This routine is a ADC1 channel_AN25 callback function.
  
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_Setchannel_AN25InterruptHandler(&ADC1_channel_AN25_CallBack);
    </code>
*/
void ADC1_channel_AN25_CallBack(uint16_t adcVal);

/**
  @Summary
    Assigns a function pointer with a ADC1 channel_AN25 callback address.

  @Description
    This routine assigns a function pointer with a ADC1 channel_AN25 callback address.
  
  @Preconditions
    None.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_Setchannel_AN25InterruptHandler(&ADC1_channel_AN25_CallBack);
    </code>
*/
void ADC1_Setchannel_AN25InterruptHandler(void* handler);


/**
  @Summary
    ADC1 FB_S_CT1_FLT callback routine.

  @Description
    This routine is a ADC1 FB_S_CT1_FLT callback function.
  
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_S_CT1_FLTInterruptHandler(&ADC1_FB_S_CT1_FLT_CallBack);
    </code>
*/
void ADC1_FB_S_CT1_FLT_CallBack(uint16_t adcVal);

/**
  @Summary
    Assigns a function pointer with a ADC1 FB_S_CT1_FLT callback address.

  @Description
    This routine assigns a function pointer with a ADC1 FB_S_CT1_FLT callback address.
  
  @Preconditions
    None.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_S_CT1_FLTInterruptHandler(&ADC1_FB_S_CT1_FLT_CallBack);
    </code>
*/
void ADC1_SetFB_S_CT1_FLTInterruptHandler(void* handler);

/**
  @Summary
    ADC1 FB_S_CT2_FLT callback routine.

  @Description
    This routine is a ADC1 FB_S_CT2_FLT callback function.
  
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_S_CT2_FLTInterruptHandler(&ADC1_FB_S_CT2_FLT_CallBack);
    </code>
*/
void ADC1_FB_S_CT2_FLT_CallBack(uint16_t adcVal);

/**
  @Summary
    Assigns a function pointer with a ADC1 FB_S_CT2_FLT callback address.

  @Description
    This routine assigns a function pointer with a ADC1 FB_S_CT2_FLT callback address.
  
  @Preconditions
    None.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        ADC1_SetFB_S_CT2_FLTInterruptHandler(&ADC1_FB_S_CT2_FLT_CallBack);
    </code>
*/
void ADC1_SetFB_S_CT2_FLTInterruptHandler(void* handler);



/**
  Section: Interface Routines: Dedicated Core
*/
/**
  @Summary
    Enables power for Core 0 

  @Description
    This routine is used to set the analog and digital power for ADC1 Core 0.
 
  @Preconditions
    None.

  @Param
    None

  @Returns
    None.

  @Example
    Refer to ADC1_Initialize() for an example
*/
void ADC1_Core0PowerEnable(void);

/**
  @Summary
    Enables power for Core 1 

  @Description
    This routine is used to set the analog and digital power for ADC1 Core 1.
 
  @Preconditions
    None.

  @Param
    None

  @Returns
    None.

  @Example
    Refer to ADC1_Initialize() for an example
*/
void ADC1_Core1PowerEnable(void);


/**
  @Summary
    Enables power for ADC1 shared Core

  @Description
    This routine is used to set the analog and digital power for ADC1 shared Core.
 
  @Preconditions
    None.

  @Param
    None

  @Returns
    None.

  @Example
    Refer to ADC1_Initialize() for an example
*/
void ADC1_SharedCorePowerEnable(void);



/*******************************************************************************

  !!! Deprecated Definitions and APIs !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/
/** ADC1 Core 0 Channel Definition

 @Summary 
   Defines the Core 0 channels available for conversion
 
 @Description
   This routine defines the channels that are available conversion.
 
 Remarks:
   None
*/
typedef enum 
{   
    ADC1_CORE0_AN0  =  0,

    ADC1_CORE0_ANA0  =  1,
} ADC1_CORE0_CHANNEL;
/** ADC1 Core 1 Channel Definition
 @Summary 
   Defines the Core 1 channels available for conversion
 
 @Description
   This routine defines the channels that are available conversion.
 
 Remarks:
   None
*/
typedef enum 
{   
    ADC1_CORE1_AN1  =  0,

    ADC1_CORE1_ANA1  =  1,
} ADC1_CORE1_CHANNEL;
/** ADC1 Data Format Type Definition
 
 @Summary 
   Defines the data format types available
 
 @Description
   This routine defines the data format types that are available for the module 
   to use.
 
 Remarks:
   None
 */
typedef enum 
{
    ADC1_FORM_INT   = 0, /* Integer */
    ADC1_FORM_FRACT = 1  /* Fraction */
} ADC1_FORM_TYPE;

/** ADC1 Resolution Type Definition
 
 @Summary 
   Defines the resolution types available
 
 @Description
   This routine defines the resolution types that are available for the module 
   to use.
 
 Remarks:
   None
 */
typedef enum 
{
    ADC1_RESOLUTION_6_BIT    = 0, /* 6-bit ADC1 operation */
    ADC1_RESOLUTION_8_BIT    = 1, /* 8-bit ADC1 operation */
    ADC1_RESOLUTION_10_BIT   = 0, /* 10-bit ADC1 operation */
    ADC1_RESOLUTION_12_BIT   = 1  /* 12-bit ADC1 operation */
} ADC1_RESOLUTION_TYPE;

/**
  @Summary
    Allows software level-sensitive common trigger to be enabled manually

  @Description
    This routine is used to enable the ADC1 software level-sensitive common trigger manually
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function. 
 
  @Returns
    None

  @Param
    None
  
  @Example
*/
inline static void __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_SoftwareLevelTriggerEnable(void)
{
    ADCON3Lbits.SWLCTRG = 1;
}

/**

  @Summary
    Sets Core 0 channel

  @Description
    This routine is used to set the ADC1 Core 0 channel manually.
 
  @Preconditions
    None.

  @Param
    None

  @Returns
    None.

  @Example
    Refer to ADC1_Initialize() for an example

*/
inline static void __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_Core0ChannelSelect(ADC1_CORE0_CHANNEL channel)
{
   ADCON4Hbits.C0CHS = channel;
}
/**
  @Summary
    Returns the ADC1 conversion value for Core 0.

  @Description
    This routine is used to get the analog to digital converted value. This
    routine gets converted values from the channel specified.
 
  @Preconditions
    The core required must be enabled and calibrated before calling this routine 
    using ADC1_Core0PowerEnable() and ADC1_Core0Calibration() 
    respectively. This routine returns the conversion value only after the 
    conversion is complete. Completion status conversion can be checked using 
    ADC1_IsCore0ConversionComplete() routine.
   
  @Returns
    Returns the buffer containing the conversion value.

  @Param
    Buffer address
  
  @Example
    Refer to ADC1_Initialize(); for an example
 */

inline static uint16_t __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_Core0ConversionResultGet(void) 
{
    return ADCBUF0;
}
/**
  @Summary
    Allows selection of a resolution mode for conversion

  @Description
    This routine is used to select desired resolution mode for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required resolution mode from the ADC1_RESOLUTION_TYPE list
  
  @Example
    Refer to ADC1_Initialize(); for an example
*/

inline static void __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_Core0ResolutionModeSet( ADC1_RESOLUTION_TYPE resolution )
{
    ADCORE0Hbits.RES = resolution;
}
/**
  @Summary
    Returns the conversion status of core0 for conversion

  @Description
    This routine is used to return the conversion status of core0 selected for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    The value of the Core Conversion register

  @Param
    None
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static bool __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_IsCore0ConversionComplete(void)
{   
    return ADSTATLbits.AN0RDY;
}

/**
  @Summary
    Allows conversion clock prescaler value to be set

  @Description
    This routine is used to allow conversion clock prescaler value to be set manually
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required prescaler integer value
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static void __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_Core0ConversionClockPrescalerSet(uint8_t prescaler)
{
    ADCORE0Hbits.ADCS = prescaler;
}

/**

  @Summary
    Sets Core 0 channel

  @Description
    This routine is used to set the ADC1 Core 1 channel manually.
 
  @Preconditions
    None.

  @Param
    None

  @Returns
    None.

  @Example
    Refer to ADC1_Initialize() for an example

*/
inline static void __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_Core1ChannelSelect(ADC1_CORE1_CHANNEL channel)
{
   ADCON4Hbits.C1CHS = channel;
}
/**
  @Summary
    Returns the ADC1 conversion value for Core 1.

  @Description
    This routine is used to get the analog to digital converted value. This
    routine gets converted values from the channel specified.
 
  @Preconditions
    The core required must be enabled and calibrated before calling this routine 
    using ADC1_Core1PowerEnable() and ADC1_Core1Calibration() 
    respectively. This routine returns the conversion value only after the 
    conversion is complete. Completion status conversion can be checked using 
    ADC1_IsCore1ConversionComplete() routine.
   
  @Returns
    Returns the buffer containing the conversion value.

  @Param
    Buffer address
  
  @Example
    Refer to ADC1_Initialize(); for an example
 */

inline static uint16_t __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_Core1ConversionResultGet(void) 
{
    return ADCBUF1;
}
/**
  @Summary
    Allows selection of a resolution mode for conversion

  @Description
    This routine is used to select desired resolution mode for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required resolution mode from the ADC1_RESOLUTION_TYPE list
  
  @Example
    Refer to ADC1_Initialize(); for an example
*/

inline static void __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_Core1ResolutionModeSet( ADC1_RESOLUTION_TYPE resolution )
{
    ADCORE1Hbits.RES = resolution;
}
/**
  @Summary
    Returns the conversion status of core1 for conversion

  @Description
    This routine is used to return the conversion status of core1 selected for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    The value of the Core Conversion register

  @Param
    None
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static bool __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_IsCore1ConversionComplete(void)
{   
    return ADSTATLbits.AN1RDY;
}

/**
  @Summary
    Allows conversion clock prescaler value to be set

  @Description
    This routine is used to allow conversion clock prescaler value to be set manually
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required prescaler integer value
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static void __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_Core1ConversionClockPrescalerSet(uint8_t prescaler)
{
    ADCORE1Hbits.ADCS = prescaler;
}

/**
  @Summary
    Allows selection of a data format type for conversion

  @Description
    This routine is used to select desired data format for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required data format type from the ADC1_FORM_TYPE list
  
  @Example
    Refer to ADC1_Initialize(); for an example
*/
inline static void __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_FormatDataSet( ADC1_FORM_TYPE form )
{
    ADCON1Hbits.FORM = form;
}
/**
  @Summary
    Allows selection of a resolution mode for conversion

  @Description
    This routine is used to select desired resolution mode for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required resolution mode from the ADC1_RESOLUTION_TYPE list
  
  @Example
    Refer to ADC1_Initialize(); for an example
*/
inline static void __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_SharedCoreResolutionModeSet( ADC1_RESOLUTION_TYPE resolution )
{
    ADCON1Hbits.SHRRES = resolution;
}
/**
  @Summary
    Allows conversion clock prescaler value to be set

  @Description
    This routine is used to allow conversion clock prescaler value to be set manually
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required prescaler integer value
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/
inline static void __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_SharedCoreConversionClockPrescalerSet(uint8_t prescaler)
{
    ADCON2Lbits.SHRADCS = prescaler;
}
/**
  @Summary
    Returns the ADC1 conversion value for the shared core channel AN6

  @Description
    This routine is used to get the analog to digital converted value for channel AN6. This
    routine gets converted values from the shared core channel AN6.
 
  @Preconditions
    The shared core must be enabled and calibrated before calling this routine 
    using ADC1_SharedCorePowerEnable() and ADC1_SharedCoreCalibration() 
    respectively. This routine returns the conversion value only after the 
    conversion is complete. Completion status conversion can be checked using 
    ADC1_IsSharedChannelAN6ConversionComplete() routine.
   
  @Returns
    Returns the buffer containing the conversion value.

  @Param
    Buffer address
  
  @Example
    Refer to ADC1_Initialize(); for an example
 */
inline static uint16_t __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_SharedChannelAN6ConversionResultGet(void) 
{
    return ADCBUF6;
}
/**
  @Summary
    Returns the conversion status of shared channel AN6 selected for conversion

  @Description
    This routine is used to return the conversion status of the shared channel AN6 
    selected for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    The value of the Channel AN6 Conversion register

  @Param
    None
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static bool __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_IsSharedChannelAN6ConversionComplete(void)
{   
    return ADSTATLbits.AN6RDY;
}
/**
  @Summary
    Returns the ADC1 conversion value for the shared core channel AN12

  @Description
    This routine is used to get the analog to digital converted value for channel AN12. This
    routine gets converted values from the shared core channel AN12.
 
  @Preconditions
    The shared core must be enabled and calibrated before calling this routine 
    using ADC1_SharedCorePowerEnable() and ADC1_SharedCoreCalibration() 
    respectively. This routine returns the conversion value only after the 
    conversion is complete. Completion status conversion can be checked using 
    ADC1_IsSharedChannelAN12ConversionComplete() routine.
   
  @Returns
    Returns the buffer containing the conversion value.

  @Param
    Buffer address
  
  @Example
    Refer to ADC1_Initialize(); for an example
 */
inline static uint16_t __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_SharedChannelAN12ConversionResultGet(void) 
{
    return ADCBUF12;
}
/**
  @Summary
    Returns the conversion status of shared channel AN12 selected for conversion

  @Description
    This routine is used to return the conversion status of the shared channel AN12 
    selected for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    The value of the Channel AN12 Conversion register

  @Param
    None
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static bool __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_IsSharedChannelAN12ConversionComplete(void)
{   
    return ADSTATLbits.AN12RDY;
}
/**
  @Summary
    Returns the ADC1 conversion value for the shared core channel AN14

  @Description
    This routine is used to get the analog to digital converted value for channel AN14. This
    routine gets converted values from the shared core channel AN14.
 
  @Preconditions
    The shared core must be enabled and calibrated before calling this routine 
    using ADC1_SharedCorePowerEnable() and ADC1_SharedCoreCalibration() 
    respectively. This routine returns the conversion value only after the 
    conversion is complete. Completion status conversion can be checked using 
    ADC1_IsSharedChannelAN14ConversionComplete() routine.
   
  @Returns
    Returns the buffer containing the conversion value.

  @Param
    Buffer address
  
  @Example
    Refer to ADC1_Initialize(); for an example
 */
inline static uint16_t __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_SharedChannelAN14ConversionResultGet(void) 
{
    return ADCBUF14;
}
/**
  @Summary
    Returns the conversion status of shared channel AN14 selected for conversion

  @Description
    This routine is used to return the conversion status of the shared channel AN14 
    selected for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    The value of the Channel AN14 Conversion register

  @Param
    None
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static bool __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_IsSharedChannelAN14ConversionComplete(void)
{   
    return ADSTATLbits.AN14RDY;
}
/**
  @Summary
    Returns the ADC1 conversion value for the shared core channel AN15

  @Description
    This routine is used to get the analog to digital converted value for channel AN15. This
    routine gets converted values from the shared core channel AN15.
 
  @Preconditions
    The shared core must be enabled and calibrated before calling this routine 
    using ADC1_SharedCorePowerEnable() and ADC1_SharedCoreCalibration() 
    respectively. This routine returns the conversion value only after the 
    conversion is complete. Completion status conversion can be checked using 
    ADC1_IsSharedChannelAN15ConversionComplete() routine.
   
  @Returns
    Returns the buffer containing the conversion value.

  @Param
    Buffer address
  
  @Example
    Refer to ADC1_Initialize(); for an example
 */
inline static uint16_t __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_SharedChannelAN15ConversionResultGet(void) 
{
    return ADCBUF15;
}
/**
  @Summary
    Returns the conversion status of shared channel AN15 selected for conversion

  @Description
    This routine is used to return the conversion status of the shared channel AN15 
    selected for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    The value of the Channel AN15 Conversion register

  @Param
    None
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static bool __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_IsSharedChannelAN15ConversionComplete(void)
{   
    return ADSTATLbits.AN15RDY;
}
/**
  @Summary
    Returns the ADC1 conversion value for the shared core channel AN17

  @Description
    This routine is used to get the analog to digital converted value for channel AN17. This
    routine gets converted values from the shared core channel AN17.
 
  @Preconditions
    The shared core must be enabled and calibrated before calling this routine 
    using ADC1_SharedCorePowerEnable() and ADC1_SharedCoreCalibration() 
    respectively. This routine returns the conversion value only after the 
    conversion is complete. Completion status conversion can be checked using 
    ADC1_IsSharedChannelAN17ConversionComplete() routine.
   
  @Returns
    Returns the buffer containing the conversion value.

  @Param
    Buffer address
  
  @Example
    Refer to ADC1_Initialize(); for an example
 */
inline static uint16_t __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_SharedChannelAN17ConversionResultGet(void) 
{
    return ADCBUF17;
}
/**
  @Summary
    Returns the conversion status of shared channel AN17 selected for conversion

  @Description
    This routine is used to return the conversion status of the shared channel AN17 
    selected for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    The value of the Channel AN17 Conversion register

  @Param
    None
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static bool __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_IsSharedChannelAN17ConversionComplete(void)
{   
    return ADSTATHbits.AN17RDY;
}
/**
  @Summary
    Returns the ADC1 conversion value for the shared core channel AN19

  @Description
    This routine is used to get the analog to digital converted value for channel AN19. This
    routine gets converted values from the shared core channel AN19.
 
  @Preconditions
    The shared core must be enabled and calibrated before calling this routine 
    using ADC1_SharedCorePowerEnable() and ADC1_SharedCoreCalibration() 
    respectively. This routine returns the conversion value only after the 
    conversion is complete. Completion status conversion can be checked using 
    ADC1_IsSharedChannelAN19ConversionComplete() routine.
   
  @Returns
    Returns the buffer containing the conversion value.

  @Param
    Buffer address
  
  @Example
    Refer to ADC1_Initialize(); for an example
 */
inline static uint16_t __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_SharedChannelAN19ConversionResultGet(void) 
{
    return ADCBUF19;
}
/**
  @Summary
    Returns the conversion status of shared channel AN19 selected for conversion

  @Description
    This routine is used to return the conversion status of the shared channel AN19 
    selected for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    The value of the Channel AN19 Conversion register

  @Param
    None
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static bool __attribute__((deprecated("\nThis will be removed in future MCC releases."))) ADC1_IsSharedChannelAN19ConversionComplete(void)
{   
    return ADSTATHbits.AN19RDY;
}

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //_ADC1_H
    
/**
 End of File
*/
