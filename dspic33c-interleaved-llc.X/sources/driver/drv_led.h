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
// @file drv_led.h
//
// @brief contains the functions for accessing the leds
//
// @version v1.0
// @date 2020-07-15
// @author M52409
//
//=======================================================================================================

#ifndef _DRV_LED_H_
#define	_DRV_LED_H_

#include <stdint.h>

#define LED_PIM_RED         0
#define LED_BOARD_RED       1
#define LED_BOARD_GREEN     2

#define NUM_OF_LEDS 3

void Drv_LED_Init(void);
void Drv_LED_On(uint8_t led_id);
void Drv_LED_Off(uint8_t led_id);
void Drv_LED_Toggle(uint8_t led_id);
void Drv_LED_Blink(uint8_t led_id);
void Drv_LED_Blink_Slow(uint8_t led_id);
void Drv_LED_Blink_Fast(uint8_t led_id);
void Drv_LED_Task_100ms();

#endif	// _DRV_LED_H_

