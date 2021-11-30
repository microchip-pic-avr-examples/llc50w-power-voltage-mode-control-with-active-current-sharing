/**
  @Company
    Microchip Technology Inc.

  @File Name
    app_hmi_boardtest.c

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for the inputs from the buttons and controls the ILLC and LEDs.
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

#include <xc.h>
#include <stdbool.h>
#include "device/dev_button.h"
#include "driver/power_controller/drv_pwrctrl_ILLC.h"
#include "../mcc_generated_files/pin_manager.h"
#include "driver/drv_led.h"
#include"driver/drv_adc.h"

//local functions
void CheckButtons(void);
void SetLEDs_100ms(void);

extern uint16_t os_resetCause;  //@sw??

bool App_HMI_useRefFromPoti = false;
bool App_HMI_useRefFromGUI = false;
bool App_HMI_useFixedRef = true;

uint8_t boardtest_state = 0;
//======================================================================================================================
// @brief initializes the HMI application
//======================================================================================================================

void App_HMI_BoardTest_Init(void)
{
    //Drv_LED_Blink(LED_PIM_RED);
}

//======================================================================================================================
// @brief this function contains the button SW2 check and following actions
//======================================================================================================================
/*
void CheckButtons(void)
{
    uint8_t buttonresponse = Dev_Button_GetEvent();

    if (buttonresponse == DEV_BUTTON_EVENT_PRESSED_SHORT)
    {
       if (pwr_ctrl_flagbits.run)
        {
            Drv_PwrCtrl_ILLC_Stop();
            Drv_LED_Off(LED_BOARD_GREEN);
        }
        else
        {
            Drv_PwrCtrl_ILLC_Start();
            Drv_LED_On(LED_BOARD_GREEN);
        }
    } 
    else if (buttonresponse == DEV_BUTTON_EVENT_PRESSED_LONG)
    {
        if (pwr_ctrl_flagbits.inopenloop)
        {  
            Drv_PwrCtrl_ILLC_SetMode_ClosedLoop();
        }  
        else
        {  
            Drv_PwrCtrl_ILLC_SetMode_OpenLoop();
        }    }
}
*/

//======================================================================================================================
// @brief this function contains LED properties called every 100ma from scheduler
//======================================================================================================================

void SetLEDs_100ms()
{
   //state < running --> toggle grenn LED on power board
    if (!pwr_ctrl_flagbits.run)
    {
        Drv_LED_Blink(LED_BOARD_GREEN);
    }

    //fault fheck
//    PWR_CTRL_FAULTS faults = Drv_PwrCtrl_ILLC_GetFaults(); //@ftx

    //any fault --> fast toggle red LED on power board
//    if (os_resetCause & 0x01 || faults.CollectiveFaults != 0)
//    {
//        Drv_LED_On(LED_BOARD_RED);
//    } 
//    else 
    if (pwr_ctrl_flagbits.inopenloop)
    {
        Drv_LED_Blink(LED_BOARD_RED);
    }
    else
    {
        //LED_Red_SetLow();
        Drv_LED_Off(LED_BOARD_RED);
    }  
}

bool Check_Values_Vdd_Ok(void)
{
    // Vdd coming from the PWM from the PIC16F Auxiliary MCU
    // ==> Minimal Voltage is 39,5V
    // ==> Maximum Voltage is 40,5V
    //TODO: calculate value
    
    //TODO: drv_adc_val_FB_Vdd is not working, always 0
    return true; //TODO: fix vdd input via pwm input line
    if ((pwr_ctrl_adc_data.drv_adc_val_FB_Vdd > 65000) || 
        (pwr_ctrl_adc_data.drv_adc_val_FB_Vdd < 1))
        return false;
    else
        return true;
}

bool Check_Values_Vaux_Ok(void)
{
    // ==> 5,11V ==> min adc 3190, max adc 3210
    if ((pwr_ctrl_adc_data.drv_adc_val_FB_Vaux > 3210) || 
        (pwr_ctrl_adc_data.drv_adc_val_FB_Vaux < 3190))
        return false;
    else
        return true;
}

bool Check_Values_Vout_off_Ok(void)
{
    // Vout-off max
    // ???? V       
    // ???? adc    
    if (pwr_ctrl_adc_data.drv_adc_val_FB_Vout > 100)    //TODO what value exactly
        return false;
    else
        return true;
}

bool Check_Values_Vout_on_Ok(void)
{
    // Vout-On min = 9,13V = 2635 adc  
    // Vout-On max = 9,35V = 2700 adc
    if ((pwr_ctrl_adc_data.drv_adc_val_FB_Vout > 2700) || 
        (pwr_ctrl_adc_data.drv_adc_val_FB_Vout < 2635))
        return false;
    else
        return true;
}

