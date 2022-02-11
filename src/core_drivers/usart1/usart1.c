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
    // TX -> PA9
    // RX -> PA10

    // Enable GPIOA Clock
    RCC->AHB1ENR   |= RCC_AHB1ENR_GPIOAEN;

    // Connect PA9 and PA10 to AF7 (USART1)
    GPIOA->AFR[1]  &= ~(0xFUL << GPIO_AFRH_AFSEL9_Pos) | \
                      ~(0xFUL << GPIO_AFRH_AFSEL10_Pos);

    GPIOA->AFR[1]  |= (0x7UL << GPIO_AFRH_AFSEL9_Pos) | \
                      (0x7UL << GPIO_AFRH_AFSEL10_Pos);

    // Configure PA9 and PA10 as alternative function I/Os
    GPIOA->MODER   &= ~(0x3UL << GPIO_MODER_MODE9_Pos) | \
                      ~(0x3UL << GPIO_MODER_MODE10_Pos);

    GPIOA->MODER   |= (0x2UL << GPIO_MODER_MODE9_Pos) | \
                      (0x2UL << GPIO_MODER_MODE10_Pos);

    // Configure PA9 and PA10 as very high speed I/Os
    GPIOA->OSPEEDR |= (0x3UL << GPIO_OSPEEDR_OSPEED9_Pos) | \
                      (0x3UL << GPIO_OSPEEDR_OSPEED10_Pos);

    // Configure PA9 and PA10 as push pull
    GPIOA->OTYPER  &= ~(0x3UL << GPIO_OTYPER_OT9_Pos) | \
                      ~(0x3UL << GPIO_OTYPER_OT10_Pos);

    // Configure PA9 and PA10 as no pull
    GPIOA->PUPDR   &= ~(0x3UL << GPIO_PUPDR_PUPD9_Pos) | \
                      ~(0x3UL << GPIO_PUPDR_PUPD10_Pos);
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