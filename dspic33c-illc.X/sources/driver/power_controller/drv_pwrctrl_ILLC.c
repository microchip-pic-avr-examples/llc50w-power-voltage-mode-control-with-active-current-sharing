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

#include "../mcc_generated_files/system.h"
#include "../mcc_generated_files/pin_manager.h"
#include "../mcc_generated_files/pwm.h"

#include "drv_pwrctrl_ILLC.h"
#include "drv_pwrctrl_ILLC_internal.h"
#include "device/dev_button.h"
#include "driver/drv_adc.h"

#include "Controller_pwrctrl_ILLC.h"

#include "drv_pwrctrl_ILLC_PHA_SRandControl.h"

#include "../sources/app/App_HMI.h"
#include "drv_pwrctrl_ILLC_fault.h"

#include "../project_settings.h"

//------------------------------------------------------------------------------

PWR_CTRL_STATE_e pwr_ctrl_state = 1;    //PCS_INIT
PWR_CTRL_FLAGBITS_t pwr_ctrl_flagbits;
SR_CTRL_STATE_e sr_ctrl_state = 1;      //SRCS_ENABLE
//==============================================================================
// @brief this function contains 
//==============================================================================

void Drv_PwrCtrl_ILLC_Init(void)
{
  pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits = 0;
  pwr_ctrl_flagbits.inopenloop = 1;
  pwr_ctrl_flagbits.enabled = 1;
    
#if defined POWER_STAGE_CONFIG_PHASE_A_ONLY
  pwr_ctrl_flagbits.PHAmode = 1; 
#endif
  
#if defined POWER_STAGE_CONFIG_INTERLEAVED
  pwr_ctrl_flagbits.ILmode = 1; 
#endif

  //============================================================================
  // @brief initialize Controllers
  VoltageController_PwrCtrl_ILLC_Init();
  //============================================================================
  //ONLY THAT PROCEDURE WORKS---------------------------------------------------
  //PENH/L must be enabled in MCC that PWM pins show up in Pin Module
  //The following procedure prevents glitch at both PWM startup
  // After that init the PWMs are in override mode
  //----------------------------------------------------------------------------
  PG1IOCONHbits.PENH = 0; // PWMxH Output Port Disable: PWM generator NOT controls the PWMxH output pin
  PG1IOCONHbits.PENL = 0; // PWMxL Output Port Disable: PWM generator NOT controls the PWMxL output pin
  PWM_GeneratorEnable(ILLCPh1);
  PG1IOCONHbits.PENH = 1; // PWMxH Output Port Enable: PWM generator controls the PWMxH output pin
  PG1IOCONHbits.PENL = 1; // WMxL Output Port Enable: PWM generator controls the PWMxL output pin
  //----------------------------------------------------------------------------
  PG2IOCONHbits.PENH = 0; // PWMxH Output Port Disable: PWM generator NOT controls the PWMxH output pin
  PG2IOCONHbits.PENL = 0; // PWMxL Output Port Disable: PWM generator NOT controls the PWMxL output pin
  PWM_GeneratorEnable(SRPh1);
  PG2IOCONHbits.PENH = 1; // PWMxH Output Port Enable: PWM generator controls the PWMxH output pin
  PG2IOCONHbits.PENL = 1; // WMxL Output Port Enable: PWM generator controls the PWMxL output pin
  //----------------------------------------------------------------------------
  PG3IOCONHbits.PENH = 0; // PWMxH Output Port Disable: PWM generator NOT controls the PWMxH output pin
  PG3IOCONHbits.PENL = 0; // PWMxL Output Port Disable: PWM generator NOT controls the PWMxL output pin
  PWM_GeneratorEnable(ILLCPh2);
  PG3IOCONHbits.PENH = 1; // PWMxH Output Port Enable: PWM generator controls the PWMxH output pin
  PG3IOCONHbits.PENL = 1; // WMxL Output Port Enable: PWM generator controls the PWMxL output pin
  //----------------------------------------------------------------------------
  PG4IOCONHbits.PENH = 0; // PWMxH Output Port Disable: PWM generator NOT controls the PWMxH output pin
  PG4IOCONHbits.PENL = 0; // PWMxL Output Port Disable: PWM generator NOT controls the PWMxL output pin
  PWM_GeneratorEnable(SRPh2);
  PG4IOCONHbits.PENH = 1; // PWMxH Output Port Enable: PWM generator controls the PWMxH output pin
  PG4IOCONHbits.PENL = 1; // WMxL Output Port Enable: PWM generator controls the PWMxL output pin

  Drv_PwrCtrl_ILLC_Init_Fault();
}

//==============================================================================
// @brief this function contains 
// @ftx document PWM startup signals; difference OL-CL
//==============================================================================

