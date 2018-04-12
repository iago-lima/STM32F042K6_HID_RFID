/*
 * wiegand_interface.c
 *
 *  Created on: 11 de abr de 2018
 *      Author: iago
 */

#include "wiegand_interface.h"
#include "stm32f0xx_gpio.h"

#define MAX_BITS 26
extern unsigned char databits[MAX_BITS];    // stores all of the data bits
extern unsigned int bitCount;              // number of bits currently captured
extern unsigned int facilityCode;        // decoded facility code
extern unsigned int cardCode;            // decoded card code

int i = 0;
void wiengand(){
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
	}
}

void clear_vars(){
	bitCount = 0;
	facilityCode = 0;
	cardCode = 0;
	for (i=0; i<MAX_BITS; i++){
		databits[i] = 0;
	}
}






