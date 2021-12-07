/**
  @Company
    Microchip Technology Inc.

  @File Name
    

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides the ILLC dedicated fault handling based on generic fault function in fault_common.
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

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "mcc_generated_files/system.h"

#include "misc/fault_common.h"
#include "driver/power_controller/drv_pwrctrl_ILLC.h"
#include "driver/drv_adc.h"
#include "drv_pwrctrl_ILLC_fault.h"
#include "drv_pwrctrl_ILLC_settings.h"

PWR_CTRL_FAULTS ILLCFaults;

FAULT_OBJ_T fltobj_ILLC_VinUv;   // Input Under Voltage Protection (UVP) handled by ADC
FAULT_OBJ_T fltobj_ILLC_VinOv;   // Input Over Voltage Protection (OVP) handled by ADC

FAULT_OBJ_T fltobj_ILLC_VoutOv;   // Output Over Voltage Protection (OVP) handled by ADC

FAULT_OBJ_T fltobj_ILLC_VauxOOR;  // Auxilliary Voltage out of range Protection handled by ADC 

FAULT_OBJ_T fltobj_ILLC_IoutOCPhA;   // Output Over Current Protection handled by ADC 
FAULT_OBJ_T fltobj_ILLC_IoutOCPhB;   // Output Over Current Protection handled by ADC 

		
//======================================================================================================================
// @brief this function contains 
//======================================================================================================================

void Drv_PwrCtrl_ILLC_Init_Fault(void)
{
    FAULT_Init( &fltobj_ILLC_VinUv, VIN_UV_THRESHOLD, VIN_UV_HYSLIMIT, 0, 0 ); //initialize with 0's that fault is immediately activ
    FAULT_Init( &fltobj_ILLC_VinOv, VIN_OV_THRESHOLD, VIN_OV_HYSLIMIT, 0, 0 ); //initialize with 0's that fault is immediately activ

    FAULT_Init( &fltobj_ILLC_VoutOv, VOUT_OV_THRESHOLD, VOUT_OV_HYS_LIMIT, VOUT_OV_THRESHOLD_CNT, VOUT_OV_HYS_CNT );
    FAULT_InitRange(&fltobj_ILLC_VauxOOR, VAUX_OOR_THRESHOLD1, VAUX_OOR_HYS_LIMIT1,VAUX_OOR_THRESHOLD2, VAUX_OOR_HYS_LIMIT2, VAUX_OOR_THRESHOLD_CNT, VAUX_OOR_HYS_CNT);
    FAULT_Init(&fltobj_ILLC_IoutOCPhA, IOUT_OCPHA_THRESHOLD, IOUT_OCPHA_HYS_LIMIT, IOUT_OCPHA_THRESHOLD_CNT, IOUT_OCPHA_HYS_CNT );
    FAULT_Init(&fltobj_ILLC_IoutOCPhB, IOUT_OCPHA_THRESHOLD, IOUT_OCPHA_HYS_LIMIT, IOUT_OCPHA_THRESHOLD_CNT, IOUT_OCPHA_HYS_CNT );
  }

//======================================================================================================================
// @brief this function contains 
//======================================================================================================================

void Drv_PwrCtrl_ILLC_SetFaultCounters(void)
{
  FAULT_SetCounters(&fltobj_ILLC_VinUv, VIN_UV_THRESHOLD_CNT, VIN_UV_HYSCNT);
  FAULT_SetCounters(&fltobj_ILLC_VinOv, VIN_OV_THRESHOLD_CNT, VIN_OV_HYS_CNT);
}

//======================================================================================================================
// @brief this function contains 
//======================================================================================================================

void Drv_PwrCtrl_ILLC_Fault_Check()
{
  //  Vin uvervoltage check
  ILLCFaults.inputUnderVoltage = FAULT_CheckMin(&fltobj_ILLC_VinUv, pwr_ctrl_adc_data.drv_adc_val_FB_Vdd, NULL);
  //  Vin overvoltage check
  ILLCFaults.inputOverVoltage = FAULT_CheckMax(&fltobj_ILLC_VinOv, pwr_ctrl_adc_data.drv_adc_val_FB_Vdd, NULL);

  //  Vout overvoltage check
  ILLCFaults.outputOverVoltage = FAULT_CheckMax(&fltobj_ILLC_VoutOv, pwr_ctrl_adc_data.drv_adc_val_FB_Vout, NULL);
  // Vaux out of range check
  ILLCFaults.auxVoltageOor = FAULT_CheckRange(&fltobj_ILLC_VauxOOR, pwr_ctrl_adc_data.drv_adc_val_FB_Vaux, NULL);
  // Iout overcurrent check
  ILLCFaults.outputOverCurrentPh1 = FAULT_CheckMax(&fltobj_ILLC_IoutOCPhA, pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT1_FLT, NULL);
  ILLCFaults.outputOverCurrentPh2 = FAULT_CheckMax(&fltobj_ILLC_IoutOCPhB, pwr_ctrl_adc_data.drv_adc_fltval_FB_S_CT2_FLT, NULL);

}

//======================================================================================================================
// @brief this function contains 
//======================================================================================================================

PWR_CTRL_FAULTS Drv_PwrCtrl_ILLC_GetFaults(void)
{
	return ILLCFaults;
}

//====================================================================================================================== 
//======================================================================================================================


