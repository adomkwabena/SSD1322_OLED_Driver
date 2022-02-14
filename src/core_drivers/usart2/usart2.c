/**
 *  @file   usart2.c
 *  @author Adom Kwabena
 *  @brief  A driver for the USART2 module of the stm32f407vgt6 microcontroller.
 * 
 *          This driver implements blocking routines for receiving and
 *          transmitting data via the USART2 peripheral.
 */

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include "usart2.h"
#include "stm32f407xx.h"

// ****************************************************************************
// * Function Prototypes of Private Functions
// ****************************************************************************

/**
 *  @brief   Initialize TX and RX pins of the USART2 peripheral.
 *  @param   None.
 *  @returns None.
 */
static inline void usart2_gpio_init(void);

// ****************************************************************************
// * Module APIs
// ****************************************************************************

static inline void usart2_gpio_init(void)
{
    // GPIO Configuration 
    // TX -> PA2
    // RX -> PA3

    // Enable GPIOA Clock
    RCC->AHB1ENR   |= RCC_AHB1ENR_GPIOAEN;

    // Connect PA2 and PA3 to AF7 (USART2)
    GPIOA->AFR[0]  &= ~(0xFUL << GPIO_AFRL_AFSEL2_Pos) | \
                      ~(0xFUL << GPIO_AFRL_AFSEL3_Pos);

    GPIOA->AFR[0]  |= (0x7UL << GPIO_AFRL_AFSEL2_Pos) | \
                      (0x7UL << GPIO_AFRL_AFSEL3_Pos);

    // Configure PA2 and PA3 as alternative function I/Os
    GPIOA->MODER   &= ~(0x3UL << GPIO_MODER_MODE2_Pos) | \
                      ~(0x3UL << GPIO_MODER_MODE3_Pos);

    GPIOA->MODER   |= (0x2UL << GPIO_MODER_MODE2_Pos) | \
                      (0x2UL << GPIO_MODER_MODE3_Pos);

    // Configure PA2 and PA3 as very high speed I/Os
    GPIOA->OSPEEDR |= (0x3UL << GPIO_OSPEEDR_OSPEED2_Pos) | \
                      (0x3UL << GPIO_OSPEEDR_OSPEED3_Pos);

    // Configure PA2 and PA3 as push pull
    GPIOA->OTYPER  &= ~(0x1UL << GPIO_OTYPER_OT2_Pos) | \
                      ~(0x1UL << GPIO_OTYPER_OT3_Pos);

    // Configure PA2 and PA3 as no pull
    GPIOA->PUPDR   &= ~(0x3UL << GPIO_PUPDR_PUPD2_Pos) | \
                      ~(0x3UL << GPIO_PUPDR_PUPD3_Pos);
}

void usart2_init(void)
{
    // Enable USART2 clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Init GPIO associated with USART2
    usart2_gpio_init();

    // Enable USART2
    USART_INSTANCE->CR1 |= USART_CR1_UE;

    // Use 1 stop bit for communication
    USART_INSTANCE->CR1 &= ~(USART_CR1_M);

    // Set baudrate of USART2
    // Baudrate = (fck) / (8 * (2 - Over8) * USARTDIV)
    // Desired baudrate = 9,600
    // USARTDIV = (fck) / (8 * (2 - Over8) * Baudrate)
    // USARTDIV = 10MHz / (8 * 2 * 9,600) ~ 65.104
    // Mantissa part   = 65 (0x41)
    // Fractional part = 0.104 * 16 ~ 2 (0x02)
    USART_INSTANCE->BRR |= (0x41UL << USART_BRR_DIV_Mantissa_Pos) |\
                           (0x02UL << USART_BRR_DIV_Fraction_Pos);

    // Enable USART transmission & reception
    USART_INSTANCE->CR1 |= USART_CR1_TE | USART_CR1_RE;
}

void usart2_transmit(uint8_t data)
{
    // Wait for previous transfer to complete
    while ((USART_INSTANCE->SR & USART_SR_TXE) == 0);

    // Transmit data
    USART_INSTANCE->DR = (uint32_t) data;

    // Wait for the transfer to complete
    while (USART_INSTANCE->SR & USART_SR_TC);
}

uint8_t usart2_receive(void)
{
    // Wait for data to be received
    while ((USART_INSTANCE->SR & USART_SR_RXNE) == 0);

    // Read received data
    uint8_t data = (uint8_t) USART_INSTANCE->DR;

    return data;
}

void usart2_put_string(char * string)
{
    while (*string != '\0')
    {
        usart2_transmit(*string++);
    }
}