void Drv_PwrCtrl_ILLC_SwitchOnPWM(void)
{
  PWM_Disable(); // disable all PWMs: ILLCPh1, ILLCPh2, SRPh1, SRPh2

#if defined POWER_STAGE_CONFIG_PHASE_A_ONLY || defined POWER_STAGE_CONFIG_INTERLEAVED
  PWM_PeriodSet(ILLCPh1, Periodmin_fmax);
  PWM_TriggerACompareValueSet(ILLCPh1, PG1PER >> 1);
  PWM_TriggerBCompareValueSet(ILLCPh1, PG1TRIGA + DTH_L);
  PWM_DutyCycleSet(ILLCPh1, DTH_L);

  PWM_OverrideLowDisable(ILLCPh1); //enable PWM output on L   
  PWM_OverrideHighDisable(ILLCPh1); //enable PWM output on H
#endif

#if defined POWER_STAGE_CONFIG_INTERLEAVED
  PWM_PeriodSet(ILLCPh2, Periodmin_fmax);
  PWM_TriggerACompareValueSet(ILLCPh2, PG3PER >> 1);
  PWM_TriggerBCompareValueSet(ILLCPh2, PG3TRIGA + DTH_L);
  
  // pre set for PHB IL trigger from SR1
  PWM_TriggerCCompareValueSet(SRPh1, PG1PER >> 2);  //90° phase shift for PHB
  PWM_PeriodSet(SRPh1, Periodmin_fmax);
 
  PWM_DutyCycleSet(ILLCPh2, DTH_L);

  PWM_OverrideLowDisable(ILLCPh2); //enable PWM output on L   
  PWM_OverrideHighDisable(ILLCPh2); //enable PWM output on H
#endif  

#if defined POWER_STAGE_CONFIG_PHASE_A_ONLY || defined POWER_STAGE_CONFIG_INTERLEAVED
  PWM_GeneratorEnable(SRPh1);
  //  PG2CONLbits.ON = 1;
#endif

#if defined POWER_STAGE_CONFIG_INTERLEAVED
  PWM_GeneratorEnable(SRPh2);
  //  PG4CONLbits.ON = 1;
#endif
  PG1CONLbits.ON = 1;
  PG3CONLbits.ON = 1;
//  PWM_GeneratorEnable(ILLCPh1);
//  PWM_GeneratorEnable(ILLCPh2);

  pwr_ctrl_flagbits.pwm_running = 1;
}

//==============================================================================
// @brief this function contains 
//==============================================================================

void Drv_PwrCtrl_ILLC_SwitchOffPWM(void)
{
  pwr_ctrl_ref_data.val_VoutRef_internal = 0; //start from zero volt	
  pwr_ctrl_ref_data.val_IinRef_internal = 0; //start from zero current

  ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG1IOCONL); //ILLCPh1
  ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG3IOCONL); //ILLCPh2

  ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG2IOCONL); //SRPh1
  pwr_ctrl_flagbits.SR1on = 0;
  ILLC_PWMx_OVERRIDE_ENABLE((uint16_t*) & PG4IOCONL); //SRPh2
  pwr_ctrl_flagbits.SR2on = 0;
  sr_ctrl_state = SRCS_STANDBY;

  pwr_ctrl_flagbits.pwm_running = 0;
}

//==============================================================================
// @brief this function contains 
//==============================================================================

