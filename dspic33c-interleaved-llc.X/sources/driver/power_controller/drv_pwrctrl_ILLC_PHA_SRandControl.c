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
#include "drv_pwrctrl_ILLC_PHA_SRandControl.h"
#include "drv_pwrctrl_ILLC_settings.h"
#include "drv_pwrctrl_ILLC_fault.h"

#include "Controller_pwrctrl_ILLC.h"

uint16_t phase2zw;

//==============================================================================
// @brief|| this function contains  
// @brief|| 
//==============================================================================
void Drv_PwrCtrl_ILLC_PH1SR_Adaptive_Enable(void)
{
//  if ((pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT > IOUT_SRON) && (!pwr_ctrl_flagbits.SR1on))
  if ((!pwr_ctrl_flagbits.SR1on)) //EMI Test
  {
    phase2zw = (((PG1PER >> 1) - DTH_L) - SRStartDC);
    pwr_ctrl_flagbits.SR1on = 1;
    Drv_PwrCtrl_ILLC_PH1SR_Adaptive_SetPWMValues(phase2zw);
    
    sr_ctrl_state = SRCS_ENABLE;
  }
}

void Drv_PwrCtrl_ILLC_PH1SR_Adaptive_Disable(void)
{
  if ((pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT < IOUT_SROFF) && (pwr_ctrl_flagbits.SR1on))
  {
    ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG2IOCONL);
    pwr_ctrl_flagbits.SR1on = 0;

    sr_ctrl_state = SRCS_STANDBY;
  }

}

//==============================================================================
// @brief|| this function contains  
// @brief|| 
//==============================================================================

void Drv_PwrCtrl_ILLC_PH1SR_Ramp(void)
{
  phase2zw -= SRPHRampStep;

  if (phase2zw <= SRDelay)
  {
    phase2zw = SRDelay;
  }
  Drv_PwrCtrl_ILLC_PH1SR_Adaptive_SetPWMValues(phase2zw);
}

//==============================================================================
// @brief|| this function contains  
// @brief|| 
//==============================================================================

void Drv_PwrCtrl_ILLC_PH1SR_Adaptive_Drive(void)
{
  static uint16_t loopCounterSR_ = 0;

  if ((loopCounterSR_++ > 3) && (PG2PHASE > SRDelay))
  {
    phase2zw -= SRPHRampStep;
    if (phase2zw <= SRDelay) phase2zw = SRDelay;
    loopCounterSR_ = 0;
  }

  Drv_PwrCtrl_ILLC_PH1SR_Adaptive_SetPWMValues(phase2zw);
  
}

//==============================================================================
// @brief|| this function contains the AVG Current Controller (every 4th PWM cycle) 
// @brief|| and the Voltage Controller every x-PWM cycles
//==============================================================================

void Drv_PwrCtrl_ILLC_PH1VoltageLoop(void)
{
  static uint16_t SRCounterSR1 = 0;

  //Debug DACout------------------------------------------------------------------
  //    CMP2_SetDACDataHighValue(PG1PER>>1);
  //Debug DACout------------------------------------------------------------------

  //VLOOP  ---------------------------------------------------------------------
  //700ns--  
  Controller_ILLC._VDCref = pwr_ctrl_ref_data.val_VoutRef_internal;
  SMPS_Controller2P2ZUpdate(&VMC2p2z, &pwr_ctrl_adc_data.drv_adc_val_FB_Vout, Controller_ILLC._VDCref, &Controller_ILLC._VoltageControllerResultPH1);
  //--

  // handle PWMs for primary switches and SR switches --------------------------
  if (pwr_ctrl_state > PCS_SOFT_START_PRE2)
  {
    if (pwr_ctrl_flagbits.inclosedloop) //closed loop
    {
      //400ns
      ILLC_PHASE1_PWM_UPDATE_PER_TRIGA_TRIGB_TRIGC(Controller_ILLC._VoltageControllerResultPH1 & 0xFFF8); //0xFFF8 needed !!
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
    }

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
            Drv_PwrCtrl_ILLC_PH1SR_Adaptive_Enable();
          }
          break;
          
          //check for phase with higher current and enable PWM after delaying by 1 ISR
        case SRCS_ENABLE:
          if (pwr_ctrl_flagbits.SR1on && PG2IOCONLbits.OVRENH && (SRCounterSR1++ > 0))
          {
            ILLC_PWMx_OVERRIDE_DISABLE((uint16_t*) & PG2IOCONL);
            SRCounterSR1 = 0;
            sr_ctrl_state = SRCS_SOFT_START;
          }
          break;

          //ramp to min delay
        case SRCS_SOFT_START:
          Drv_PwrCtrl_ILLC_PH1SR_Ramp();
          if (phase2zw == SRDelayIL)
          {
            sr_ctrl_state = SRCS_UP_AND_RUNNING;
          }
          break;

          //SR handling with current sharing is up and running
        case SRCS_UP_AND_RUNNING:
          Drv_PwrCtrl_ILLC_PH1SR_Adaptive_Drive();
////EMI Test comment out          Drv_PwrCtrl_ILLC_PH1SR_Adaptive_Disable();
          break;

        default:
          break;
      }
      //END SR state machine 
    }
    //END SR 
    
    ILLC_PHASE1_PWM_UPDATE_REGS(); //Update all PG1 and PG2 regs
  }
  // END handle PWMs for primary switches and SR switches ----------------------

  //handle light load detect: VoutRef && PG1PER = 1MHz MOVE TO SCHEDULER OUT OF ISR
  //100ns
  if ((PG1PER == 4000) && (pwr_ctrl_ref_data.val_VoutRef_internal <
          (pwr_ctrl_adc_data.drv_adc_val_FB_Vout - (pwr_ctrl_adc_data.drv_adc_val_FB_Vout >> 5)))
          && !pwr_ctrl_flagbits.SR1on)
  {
    if (pwr_ctrl_state == PCS_UP_AND_RUNNING)
    {
      pwr_ctrl_flagbits.lightloadmode = 1;
      ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG1IOCONL);
    }
  }
  //END handle light load detect: VoutRef && PG1PER = 1MHz

}

//==============================================================================
//==============================================================================
