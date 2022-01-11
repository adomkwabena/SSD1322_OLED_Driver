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

// ****************************************************************************
// * Application Implementation.
// ****************************************************************************

int main(void)
{
    gpio_init();

    while (1)
    {
        // Turn all LEDs on
        RED_LED_ON();
        BLUE_LED_ON();
        GREEN_LED_ON();
        ORANGE_LED_ON();
    }
}