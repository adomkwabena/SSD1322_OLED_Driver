/** 
 *  @file   gpio.h
 *  @author Adom Kwabena
 *  @brief  A driver to simplify system wide GPIO     
 */

// Prevent multiple file inclusion.
#ifndef     __GPIO_INC__
#define     __GPIO_INC__

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include "stm32f407xx.h"

// ****************************************************************************
// * Definitions and Macros
// ****************************************************************************

// Macros for LEDs on STM32F4 discovery board
#define RED_LED_ON()      GPIOD->BSRR |= GPIO_BSRR_BS14
#define BLUE_LED_ON()     GPIOD->BSRR |= GPIO_BSRR_BS15
#define GREEN_LED_ON()    GPIOD->BSRR |= GPIO_BSRR_BS12
#define ORANGE_LED_ON()   GPIOD->BSRR |= GPIO_BSRR_BS13

#define RED_LED_OFF()     GPIOD->BSRR |= GPIO_BSRR_BR14
#define BLUE_LED_OFF()    GPIOD->BSRR |= GPIO_BSRR_BR15
#define GREEN_LED_OFF()   GPIOD->BSRR |= GPIO_BSRR_BR12
#define ORANGE_LED_OFF()  GPIOD->BSRR |= GPIO_BSRR_BR13

// ****************************************************************************
// * Function Prototypes
// ****************************************************************************

/**
 *  @brief   Initializes application specific gpio.
 *  @param   None.
 *  @returns None.
 */
void gpio_init(void);

#endif