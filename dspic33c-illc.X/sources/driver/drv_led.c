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
// @file drv_led.c
//
// @brief contains the functions for accessing the leds
//
// @version v1.0
// @date 2019-08-16
// @author M52409
//
//=======================================================================================================

#include <xc.h> // include processor file
#include "driver/drv_led.h"
//#include "main/project_settings.h"
//#include"../../mcc_generated_files/pin_manager.h"
#include"./mcc_generated_files/pin_manager.h"

#define LED_STATE_OFF 0
#define LED_STATE_ON 1
#define LED_STATE_BLINK 2
#define LED_STATE_BLINK_SLOW 3
#define LED_STATE_BLINK_FAST 4

uint8_t ledstates[NUM_OF_LEDS];
uint8_t ledtimer[NUM_OF_LEDS];

void Drv_LED_Init(void)
{
    //Peripherals are initialized by MCC
}


void Drv_LED_On(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    if (ledstates[led_id] == LED_STATE_ON)
        return;
    ledstates[led_id] = LED_STATE_ON;
    switch(led_id)
    {
        case LED_PIM_RED:       PIM_LEDred_SetHigh(); break;
        case LED_BOARD_RED:     LED_Red_SetHigh(); break;
        case LED_BOARD_GREEN:   LED_Green_SetHigh(); break;
        default: Nop(); break;
    }
}

void Drv_LED_Off(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    if (ledstates[led_id] == LED_STATE_OFF)
        return;
    ledstates[led_id] = LED_STATE_OFF;
    switch(led_id)
    {
        case LED_PIM_RED:       PIM_LEDred_SetLow(); break;
        case LED_BOARD_RED:     LED_Red_SetLow(); break;
        case LED_BOARD_GREEN:   LED_Green_SetLow(); break;
        default: Nop(); break;
    }
}

void Drv_LED_Toggle(uint8_t led_id)
{
    switch(led_id)
    {
        case LED_PIM_RED:       PIM_LEDred_Toggle(); break;
        case LED_BOARD_RED:     LED_Red_Toggle(); break;
        case LED_BOARD_GREEN:   LED_Green_Toggle(); break;
        default: Nop(); break;
    }
}

void Drv_LED_Blink(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledstates[led_id] = LED_STATE_BLINK;
}

void Drv_LED_Blink_Slow(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledstates[led_id] = LED_STATE_BLINK_SLOW;
}

void Drv_LED_Blink_Fast(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledstates[led_id] = LED_STATE_BLINK_FAST;
}

void Drv_LED_Task_100ms()
{
    uint8_t led_index=0;
    uint8_t timermax=0; 

    for (led_index = 0; led_index < NUM_OF_LEDS; led_index++)
    {
        if (ledstates[led_index] > LED_STATE_ON)
        {
            if (ledstates[led_index] == LED_STATE_BLINK_SLOW)
                timermax = 10;
            else if (ledstates[led_index] == LED_STATE_BLINK_FAST)
                timermax = 2;
            else    // normal speed
                timermax = 5;
            if (++ledtimer[led_index] >= timermax)
            {
                ledtimer[led_index] = 0;
                switch(led_index)
                {
                    case LED_PIM_RED:       PIM_LEDred_Toggle(); break;
                    case LED_BOARD_RED:     LED_Red_Toggle(); break;
                    case LED_BOARD_GREEN:   LED_Green_Toggle(); break;
                    default: Nop(); break;
                }
            }
        }
    }
}

 

#define NUM_OF_LEDS 3
