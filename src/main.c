/**
  ******************************************************************************
  * @file    main.c
  * @author  Iago Lima
  * @version V1.0
  * @date    07-March-2018
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "usart.h"
#include "printBits.h"
#include "wiegand_interface.h"
#include <stdlib.h>

#define MAX_BITS 26                 // max number of bits
unsigned char databits[MAX_BITS];    // stores all of the data bits
unsigned int bitCount = 0;              // number of bits currently captured
unsigned int facilityCode = 0;        // decoded facility code
unsigned int cardCode = 0;            // decoded card code


int main(void){
	GPIO_InitTypeDef GPIO_Configure_In;
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

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

	GPIO_Configure_In.GPIO_Pin = (GPIO_Pin_5 | GPIO_Pin_4); //Pino 4 Data0, Pino 5 Data1
	GPIO_Configure_In.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Configure_In.GPIO_OType = GPIO_OType_PP;
	GPIO_Configure_In.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Configure_In.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_Configure_In);

	int i;
	USART1_DeInit();
	USART1_PutString("Read RFID\n");

	clear_vars();

	while(1){
		wiengand();
		if(bitCount == 26){
			printBits(facilityCode, cardCode);
			USART1_PutString("\n");
			// cleanup and get ready for the next card
			clear_vars();
		}
	}
}

