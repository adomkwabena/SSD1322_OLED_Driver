/**
 *  @file   main.c
 *  @author Adom Kwabena
 *  @brief  Main application program
 */

// ****************************************************************************
// * Include Files.
// ****************************************************************************
#include "stm32f407xx.h"

// ****************************************************************************
// * Application Implementation.
// ****************************************************************************

int main(void)
{
    // Enable GPIOD clock
    RCC->AHB1ENR   |= RCC_AHB1ENR_GPIODEN;
    // Configure GPIOD pins 12-15 as output
    GPIOD->MODER   |= GPIO_MODER_MODER15_0 | GPIO_MODER_MODER14_0 |\
                      GPIO_MODER_MODER13_0 | GPIO_MODER_MODER12_0;
    // Configure all GPIOD pins as push pull
    GPIOD->OTYPER   = 0x00000000;
    // Configure all GPIOD pins as no pull
    GPIOD->PUPDR    = 0x00000000;


    // Turn on LEDs on PD15, PD14, PD13 and PD12.
    while (1)
    {
        GPIOD->BSRR |= GPIO_BSRR_BS15;
        GPIOD->BSRR |= GPIO_BSRR_BS14;
        GPIOD->BSRR |= GPIO_BSRR_BS13;
        GPIOD->BSRR |= GPIO_BSRR_BS12;
    }
}