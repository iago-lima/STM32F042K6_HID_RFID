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
#include <stdlib.h>

#define MAX_BITS 26                 // max number of bits

void printBits(unsigned int facilityCode, unsigned int cardCode){

	char facility_aux[30];
	char card_aux[30];
	sprintf(facility_aux, "%u", facilityCode);
	sprintf(card_aux, "%u", cardCode);
	USART1_PutString("Decimal: ");
	USART1_PutString("FC = ");
	USART1_PutString(facility_aux);
	USART1_PutString(", CC = ");
	USART1_PutString(card_aux);
	USART1_PutString(" ");


	sprintf(facility_aux, "%x", facilityCode);
	sprintf(card_aux, "%x", cardCode);
	USART1_PutString("Hexadecimal: ");
	USART1_PutString("FC = ");
	USART1_PutString(facility_aux);
	USART1_PutString(", CC = ");
	USART1_PutString(card_aux);

}
int main(void){
	GPIO_InitTypeDef GPIO_Configure;
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

	unsigned char databits[MAX_BITS];    // stores all of the data bits
	unsigned int bitCount = 0;              // number of bits currently captured
	unsigned int facilityCode = 0;        // decoded facility code
	unsigned int cardCode = 0;            // decoded card code

	int i;
	USART1_DeInit();
	USART1_PutString("Read RFID\n");

	for (i=0; i<MAX_BITS; i++){
		databits[i] = 0;
	}
	while(1){
		if((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)){
			bitCount++;
			while((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0));
			//USART1_PutString("0");
		}if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0){
			//USART1_PutString("1");
			databits[bitCount] = 1;
			bitCount++;
			while((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0));
		}if(bitCount == 26){

			// facility code = bits 2 to 9
			for (i=1; i<9; i++){
				facilityCode <<=1;
				facilityCode |= databits[i];
			}

			// card code = bits 10 to 23
			for (i=9; i<25; i++){
				cardCode <<=1;
				cardCode |= databits[i];
			}


			printBits(facilityCode, cardCode);
			USART1_PutString("\n");
			// cleanup and get ready for the next card
			bitCount = 0;
			facilityCode = 0;
			cardCode = 0;
			for (i=0; i<MAX_BITS; i++){
				databits[i] = 0;
			}
		}
	}
}
