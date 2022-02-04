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
#include "usart1.h"

// ****************************************************************************
// * Application Implementation.
// ****************************************************************************

int main(void)
{
    gpio_init();
    spi1_init();
    spi1_test_init();
    usart1_init();

    while (1)
    {
        // Turn all LEDs on
        RED_LED_ON();
        BLUE_LED_ON();
        GREEN_LED_ON();
        ORANGE_LED_ON();

        // Run the spi1 demo - waveforms can be viewed using a logic analyzer.
        // The pinouts for the spi1 module can be found in the 
        // "spi1_gpio_init" function.
        spi1_demo();
        usart1_put_string("It is that time! ");

        // Turn all LEDs off
        RED_LED_OFF();
        BLUE_LED_OFF();
        GREEN_LED_OFF();
        ORANGE_LED_OFF();

    }
}