/**
  @Company
    Microchip Technology Inc.

  @File Name
    App_GUI.c

  @Summary
    This source file provides the application specific HUI functions

  @Description
    This source file provides the application specific HUI functions
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


#include <xc.h>
#include <stdbool.h>
#include "../mcc_generated_files/pin_manager.h"
#include "device/dev_button.h"
#include "driver/drv_led.h"
#include "driver/drv_adc.h"
#include "device/dev_gui_comm.h"
#include "App_HMI.h"
#include "driver/power_controller/drv_pwrctrl_ILLC.h"
#include "driver/power_controller/Controller_pwrctrl_ILLC.h"
#include "driver/power_controller/drv_pwrctrl_ILLC_internal.h"
#include "driver/power_controller/drv_pwrctrl_ILLC_fault.h"

#ifdef BORDTEST
extern uint8_t boardtest_state;
#endif

#define APP_GUI_PROTOCOL_ID 0x40EF

void App_GUI_ProtocolHandler(uint16_t length, uint8_t* data);

//======================================================================================================================
// @brief initializes the GUI application
//======================================================================================================================
void App_GUI_Init(void)
{
    Dev_GuiComm_Register_ProtocolHandler(App_GUI_ProtocolHandler, APP_GUI_PROTOCOL_ID);
}


void App_GUI_Task_1ms(void)
{
    if (Dev_GuiComm_Stream_ReadyToSend())
    {
        //Message pos 0
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_val_FB_Vdd);

        //Message pos 1
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_val_FB_Vout);

        //Message pos 2
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT);

        //Message pos 3
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_val_FB_P_CT1_FLT);

        //Message pos 4
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_val_AN15);

        //Message pos 5
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_val_FB_Vaux);

        //Message pos 6
        Dev_GuiComm_Stream_SendWord(1 << (pwr_ctrl_state-1));

        //Message pos 7
        Dev_GuiComm_Stream_SendWord(ILLCFaults.CollectiveFaults);

        //Message pos 8
        if(App_HMI_useRefFromPoti)
			pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits |= 0x0001;
        else
			pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits &= 0xFFFE;        
        if(App_HMI_useRefFromGUI)
			pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits |= 0x0002;
        else
			pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits &= 0xFFFD;        
        if(App_HMI_useFixedRef)
			pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits |= 0x0004;
        else
			pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits &= 0xFFFB;        

        Dev_GuiComm_Stream_SendWord(pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits);

        //Message pos 9
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT);

        //Message pos 10
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_val_FB_P_CT2_FLT);

        //Message pos 11
        if (pwr_ctrl_flagbits.SR1on)
            Dev_GuiComm_Stream_SendWord(PG2PHASE);
        else
            Dev_GuiComm_Stream_SendWord(0);

        //Message pos 12
        if (pwr_ctrl_flagbits.SR2on)
            Dev_GuiComm_Stream_SendWord(PG4PHASE);
        else
            Dev_GuiComm_Stream_SendWord(0);
        
        //Message pos 13
        Dev_GuiComm_Stream_SendWord(PG1PER);

        //Message pos 14
        //Dev_GuiComm_Stream_SendWord(pwr_ctrl_ref_data.val_VoutRef_internal);
        Dev_GuiComm_Stream_SendWord(1 << (sr_ctrl_state - 1));
        
        //Message pos 15
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_val_FB_Temp);
        
        //Message pos 16
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_ref_data.val_VoutRef_internal);
        
        Dev_GuiComm_Stream_FinishSending();
    }
}

//======================================================================================================================
// @brief handle the data that is coming from the GUI
//======================================================================================================================

#define GUI_CMD_SET_VOLTAGE         01
#define GUI_CMD_SET_REF_FROM_POTI   02


void App_GUI_ProtocolHandler(uint16_t length, uint8_t* data)
{
    uint8_t cmd = data[0];
    
    if (cmd == GUI_CMD_SET_REF_FROM_POTI)
    {
        if (!App_HMI_useFixedRef)
            App_HMI_useRefFromPoti = true;
        App_HMI_useRefFromGUI = false;
    }
    else if (cmd == GUI_CMD_SET_VOLTAGE)
    {
        if (length >= 3)
        {
            //volatile uint8_t highbyte = data[1];
            //volatile uint8_t lowbyte = data[2];
            uint16_t Gui_VoutRef = (data[1] << 8) + data[2];
            //uint16_t Gui_VoutRef = (highbyte << 8) + lowbyte;
            Drv_PwrCtrl_ILLC_SetReferenceRaw(Gui_VoutRef); // raw adc vaule
            App_HMI_useRefFromPoti = false;
            App_HMI_useRefFromGUI = true;
        }
    }
}
