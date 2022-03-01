/**
 *  @file   main.c
 *  @author Adom Kwabena
 *  @brief  Main application program
 */

// ****************************************************************************
// * Include Files.
// ****************************************************************************

#include "stm32f407xx.h"
#include "gpio.h"
#include "spi1.h"
#include "spi1_test.h"
#include "usart2.h"
#include "i2c1.h"
#include "i2c1_test.h"
#include "delay.h"

// ****************************************************************************
// * Application Implementation.
// ****************************************************************************

int main(void)
{
    gpio_init();
    spi1_init();
    spi1_test_init();
    i2c1_init();
    i2c1_test_init();
    usart2_init();
    delay_init();


    while (1)
    {
        // Turn all LEDs on
        RED_LED_ON();
        BLUE_LED_ON();
        GREEN_LED_ON();
        ORANGE_LED_ON();

        // Wait for 200ms
        delay_ms(200);

        // Run demos of the communication protocols.
        // All - waveforms can be viewed using a logic analyzer.

        // Run the spi1 demo 
        // Pinout information is in the "spi1_gpio_init" function.
        spi1_demo();
        // Run the i2c1 demo - ensure the right peripheral is connected,
        // otherwise the application will hang.
        // Pinout information is in the "i2c1_gpio_init" function.
        i2c1_demo();
        // Run usart2 demo
        // Pinout information is in the "usart2_gpio_init" function.
        // USART1 is not used here because of a GPIO conflict with i2c1
        usart2_put_string("It is that time! ");

        // Turn all LEDs off
        RED_LED_OFF();
        BLUE_LED_OFF();
        GREEN_LED_OFF();
        ORANGE_LED_OFF();

        // Wait for 200ms
        delay_ms(200);

    }
}