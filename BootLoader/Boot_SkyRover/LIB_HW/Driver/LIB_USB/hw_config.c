/**
  ******************************************************************************
  * @file    hw_config.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Hardware Configuration & Setup
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
/*
	
	1. USB 클럭 소스 설정
	
	2. 

*/





/* Includes ------------------------------------------------------------------*/

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_istr.h"
#include "Lb_Printf.h"
#include "Hw_Uart.h"
#include "Hw_VCom.h"

//ErrorStatus HSEStartUpStatus;
//USART_InitTypeDef USART_InitStructure;
//EXTI_InitTypeDef EXTI_InitStructure;

uint8_t  USART_Rx_Buffer [USART_RX_DATA_SIZE]; 
uint32_t USART_Rx_ptr_in = 0;
uint32_t USART_Rx_ptr_out = 0;
uint32_t USART_Rx_length  = 0;

uint8_t  USB_Tx_State = 0;
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);


extern LINE_CODING linecoding;





/*******************************************************************************
* Function Name  : USB_IRQHandler
* Description    : This function handles USB Low Priority interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_IRQHandler(void)
{
	//printf("USB\n");	
	USB_Istr();
}


void USBWakeUp_IRQHandler(void)
{
	//EXTI_ClearITPendingBit(EXTI_Line18);
	REG_EXTI_PR = (1 << 18 );
	
	//printf("wake");
}



extern void Hw_Wait_Usec( u32 usec );


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{  	


  
	//-- PC.13
	REG_GPIOC_CRH &= ~(0x0F << (20));	// Clear
	REG_GPIOC_CRH |=  (0x02 << (20));	// MODE, PC.13 Output mode, max speed 2Mhz
	REG_GPIOC_CRH |=  (0x00 << (22));	// CNF,  PC.13 General purpose output push-pul
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
	//-- USB 72M * 2 / 3 = 48Mhz
	//
	CLR_BIT( REG_RCC_CFGR, 22 );	
	
	//-- USB Clock Enable 
	//
	SET_BIT( REG_RCC_APB1ENR, 23 );
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
  /*Enable SystemCoreClock*/
  //SystemInit();
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{

	//-- EXTI Line 18 Interrupt Enabled
	//
	REG_EXTI_PR = (1 << 18 );		// EXTI line 18 Clear Pending Bit connected internally to the USB IP
	
	CLR_BIT( REG_EXTI_IMR,  18 );
	CLR_BIT( REG_EXTI_EMR,  18 );
	SET_BIT( REG_EXTI_RTSR, 18 );	// Rising Edge
	CLR_BIT( REG_EXTI_FTSR, 18 );
	SET_BIT( REG_EXTI_IMR,  18 );	// Enable

  
	Hw_ISR_SetIRQFuncByAddr( 0x0090, (u32)USB_LP_IRQHandler, 0 );
	SET_BIT( REG_NVIC_ISER( 20/32 ), 20%32 );	// NVIC 20번 USB_LP 인터럽트 활성화 
	
	Hw_ISR_SetIRQFuncByAddr( 0x00E8, (u32)USBWakeUp_IRQHandler, 0 );
	SET_BIT( REG_NVIC_ISER( 42/32 ), 42%32 );	// NVIC 42번 USB Wake Up 인터럽트 활성화 

	SET_BIT( REG_EXTI_SWIER, 18 );
		 
}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (char NewState)
{
  if (NewState != DISABLE)
  {
  	#if HW_USE_USB_SMARTROBOT_BD == 1
	CLR_BIT( REG_GPIOC_ODR, 13 );
	#else
	CLR_BIT( REG_GPIOC_ODR, 2 );
	#endif
  }
  else
  {
  	#if HW_USE_USB_SMARTROBOT_BD == 1
	SET_BIT( REG_GPIOC_ODR, 13 );    
	#else
	SET_BIT( REG_GPIOC_ODR, 2 );    
	#endif
  }

}

/*******************************************************************************
* Function Name  : USB_To_USART_Send_Data.
* Description    : send the received data from USB to the UART 0.
* Input          : data_buffer: data address.
                   Nb_bytes: number of bytes to send.
* Return         : none.
*******************************************************************************/
void USB_To_USART_Send_Data(uint8_t* data_buffer, uint8_t Nb_bytes)
{
  
  uint32_t i;
  u8 UartData;
  
  for (i = 0; i < Nb_bytes; i++)
  {
    //USART_SendData(EVAL_COM1, *(data_buffer + i));
    //while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET); 
    
    //-- PC로부터 받은 데이터 처리
    //
    UartData = *(data_buffer + i);
    Hw_VCom_Q_Push( 0, &UartData );	//

    if( Hw_VCom_RxdFuncPtr != NULL )
    {
      Hw_VCom_RxdFuncPtr( UartData );      
    }
  }  
}

