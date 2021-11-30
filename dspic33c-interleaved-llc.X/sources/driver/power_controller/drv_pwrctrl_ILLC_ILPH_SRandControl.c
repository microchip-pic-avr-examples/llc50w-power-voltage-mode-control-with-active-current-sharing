/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    This software and any accompanying information is for suggestion only. It
    does not modify Microchip's standard warranty for its products. You agree
    that you are solely responsible for testing the software and determining its
    suitability. Microchip has no obligation to modify, test, certify, or
    support the software.

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
#include <stdint.h>

#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/pwm.h"
#include "mcc_generated_files/cmp2.h"

#include "drv_pwrctrl_ILLC.h"
#include "drv_pwrctrl_ILLC_internal.h"
#include "driver/drv_adc.h"
#include "Controller_pwrctrl_ILLC.h"
#include "drv_pwrctrl_ILLC_ILPH_SRandControl.h"
#include "drv_pwrctrl_ILLC_settings.h"
#include "drv_pwrctrl_ILLC_fault.h"

#include "Controller_pwrctrl_ILLC.h"
#include "device/dev_gui_comm.h"

static uint16_t phaseIL2zw, phaseIL4zw;

//==============================================================================
// @brief|| this function contains  
// @brief|| 
//==============================================================================

void Drv_PwrCtrl_ILLC_ILSR_Adaptive_Enable(void)
{
  if (((pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT + pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT) > IOUT_SRONIL)
          && (!pwr_ctrl_flagbits.SR1on) && (!pwr_ctrl_flagbits.SR2on))
  {
//    TP192_SetHigh();
    phaseIL2zw = (((PG1PER >> 1) - DTH_L) - SRStartDC);
    pwr_ctrl_flagbits.SR1on = 1;

    phaseIL4zw = (((PG3PER >> 1) - DTH_L) - SRStartDC);
    pwr_ctrl_flagbits.SR2on = 1;

    Drv_PwrCtrl_ILLC_ILSR_Adaptive_SetPWMValues(phaseIL2zw, phaseIL4zw);

    sr_ctrl_state = SRCS_ENABLE;
//    TP192_SetLow();
  }
}

void Drv_PwrCtrl_ILLC_ILSR_Adaptive_Disable(void)
{
  if (((pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT + pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT) < IOUT_SROFFIL)
          && (pwr_ctrl_flagbits.SR1on) && (pwr_ctrl_flagbits.SR2on))
  {
    ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG2IOCONL);
    pwr_ctrl_flagbits.SR1on = 0;

    ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG4IOCONL);
    pwr_ctrl_flagbits.SR2on = 0;

    pwr_ctrl_flagbits.SRControlPHA = 0;
    pwr_ctrl_flagbits.SRControlPHB = 0;

    sr_ctrl_state = SRCS_STANDBY;
  }

}
//==============================================================================
// @brief|| this function contains  
// @brief|| 
//==============================================================================

//==============================================================================

void Drv_PwrCtrl_ILSR_Ramp(void)
{
  phaseIL2zw -= SRPHRampStep;
  if (phaseIL2zw <= SRDelayIL)
  {
    phaseIL2zw = SRDelayIL;
  }

  phaseIL4zw -= SRPHRampStep;
  if (phaseIL4zw <= SRDelayIL)
  {
    phaseIL4zw = SRDelayIL;
  }

  Drv_PwrCtrl_ILLC_ILSR_Adaptive_SetPWMValues(phaseIL2zw, phaseIL4zw);
}

//==============================================================================
// @brief|| this function contains  
// @brief|| 
//==============================================================================

void Drv_PwrCtrl_ILSR_Check_Current_Sharing(void)
{
  if (pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT > pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT)
  {
    pwr_ctrl_flagbits.SRControlPHA = 1;
    pwr_ctrl_flagbits.SRControlPHB = 0;
  }
  else
  {
    pwr_ctrl_flagbits.SRControlPHB = 1;
    pwr_ctrl_flagbits.SRControlPHA = 0;
  }
}

//==============================================================================
// @brief|| this function contains  
// @brief|| 
//==============================================================================

