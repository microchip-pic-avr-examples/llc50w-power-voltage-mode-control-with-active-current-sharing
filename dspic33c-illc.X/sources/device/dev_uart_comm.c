/**
  @Company
    Microchip Technology Inc.

  @File Name
    dev_uart_comm.c

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

#include <p33CK256MP506.h>

#include "xc.h"
#include "dev_uart_comm.h"
#include "../../mcc_generated_files/uart1.h"
#include "../../mcc_generated_files/pin_manager.h"
#include "driver/drv_adc.h"
#include "../app/App_HMI.h"
#include "driver/power_controller/drv_pwrctrl_ILLC.h"
#include "../driver/power_controller/Controller_pwrctrl_ILLC.h"
#include "../driver/power_controller/drv_pwrctrl_ILLC_internal.h"
#include "driver/power_controller/drv_pwrctrl_ILLC_fault.h"


UART_T UART_RxTx;

//======================================================================================================================
// @brief this function contains the send of one data byte every 1ms with 
//        CRC calculation and check if the dat stream is complete
//======================================================================================================================

void UARTsend(void)
{
  uint8_t i_;

  if ((UART_RxTx.UartSendCounter >= 0) && (UART_RxTx.UartSendCounter < UART_RxTx.UartSendLength + 5u))
  {
    //50us @ 16MHz
    UART_RxTx.SendCRC ^= (uint16_t) UART_RxTx.TXBytes[UART_RxTx.UartSendCounter];
    for (i_ = 0; i_ < 8; ++i_)
    {
      if ((UART_RxTx.SendCRC & 1) == 1)
        UART_RxTx.SendCRC = (UART_RxTx.SendCRC >> 1) ^ 0xA001;
      else
        UART_RxTx.SendCRC = (UART_RxTx.SendCRC >> 1);
    }
  }
  if ((UART_RxTx.UartSendCounter == UART_RxTx.UartSendLength + 5u))
  {
    UART_RxTx.TXBytes[UART_RxTx.UartSendLength + 5u] = (uint8_t) (UART_RxTx.SendCRC >> 8);
    UART_RxTx.TXBytes[UART_RxTx.UartSendLength + 6u] = (uint8_t) (UART_RxTx.SendCRC & 0x00FF);
  }

  UART1_Write(UART_RxTx.TXBytes[UART_RxTx.UartSendCounter]);

  UART_RxTx.UartSendCounter++;

  if (UART_RxTx.UartSendCounter > (UART_RxTx.UartSendLength + 7u))
  {
    UART_RxTx.UartSendCounter = 0;
    UART_RxTx.SendCRC = 0;
    UART_RxTx.TXSendDone = 1;
  }
}

//======================================================================================================================
// @brief this function contains the load of data words into the data stream
//======================================================================================================================

uint8_t messageindex = 0;

void Message_AddWord(uint16_t data)
{
  UART_RxTx.TXBytes[messageindex++] = data >> 8;
  UART_RxTx.TXBytes[messageindex++] = data & 0x00FF;
}

//======================================================================================================================
// @brief this function contains loading the values into the data stream
//======================================================================================================================

void UART_load_for_send(void)
{
  if (UART_RxTx.TXSendDone)
  {
    messageindex = 5;
    //Message pos 0
    Message_AddWord(pwr_ctrl_adc_data.drv_adc_val_FB_Vdd);
    //Message pos 1
    Message_AddWord(pwr_ctrl_adc_data.drv_adc_val_FB_Vout);
    //Message pos 2
    Message_AddWord(pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT);
    //Message pos 3
    Message_AddWord(pwr_ctrl_adc_data.drv_adc_val_FB_P_CT1_FLT);
    
    
    //Message pos 4
    Message_AddWord(pwr_ctrl_adc_data.drv_adc_val_AN15);
    //Message pos 5
    Message_AddWord(pwr_ctrl_adc_data.drv_adc_val_FB_Vaux);
    

    //Message pos 6
    Message_AddWord(1 << (pwr_ctrl_state - 1));

    //Message pos 7 ILLCFaults.CollectiveFaults
    Message_AddWord(ILLCFaults.CollectiveFaults);

    //Message pos 8
    if (App_HMI_useRefFromPoti) pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits |= 0x0001;
    else pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits &= 0xFFFE;
    if (App_HMI_useRefFromGUI) pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits |= 0x0002;
    else pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits &= 0xFFFD;
    if (App_HMI_useFixedRef) pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits |= 0x0004;
    else pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits &= 0xFFFB;
    Message_AddWord(pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits);

    //Message pos 9 Debug Value
    Message_AddWord(pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT);

    //Message pos 10
    Message_AddWord(pwr_ctrl_adc_data.drv_adc_val_FB_P_CT2_FLT);

    //Message pos 11
    if (pwr_ctrl_flagbits.SR1on)
      Message_AddWord(PG2PHASE);
    else
      Message_AddWord(0);

    //Message pos 12
    if (pwr_ctrl_flagbits.SR2on)
      Message_AddWord(PG4PHASE);
    else
      Message_AddWord(0);

    //Message pos 13
    Message_AddWord(PG1PER);

//    //Message pos 14
//    Message_AddWord(pwr_ctrl_ref_data.val_VoutRef_internal);

    //Message pos 14
    Message_AddWord(1 << (sr_ctrl_state - 1));

    //Message pos 15
    Message_AddWord(pwr_ctrl_adc_data.drv_adc_val_FB_Temp);

    UART_RxTx.TXSendDone = 0;
  }
}

//======================================================================================================================
// @brief this function contains the initialization of the data stream and protocol handling
//======================================================================================================================

void UARTTxInit(void)
{
  uint8_t i_;

  UART_RxTx.TXBytes[0] = 0x55;
  UART_RxTx.TXBytes[1] = 0x00;
  UART_RxTx.TXBytes[2] = 0x01;

//  UART_RxTx.UartSendLength = 34;
  UART_RxTx.UartSendLength = 32;
  UART_RxTx.TXBytes[3] = 0;
  UART_RxTx.TXBytes[4] = UART_RxTx.UartSendLength;

  for (i_ = 5; i_ < (UART_RxTx.UartSendLength + 5u); i_++)
  {
    UART_RxTx.TXBytes[i_] = i_;
  }
  UART_RxTx.SendCRC = 0;

  UART_RxTx.TXBytes[UART_RxTx.UartSendLength + 5u] = 0x00;
  UART_RxTx.TXBytes[UART_RxTx.UartSendLength + 6u] = 0x00;
  UART_RxTx.TXBytes[UART_RxTx.UartSendLength + 7u] = 0x0D;
}

//======================================================================================================================
// @brief this function contains the receive handler if a complete protocol message is received
//======================================================================================================================

void UARTreceive(void)
{
  uint16_t i_, j_, CRC_;

  UART_RxTx.ReceiveCRC = 0;
  for (j_ = 0; j_ < (UART_RxTx.UartRecLength + 5); j_++)
  {
    UART_RxTx.ReceiveCRC ^= UART_RxTx.RXBytes[j_];
    for (i_ = 0; i_ < 8; ++i_)
    {
      if ((UART_RxTx.ReceiveCRC & 1) == 1)
        UART_RxTx.ReceiveCRC = (UART_RxTx.ReceiveCRC >> 1) ^ 0xA001;
      else
        UART_RxTx.ReceiveCRC = (UART_RxTx.ReceiveCRC >> 1);
    }
  }
  CRC_ = (UART_RxTx.RXBytes[UART_RxTx.UartRecLength + 5] << 8) | \
            UART_RxTx.RXBytes[UART_RxTx.UartRecLength + 6];

  if (UART_RxTx.ReceiveCRC == CRC_)
  {
    if ((UART_RxTx.UartRecActionID == 0x40EF) && (UART_RxTx.UartRecLength == 4))
    {
      //pwr_ctrl_adc_data.drv_comm_val_VoutRef = ((UART_RxTx.RXBytes[5] & 0x7F) << 8) + UART_RxTx.RXBytes[6];
      uint16_t Gui_VoutRef = ((UART_RxTx.RXBytes[5] & 0x7F) << 8) + UART_RxTx.RXBytes[6];

      if ((UART_RxTx.RXBytes[5] & 0x80) > 0)
      {
        Drv_PwrCtrl_ILLC_SetReferenceRaw(Gui_VoutRef); // raw adc vaule
        App_HMI_useRefFromPoti = false;
        App_HMI_useRefFromGUI = true;
        App_HMI_useFixedRef = false;
      }
      if ((UART_RxTx.RXBytes[5] & 0x80) == 0)
      {
        if (!App_HMI_useFixedRef)
          App_HMI_useRefFromPoti = true;

        App_HMI_useRefFromGUI = false;
        App_HMI_useFixedRef = false;
      }

      //      VMC3p3z.maxOutput = ((UART_RxTx.RXBytes[5] & 0x7F) << 8) + UART_RxTx.RXBytes[6];
    }
    UART_RxTx.UartRecActionID = 0;
  }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
