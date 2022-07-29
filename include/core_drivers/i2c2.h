/** 
 *  @file   i2c2.h
 *  @author Adom Kwabena
 *  @brief  A driver for the I2C2 module of the stm32f407vgt6 microcontroller.
 * 
 *          This driver implements blocking routines for receiving and
 *          transmitting data via the I2C2 peripheral in master mode.
 */

// Prevent multiple file inclusion.
#ifndef     __I2C2_INC__
#define     __I2C2_INC__

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include <stdint.h>

// ****************************************************************************
// * Definitions and Macros 
// ****************************************************************************

// Define I2C module instance to ease portability
#define I2C_INSTANCE I2C2

// ****************************************************************************
// * Function Prototypes
// ****************************************************************************

/**
 *  @brief   Initializes the I2C2 peripheral.
 * 
 *  @param   None.
 * 
 *  @returns None.
 */
void i2c2_init(void);

/**
 *  @brief   Transmits a single byte via the i2c2 peripheral.
 * 
 *  @param   address: The address of the connneced i2c device. 
 *  @param   data: Byte to be sent. 
 * 
 *  @returns None.
 */
void i2c2_transmit(uint8_t address, uint8_t data);

/**
 *  @brief   Transmits one or more bytes of data via the i2c2 bus.
 * 
 *  @param   address: The address of the connected i2c device. 
 *  @param   buffer: A pointer to the data to be transmitted. 
 *  @param   buffer_size: The number of bytes to be sent.
 * 
 *  @returns None
 */
void i2c2_transmit_buffer(uint8_t address, uint8_t * buffer, uint32_t buffer_size);

/**
 *  @brief   Receives a byte in master mode via the i2c2 bus.
 * 
 *  @param   address: The address of the connected i2c device.
 * 
 *  @returns Received byte. 
 */
uint8_t i2c2_receive(uint8_t address);

/**
 *  @brief   Receives one or more bytes from via the i2c2 bus.
 * 
 *  @param   address: The address of the connected i2c device.
 *  @param   buffer: A pointer to an array or byte for receiving data. 
 *  @param   buffer_size: The number of bytes to be read.
 * 
 *  @returns None
 */
void i2c2_receive_buffer(uint8_t address, uint8_t * buffer, \
                         uint32_t buffer_size);

#endif