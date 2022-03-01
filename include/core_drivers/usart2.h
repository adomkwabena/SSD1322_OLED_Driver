/** 
 *  @file   usart2.h
 *  @author Adom Kwabena
 *  @brief  A driver for the USART2 module of the stm32f407vgt6 microcontroller.
 * 
 *          This driver implements blocking routines for receiving and
 *          transmitting data via the USART2 peripheral.
 */

// Prevent multiple file inclusion.
#ifndef     __USART2_INC__
#define     __USART2_INC__

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include <stdint.h>

// ****************************************************************************
// * Definitions and Macros
// ****************************************************************************

// Create an instance definition for the peripheral register to aid
// portability
#define USART_INSTANCE USART2

// ****************************************************************************
// * Function Prototypes.
// ****************************************************************************

/**
 *  @brief   Initializes the usart2 peripheral.
 *  @param   None.
 *  @returns None.
 */
void usart2_init(void);

/**
 *  @brief   Transmits a byte of data via the USART2 peripheral.
 *  @param   data: Data to be transmitted.
 *  @returns None.
 */
void usart2_transmit(uint8_t data);

/**
 *  @brief   Receives a byte of data via the USART2 peripheral.
 *  @param   None.
 *  @returns Received byte. 
 */
uint8_t usart2_receive(void);

/**
 *  @brief   Transmits a string via the USART2 peripheral.
 *  @param   string: The address of the first character of the string.
 *  @returns None.
 */
void usart2_put_string(char * string);

#endif