/** 
 *  @file   rcc.h
 *  @author Adom Kwabena
 *  @brief  A basic RCC driver for the stm32f407vgt6 microcontroller.
 */

// Prevent multiple file inclusion.
#ifndef     __RCC_INC__
#define     __RCC_INC__

// ****************************************************************************
// * Definitions and Macros.
// ****************************************************************************

// Define system core clock in MHz.
#define SYSTEM_CORE_CLOCK 160

// ****************************************************************************
// * Function Prototypes.
// ****************************************************************************

/**
 *  @brief   Resets and initializes system clocks.
 *  @param   None.
 *  @returns None
 */
extern void system_init(void);

#endif