void Drv_PwrCtrl_ILLC_Task_100us(void)
{
  //Check for Fault
  Drv_PwrCtrl_ILLC_Fault_Check();

  switch (pwr_ctrl_state)
  {
      //------------------------------------------------------------------------
    case PCS_INIT: //1

      //peripherals should be already initialized by MCC
      Drv_PwrCtrl_ILLC_SetFaultCounters();

      if (!CheckForBoardSetup())
        pwr_ctrl_state = PCS_WAIT_IF_FAULT_ACTIVE;

      break;

      //------------------------------------------------------------------------
    case PCS_WAIT_IF_FAULT_ACTIVE: //2
      if ((ILLCFaults.CollectiveFaults == 0))
      {
        pwr_ctrl_state = PCS_STANDBY;
      }
      break;
      //------------------------------------------------------------------------
    case PCS_STANDBY: //3
      if (ILLCFaults.CollectiveFaults)
      {
        pwr_ctrl_state = PCS_WAIT_IF_FAULT_ACTIVE;
      }
      else if (pwr_ctrl_ref_data.drv_val_VoutRef > 0 && pwr_ctrl_flagbits.run == 1)
        //      else if ((pwr_ctrl_ref_data.drv_val_VoutRef > 0) && (pwr_ctrl_flagbits.run == 1) && (!pwr_ctrl_flagbits.lightloadmode))
      {
        Drv_PwrCtrl_ILLC_SwitchOnPWM();
        pwr_ctrl_state = PCS_POWER_ON_DELAY;
      }
      break;
      //------------------------------------------------------------------------
    case PCS_POWER_ON_DELAY: //4
      pwr_ctrl_state = PCS_SOFT_START_PRE1;
      break;
      //------------------------------------------------------------------------
    case PCS_SOFT_START_PRE1: //5 
      if (ILLCFaults.CollectiveFaults)
      {
        pwr_ctrl_state = PCS_WAIT_IF_FAULT_ACTIVE;
        Drv_PwrCtrl_ILLC_SwitchOffPWM();
      }
      else
      {
#if defined POWER_STAGE_CONFIG_PHASE_A_ONLY
        StartUpMaxFrequencyDCSweep(0xA);
#endif  
#if defined POWER_STAGE_CONFIG_INTERLEAVED   
        StartUpMaxFrequencyDCSweep(0xC);
#endif  
      }

      break;
      //------------------------------------------------------------------------
    case PCS_SOFT_START_PRE2: //6 

      if (pwr_ctrl_flagbits.run == 0)
      {
        pwr_ctrl_state = PCS_STANDBY;
        Drv_PwrCtrl_ILLC_SwitchOffPWM();
      }

      if (ILLCFaults.CollectiveFaults)
      {
        pwr_ctrl_state = PCS_WAIT_IF_FAULT_ACTIVE;
        Drv_PwrCtrl_ILLC_SwitchOffPWM();
      }
        //Comment out for basic test
      else
      {
        App_HMI_Task_100ms(); //check ref value instead waiting on the 100ms task
        pwr_ctrl_state = PCS_SOFT_START;

        SMPS_Controller2P2ZInitialize(&VMC2p2z); // Clear histories Voltage Controller
      }

      break;
      //------------------------------------------------------------------------
    case PCS_SOFT_START: //7
//      tp24_SetHigh();
      if (ILLCFaults.CollectiveFaults)
      {
        pwr_ctrl_state = PCS_WAIT_IF_FAULT_ACTIVE;
        Drv_PwrCtrl_ILLC_SwitchOffPWM();
      }
      else
      {
        StartUpFrequencySweepCL();

        if (pwr_ctrl_flagbits.run == 0)
        {
          pwr_ctrl_state = PCS_STANDBY;
          Drv_PwrCtrl_ILLC_SwitchOffPWM();
        }
      }

      break;
      //------------------------------------------------------------------------
    case PCS_UP_AND_RUNNING: //8
//      tp24_SetLow();
      if (ILLCFaults.CollectiveFaults)
      {
        pwr_ctrl_state = PCS_WAIT_IF_FAULT_ACTIVE;
        Drv_PwrCtrl_ILLC_SwitchOffPWM();
      }
      else
      {
        if (pwr_ctrl_flagbits.run == 0)
        {
          pwr_ctrl_state = PCS_STANDBY;
          Drv_PwrCtrl_ILLC_SwitchOffPWM();
          break;
        }

        if //Vref change detect to ramp to new ref
          ((pwr_ctrl_ref_data.val_VoutRef_internal > (pwr_ctrl_ref_data.drv_val_VoutRef + 20)) ||
                (pwr_ctrl_ref_data.val_VoutRef_internal < (pwr_ctrl_ref_data.drv_val_VoutRef - 20)))
        {
          pwr_ctrl_state = PCS_SOFT_START;
          break;
        }

        //test 1.2.21 neu rein
        if ((pwr_ctrl_ref_data.val_VoutRef_internal >
                (pwr_ctrl_adc_data.drv_adc_val_FB_Vout - (pwr_ctrl_adc_data.drv_adc_val_FB_Vout >> 6)))
                && pwr_ctrl_flagbits.lightloadmode)
        {

#if defined POWER_STAGE_CONFIG_PHASE_A_ONLY || defined POWER_STAGE_CONFIG_INTERLEAVED   
          ILLC_PWMx_OVERRIDE_DISABLE((uint16_t*) & PG1IOCONL);
#endif
#if defined POWER_STAGE_CONFIG_INTERLEAVED 
          ILLC_PWMx_OVERRIDE_DISABLE((uint16_t*) & PG3IOCONL);
#endif
          
          pwr_ctrl_flagbits.lightloadmode = 0;
        }

      }
      break;

      //------------------------------------------------------------------------
    case PCS_SHUTDOWN: //9

      break;

      //------------------------------------------------------------------------
    default: // we should never come here

      Drv_PwrCtrl_ILLC_SwitchOffPWM();
      pwr_ctrl_state = PCS_WAIT_IF_FAULT_ACTIVE;

      break;
  }
}

//==============================================================================
//==============================================================================
