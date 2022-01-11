/** 
 *  @file   spi1_test.h
 *  @author Adom Kwabena
 *  @brief  This file implements test routines for the SPI1 module.
 *          All test data can be verified via an oscilloscope or logic analyzer.
 */

// Prevent multiple file inclusion.
#ifndef     __SPI1_TEST_INC__
#define     __SPI1_TEST_INC__

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include "stm32f407xx.h"
#include "spi1.h"

// ****************************************************************************
// * Definitions and Macros.
// ****************************************************************************

// This macro is used to transmit data via the spi1_transceive command.
#define __spi1_transmit__(x)    __spi1_transceive__((x))

// This macro is used to receive data via the spi1_transceive command.
// Dummy data is transmitted to facilitate data reception.
#define __spi1_receive__()      __spi1_transceive__(0x00)

// Use PA3 as chip select pin.
#define SPI1_CS_HIGH()  GPIOA->BSRR |= GPIO_BSRR_BS3
#define SPI1_CS_LOW()   GPIOA->BSRR |= GPIO_BSRR_BR3

// ****************************************************************************
// * Function Prototypes.
// ****************************************************************************

/**
 *  @brief   Initializes the SPI1 test module.
 *  @pre     Initialize the SPI1 module prior to use.
 *  @param   None.
 *  @returns None.
 */
void spi1_test_init();

/**
 *  @brief   Generates waveforms using the "spi1_transmit" function.
 *  @param   None.
 *  @returns None.
 */
void spi1_generate_waveform_1(void);

/**
 *  @brief   Generates waveforms using the "spi1_transmit" function.
 *  @param   None.
 *  @returns None.
 */
void spi1_generate_waveform_2(void);

/**
 *  @brief   Generates waveforms using the "spi1_transmit_buffer" function.
 *  @param   None.
 *  @returns None.
 */
void spi1_generate_waveform_3(void);

/**
 *  @brief   Generates waveforms using the "spi1_transceive_buffer" function.
 *  @param   None.
 *  @returns None.
 */
void spi1_generate_waveform_4(void);

/**
 *  @brief   Generates waveforms using the "spi1_transmit" and "spi1_receive" functions.
 *  @param   None.
 *  @returns None.
 */
void spi1_generate_waveform_5(void);

/**
 *  @brief   Generates waveforms using the "spi1_transmit_buffer"
 *           and "spi1_receive_buffer" functions.
 *  @param   None.
 *  @returns None.
 */
void spi1_generate_waveform_6(void);

/**
 *  @brief   Runs all waveform generation functions to provide a demo.
 *  @param   None.
 *  @returns None.
 */
void spi1_demo();

#endif