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

/**
 * @file dev_gui_comm.c
 * @author M52409
 * @date 2020-ß7-30
 * @brief contains API functions to use the GUI via serial interface
 *
 * there are two standard communication data packages to the GUI:
 * 1.: data stream package: this package contains different single values
 *     that need to be transmitted cyclical like 10 times a second.
 *     for example, temperatires, voltages, statemachine states, bitfields,...
 * 2.: scope data package: this is a block of data that contains one page of scope data
 *     it can contain data from multiple data channels.
 *     example: to record real-time data for a step response of output voltage and current
 */

#include "stdint.h"
#include "stdbool.h"
#include <stddef.h>
#include "dev_gui_comm.h"
#include "mcc_generated_files/pin_manager.h"

//-----------------------------------------------------------
// Configure Datastream function

//-----------------------------------------------------------


#define GUICOMM_RCV_DATABUFFER_SIZE     10
#define GUICOMM_PROTOCOL_ID_DATASTREAM_H  0x00
#define GUICOMM_PROTOCOL_ID_DATASTREAM_L  0x01
#define GUICOMM_PROTOCOL_ID_RCV_COMMANDS_H  0x00
#define GUICOMM_PROTOCOL_ID_RCV_COMMANDS_L  0x02
#define GUICOMM_PROTOCOL_ID_RCV_COMMANDS  0x0002

#define GUICOMM_BEGIN_OF_FRAME	0x55
#define GUICOMM_END_OF_FRAME	0x0d
#define GUICOMM_CRC_POLYNOM		0xA001

//-----------------------------------------------------------
// Datastream defines and variables
#define DATASTREAM_MODE_WAITING         0
#define DATASTREAM_MODE_RECORDING       1
#define DATASTREAM_MODE_TRANSMIT_DATA   2

uint8_t  gui_stream_mode = DATASTREAM_MODE_WAITING;
uint8_t  gui_stream_sm_state = 0;
uint16_t gui_stream_waitcounter = 0;
uint16_t gui_stream_length;
uint16_t gui_stream_data[DATASTREAM_NUM_OF_VALUES];

//typedef void (*GuiComm_CommandHandlerFunc)(uint16_t, uint8_t*);

typedef struct
{
    uint16_t protocol_ID;
//    void (*rcv_commandHandlerPtr)(uint16_t, uint8_t*);
    GuiComm_CommandHandlerFunc func;
} rcvCommandHandler_t;

rcvCommandHandler_t rcvCommandHandler;

//-----------------------------------------------------------


//-----------------------------------------------------------
uint8_t gui_header[20];
uint16_t gui_transfer_index = 0;
bool transfer_highbyte = true;

uint8_t rcv_data_buffer[GUICOMM_RCV_DATABUFFER_SIZE];
uint16_t guicomm_CRC_Rcv;
uint16_t guicomm_CRC_Send;


void GuiComm_DataStream_TransmitData(void);

void GuiComm_Rcv_Task(void);
void GuiComm_InitCRC(uint16_t* pCRC);
void GuiComm_CalcCRC16(uint8_t data, uint16_t* pCRC);
void GuiComm_CalcCRC16Buffer(uint8_t const *data, uint16_t length, uint16_t* pCRC);


 void Dev_GuiComm_Register_ProtocolHandler(GuiComm_CommandHandlerFunc rcvCmdFunc, uint16_t cmdID)
{
    rcvCommandHandler.protocol_ID = cmdID;
    rcvCommandHandler.func = rcvCmdFunc;
}

void Dev_GuiComm_Init(void)
{
    rcvCommandHandler.protocol_ID = 0;
    rcvCommandHandler.func = NULL;    
}

void Dev_GuiComm_Task_100us(void)
{
    TP190_SetHigh();
    GuiComm_Rcv_Task();
//    if (gui_scope_mode == SCOPE_MODE_TRANSMIT_DATA)
//    {
//        GuiComm_Scope_TransmitData();
//    }
//    else 
        if (gui_stream_mode == DATASTREAM_MODE_TRANSMIT_DATA)
    {
        GuiComm_DataStream_TransmitData();
    }
    
    if (gui_stream_mode == DATASTREAM_MODE_WAITING)
    {
        if (gui_stream_waitcounter < DATASTREAM_WAITTICKS)
        {
            gui_stream_waitcounter++;
        }
        else
        {
            gui_stream_waitcounter = 0;
            gui_stream_length = 0;
            gui_stream_mode = DATASTREAM_MODE_RECORDING;
        }
    }
    TP190_SetLow();
}

bool Dev_GuiComm_Stream_ReadyToSend(void)
{
    if (gui_stream_mode == DATASTREAM_MODE_RECORDING)
        return true;
    else
        return false;
}

