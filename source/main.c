#include <stdio.h>
#include <stdint.h>
#include "MK64F12.h"

#define default_time 1000000

void delay(int32_t delay_time) {
	for(int32_t decrementer = delay_time; decrementer > 0; decrementer--) {
		__asm("nop");
	}
}

void red_on() {
	GPIOB->PCOR |= (1<<22);
	GPIOB->PSOR |= (1<<21);
	GPIOE->PSOR |= (1<<26);
}
void blue_on() {
	GPIOB->PSOR |= (1<<22);
	GPIOB->PCOR |= (1<<21);
	GPIOE->PSOR |= (1<<26);
}
void green_on() {
	GPIOB->PSOR |= (1<<22);
	GPIOB->PSOR |= (1<<21);
	GPIOE->PCOR |= (1<<26);
}

void white_toggle(){
	GPIOB->PTOR |= (1<<22);
	GPIOB->PTOR |= (1<<21);
	GPIOE->PTOR |= (1<<26);
}

void all_off(){
	GPIOB->PSOR |= (1<<22);
	GPIOB->PSOR |= (1<<21);
	GPIOE->PSOR |= (1<<26);
}

int main(void){
	SIM->SCGC5 = (1<<9) | (1<<10) | (1<<11) | (1<<13);	// Port A, B, C, E

	PORTB->PCR[22] = PORT_PCR_MUX(1);	// GPIO
	PORTB->PCR[21] = PORT_PCR_MUX(1);
	PORTE->PCR[26] = PORT_PCR_MUX(1);
	PORTA->PCR[4] = PORT_PCR_MUX(1) | 3;	// GPIO and PULL up resistor
	PORTC->PCR[6] = PORT_PCR_MUX(1) | 3;
	
	GPIOB->PDOR |= (1<<22) | (1<<21);	// Turn off led red and blue
	GPIOE->PDOR |= (1<<26);				// Turn off led green

	GPIOB->PDDR |= (1<<22) | (1<<21);	//LEDS as output
	GPIOE->PDDR |= (1<<26);
	GPIOA->PDDR &= ~(1<<4);	//SW as INPUT
	GPIOC->PDDR &= ~(1<<6);

	while(1) {
		int32_t sw2 = GPIOC->PDIR & (1<<6);		//Read sw value
		int32_t sw3 = GPIOA->PDIR & (1<<4);

		int32_t delay_time = ((1<<6) == sw2)?default_time:default_time/3; //Ternary operator to select between default time and its third

		if((1<<4)==sw3){	//Check if sw3 is pressed, if it's not pressed do the RGB sequence
			red_on();
			delay(delay_time);
			all_off();
			green_on();
			delay(delay_time);
			all_off();
			blue_on();
			delay(delay_time);
			all_off();
		} else {	//If it's pressed do the black and white sequence
			white_toggle();
			delay(delay_time);
			all_off();
		}
	}

	return 0;
}
