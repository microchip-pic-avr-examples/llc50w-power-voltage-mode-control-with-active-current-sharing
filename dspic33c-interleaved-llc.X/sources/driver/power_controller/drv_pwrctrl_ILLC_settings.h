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

//-------------------------------------------------------------------------------------------------------
// Defines for thresholds and counters for faults
//-------------------------------------------------------------------------------------------------------

//Vin Under Voltage ADC Calc: 50V=96%, 0V=0%, 96%=9600digit by 10ns
//                            50V/9600 = 0.00521  
#define VIN_UV_FLOAT        (float)(35/0.00521)   //35V
#define VIN_UV_THRESHOLD     (uint16_t) VIN_UV_FLOAT     

#define VIN_UV_HYS_FLOAT 		(float)(36/0.00521)  //36V
#define VIN_UV_HYSLIMIT      (uint16_t) VIN_UV_HYS_FLOAT 

#define VIN_UV_THRESHOLD_CNT  30
#define VIN_UV_HYSCNT        10000

//Vin Over Voltage ADC
#define VIN_OV_FLOAT        (float)(43/0.00521)   //43V
#define VIN_OV_THRESHOLD     (uint16_t) VIN_OV_FLOAT     

#define VIN_OV_HYS_FLOAT 		 (float)(42/0.00521)  //42V
#define VIN_OV_HYSLIMIT      (uint16_t) VIN_OV_HYS_FLOAT 

#define VIN_OV_THRESHOLD_CNT  30
#define VIN_OV_HYS_CNT        10000

//Vout Over Voltage ADC
#define VOUT_OV_FLOAT        (float)((4096/3.3/4.3)*12)   //12V
#define VOUT_OV_THRESHOLD     (uint16_t) VOUT_OV_FLOAT   

#define VOUT_OV_HYS_FLOAT 		(float)((4096/3.3/4.3)*11) //11V
#define VOUT_OV_HYS_LIMIT      (uint16_t) VOUT_OV_HYS_FLOAT 

#define VOUT_OV_THRESHOLD_CNT  30
#define VOUT_OV_HYS_CNT        10000

#define VOUT_SR_ACTIVE_FLOAT   (float)((4096/3.3/4.3)*6) //6V
#define VOUT_SR_ACTIVE         (uint16_t) VOUT_SR_ACTIVE_FLOAT 
// Done and tested until here onlyFT comment------------------------------------
//Vaux out of range
#define VAUX_OOR1_FLOAT          (float)((4096/2/3.3)*5)   //5V
#define VAUX_OOR_THRESHOLD1      (uint16_t) VAUX_OOR1_FLOAT 
#define VAUX_OOR2_FLOAT          (float)((4096/2/3.3)*6)   //6V
#define VAUX_OOR_THRESHOLD2      (uint16_t) VAUX_OOR2_FLOAT  

#define VAUX_OOR_HYS1_FLOAT      (float)((4096/2/3.3)*5)   //5V
#define VAUX_OOR_HYS_LIMIT1      (uint16_t) VAUX_OOR_HYS1_FLOAT 
#define VAUX_OOR_HYS2_FLOAT      (float)((4096/2/3.3)*6)   //6V
#define VAUX_OOR_HYS_LIMIT2      (uint16_t) VAUX_OOR_HYS2_FLOAT 

#define VAUX_OOR_THRESHOLD_CNT  30
#define VAUX_OOR_HYS_CNT        10000

//Iout Over Current ADC
#define IOUT_OCPHA_FLOAT           (float)((((3.2*0.384)+0.517)*4096)/3.3) //3.2A
#define IOUT_OCPHA_THRESHOLD       (uint16_t) IOUT_OCPHA_FLOAT    

#define IOUT_OCPHA_HYS_FLOAT       (float)((((3.0*0.384)+0.517)*4096)/3.3) //3.0A
#define IOUT_OCPHA_HYS_LIMIT       (uint16_t) IOUT_OCPHA_HYS_FLOAT 

#define IOUT_OCPHA_THRESHOLD_CNT  30
#define IOUT_OCPHA_HYS_CNT        10000

//-------------------------------------------------------------------------------------------------------
// Defines for voltage and current settings
//-------------------------------------------------------------------------------------------------------

//#define VOUT_3_3V_FLOAT 	(float)((4096/8/3.3)*3.3)   //3.3V
//#define VOUT_3_3V         (uint16_t)VOUT_3_3V_FLOAT

#define IOUT_SRON_FLOAT 	(float)((((0.7*0.384)+0.517)*4096)/3.3) //0.7A tt
#define IOUT_SRON         (uint16_t) IOUT_SRON_FLOAT 
#define IOUT_SRON_FLOATIL 	(float)((((0.7*0.384)+0.517)*2*4096)/3.3) //IL 0.7A tt
#define IOUT_SRONIL         (uint16_t) IOUT_SRON_FLOATIL 

#define IOUT_SROFF_FLOAT 	(float)((((0.5*0.384)+0.517)*4096)/3.3) //IL 0.5A tt
#define IOUT_SROFF        (uint16_t) IOUT_SROFF_FLOAT 
#define IOUT_SROFF_FLOATIL 	(float)((((0.5*0.384)+0.517)*2*4096)/3.3) //IL 0.5A tt
#define IOUT_SROFFIL        (uint16_t) IOUT_SROFF_FLOATIL 

#define VREF_FIXED12V_FLOAT (float)((4095/4.3/3.3)*12)              //12V
#define VREF_FIXED12V       (uint16_t) VREF_FIXED12V_FLOAT          // 3463
#define VREF_FIXED8V_FLOAT (float)((4095/4.3/3.3)*8)                //8V
#define VREF_FIXED8V       (uint16_t) VREF_FIXED8V_FLOAT            // 2308

