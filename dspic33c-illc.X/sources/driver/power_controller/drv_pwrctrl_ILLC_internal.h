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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _DRV_PWRCTRL_ILLC_INTERNAL_H_
#define	_DRV_PWRCTRL_ILLC_INTERNAL_H_

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>

#include "drv_pwrctrl_ILLC_PHA_SRandControl.h"
#include "drv_pwrctrl_ILLC_ILPH_SRandControl.h"
#include "drv_pwrctrl_ILLC.h"
//------------------------------------------------------------------------------

//#define sQ15val(val_) ((val_ < 0.0) ? (32768 * (val_) - 0.5) : (32767 * (val_) + 0.5))
//#define uQ15val(val_) (uint16_t)((float)val_ * 0xFFFF)
//
//#define Prozent90value (uint16_t) uQ15val(90) //90% clamp
//#define Prozent7value uQ15val(7) //7% clamp     //200ns = 800*250ps

#define SOFT_START_RAMP_SPEED_DC   10//1   //DC 50% == 2500 == PGxPER (push-pull))
#define SOFT_START_RAMP_SPEED_FR   10//5

#define ILLCPh1  1
#define ILLCPh2  3
#define SRPh1    2
#define SRPh2    4

#define Periodmax_fmin 5000//>>3
#define Periodmin_fmax 4000//>>3

#define DTH_L 200//25   //50ns
#define SRPHRampStep 40 //40 = 10ns steps for SR startup ramp
#define SRPHRampStepCurrentBalance 8 //8 = 2ns
#define SRDelay 296 //74ns tt
#define SRDelayIL 296 //400//xxns  
#define SRStartDC 400   //100ns

//------------------------------------------------------------------------------

typedef struct
{
  volatile uint16_t val_VoutRef_internal;
  volatile uint16_t drv_val_VoutRef;
  volatile uint16_t val_IinRef_internal;
  volatile uint16_t drv_val_IinRef;
  volatile uint16_t drv_comm_val_VoutRef;
} PWR_CTRL_REFDATA_t; // power control soft-start settings and variables
extern PWR_CTRL_REFDATA_t pwr_ctrl_ref_data;

//------------------------------------------------------------------------------

void Drv_PwrCtrl_ILLC_SwitchOnPWM(void);
void Drv_PwrCtrl_ILLC_SwitchOffPWM(void);
void StartUpMaxFrequencyDCSweep(uint16_t PhaseX);
void StartUpFrequencySweepOL(void);
void StartUpFrequencySweepCL(void);
void PreBias(void);
bool CheckForBoardSetup(void);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
  @Summary
    Updates the ILLC-Ph1 specific PWM generator.

  @Description
    This routine is used to update the register need for ILLC Phase 1.
    Check for min and max values.

  @Param
    Controller output

  @Returns
    None
 
  @Example 
    <code>
    ILLC_PHASE1_PWM_UPDATE();
    </code>
 */
static inline __attribute__((always_inline)) void ILLC_PHASE1_PWM_UPDATE_PER_TRIGA_TRIGB_TRIGC(uint16_t controllerresult_)
{
  uint16_t pg1per_, pg1dc_;

  if (controllerresult_ > Periodmax_fmin)
  {
    pg1per_ = Periodmax_fmin;
  }
  else if (controllerresult_ < Periodmin_fmax)
  {
    pg1per_ = Periodmin_fmax;
  }
  else
  {
    pg1per_ = controllerresult_;
  }

  PG1PHASE = 0;
  PG1PER = pg1per_;
  PG1TRIGA = pg1per_ >> 1;
  PG1TRIGB = pg1per_ - DTH_L;

  pg1dc_ = (pg1per_ >> 1) - DTH_L;

  PG1TRIGC = ((pg1dc_ - PG1PHASE) >> 1) + PG1PHASE;
}