bool Check_Values_Iout_off_Ok(void)
{
    // Vout-off max
    // ???? V       
    // ???? adc    

//    volatile uint16_t drv_adc_val_FB_S_CT1_FLT; 
//    volatile uint16_t drv_adc_fltval_FB_S_CT1_FLT; 
//    volatile uint16_t drv_adc_val_FB_S_CT2_FLT;
    
//    volatile uint16_t drv_adc_val_FB_P_CT1_FLT; 
//    volatile uint16_t drv_adc_val_FB_P_CT2_FLT;

    
    if (pwr_ctrl_adc_data.drv_adc_val_FB_S_CT1_FLT > 100)    //TODO what value exactly
        return false;
    else
        return true;
}

bool Check_Values_Iout_on_Ok(void)
{
    // Iout: 1A
    // Iout-min: 1100 sdc     
    // Iout-max: 1120 adc    

//    volatile uint16_t drv_adc_val_FB_S_CT1_FLT; 
//    volatile uint16_t drv_adc_fltval_FB_S_CT1_FLT; 
//    volatile uint16_t drv_adc_val_FB_S_CT2_FLT;
    
//    volatile uint16_t drv_adc_val_FB_P_CT1_FLT; 
//    volatile uint16_t drv_adc_val_FB_P_CT2_FLT;

    if ((pwr_ctrl_adc_data.drv_adc_val_FB_S_CT1_FLT > 1200) || //TODO what value exactly
        (pwr_ctrl_adc_data.drv_adc_val_FB_S_CT1_FLT < 1000))
        return false;
    else
        return true;
}

bool Check_ProtectionTripped(void)
{
    if (Check_Values_Vout_on_Ok())
        return false;
    if (Check_Values_Iout_off_Ok() == false)
        return false;
    
    return true;
}

bool Check_Values_Standby(void)
{
    //Check Vaux, Vout, Iout, Iin, Vdd(PWM input)

    if (Check_Values_Vdd_Ok() == false)
        return false;

    if (Check_Values_Vaux_Ok() == false)
        return false;

    if (Check_Values_Vout_off_Ok() == false)
        return false;

    if (Check_Values_Iout_off_Ok() == false)
        return false;

    if (Check_ProtectionTripped())  // check protection off
        return false;
    
    return true;
}


bool Check_Values_PhaseA(void)
{
    //Check Vaux, Vout, Iout, Iin, Vdd(PWM input)

    if (Check_Values_Vdd_Ok() == false)
        return false;

    if (Check_Values_Vaux_Ok() == false)
        return false;

    if (Check_Values_Vout_on_Ok() == false)
        return false;

    if (Check_Values_Iout_on_Ok() == false)
        return false;
    
//    volatile uint16_t drv_adc_val_FB_S_CT1_FLT; 
//    volatile uint16_t drv_adc_fltval_FB_S_CT1_FLT; 
//    volatile uint16_t drv_adc_val_FB_S_CT2_FLT;
    
//    volatile uint16_t drv_adc_val_FB_P_CT1_FLT; 
//    volatile uint16_t drv_adc_val_FB_P_CT2_FLT;
    
    return true;
}

bool Check_Values_PhaseB(void)
{
    //Check Vaux, Vout, Iout, Iin, Vdd(PWM input)

    if (Check_Values_Vdd_Ok() == false)
        return false;

    if (Check_Values_Vaux_Ok() == false)
        return false;

    if (Check_Values_Vout_on_Ok() == false)
        return false;
    
//    volatile uint16_t drv_adc_val_FB_S_CT1_FLT; 
//    volatile uint16_t drv_adc_fltval_FB_S_CT1_FLT; 
//    volatile uint16_t drv_adc_val_FB_S_CT2_FLT;
    
//    volatile uint16_t drv_adc_val_FB_P_CT1_FLT; 
//    volatile uint16_t drv_adc_val_FB_P_CT2_FLT;
    
    return true;
}

//======================================================================================================================
// @brief this function contains the HMI board test application
//======================================================================================================================

