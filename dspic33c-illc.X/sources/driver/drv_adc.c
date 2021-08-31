//=======================================================================================================
// Copyright(c) 2018 Microchip Technology Inc. and its subsidiaries.
// Subject to your compliance with these terms, you may use Microchip software and any derivatives
// exclusively with Microchip products. It is your responsibility to comply with third party license
// terms applicable to your use of third-party software (including open source software) that may
// accompany Microchip software.
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
// APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND
// FITNESS FOR A PARTICULAR PURPOSE.
// IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
// LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
// MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT
// ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT
// EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
//=======================================================================================================

//=======================================================================================================
// @file 
//
// @brief contains the functions for ADC measurements
//
// @version v1.0
// @date 2020-07-15
// @author M52409
//
//=======================================================================================================

#include <p33CK256MP506.h>

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "drv_adc.h"
#include "power_controller/drv_pwrctrl_ILLC_internal.h"
#include "mcc_generated_files/sccp1_capture.h"
#include "power_controller/drv_pwrctrl_ILLC_fault.h"
#include "../project_settings.h"

#define ADC_CHANNEL_AN06_FB_VAUX      6
#define ADC_CHANNEL_AN12_FB_VOUT      12
#define ADC_CHANNEL_AN14_FB_TEMP      14
#define ADC_CHANNEL_AN15_POTIREF      15
#define ADC_CHANNEL_AN17_FB_P_CT2_FLT 17  
#define ADC_CHANNEL_AN19_FB_P_CT1_FLT 19

PWR_CTRL_DATA_t pwr_ctrl_adc_data;

//-------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------

void __attribute__((__interrupt__, auto_psv)) _ADCAN0Interrupt(void)
{
  static uint32_t s_ct1_flt = 1;
  uint16_t SHRChannel_zw;

  TP34_RX_SetHigh();
  
  //200ns
  pwr_ctrl_adc_data.drv_adc_val_FB_S_CT1_FLT = ADCBUF0;
  s_ct1_flt += pwr_ctrl_adc_data.drv_adc_val_FB_S_CT1_FLT;

  //moving average filtering of I1sec ------------------------------------------
  //see excel file for details
  pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT = (uint16_t) (s_ct1_flt >> 4);
  s_ct1_flt -= pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT;
  //--

  tp26_SetHigh();
  SHRChannel_zw = ADCON3Lbits.CNVCHSEL;
  ADCON3Lbits.CNVCHSEL = ADC_CHANNEL_AN12_FB_VOUT;
  while (!ADSTATLbits.AN12RDY);
  pwr_ctrl_adc_data.drv_adc_val_FB_Vout = ADCBUF12;
  tp26_SetLow();
#if defined POWER_STAGE_CONFIG_PHASE_A_ONLY  
  Drv_PwrCtrl_ILLC_PH1VoltageLoop();
#endif
#if defined POWER_STAGE_CONFIG_INTERLEAVED  
  Drv_PwrCtrl_ILLC_ILPHVoltageLoop();
#endif
  tp26_SetLow();
  
  if(SHRChannel_zw == ADC_CHANNEL_AN15_POTIREF)
    ADCON3Lbits.CNVCHSEL = ADC_CHANNEL_AN19_FB_P_CT1_FLT;
  
  else if(SHRChannel_zw == ADC_CHANNEL_AN19_FB_P_CT1_FLT)
    ADCON3Lbits.CNVCHSEL = ADC_CHANNEL_AN06_FB_VAUX;
  
  else if(SHRChannel_zw == ADC_CHANNEL_AN06_FB_VAUX)
    ADCON3Lbits.CNVCHSEL = ADC_CHANNEL_AN17_FB_P_CT2_FLT;

  else if(SHRChannel_zw == ADC_CHANNEL_AN17_FB_P_CT2_FLT)
    ADCON3Lbits.CNVCHSEL = ADC_CHANNEL_AN14_FB_TEMP;
   
  else if(SHRChannel_zw == ADC_CHANNEL_AN14_FB_TEMP)
    ADCON3Lbits.CNVCHSEL = ADC_CHANNEL_AN15_POTIREF;
   
  else  
   ADCON3Lbits.CNVCHSEL = ADC_CHANNEL_AN12_FB_VOUT;

  switch (ADCON3Lbits.CNVCHSEL)
  { //320ns max
    case ADC_CHANNEL_AN15_POTIREF: //Read the ADC value from the ADCBUF15
      while (!ADSTATLbits.AN15RDY);
    {
      pwr_ctrl_adc_data.drv_adc_val_AN15 = ADCBUF15;
    }
      break;

    case ADC_CHANNEL_AN19_FB_P_CT1_FLT: //Read the ADC value from the ADCBUF19
      while (!ADSTATHbits.AN19RDY);
    {
      pwr_ctrl_adc_data.drv_adc_val_FB_P_CT1_FLT = ADCBUF19;
    }
      break;

    case ADC_CHANNEL_AN06_FB_VAUX: //Read the ADC value from the ADCBUF6
      while (!ADSTATLbits.AN6RDY);
    {
      pwr_ctrl_adc_data.drv_adc_val_FB_Vaux = ADCBUF6;
    }
      break;

   case ADC_CHANNEL_AN14_FB_TEMP: //Read the ADC value from the ADCBUF14
      while (!ADSTATLbits.AN14RDY);
    {
      pwr_ctrl_adc_data.drv_adc_val_FB_Temp = ADCBUF14;
    }
      break;

    default:
      break;
  }

  IFS5bits.ADCAN0IF = 0; //clear the channel_AN0 interrupt flag

  TP34_RX_SetLow();
}

