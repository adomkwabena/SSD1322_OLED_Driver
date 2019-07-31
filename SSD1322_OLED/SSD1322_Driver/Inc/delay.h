/**
 * delay.h
 *
 * Based on code written by Carmine Noviello, check it out using link below
 * https://www.carminenoviello.com/2015/09/04/precisely-measure-microseconds-stm32/
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include <stdint.h>

/**
 * @Summary
 *   Initializes stm32 DWT peripheral for delay routine
 *
 * @Param
 *   None
 *
 * @Returns
 *   None
 */
void delay_initialize(void);

/**
 * @Summary
 *   Spins for a duration specified by user
 *
 * @Param
 *   delay - the duration of the spin in microseconds
 *
 * @Returns
 *   None
 */
void __delay_us(uint32_t delay);


#endif /* INC_DELAY_H_ */
