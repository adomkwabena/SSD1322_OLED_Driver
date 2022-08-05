/** 
 *  @file   gpio.c
 *  @author Adom Kwabena
 *  @brief  A driver to simplify system wide GPIO setup.
 */

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include "stm32f407xx.h"

// ****************************************************************************
// * Private Functions
// ****************************************************************************

/**
 *  @brief   Initialize GPIOs connected to LEDs on board.
 *  @param   None.
 *  @returns None.
 */
static inline void led_gpio_init(void)
{
    // Enable GPIOD clock
    RCC->AHB1ENR   |= RCC_AHB1ENR_GPIODEN;

    // Configure PD4, PD12, PD13, PD14 & PD15 as output I/O.
    GPIOD->MODER   &= ~(0x3UL << GPIO_MODER_MODER12_Pos) | 
                      ~(0x3UL << GPIO_MODER_MODER13_Pos) |
                      ~(0x3UL << GPIO_MODER_MODER14_Pos) |
                      ~(0x3UL << GPIO_MODER_MODER15_Pos);

    GPIOD->MODER   |=  (0x1UL << GPIO_MODER_MODER12_Pos) | 
                       (0x1UL << GPIO_MODER_MODER13_Pos) |
                       (0x1UL << GPIO_MODER_MODER14_Pos) |
                       (0x1UL << GPIO_MODER_MODER15_Pos);

    // Configure PD4, PD12, PD13, PD14 & PD15 as very high speed I/O.
    GPIOD->OSPEEDR |=  (0x3UL << GPIO_OSPEEDR_OSPEED12_Pos) |
                       (0x3UL << GPIO_OSPEEDR_OSPEED13_Pos) |
                       (0x3UL << GPIO_OSPEEDR_OSPEED14_Pos) |
                       (0x3UL << GPIO_OSPEEDR_OSPEED15_Pos);

    // Configure PD4, PD12, PD13, PD14 & PD15 as push pull I/O.
    GPIOD->OTYPER  &= ~(0x1UL << GPIO_OTYPER_OT12_Pos) | 
                      ~(0x1UL << GPIO_OTYPER_OT13_Pos) |
                      ~(0x1UL << GPIO_OTYPER_OT14_Pos) |
                      ~(0x1UL << GPIO_OTYPER_OT15_Pos);

    // Disable pull up/pull down functionality on PD4, PD12, PD13, PD14 & PD15.
    GPIOD->PUPDR   &= ~(0x3UL << GPIO_PUPDR_PUPD12_Pos) | 
                      ~(0x3UL << GPIO_PUPDR_PUPD13_Pos) |
                      ~(0x3UL << GPIO_PUPDR_PUPD14_Pos) |
                      ~(0x3UL << GPIO_PUPDR_PUPD15_Pos);
}

// ****************************************************************************
// * Module APIs
// ****************************************************************************

void gpio_init(void)
{
    // Initialize GPIOs connected to LEDs and audio reset pin.
    led_gpio_init();
}