void Dev_GuiComm_Stream_FinishSending(void)
{
    if (gui_stream_mode == DATASTREAM_MODE_RECORDING)
        gui_stream_mode = DATASTREAM_MODE_TRANSMIT_DATA;
}

void GuiComm_DataStream_TransmitData(void)
{
    static uint16_t MsCounter_=0;  //@ftx
    MsCounter_ = 2;
    switch (gui_stream_sm_state)
    {
        case 0:     //prepare header
            GuiComm_InitCRC(&guicomm_CRC_Send);
            gui_header[0] = GUICOMM_BEGIN_OF_FRAME;
            gui_header[1] = GUICOMM_PROTOCOL_ID_DATASTREAM_H;
            gui_header[2] = GUICOMM_PROTOCOL_ID_DATASTREAM_L;
            uint16_t length =  gui_stream_length*2;
#if GUICOMM_DATASTREAM_USE_CRC == 1            
            gui_header[3] = (length >> 8) | 0x80;   // highest bit means that we use CRC calculation
#else
            gui_header[3] = (length >> 8);          // highest bit is not set ==> we don't use CRC calculation
#endif
            gui_header[4] = length & 0xff;
            gui_transfer_index = 0;
#if GUICOMM_DATASTREAM_USE_CRC == 1            
            gui_stream_sm_state = 1;
#else
            gui_stream_sm_state = 2;
#endif
            break;
        
        case 1:     //Calc CRC from header header
            GuiComm_CalcCRC16Buffer(gui_header, 5, &guicomm_CRC_Send);
            gui_stream_sm_state = 2;
            break;

        case 2:     //transfer header
            while (GuiComm_ReadyToSend()&& (MsCounter_++ > 1)) //@ftx
            {
                GuiComm_Write(gui_header[gui_transfer_index]);
                gui_transfer_index++;
                if (gui_transfer_index > 4)
                {
                    gui_transfer_index = 0;
                    transfer_highbyte = true;
                    gui_stream_sm_state = 3;
                    break;
                }
                MsCounter_ = 0; //@ftx
            }
            break;

        case 3:   //transfer data
            while (GuiComm_ReadyToSend() && (MsCounter_++ > 1)) //@ftx
            {
                uint8_t databyte;
                if (transfer_highbyte)
                {
                    //GuiComm_Write(gui_stream_data[gui_transfer_index] >> 8);
                    databyte = gui_stream_data[gui_transfer_index] >> 8;
                    transfer_highbyte = false;
                }
                else
                {
                    //GuiComm_Write(gui_stream_data[gui_transfer_index] & 0xff);
                    databyte = gui_stream_data[gui_transfer_index] & 0xff;
                    transfer_highbyte = true;
                    gui_transfer_index++;
                }
                GuiComm_Write(databyte);
#if GUICOMM_DATASTREAM_USE_CRC == 1            
                GuiComm_CalcCRC16(databyte, &guicomm_CRC_Send);
#endif
                if (gui_transfer_index >= gui_stream_length)
                {
                    gui_stream_sm_state = 4;
                    gui_transfer_index = 0;
                    break;
                }
                MsCounter_ = 0; //@ftx
            }
            break;
    
        case 4:   //transfer end header
#if GUICOMM_DATASTREAM_USE_CRC == 1            
            //guicomm_CRC_Send = 0x1234;
            gui_header[0] = guicomm_CRC_Send >> 8;
            gui_header[1] = guicomm_CRC_Send & 0xff;
#else
            gui_header[0] = 0;  //no CRC
            gui_header[1] = 0;  //no CRC
#endif
            gui_header[2] = GUICOMM_END_OF_FRAME;
            gui_transfer_index = 0;
            gui_stream_sm_state = 5;
            break;

        case 5:   //transfer header
            while (GuiComm_ReadyToSend() && (MsCounter_++ > 1)) //@ftx
            {
                GuiComm_Write(gui_header[gui_transfer_index]);
                gui_transfer_index++;
                if (gui_transfer_index >= 3)
                {
                    gui_stream_sm_state = 0;
                    gui_transfer_index = 0;
                    gui_stream_mode = DATASTREAM_MODE_WAITING;
                    break;
                }
                MsCounter_ = 0; //@ftx
            }
            break;
    }
}

void Dev_GuiComm_Stream_SendWord(uint16_t data)
{
    if (gui_stream_length < (DATASTREAM_NUM_OF_VALUES))
    {
        gui_stream_data[gui_stream_length++] = data;
    }    
}



#define RCV_WAIT_FOR_STARTBYTE		0
#define RCV_READ_ID_HIGHBYTE		1
#define RCV_READ_ID_LOWBYTE			2
#define RCV_READ_LENGTH_HIGHBYTE	3
#define RCV_READ_LENGTH_LOWBYTE		4
#define RCV_READ_DATA				5
#define RCV_READ_CRC_HIGHBYTE		6
#define RCV_READ_CRC_LOWBYTE		7
#define RRC_READ_EOF				8