/*******************************************************************************
* Function Name  : Handle_USBAsynchXfer.
* Description    : send data to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void Handle_USBAsynchXfer (void)
{
  
  uint16_t USB_Tx_ptr;
  uint16_t USB_Tx_length;
  
  if(USB_Tx_State != 1)
  {
    if (USART_Rx_ptr_out == USART_RX_DATA_SIZE)
    {
      USART_Rx_ptr_out = 0;
    }
    
    if(USART_Rx_ptr_out == USART_Rx_ptr_in) 
    {
      USB_Tx_State = 0; 
      return;
    }
    
    if(USART_Rx_ptr_out > USART_Rx_ptr_in) /* rollback */
    { 
      USART_Rx_length = USART_RX_DATA_SIZE - USART_Rx_ptr_out;
    }
    else 
    {
      USART_Rx_length = USART_Rx_ptr_in - USART_Rx_ptr_out;
    }
    
    if (USART_Rx_length > VIRTUAL_COM_PORT_DATA_SIZE)
    {
      USB_Tx_ptr = USART_Rx_ptr_out;
      USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;
      
      USART_Rx_ptr_out += VIRTUAL_COM_PORT_DATA_SIZE;	
      USART_Rx_length -= VIRTUAL_COM_PORT_DATA_SIZE;	
    }
    else
    {
      USB_Tx_ptr = USART_Rx_ptr_out;
      USB_Tx_length = USART_Rx_length;
      
      USART_Rx_ptr_out += USART_Rx_length;
      USART_Rx_length = 0;
    }
    USB_Tx_State = 1; 
    UserToPMABufferCopy(&USART_Rx_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
    SetEPTxCount(ENDP1, USB_Tx_length);
    SetEPTxValid(ENDP1); 
  }  
  
}

/*******************************************************************************
* Function Name  : UART_To_USB_Send_Data.
* Description    : send the received data from UART 0 to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void USART_To_USB_Send_Data(void)
{
#if 0  
  if (linecoding.datatype == 7)
  {
    USART_Rx_Buffer[USART_Rx_ptr_in] = USART_ReceiveData(EVAL_COM1) & 0x7F;
  }
  else if (linecoding.datatype == 8)
  {
    USART_Rx_Buffer[USART_Rx_ptr_in] = USART_ReceiveData(EVAL_COM1);
  }
  
  USART_Rx_ptr_in++;
  
  /* To avoid buffer overflow */
  if(USART_Rx_ptr_in == USART_RX_DATA_SIZE)
  {
    USART_Rx_ptr_in = 0;
  }
#endif  
}
/*******************************************************************************
* Function Name  : UART_To_USB_Send_Data.
* Description    : send the received data from UART 0 to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void USB_Send_Data( u8 SendData )
{
	
	//-- 전송 버퍼가 꽉 차면 기다린다.
	//
	while(1)
	{
		if( (USART_Rx_length + 1) < USART_RX_DATA_SIZE ) break;
	}
	
	
	USART_Rx_Buffer[USART_Rx_ptr_in] = SendData;
	USART_Rx_ptr_in++;
  
	/* To avoid buffer overflow */
	if(USART_Rx_ptr_in == USART_RX_DATA_SIZE)
	{
		USART_Rx_ptr_in = 0;
	}
}



/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(uint32_t*)ID1;
  Device_Serial1 = *(uint32_t*)ID2;
  Device_Serial2 = *(uint32_t*)ID3;  

  Device_Serial0 += Device_Serial2;

  if (Device_Serial0 != 0)
  {
    IntToUnicode (Device_Serial0, &Virtual_Com_Port_StringSerial[2] , 8);
    IntToUnicode (Device_Serial1, &Virtual_Com_Port_StringSerial[18], 4);
  }
}

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;
  
  for( idx = 0 ; idx < len ; idx ++)
  {
    if( ((value >> 28)) < 0xA )
    {
      pbuf[ 2* idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2* idx] = (value >> 28) + 'A' - 10; 
    }
    
    value = value << 4;
    
    pbuf[ 2* idx + 1] = 0;
  }
}


void __WFI(void)
{
	 __asm("wfi");
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
