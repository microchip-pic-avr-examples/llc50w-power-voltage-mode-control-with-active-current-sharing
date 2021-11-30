/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    This software and any accompanying information is for suggestion only. It
    does not modify Microchip's standard warranty for its products. You agree
    that you are solely responsible for testing the software and determining its
    suitability. Microchip has no obligation to modify, test, certify, or
    support the software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY I
 * 025MPLIED
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
#include <stdint.h>
#include "drv_pwrctrl_ILLC.h"
#include "drv_pwrctrl_ILLC_internal.h"
#include "sources/driver/drv_adc.h"
#include "mcc_generated_files/pwm.h"
#include "mcc_generated_files/pin_manager.h"
#include "sources/app/App_HMI.h"
#include "Controller_pwrctrl_ILLC.h"
#include "drv_pwrctrl_ILLC_settings.h"

PWR_CTRL_REFDATA_t pwr_ctrl_ref_data;

//==============================================================================
// @brief this function contains 
//==============================================================================

void Drv_PwrCtrl_ILLC_Start(void)
{
  while (pwr_ctrl_flagbits.run == 0) //it needs to be a while function to avoid interrupt access conflicts
  {
    pwr_ctrl_flagbits.run = 1;
  }
}

//==============================================================================
// @brief this function contains 
//==============================================================================

void Drv_PwrCtrl_ILLC_Stop(void)
{
  while (pwr_ctrl_flagbits.run == 1) //it needs to be a while function to avoid interrupt access conflicts
  {
    pwr_ctrl_flagbits.run = 0;
  }
}
//==============================================================================
// @brief this function contains 
//==============================================================================

void Drv_PwrCtrl_ILLC_SetMode_OpenLoop(void)
{
  //it needs to be a while function to avoid interrupt access conflicts
  while (pwr_ctrl_flagbits.inopenloop == 0 || pwr_ctrl_flagbits.inclosedloop == 1)
  {
    pwr_ctrl_flagbits.inopenloop = 1;
    pwr_ctrl_flagbits.inclosedloop = 0;
  }
}

//==============================================================================
// @brief this function contains 
//==============================================================================

void Drv_PwrCtrl_ILLC_SetMode_ClosedLoop(void)
{
  //it needs to be a while function to avoid interrupt access conflicts
  while (pwr_ctrl_flagbits.inopenloop == 1 || pwr_ctrl_flagbits.inclosedloop == 0)
  {
    pwr_ctrl_flagbits.inopenloop = 0;
    pwr_ctrl_flagbits.inclosedloop = 1;
  }
}

//==============================================================================
// @brief this function contains 
//==============================================================================

void Drv_PwrCtrl_ILLC_SetReferenceRaw(uint16_t ref) //@FTX
{
    if (pwr_ctrl_flagbits.inclosedloop)
    {
        if(ref < VREF_FIXED8V)          // 2308
            ref = VREF_FIXED8V;
        else if(ref > VREF_FIXED12V)    // 3463
            ref = VREF_FIXED12V;
    }
    pwr_ctrl_ref_data.drv_val_VoutRef = ref;
}

//==============================================================================
// @brief this function contains 
//==============================================================================

void Drv_PwrCtrl_ILLC_SetReference_mV(uint16_t ref_mV)
{
  //Todo
}

//==============================================================================
// @brief this function contains 
//==============================================================================

void StartUpMaxFrequencyDCSweep(uint16_t PhaseX)
{
  uint16_t DCzwA_, DCzwB_;
  
  //----------------------------------------------------------------------------  
  if (PhaseX == 0x0A)
  {
    if (PG1DC + SOFT_START_RAMP_SPEED_DC < ((PG1PER >> 1) - DTH_L))
    {
      DCzwA_ = PG1DC + SOFT_START_RAMP_SPEED_DC;
      PG1TRIGA = PG1PER >> 1;
      PG1TRIGB = PG1TRIGA + DCzwA_;
      PG1DC = DCzwA_;
    }
    else
    {
      DCzwA_ = (PG1PER >> 1) - DTH_L;
      PG1TRIGA = PG1PER >> 1;
      PG1TRIGB = PG1TRIGA + DCzwA_;
      PG1DC = DCzwA_;

      pwr_ctrl_state = PCS_SOFT_START_PRE2;
      if (pwr_ctrl_flagbits.inclosedloop)
      {
        pwr_ctrl_ref_data.val_VoutRef_internal = pwr_ctrl_adc_data.drv_adc_val_FB_Vout;
      }
      else
      {
        pwr_ctrl_ref_data.val_VoutRef_internal = Periodmin_fmax;
      }
    }
  }
  //----------------------------------------------------------------------------  
  if (PhaseX == 0x0B)
  {
    if (PG3DC + SOFT_START_RAMP_SPEED_DC < ((PG3PER >> 1) - DTH_L))
    {
      DCzwB_ = PG3DC + SOFT_START_RAMP_SPEED_DC;
      PG3TRIGA = PG3PER >> 1;
      PG3TRIGB = PG3TRIGA + DCzwB_;
      PG3DC = DCzwB_;
    }
    else
    {
      DCzwB_ = (PG3PER >> 1) - DTH_L;
      PG3TRIGA = PG3PER >> 1;
      PG3TRIGB = PG3TRIGA + DCzwB_;
      PG3DC = DCzwB_;

      pwr_ctrl_state = PCS_SOFT_START_PRE2;
      if (pwr_ctrl_flagbits.inclosedloop)
      {
        pwr_ctrl_ref_data.val_VoutRef_internal = pwr_ctrl_adc_data.drv_adc_val_FB_Vout;
      }
      else
      {
        pwr_ctrl_ref_data.val_VoutRef_internal = Periodmin_fmax;
      }
    }
  }
  
    //----------------------------------------------------------------------------  
  if (PhaseX == 0x0C)
  {
    PG2TRIGC = PG1PER >> 2; //SOC trigger for PHB/PG3
    if (PG1DC + SOFT_START_RAMP_SPEED_DC < ((PG1PER >> 1) - DTH_L))
    {
      DCzwA_ = PG1DC + SOFT_START_RAMP_SPEED_DC;
      PG1TRIGA = PG1PER >> 1;
      PG1TRIGB = PG1TRIGA + DCzwA_;
      PG1DC = DCzwA_;
            
      DCzwB_ = PG3DC + SOFT_START_RAMP_SPEED_DC;
      PG3TRIGA = (PG3PER >> 1);
      PG3TRIGB = PG3TRIGA + DCzwB_;
      PG3DC = DCzwB_;
    }
    else
    {
      DCzwA_ = (PG1PER >> 1) - DTH_L;
      PG1TRIGA = PG1PER >> 1;
      PG1TRIGB = PG1TRIGA + DCzwA_;
      PG1DC = DCzwA_;
      
      DCzwB_ = (PG3PER >> 1) - DTH_L;
      PG3TRIGA = (PG3PER >> 1);
      PG3TRIGB = PG3TRIGA + DCzwB_;
      PG3DC = DCzwB_;

      pwr_ctrl_state = PCS_SOFT_START_PRE2;
      if (pwr_ctrl_flagbits.inclosedloop)
      {
        pwr_ctrl_ref_data.val_VoutRef_internal = pwr_ctrl_adc_data.drv_adc_val_FB_Vout;
      }
      else
      {
        pwr_ctrl_ref_data.val_VoutRef_internal = Periodmin_fmax;
      }
    }
    
//    PG7PER = PG1PER;
//    PG7DC = PG7PER >> 1;
//    PG7TRIGA = PG7PER >> 2;
            
  }

  //----------------------------------------------------------------------------  

}

