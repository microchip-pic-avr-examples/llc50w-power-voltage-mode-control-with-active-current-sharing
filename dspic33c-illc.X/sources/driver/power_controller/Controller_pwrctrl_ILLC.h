/**
    (c) 2017 Microchip Technology Inc. and its subsidiaries. You may use this
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

#ifndef _CONTROLLER_PWRCTRL_4SWBB_H
#define	_CONTROLLER_PWRCTRL_4SWBB_H

#include <xc.h> 
#include "smps_control_.h"   //interfaces toSwitch Mode Power Supply compensator library.
#include "../dcdt/vmc/dcdt_generated_code/vmc_dcdt.h"

//------------------------------------------------------------------------------

extern SMPS_2P2Z_T VMC2p2z;

//------------------------------------------------------------------------------

typedef volatile struct
{
    uint16_t _VDCref;
    uint16_t _IACref;
//    uint16_t _DCset;
//    uint16_t _PERIODsetPH1;
//    uint16_t _PERIODsetPH2;
    uint16_t _CurrentControllerResultPH1;
    uint16_t _CurrentControllerResultILPH;
    uint16_t _CurrentControllerResultPH2;
    uint16_t _VoltageControllerResult;    
    uint16_t _OpenLoopPeriod;    
} t_Controller;
extern t_Controller Controller_ILLC;

//------------------------------------------------------------------------------

void VoltageController_PwrCtrl_ILLC_Init(void);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#endif	/* XC_HEADER_TEMPLATE_H */

