/** 
 *  @file   usart1.h
 *  @author Adom Kwabena
 *  @brief  A driver for the USART1 module of the stm32f407vgt6 microcontroller.
 * 
 *          This driver implements blocking routines for receiving and
 *          transmitting data via the USARTI1 peripheral.
 */

// Prevent multiple file inclusion.
#ifndef     __USART1_INC__
#define     __USART1_INC__

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include <stdint.h>

// ****************************************************************************
// * Definitions and Macros
// ****************************************************************************

// Create an instance definition for the peripheral register to aid
// portability
#define USART_INSTANCE USART1

// ****************************************************************************
// * Function Prototypes.
// ****************************************************************************

/**
 *  @brief   Initializes the usart1 peripheral.
 *  @param   None.
 *  @returns None.
 */
void usart1_init(void);

/**
 *  @brief   Transmits a byte of data via the USART1 peripheral.
 *  @param   data: Data to be transmitted.
 *  @returns None.
 */
void usart1_transmit(uint8_t data);

/**
 *  @brief   Receives a byte of data via the USART1 peripheral.
 *  @param   None.
 *  @returns Received byte. 
 */
uint8_t usart1_receive(void);

/**
 *  @brief   Transmits a string via the USART1 peripheral.
 *  @param   string: The address of the first character of the string.
 *  @returns None.
 */
void usart1_put_string(char * string);

#endif