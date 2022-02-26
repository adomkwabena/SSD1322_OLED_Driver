/** 
 *  @file   spi1_test.c
 *  @author Adom Kwabena
 *  @brief  This file implements test routines for the SPI1 module.
 *          All test data can be verified via an oscilloscope or logic analyzer.
 */

// ****************************************************************************
// * Included Files.
// ****************************************************************************

#include "stm32f407xx.h"
#include "spi1_test.h"
#include "spi1.h"
#include "usart2.h"

// ****************************************************************************
// * Private Functions
// ****************************************************************************

/**
 *  @brief   Add chip select support to "spi1_transceive" function.
 *  @param   None.
 *  @returns None.
 */
static inline uint8_t __spi1_transceive__(uint8_t data)
{
    SPI1_CS_LOW();
    uint8_t received_data = spi1_transceive(data);
    SPI1_CS_HIGH();

    return received_data;
}

/**
 *  @brief   Add chip select support to "spi1_transmit_buffer" function.
 *  @param   None.
 *  @returns None.
 */
static inline void __spi1_transmit_buffer__(uint8_t * buffer, uint32_t buffer_size)
{
    SPI1_CS_LOW();
    spi1_transmit_buffer(buffer, buffer_size);
    SPI1_CS_HIGH();
}

/**
 *  @brief   Add chip select support to "spi1_receive_buffer" function.
 *  @param   None.
 *  @returns None.
 */
static inline void __spi1_receive_buffer__(uint8_t * buffer, uint32_t buffer_size)
{
    SPI1_CS_LOW();
    spi1_receive_buffer(buffer, buffer_size);
    SPI1_CS_HIGH();
}

/**
 *  @brief   Add chip select support to "spi1_transceive_buffer" function.
 *  @param   None.
 *  @returns None.
 */
static inline void __spi1_transceive_buffer__(uint8_t * tx_buffer, 
                                              uint8_t * rx_buffer,
                                              uint32_t buffer_size)
{
    SPI1_CS_LOW();
    spi1_transceive_buffer(tx_buffer, rx_buffer, buffer_size);
    SPI1_CS_HIGH();
}

// ****************************************************************************
// * Module APIs
// ****************************************************************************

void spi1_test_init(void)
{
    // Initialize PA3 as CS GPIO.

    // Enable GPIOA clock
    RCC->AHB1ENR   |=  RCC_AHB1ENR_GPIOAEN;

    // Configure PA3 as an output I/O.
    GPIOA->MODER   &= ~(0x3UL << GPIO_MODER_MODER3_Pos);
    GPIOA->MODER   |=  (0x1UL << GPIO_MODER_MODER3_Pos);

    // Configure PA3 as a very high speed I/O.
    GPIOA->OSPEEDR |=  (0x3UL << GPIO_OSPEEDR_OSPEED3_Pos);

    // Configure PA3 as a push pull I/O.
    GPIOA->OTYPER  &= ~(0x1UL << GPIO_OTYPER_OT3_Pos);

    // Disable pull up/pull down functionality on PA3.
    GPIOA->PUPDR   &= ~(0x3UL << GPIO_PUPDR_PUPD3_Pos); 

    SPI1_CS_HIGH();
}

void spi1_generate_waveform_1(void)
{
    SPI1_CS_LOW();
    spi1_transmit(0xAA);
    spi1_transmit(0x55);
    spi1_transmit(0xAA);
    spi1_transmit(0x55);
    spi1_transmit(0xAA);
    SPI1_CS_HIGH();
}

void spi1_generate_waveform_2(void)
{
    __spi1_transmit__(0xFF);
    __spi1_transmit__(0x00);
    __spi1_transmit__(0xFF);
    __spi1_transmit__(0x00);
    __spi1_transmit__(0xFF);
}

void spi1_generate_waveform_3(void)
{
    // SPI buffers
    uint8_t buffer_1[11] = {0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0x44};
    uint8_t buffer_2[11] = {0xD1, 0xD2, 0xD3, 0xD4, 0XD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0x77};

    // Transmit the first 10 elements of each buffer.
    __spi1_transmit_buffer__(&buffer_1[0], 10);
    __spi1_transmit_buffer__(&buffer_2[0], 10);

    // Transmit the last item of each buffer.
    __spi1_transmit_buffer__(&buffer_1[10], 1);
    __spi1_transmit_buffer__(&buffer_2[10], 1);
}

void spi1_generate_waveform_4(void)
{
    // In this function the tx and rx buffers are swapped at a point to verify
    // the correct functionality of the receive function.
    // Connect the MISO pin to VCC or GND and verify that 
    // the data received is correct (all 0x00s or all 0xFFs).
    // Another alternative is to connect the MOSI pin to the MISO pin,
    // in such a case the data transmitted will be received
    // and transmitted again.


    // SPI buffers
    uint8_t rx_buffer[51] = {0};
    uint8_t tx_buffer[51] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
                             0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14,
                             0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
                             0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
                             0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32,
                             0x99};

    // Transmit data in tx buffer and store input data in the rx buffer.
    __spi1_transceive_buffer__(tx_buffer, rx_buffer, 50);
    // Tx and rx buffers swapped to test receive functionality.
    __spi1_transceive_buffer__(rx_buffer, tx_buffer, 50);
    // Ensure that the last byte wasn't corrupted by the previous transfers.
    __spi1_transceive_buffer__(&tx_buffer[50], &rx_buffer[50], 1);
}

void spi1_generate_waveform_5(void)
{
    // This function tests single byte receive functionality.
    // The MISO pin can be connected to VDD to verify receive functionality.

    uint8_t data = 0x55;

    __spi1_transmit__(data);
    data = __spi1_receive__();
    __spi1_transmit__(data);
}

void spi1_generate_waveform_6(void)
{
    // This function tests the receive buffer functionality.
    // Tie MISO to GND or VDD and verify that the data sent are 0x00s or 0xFFs.

    uint8_t rx_buffer[5] = {0};

    __spi1_receive_buffer__(rx_buffer, 5);
    __spi1_transmit_buffer__(rx_buffer, 5);
}

void spi1_generate_waveform_7(void)
{
    // Test full duplex capability.
    // Tie MISO to MOSI and verify that the following sequence is received:
    // [0xCC, 0xCC, 0xDD, 0xDD, 0xEE, 0xEE].

    uint8_t data = 0xFF;
    data = __spi1_transceive__(0xCC);
    data = __spi1_transceive__(data);
    data = __spi1_transceive__(0xDD);
    data = __spi1_transceive__(data);
    data = __spi1_transceive__(0xEE);
    data = __spi1_transceive__(data);
}

void spi1_demo(void)
{
    spi1_generate_waveform_1();
    spi1_generate_waveform_2();
    spi1_generate_waveform_3();
    spi1_generate_waveform_4();
    spi1_generate_waveform_5();
    spi1_generate_waveform_6();
    spi1_generate_waveform_7();
}