void App_HMI_BoardTest_Task_100ms(void)
{    
    static uint8_t bt_test_timeout = 0;
    static uint8_t bt_testtime_ok = 0;
    static uint16_t bt_wait_ticks = 0;
    
    if (bt_wait_ticks)
    {
        bt_wait_ticks--;
        return;
    }
    
    switch (boardtest_state)
    {
        case 0: // init, red & green leds should blink
            Drv_LED_Blink(LED_PIM_RED);
            Drv_LED_Blink(LED_BOARD_RED);
            Drv_LED_Blink(LED_BOARD_GREEN);
            bt_wait_ticks = 10; // wait one seconds for booting and get everything stable
            bt_test_timeout = 0;
            bt_testtime_ok = 0;
            boardtest_state = 1;
            break;
            
        case 1: // Check initial adc values before switching on
            if (Check_Values_Standby())
                bt_testtime_ok++;
            else
                bt_testtime_ok = 0;
            if (++bt_test_timeout >= 30)    // wait 3 seconds
            {
                if (bt_testtime_ok >= 20)   // test result ok ??
                {
                    bt_test_timeout = 0;
                    bt_testtime_ok = 0;
                    boardtest_state = 2;
                }
                else
                    boardtest_state = 50; //check failed            
            }
            break;

        case 2: // Wait for user button
            if (Dev_Button_GetEvent() == DEV_BUTTON_EVENT_PRESSED_SHORT)
                boardtest_state = 3;
            break;
            
        case 3: // start power controller - Phase A
            Drv_LED_On(LED_BOARD_GREEN);
            Drv_LED_Off(LED_BOARD_RED);
            Drv_PwrCtrl_ILLC_SetMode_OpenLoop();
            Drv_PwrCtrl_ILLC_SetPhase(PWRCTRL_ILLC_PHASE_A);  // TODO: @FTX please implement
            Drv_PwrCtrl_ILLC_SetReferenceRaw(2048); //900kHz // TODO: @FTX please check value if 850kHz
            Drv_PwrCtrl_ILLC_Start();
            bt_wait_ticks = 30; // wait three seconds
            boardtest_state = 4;
            break;
            
        case 4: // // Check adc values - Phase A
            if (Check_Values_PhaseA() == false)
            {
                Drv_PwrCtrl_ILLC_Stop();
                boardtest_state = 50; //check failed
            }
            else
            {
                //Drv_PwrCtrl_ILLC_SetReferenceRaw(1800); //900kHz // TODO: @FTX please check value if 900kHz 
                boardtest_state = 5;
                //Drv_LED_Blink(LED_BOARD_RED);
                Drv_LED_Blink(LED_BOARD_GREEN);
            }
            break;

        case 5: // Wait for user button and Trigger Protection
            if (Dev_Button_GetEvent() == DEV_BUTTON_EVENT_PRESSED_SHORT)
            {
                Drv_LED_Off(LED_BOARD_GREEN);
                Drv_LED_Blink(LED_BOARD_RED);
                // Trigger Protection
                Drv_PwrCtrl_ILLC_SetReferenceRawDirect(2200); //800kHz // TODO: @FTX please check value if 800kHz
                bt_wait_ticks = 5; // wait half a seconds
                boardtest_state = 6;
            }
            break;
            
        case 6: // Check for protection
            if (Check_ProtectionTripped() == false)
            {
                Drv_PwrCtrl_ILLC_Stop();
                boardtest_state = 50; //check failed
            }
            else
            {
                boardtest_state = 7;
            }
            break;
                
        case 7: // Check for user button
            if (Dev_Button_GetEvent() == DEV_BUTTON_EVENT_PRESSED_SHORT)
            {
                Drv_PwrCtrl_ILLC_Stop();
                boardtest_state = 8;
            }
            break;
            
        case 8: // Check for protection release
            if (Check_ProtectionTripped() == false)
            {
                boardtest_state = 9;
            }
            break;
            
        case 9: // start power controller - Phase B
            Drv_LED_On(LED_BOARD_GREEN);
            Drv_LED_Off(LED_BOARD_RED);
            Drv_PwrCtrl_ILLC_SetMode_OpenLoop();
            Drv_PwrCtrl_ILLC_SetPhase(PWRCTRL_ILLC_PHASE_B);  // TODO: @FTX please implement
            Drv_PwrCtrl_ILLC_SetReferenceRaw(2048); //850kHz // TODO: @FTX please check value if 850kHz
            Drv_PwrCtrl_ILLC_Start();
            bt_wait_ticks = 30; // wait three seconds
            boardtest_state = 10;
            break;

        case 10: // // Check adc values - Phase B
            if (Check_Values_PhaseB() == false)
            {
                Drv_PwrCtrl_ILLC_Stop();
                boardtest_state = 50; //check failed
            }
            else
            {
                boardtest_state = 11;
                //Drv_LED_Blink(LED_BOARD_RED);
                Drv_LED_Blink(LED_BOARD_GREEN);
            }
            break;

        case 11: // Wait for user button and Trigger Protection
            if (Dev_Button_GetEvent() == DEV_BUTTON_EVENT_PRESSED_SHORT)
            {
                Drv_LED_Off(LED_BOARD_GREEN);
                Drv_LED_Blink(LED_BOARD_RED);
                // Trigger Protection
                Drv_PwrCtrl_ILLC_SetReferenceRawDirect(2200); //800kHz // TODO: @FTX please check value if 800kHz
                bt_wait_ticks = 5; // wait half a seconds
                boardtest_state = 12;
            }
            break;
            
        case 12: // Check for protection
            if (Check_ProtectionTripped() == false)
            {
                Drv_PwrCtrl_ILLC_Stop();
                boardtest_state = 50; //check failed
            }
            else
            {
                Drv_PwrCtrl_ILLC_Stop();
                boardtest_state = 40; //check finished successful
            }
            break;

        case 40: //check finished successful
            Drv_LED_On(LED_BOARD_GREEN);
            Drv_LED_Off(LED_BOARD_RED);
            boardtest_state = 60;
            break;
            
        case 50: //check failed
            Drv_LED_Off(LED_BOARD_GREEN);
            Drv_LED_On(LED_BOARD_RED);
            boardtest_state = 60;
            break;
            
        case 60: //the end: do nothing
            //test is over, do nothing
            break;
        
        default:
            break;
    }
}

//======================================================================================================================
//======================================================================================================================
