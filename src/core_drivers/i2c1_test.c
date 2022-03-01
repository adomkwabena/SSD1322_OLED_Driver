/** 
 *  @file   i2c1_test.c
 *  @author Adom Kwabena
 *  @brief  This file implements test routines for the I2C1 module.
 *          All test data can be verified via an oscilloscope or logic analyzer.
 */

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include "stm32f407xx.h"
#include "i2c1_test.h"
#include "i2c1.h"

// ****************************************************************************
// * Private Functions
// ****************************************************************************

/**
 *  @brief Generate an i2c1 demo using the onboard CS43L22 chip. 
 *         Refer to CS43L22 datasheet for more details.
 */
static inline void i2c1_cs43l22_demo(void)
{
    // Enable CS43L22 chip
    AUDIO_RST_ON();

    // ************************************************************************
    // *
    // Test transmission and reception of single bytes
    // Request a non auto-incrementing access of register 0x01
    i2c1_transmit(0x94, 0x01);
    // Read from fregister 0x01 - Read value should be 0b11100xxx
    i2c1_receive(0x94);

    // Request a non auto-incrementing access of register 0x28
    i2c1_transmit(0x94, 0x28);
    // Read from fregister 0x28 - Read value should be 0x7F
    i2c1_receive(0x94);
    // End of test
    // *
    // ************************************************************************

    // ************************************************************************
    // *
    // Test transmission and reception of multiple bytes
    // Create an array for transmitting and receiving multiple bytes
    uint8_t buffer[10] = {0};

    // Request auto-incrementing access of CS43L22 registers from address 0x02
    i2c1_transmit(0x94, 0x02 | 0x80);
    // Read 9 bytes from the chip starting from address 0x02
    // Read data should be [0x01, 0x07, 0x05, 0xA0, 0x00, 0x00, 0x81, 0x81, 0xA5]
    i2c1_receive_buffer(0x94, buffer, 9);

    // Request auto-incrementing access of CS43L22 registers from address 0x02
    i2c1_transmit(0x94, 0x02 | 0x80);
    // Transmit the data that was previously read.
    // This data should be [0x01, 0x07, 0x05, 0xA0, 0x00, 0x00, 0x81, 0x81, 0xA5]
    i2c1_transmit_buffer(0x94, buffer, 9);

    // Test reception of two bytes
    // Request auto-incrementing access of CS43L22 registers from address 0x1F
    i2c1_transmit(0x94, 0x1F | 0x80);
    // Read data should be [0x88, 0x00]
    i2c1_receive_buffer(0x94, buffer, 2);

    // Test reception of three bytes
    // Request auto-incrementing access of CS43L22 registers from address 0x32
    i2c1_transmit(0x94, 0x32 | 0x80);
    // Read data should be [0x3B, 0x00, 0x5F]
    i2c1_receive_buffer(0x94, buffer, 3);
    // End of test
    // *
    // ************************************************************************
}

// ****************************************************************************
// * Module APIs
// ****************************************************************************

void i2c1_test_init(void)
{
    // Initialize PD4 as a GPIO.
    // PD4 is connected to the enable pin of the CS43L22 chip on board 
    // the STM32F4 discovery board.

    // Enable GPIOD clock
    RCC->AHB1ENR   |=  RCC_AHB1ENR_GPIODEN;

    // Configure PD4 as an output I/O.
    GPIOD->MODER   &= ~(0x3UL << GPIO_MODER_MODER4_Pos);
    GPIOD->MODER   |=  (0x1UL << GPIO_MODER_MODER4_Pos);

    // Configure PD4 as a very high speed I/O.
    GPIOD->OSPEEDR |=  (0x3UL << GPIO_OSPEEDR_OSPEED4_Pos);

    // Configure PD4 as a push pull I/O.
    GPIOD->OTYPER  &= ~(0x1UL << GPIO_OTYPER_OT4_Pos);

    // Disable pull up/pull down functionality on PD4.
    GPIOD->PUPDR   &= ~(0x3UL << GPIO_PUPDR_PUPD4_Pos);

    i2c1_init(); 
}

void i2c1_demo(void)
{
    // Run CS43L22 i2c demo
    i2c1_cs43l22_demo();
}