#define RCV_TIMEOUT 10000

void GuiComm_Rcv_Task(void)
{
	static uint8_t	rcv_state = 0;
	static uint16_t	rcv_protocol_id = 0;
	static uint16_t	rcv_data_length = 0;
	static uint16_t	rcv_data_index = 0;
	static uint16_t	rcv_CRC_calculated = 0;
	static uint16_t	rcv_CRC = 0;
	static uint16_t	rcv_timeout = 0;
	uint8_t	data;
	
	while (1)
	{
		if (GuiComm_IsRxReady() == false)
		{
			if (++rcv_timeout >= 10000)
			{
				rcv_timeout = 0;
				rcv_state = RCV_WAIT_FOR_STARTBYTE;
			}
			break;
		}
        // ok, we have some data coming in, lets read and process
		rcv_timeout = 0;
		data = GuiComm_Read();
		switch (rcv_state)
		{
			case RCV_WAIT_FOR_STARTBYTE:
				rcv_CRC_calculated = 0;
				if (data == GUICOMM_BEGIN_OF_FRAME)
					rcv_state = RCV_READ_ID_HIGHBYTE;
				break;
				
			case RCV_READ_ID_HIGHBYTE:
				rcv_protocol_id = data<<8;
					rcv_state = RCV_READ_ID_LOWBYTE;
				break;

			case RCV_READ_ID_LOWBYTE:
				rcv_protocol_id |= data;
				rcv_state = RCV_READ_LENGTH_HIGHBYTE;
				break;

			case RCV_READ_LENGTH_HIGHBYTE:
				rcv_data_length = data<<8;
				rcv_state = RCV_READ_LENGTH_LOWBYTE;
				break;

			case RCV_READ_LENGTH_LOWBYTE:
				rcv_data_length |= data;
				rcv_data_index = 0;
				rcv_state = RCV_READ_DATA;
				break;

			case RCV_READ_DATA:
				if (rcv_data_index < GUICOMM_RCV_DATABUFFER_SIZE)
				{
					rcv_data_buffer[rcv_data_index] = data;
					// todo: update calculated crc if needed (not implemented yet)
				}
				rcv_data_index++;
				if (rcv_data_index >= rcv_data_length)	//are we finished receiving data???
				{
					rcv_state = RCV_READ_CRC_HIGHBYTE;
				}
				break;

			case RCV_READ_CRC_HIGHBYTE:
				rcv_CRC = data<<8;
				rcv_state = RCV_READ_CRC_LOWBYTE;
				break;

			case RCV_READ_CRC_LOWBYTE:
				rcv_CRC |= data;
				rcv_data_index = 0;
				rcv_state = RRC_READ_EOF;
				break;
                
            case RRC_READ_EOF:
                if (data == 0x0d)
                {
                    // do we have to check the CRC first?
                    if ((rcv_protocol_id == rcvCommandHandler.protocol_ID) && (rcvCommandHandler.func != NULL))
                        rcvCommandHandler.func(rcv_data_length, rcv_data_buffer);
//                    else if (rcv_protocol_id == GUICOMM_PROTOCOL_ID_RCV_SCOPE_COMMAND)
//                        GuiComm_Scope_Protocol_Rcv(rcv_data_length, rcv_data_buffer);
                }
                rcv_protocol_id = 0;
                rcv_data_length = 0;
                rcv_data_index = 0;
                rcv_CRC_calculated = 0;
                rcv_CRC = 0;
                rcv_state = RCV_WAIT_FOR_STARTBYTE;
                break;
		}
	}
}



#define CRC16_POLYNOM 0xA001
#define CRC16_INITVAL 0x0000

void GuiComm_InitCRC(uint16_t* pCRC)
{
    *pCRC = CRC16_INITVAL;
}

void GuiComm_CalcCRC16(uint8_t data, uint16_t* pCRC)
{
    uint8_t j;

    *pCRC = *pCRC ^ data;
    for (j = 0; j < 8; j++)
    {
        if (*pCRC & 0x0001)
        {
            *pCRC = (*pCRC >> 1) ^ CRC16_POLYNOM;
        }
        else
        {
            *pCRC = *pCRC >> 1;
        }
    }
}

void GuiComm_CalcCRC16Buffer(uint8_t const *data, uint16_t length, uint16_t* pCRC)
{
    uint16_t i;
    uint8_t j;
    
    if (data != NULL)
    {
        for (i = 0; i < length; i++)
        {
            *pCRC = *pCRC ^ data[i];
            for (j = 0; j < 8; j++)
            {
                if (*pCRC & 0x0001)
                {
                    *pCRC = (*pCRC >> 1) ^ CRC16_POLYNOM;
                }
                else
                {
                    *pCRC = *pCRC >> 1;
                }
            }
        }
    }
}
