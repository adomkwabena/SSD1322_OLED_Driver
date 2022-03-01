/** 
 *  @file   delay.h
 *  @author Adom Kwabena
 *  @brief  Implements a blocking delay routine using the DWT module.
 */

// Prevent multiple file inclusion.
#ifndef     __DELAY_INC__
#define     __DELAY_INC__

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include <stdint.h>

// ****************************************************************************
// * Definitions and Macros
// ****************************************************************************

// Delay macros.
#define delay_us(x)    __delay_us((x))
#define delay_ms(x)    __delay_us((uint32_t) (1000UL * (x)))

// ****************************************************************************
// * Function Prototypes
// ****************************************************************************

/**
 *  @brief Enables the DWT peripheral.
 */
void delay_init(void);

/**
 *  @brief Generates a blocking delay for the specified period in microseconds.
 * 
 *  @param period: The time delay required in microseconds.
 */
void __delay_us(uint32_t period);

#endif