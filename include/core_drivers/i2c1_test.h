/** 
 *  @file   i2c1_test.h
 *  @author Adom Kwabena
 *  @brief  This file implements test routines for the I2C1 module.
 *          All test data can be verified via an oscilloscope or logic analyzer.
 */

// Prevent multiple file inclusion.
#ifndef     __I2C1_TEST_INC__
#define     __I2C1_TEST_INC__

// ****************************************************************************
// * Definitions and Macros
// ****************************************************************************

// Macros for the reset pin of the audio chip on the STM32F4 discovery board
#define AUDIO_RST_ON()    GPIOD->BSRR |= GPIO_BSRR_BS4
#define AUDIO_RST_OFF()   GPIOD->BSRR |= GPIO_BSRR_BR4

// ****************************************************************************
// * Function Prototypes
// ****************************************************************************

/**
 *  @brief Performs initialization needed to test the i2c1 module.
 */
void i2c1_test_init(void);

/**
 *  @brief Generate an i2c1 demo using connected peripherals.
 */
void i2c1_demo(void);

#endif