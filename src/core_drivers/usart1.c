/**
 *  @file   usart1.c
 *  @author Adom Kwabena
 *  @brief  A driver for the USART1 module of the stm32f407vgt6 microcontroller.
 * 
 *          This driver implements blocking routines for receiving and
 *          transmitting data via the USART1 peripheral.
 */

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include "usart1.h"
#include "stm32f407xx.h"

// ****************************************************************************
// * Function Prototypes of Private Functions
// ****************************************************************************

/**
 *  @brief   Initialize TX and RX pins of the USART1 peripheral.
 *  @param   None.
 *  @returns None.
 */
static inline void usart1_gpio_init(void);

// ****************************************************************************
// * Module APIs
// ****************************************************************************

static inline void usart1_gpio_init(void)
{
    // GPIO Configuration 
    // TX -> PB6
    // RX -> PB7

    // Enable GPIOA Clock
    RCC->AHB1ENR   |= RCC_AHB1ENR_GPIOBEN;

    // Connect PB6 and PB7 to AF7 (USART1)
    GPIOB->AFR[0]  &= ~(0xFUL << GPIO_AFRL_AFSEL6_Pos) | \
                      ~(0xFUL << GPIO_AFRL_AFSEL7_Pos);

    GPIOB->AFR[0]  |= (0x7UL << GPIO_AFRL_AFSEL6_Pos) | \
                      (0x7UL << GPIO_AFRL_AFSEL7_Pos);

    // Configure PB6 and PB7 as alternative function I/Os
    GPIOB->MODER   &= ~(0x3UL << GPIO_MODER_MODE6_Pos) | \
                      ~(0x3UL << GPIO_MODER_MODE7_Pos);

    GPIOB->MODER   |= (0x2UL << GPIO_MODER_MODE6_Pos) | \
                      (0x2UL << GPIO_MODER_MODE7_Pos);

    // Configure PB6 and PB7 as very high speed I/Os
    GPIOB->OSPEEDR |= (0x3UL << GPIO_OSPEEDR_OSPEED6_Pos) | \
                      (0x3UL << GPIO_OSPEEDR_OSPEED7_Pos);

    // Configure PB6 and PB7 as push pull
    GPIOB->OTYPER  &= ~(0x3UL << GPIO_OTYPER_OT6_Pos) | \
                      ~(0x3UL << GPIO_OTYPER_OT7_Pos);

    // Configure PB6 and PB7 as no pull
    GPIOB->PUPDR   &= ~(0x3UL << GPIO_PUPDR_PUPD6_Pos) | \
                      ~(0x3UL << GPIO_PUPDR_PUPD7_Pos);
}

void usart1_init(void)
{
    // Enable USART1 clock
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // Init GPIO associated with USART1
    usart1_gpio_init();

    // Enable USART1
    USART_INSTANCE->CR1 |= USART_CR1_UE;

    // Use 1 stop bit for communication
    USART_INSTANCE->CR1 &= ~(USART_CR1_M);

    // Set baudrate of USART1
    // Baudrate = (fck) / (8 * (2 - Over8) * USARTDIV)
    // Desired baudrate = 9,600
    // USARTDIV = (fck) / (8 * (2 - Over8) * Baudrate)
    // USARTDIV = 20MHz / (8 * 2 * 9,600) ~ 130.208
    // Mantissa part   = 130 (0x82)
    // Fractional part = 0.208 * 16 ~ 3 (0x03)
    USART_INSTANCE->BRR |= (0x82UL << USART_BRR_DIV_Mantissa_Pos) |\
                           (0x03UL << USART_BRR_DIV_Fraction_Pos);

    // Enable USART transmission & reception
    USART_INSTANCE->CR1 |= USART_CR1_TE | USART_CR1_RE;
}

void usart1_transmit(uint8_t data)
{
    // Wait for previous transfer to complete
    while ((USART_INSTANCE->SR & USART_SR_TXE) == 0);

    // Transmit data
    USART_INSTANCE->DR = (uint32_t) data;

    // Wait for the transfer to complete
    while (USART_INSTANCE->SR & USART_SR_TC);
}

uint8_t usart1_receive(void)
{
    // Wait for data to be received
    while ((USART_INSTANCE->SR & USART_SR_RXNE) == 0);

    // Read received data
    uint8_t data = (uint8_t) USART_INSTANCE->DR;

    return data;
}

void usart1_put_string(char * string)
{
    while (*string != '\0')
    {
        usart1_transmit(*string++);
    }
}