void Drv_PwrCtrl_ILLC_ILSR_Adaptive_Drive(void)
{
  static uint16_t loopCounterSR_ = 0;

  if (loopCounterSR_++ > 2)  //63
  {
    if (pwr_ctrl_flagbits.SRControlPHA)
    {
      if (pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT > (pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT + (pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT >> 5)))
      {
        phaseIL2zw += SRPHRampStepCurrentBalance;
        if (phaseIL2zw >= (((PG2PER >> 1) - DTH_L) - SRStartDC))
        {
          phaseIL2zw = (((PG2PER >> 1) - DTH_L) - SRStartDC);
        }    
      }
      else if (pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT < (pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT - (pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT >> 5)))
      {
        phaseIL2zw -= SRPHRampStepCurrentBalance;
        if (phaseIL2zw <= SRDelayIL) phaseIL2zw = SRDelayIL;
      }
    }

    if (pwr_ctrl_flagbits.SRControlPHB)
    {
      if (pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT > (pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT + (pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT >> 5)))
      {
//        TP194_SetHigh();
        phaseIL4zw += SRPHRampStepCurrentBalance;
        if (phaseIL4zw >= (((PG4PER >> 1) - DTH_L) - SRStartDC))
        {
          phaseIL4zw = (((PG4PER >> 1) - DTH_L) - SRStartDC);
        }
      }
      else if (pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT < (pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT - (pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT >> 5)))
      {
//        TP194_SetHigh();
        phaseIL4zw -= SRPHRampStepCurrentBalance;
        if (phaseIL4zw <= SRDelayIL) phaseIL4zw = SRDelayIL;
//        TP194_SetLow();
      }
    }

    loopCounterSR_ = 0;
  }

  Drv_PwrCtrl_ILLC_ILSR_Adaptive_SetPWMValues(phaseIL2zw, phaseIL4zw);
}

//==============================================================================
// @brief|| this function contains the AVG Current Controller (every 4th PWM cycle) 
// @brief|| and the Voltage Controller every x-PWM cycles
//==============================================================================

