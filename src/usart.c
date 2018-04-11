/**
  ******************************************************************************
  * @file    usart.c
  * @author  thiagosilva@trixlog.com
  * @version V1.0.0
  * @date    19-April-2017
  * @brief   This file provides firmware functions to manage functionalities
  *          of the USART peripheral.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RX_Buffer recvBuffer;

/* Private function prototypes -----------------------------------------------*/
void USART1_LowLevel_Init();

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the peripheral used by the USART1 driver.
  * @param  None
  * @retval None
  */
void USART1_LowLevel_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Enable APB2 peripheral clock for USART1 */
	RCC_APB2PeriphClockCmd(USART1_PERIPHERAL_CLK,ENABLE);

	/* Enable peripheral clock for pins used by USART1 */
	RCC_AHBPeriphClockCmd(USART1_GPIO_CLK,ENABLE);

	// Pins 9(TX) and 10(RX) are used
	GPIO_InitStruct.GPIO_Pin 						= (USART1_TX_PIN | USART1_RX_PIN);
	GPIO_InitStruct.GPIO_Mode	 					= GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType 						= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd 						= GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed 						= GPIO_Speed_50MHz;
	GPIO_Init(USART1_GPIO_PORT,&GPIO_InitStruct);

	/* The RX and TX pins are now connected to their AF
	 * so that the USART1 can take over control of the
	 * pins
	 */
	GPIO_PinAFConfig(USART1_GPIO_PORT,USART1_TX_SOURCE,USART1_AF);
	GPIO_PinAFConfig(USART1_GPIO_PORT,USART1_RX_SOURCE,USART1_AF);
}

/**
  * @brief  Initialize USART peripheral.
  * @param  (uint32_t) USART baudrate
  * @retval None
  */
void USART1_Init(uint32_t baudrate)
{
	USART_InitTypeDef USART_InitStruct;

	USART1_LowLevel_Init(); // Initial configuration for clock and GPIO pins used as TX and RX

	USART_InitStruct.USART_BaudRate 				= baudrate;
	USART_InitStruct.USART_WordLength 				= USART_WordLength_8b;
	USART_InitStruct.USART_StopBits 				= USART_StopBits_1;
	USART_InitStruct.USART_Parity 					= USART_Parity_No;
	USART_InitStruct.USART_Mode 					= (USART_Mode_Tx | USART_Mode_Rx);
	USART_InitStruct.USART_HardwareFlowControl 		= USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStruct);

	USART_Cmd(USART1,ENABLE);
}

/**
  * @brief  Initialize USART peripheral with default configuration.
  * @param  None
  * @retval None
  */
void USART1_DeInit()
{
	USART1_Init(USART1_DEFAULT_BAUDRATE);
}

/**
  * @brief  Configure and enable USART1 rx interrupt.
  * @param  None
  * @retval None
  */
void USART1_EnableRxInterrupt()
{
	NVIC_InitTypeDef NVIC_InitStruct;

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //enable the USART1 receive interrupt

	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief	Send a byte by the USART1.
  * @param  uint8_t byte
  * @retval None
  */
void USART1_PutByte(uint8_t byte)
{
	while(!USART_GetFlagStatus(USART1,USART_ISR_TC));
	USART_SendData(USART1,byte);
}

/**
  * @brief	Send a string by the USART1.
  * @param  char[] string
  * @retval None
  */
void USART1_PutString(volatile char *string)
{
	while(*string)
	{
		USART1_PutByte(*string++);
	}
}

/**
  * @brief	Send a array message by the USART1.
  * @param  uint8_t *byte
  * @retval None
  */
void USART1_SendMessage(uint8_t *msg)
{
	while(*msg)
	{
		USART1_PutByte(*msg++);
	}
}

/**
  * @brief	Handler for USART1 rx interrupt.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		// TODO USART message reception
	}
}

/*******************************END OF FILE************************************/
