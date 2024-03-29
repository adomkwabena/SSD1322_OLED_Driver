/** 
 *  @file   spi1.h
 *  @author Adom Kwabena
 *  @brief  A driver for the SPI1 module of the stm32f407vgt6 microcontroller.
 * 
 *          This driver implements blocking routines for receiving and
 *          transmitting data via the SPI1 peripheral.
 */

// Prevent multiple file inclusion.
#ifndef     __SPI1_INC__
#define     __SPI1_INC__

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include <stdint.h>

// ****************************************************************************
// * Definitions and Macros.
// ****************************************************************************

// Possible SPI modes
#define SPI_MODE_0      0xA1 
#define SPI_MODE_1      0xB2
#define SPI_MODE_2      0xC3
#define SPI_MODE_3      0xD4

// This macro aids in porting this driver to other SPI instances on the chip,
// this is because all SPI instances share the same register set.
#define SPI_INSTANCE SPI1

// This macro is used to transmit data via the spi1_transceive command.
#define spi1_transmit(x)   spi1_transceive((x))

// This macro is used to receive data via the spi1_transceive command.
// Dummy data is transmitted to facilitate data reception.
#define spi1_receive()     spi1_transceive(0x00)

// ****************************************************************************
// * Function Prototypes.
// ****************************************************************************

/**
 *  @brief   Initialize the spi1 module as a master in full duplex mode.
 *  @param   mode: Selects the mode (phase & polarity) of SPI data transmission.
 *  @returns None.
 */
void spi1_init(uint8_t mode);

/**
 *  @brief   Transmits and receives 8 bits of data via the SPI1 peripheral.
 *  @param   data: 8-bit wide data to be transmitted.
 *  @returns Received data (8-bit wide).
 */
uint8_t spi1_transceive(uint8_t data);

/**
 *  @brief   Transmits a buffer of 8-bit data via the SPI1 peripheral.
 *  @param   buffer: The address of the data to be transmitted.
 *  @param   buffer_size: The number of bytes to be transmitted.
 *  @returns None.
 */
void spi1_transmit_buffer(uint8_t * buffer, uint32_t buffer_size);

/**
 *  @brief   Receives a buffer of 8-bit data via the SPI1 peripheral.
 *  @param   buffer: The address where the received data will be stored.
 *  @param   buffer_size: The number of bytes to be received.
 *  @returns None.
 */
void spi1_receive_buffer(uint8_t * buffer, uint32_t buffer_size);

/**
 *  @brief   Transmits and receives 8-bit wide data via the SPI1 peripheral.
 *  @param   tx_buffer: The address of the data to be transmitted.
 *  @param   rx_buffer: The address where the received data will be stored.
 *  @returns None. 
 */
void spi1_transceive_buffer(uint8_t * tx_buffer, uint8_t * rx_buffer,
                            uint32_t buffer_size);

#endif