void Drv_PwrCtrl_ILLC_ILPHVoltageLoop(void)
{
  static uint16_t SRCounterSR1 = 0, SRCounterSR2 = 0;

  //VLOOP  ---------------------------------------------------------------------
  //700ns--  
  Controller_ILLC._VDCref = pwr_ctrl_ref_data.val_VoutRef_internal;
  SMPS_Controller2P2ZUpdate(&VMC2p2z, &pwr_ctrl_adc_data.drv_adc_val_FB_Vout, Controller_ILLC._VDCref, &Controller_ILLC._VoltageControllerResultILPH);

  //--

  // handle PWMs for primary switches and SR switches --------------------------
  if (pwr_ctrl_state > PCS_SOFT_START_PRE2)
  {
    if (pwr_ctrl_flagbits.inclosedloop) //closed loop
    {
      //400ns
      ILLC_PHASE1_PWM_UPDATE_PER_TRIGA_TRIGB_TRIGC(Controller_ILLC._VoltageControllerResultILPH & 0xFFF8); //0xFFF8 needed !!
      ILLC_PHASE2_PWM_UPDATE_PER_TRIGA_TRIGB_TRIGC(Controller_ILLC._VoltageControllerResultILPH & 0xFFF8); //0xFFF8 needed !!
    }
    else //open loop
    {
      //-------------------------------------------
      // AN15:      0...........2048.........4096
      // AN15 Range:100......................3900
      // Period:    5000.....................4000
      // f:         800kHz...................1MHz
      //-------------------------------------------
      Controller_ILLC._OpenLoopPeriod = ((20160u - pwr_ctrl_ref_data.val_VoutRef_internal) >> 2); // & 0xFFF8; //4GHz-PWM
      ILLC_PHASE1_PWM_UPDATE_PER_TRIGA_TRIGB_TRIGC((Controller_ILLC._OpenLoopPeriod) & 0xFFF8); //0xFFF8 needed !!
      ILLC_PHASE2_PWM_UPDATE_PER_TRIGA_TRIGB_TRIGC((Controller_ILLC._OpenLoopPeriod) & 0xFFF8); //0xFFF8 needed !!
    }
    
//    TP193_SetHigh();
    ///*
    //SR enable check during softstart at Vin depending or at state RUN 
    //1070ns
    if (pwr_ctrl_state > PCS_POWER_ON_DELAY)
    {
      //SR stae machine
      switch (sr_ctrl_state)
      {
          //check for current/voltage
        case SRCS_STANDBY:
          if (((pwr_ctrl_flagbits.inclosedloop) && (pwr_ctrl_adc_data.drv_adc_val_FB_Vout > VOUT_SR_ACTIVE))
                  || (pwr_ctrl_flagbits.inopenloop))
          {
            Drv_PwrCtrl_ILLC_ILSR_Adaptive_Enable();
          }
          break;

          //check for phase with higher current and enable PWM after delaying by 1 ISR
        case SRCS_ENABLE:
          if ((pwr_ctrl_flagbits.SR1on && PG2IOCONLbits.OVRENH && (SRCounterSR1++ > 0)) &&
                  (pwr_ctrl_flagbits.SR2on && PG4IOCONLbits.OVRENH && (SRCounterSR2++ > 0)))
          {

            Drv_PwrCtrl_ILSR_Check_Current_Sharing();

            ILLC_PWMx_OVERRIDE_DISABLE((uint16_t*) & PG2IOCONL);
            ILLC_PWMx_OVERRIDE_DISABLE((uint16_t*) & PG4IOCONL);
            SRCounterSR2 = 0;
            SRCounterSR1 = 0;
            sr_ctrl_state = SRCS_SOFT_START;
          }
          break;

          //ramp to min delay
        case SRCS_SOFT_START:
//          TP194_SetHigh();
          Drv_PwrCtrl_ILSR_Ramp();
          if ((phaseIL2zw == SRDelayIL) && (phaseIL4zw == SRDelayIL) &&
                  (pwr_ctrl_flagbits.SRControlPHA || pwr_ctrl_flagbits.SRControlPHB))
          {
            sr_ctrl_state = SRCS_UP_AND_RUNNING;
          }
//          TP194_SetLow();
          break;

          //SR handling with current sharing is up and running
        case SRCS_UP_AND_RUNNING:
          Drv_PwrCtrl_ILLC_ILSR_Adaptive_Drive();
          Drv_PwrCtrl_ILLC_ILSR_Adaptive_Disable();
          break;

        default:
          break;
      }
      //END SR state machine 
//      TP193_SetHigh();
    }
    //END SR 
    //*/
//    TP193_SetLow();
    
    ILLC_PHASE1_PWM_UPDATE_REGS(); //Update all PG3 and PG4 regs
    ILLC_PHASE2_PWM_UPDATE_REGS(); //Update all PG1 and PG1 regs

  }
  // END handle PWMs for primary switches and SR switches ----------------------
  //TP193_SetHigh();
  //100ns
  if ((PG1PER == 4000 || PG3PER == 4000) && (pwr_ctrl_ref_data.val_VoutRef_internal <
          (pwr_ctrl_adc_data.drv_adc_val_FB_Vout - (pwr_ctrl_adc_data.drv_adc_val_FB_Vout >> 5)))
          && (!pwr_ctrl_flagbits.SR1on || !pwr_ctrl_flagbits.SR2on))
  {
    if (pwr_ctrl_state == PCS_UP_AND_RUNNING)
    {
      pwr_ctrl_flagbits.lightloadmode = 1;
      ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG1IOCONL);
      ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG3IOCONL);
    }
  }
  //END handle light load detect: VoutRef && PG1PER = 1MHz

  //Debug DACout----------------------------------------------------------------
  CMP2_SetDACDataHighValue(pwr_ctrl_ref_data.val_VoutRef_internal);
  //      CMP2_SetDACDataHighValue(pwr_ctrl_ref_data.drv_val_VoutRef);
  //Debug DACout----------------------------------------------------------------
}

//==============================================================================
//==============================================================================