//-------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------

void __attribute__((__interrupt__, auto_psv)) _ADCAN1Interrupt(void)
{

  static uint32_t s_ct2_flt = 1;

  TP36_TX_SetHigh();

  pwr_ctrl_adc_data.drv_adc_val_FB_S_CT2_FLT = ADCBUF1;
  s_ct2_flt += pwr_ctrl_adc_data.drv_adc_val_FB_S_CT2_FLT;

  //moving average filtering of I1sec ------------------------------------------
  //see excel file for details
  pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT = (uint16_t) (s_ct2_flt >> 4);
  s_ct2_flt -= pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT;

  if ((ADCON3Lbits.CNVCHSEL == ADC_CHANNEL_AN17_FB_P_CT2_FLT))// && (ADSTATHbits.AN17RDY))
  {
    while(!ADSTATHbits.AN17RDY);
    pwr_ctrl_adc_data.drv_adc_val_FB_P_CT2_FLT = ADCBUF17;
//    ADCON3Lbits.CNVCHSEL = ADC_CHANNEL_AN15_POTIREF; //ADC_CHANNEL_AN14_FB_TEMP;
  }

  IFS5bits.ADCAN1IF = 0;

  TP36_TX_SetLow();
}

//-------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------

void SCCP1_CAPTURE_CallBack(void)
{
  uint16_t i_;
  static uint16_t VDDarr[4];

  if (CCP1STATLbits.ICOV)
  {
    i_ = 0;
    while (CCP1STATLbits.ICBNE)
    {
      VDDarr[i_++] = CCP1BUFL;
    }
    pwr_ctrl_adc_data.drv_adc_val_FB_Vdd = VDDarr[2] - VDDarr[1];
    CCP1TMRH = 0;
    CCP1TMRL = 0;
  }
  IFS0bits.CCP1IF = 0;
  i_ = 0;
  while ((_RC4 == 1) && (i_++ < 100));
  //  if (i_ > 98)
  //    ILLCFaults.Catastrophic = 1;

}

//-------------------------------------------------------------------------------------------------------
// S_CT1 overcurrent switch off pair wise
//-------------------------------------------------------------------------------------------------------

void CMP1_CallBack(void)
{
//  tp24_SetHigh();
 
  ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG1IOCONL); //ILLCPh1
  ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG2IOCONL); //ILLCPh1SR
  ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG3IOCONL); //ILLCPh2
  ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG4IOCONL); //ILLCPh2SR

  pwr_ctrl_state = PCS_WAIT_IF_FAULT_ACTIVE;
  ILLCFaults.Catastrophic = 1;

  IFS4bits.CMP1IF = 0;
//  tp24_SetLow();
}

//-------------------------------------------------------------------------------------------------------
//  S_CT2 overcurrent switch off pair wise
//-------------------------------------------------------------------------------------------------------

void CMP3_CallBack(void)
{
//  tp24_SetHigh();
  ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG1IOCONL); //ILLCPh1
  ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG2IOCONL); //ILLCPh1SR
  ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG3IOCONL); //ILLCPh2
  ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG4IOCONL); //ILLCPh2SR
  pwr_ctrl_state = PCS_WAIT_IF_FAULT_ACTIVE;
  ILLCFaults.Catastrophic = 1;

  IFS4bits.CMP3IF = 0;
//  tp24_SetLow();
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------

