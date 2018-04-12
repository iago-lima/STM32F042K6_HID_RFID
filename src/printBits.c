/*
 * printBits.c
 *
 *  Created on: 11 de abr de 2018
 *      Author: iago
 */

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