//==============================================================================
// @brief this function contains 
//==============================================================================

void StartUpFrequencySweepCL(void)
{
    static uint16_t SweepCLCounter = 0;

    if (SweepCLCounter++ > 3)
    {
        if (pwr_ctrl_ref_data.val_VoutRef_internal <= pwr_ctrl_ref_data.drv_val_VoutRef)
        {
            if (((pwr_ctrl_ref_data.val_VoutRef_internal + SOFT_START_RAMP_SPEED_FR) < pwr_ctrl_ref_data.drv_val_VoutRef)
              && ((pwr_ctrl_ref_data.val_VoutRef_internal + SOFT_START_RAMP_SPEED_FR) >= SOFT_START_RAMP_SPEED_FR)) //@FTX check is important to be > 0 all the time
            {
                pwr_ctrl_ref_data.val_VoutRef_internal += SOFT_START_RAMP_SPEED_FR;
            }
            else
            {
                pwr_ctrl_state = PCS_UP_AND_RUNNING;
            }
        }
        else
        {
            if (((pwr_ctrl_ref_data.val_VoutRef_internal + SOFT_START_RAMP_SPEED_FR) > pwr_ctrl_ref_data.drv_val_VoutRef)
              && ((pwr_ctrl_ref_data.val_VoutRef_internal + SOFT_START_RAMP_SPEED_FR) > SOFT_START_RAMP_SPEED_FR)) //@FTX check is important to be > 0 all the time
            {
                pwr_ctrl_ref_data.val_VoutRef_internal -= SOFT_START_RAMP_SPEED_FR;
            }
            else
            { //wird nie erreicht !!!
                pwr_ctrl_ref_data.val_VoutRef_internal = pwr_ctrl_ref_data.drv_val_VoutRef;
                pwr_ctrl_state = PCS_UP_AND_RUNNING;
            }
        }
        SweepCLCounter = 0;
    }
 }

//==============================================================================
// @brief this function contains 
//==============================================================================

void PreBias(void)
{

}

//==============================================================================
// @brief this function contains 
//==============================================================================

bool CheckForBoardSetup (void)
{
  static uint16_t CFB_task_counter = 0, ADCvaluePD, ADCvaluePU;
  
  switch (CFB_task_counter++)
  {
    case 1:
      CNPDCbits.CNPDC3 = 0;
      CNPUCbits.CNPUC3 = 0;
      break;
    
    case 2:
      CNPDCbits.CNPDC3 = 1;
      CNPUCbits.CNPUC3 = 0;
      break;

    case 4:
      ADCvaluePD = pwr_ctrl_adc_data.drv_adc_val_AN15;
      CNPDCbits.CNPDC3 = 0;
      CNPUCbits.CNPUC3 = 1;
      break;
    
    case 6:
      ADCvaluePU = pwr_ctrl_adc_data.drv_adc_val_AN15;
      if((ADCvaluePD < 200) && (ADCvaluePU > 3900))
      {  
        App_HMI_useRefFromPoti = false;
        App_HMI_useFixedRef = true;
      }
      else
      {
        App_HMI_useRefFromPoti = true;
        App_HMI_useFixedRef = false;        
      }  
      CNPDCbits.CNPDC3 = 0;
      CNPUCbits.CNPUC3 = 0;
      CFB_task_counter = 0;
      break;
  }
  
  if(CFB_task_counter > 0)
    return (1);
  else
    return (0);
}

//==============================================================================
//==============================================================================
