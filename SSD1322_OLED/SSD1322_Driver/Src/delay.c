/**
 * delay.c
 *
 * Based on code written by Carmine Noviello, check it out using link below
 * https://www.carminenoviello.com/2015/09/04/precisely-measure-microseconds-stm32/
 */

#include "delay.h"
#include "stm32f4xx_hal.h"


void delay_initialize(void)
{
	// Enable counter
	DWT->CTRL |= 1;
	// Reset counter
	DWT->CYCCNT = 0;
}

void __delay_us(uint32_t delay)
{
	uint32_t cycles = (SystemCoreClock / 1000000L) * delay;
	uint32_t start = DWT->CYCCNT;

	// Delay
	while (DWT->CYCCNT - start < cycles);
}