static inline __attribute__((always_inline)) void ILLC_PHASE2_PWM_UPDATE_PER_TRIGA_TRIGB_TRIGC(uint16_t controllerresult_)
{
  uint16_t pg3per_, pg3dc_;

  if (controllerresult_ > Periodmax_fmin)
  {
    pg3per_ = Periodmax_fmin;
  }
  else if (controllerresult_ < Periodmin_fmax)
  {
    pg3per_ = Periodmin_fmax;
  }
  else
  {
    pg3per_ = controllerresult_;
  }

  PG3PHASE = 0;
  ;
  PG3PER = pg3per_;
  PG3TRIGA = pg3per_ >> 1;
  PG3TRIGB = pg3per_ - DTH_L;
  pg3dc_ = (pg3per_ >> 1) - DTH_L;
  PG3TRIGC = ((pg3dc_ - PG3PHASE) >> 1) + PG3PHASE;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
  @Summary
    Updates the ILLC-Ph1 DC to trigger updates.

  @Description
    This routine is used to update the register need for ILLC Phase 1.

  @Param
 none

  @Returns
    None
 
  @Example 
    <code>
    ILLC_PHASE1_PWM_UPDATE();
    </code>
 */
static inline __attribute__((always_inline)) void ILLC_PHASE1_PWM_UPDATE_REGS()
{
//    PG2TRIGC = PG1PER >> 2;
  PG1DC = (PG1PER >> 1) - DTH_L;
}

static inline __attribute__((always_inline)) void ILLC_PHASE2_PWM_UPDATE_REGS()
{
  PG2TRIGC = PG1PER >> 2;
  PG3DC = (PG3PER >> 1) - DTH_L;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
  @Summary
    Override ILLC-Phx specific PWM generator.

  @Description
    This routine is used to override PWMH/L for ILLC Phase X.
    PG1IOCONLbits.OVRENH = 1; 
    PG1IOCONLbits.OVRENL = 1; 
  @Param
  address of PGxIOCONL register

  @Returns
    None
 
  @Example 
    <code>
    ILLC_PWMx_OVERRIDE_ENABLE(&PG1IOCONL);
    </code>
 */
static inline __attribute__((always_inline)) void ILLC_PWMx_OVERRIDE_ENABLE(uint16_t *pgxconl)
{
  *pgxconl |= 0x3000;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
  @Summary
    Override ILLC-Phx specific PWM generator.

  @Description
    This routine is used to override PWMH/L for ILLC Phase X.
    PG1IOCONLbits.OVRENH = 0; 
    PG1IOCONLbits.OVRENL = 0; 

  @Param
  address of PGxIOCONL register

  @Returns
    None
 
  @Example 
    <code>
    ILLC_PWMx_OVERRIDE_DISABLE(&PG1IOCONL);
    </code>
 */
static inline __attribute__((always_inline)) void ILLC_PWMx_OVERRIDE_DISABLE(uint16_t *pgxconl)
{
  *pgxconl &= 0xCFFF;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
  @Summary
    Update ILLC-Phx PWM generator fro SR.

  @Description
    This routine is used to calculate and set the PWM register for dual output mode

  @Param
  address of PGx register

  @Returns
    None
 
  @Example 
    <code>
     Drv_PwrCtrl_ILLC_ILSR_Adaptive_SetPWMValues(phaseIL2zw, phaseIL4zw);
    </code>
 */

static inline __attribute__((always_inline)) void Drv_PwrCtrl_ILLC_ILSR_Adaptive_SetPWMValues(uint16_t phaseIL2zw_, uint16_t phaseIL4zw_)
{
  PG2PHASE = phaseIL2zw_;
  if (PG2PHASE <= SRDelay) PG2PHASE = SRDelayIL;
  PG2PER = PG1PER;
  PG2TRIGA = ((PG2PER >> 1) + PG2PHASE);
  PG2TRIGB = (PG2PER - DTH_L);
  PG2DC = ((PG2PER >> 1) - DTH_L);

  PG4PHASE = phaseIL4zw_;
  if (PG4PHASE <= SRDelay) PG4PHASE = SRDelayIL;
  PG4PER = PG3PER;
  PG4TRIGA = ((PG4PER >> 1) + PG4PHASE);
  PG4TRIGB = (PG4PER - DTH_L);
  PG4DC = ((PG4PER >> 1) - DTH_L);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


























/**
  @Summary
    Update ILLC-Phx PWM generator fro SR.

  @Description
    This routine is used to calculate and set the PWM register for dual output mode

  @Param
  address of PGx register

  @Returns
    None
 
  @Example 
    <code>
     Drv_PwrCtrl_ILLC_ILSR_Adaptive_SetPWMValues(phaseIL2zw, phaseIL4zw);
    </code>
 */

static inline __attribute__((always_inline)) void Drv_PwrCtrl_ILLC_PH1SR_Adaptive_SetPWMValues(uint16_t phaseIL2zw_)
{
  PG2PHASE = phaseIL2zw_;
  if (PG2PHASE <= SRDelay) PG2PHASE = SRDelayIL;
  PG2PER = PG1PER;
  PG2TRIGA = ((PG2PER >> 1) + PG2PHASE);
  PG2TRIGB = (PG2PER - DTH_L);
  PG2DC = ((PG2PER >> 1) - DTH_L);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


















#endif	// _DRV_PWRCTRL_4SWBB_INTERNAL_H_

