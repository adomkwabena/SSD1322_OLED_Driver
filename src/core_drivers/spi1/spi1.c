/**
 *  @file   spi1.c
 *  @author Adom Kwabena
 *  @brief  A driver for the SPI1 module of the stm32f407vgt6 microcontroller.
 * 
 *          This driver implements blocking routines for receiving and
 *          transmitting data via the SPI1 peripheral.
 */

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include "spi1.h"
#include "stm32f407xx.h"

// ****************************************************************************
// * Function Prototypes of Private Functions
// ****************************************************************************

/**
 *  @brief   Initializes GPIO associated with SPI1.
 *  @param   None.
 *  @returns None.
 */
static inline void spi1_gpio_init(void);

/**
 *  @brief   Initializes CR1 register of SPI1.
 *  @pre     "spi1_cr2_init()" should be called before this function.
 *  @param   None.
 *  @returns None.
 */
static inline void spi1_cr1_init(void);

/**
 *  @brief   Initialize CR2 register of SPI1.
 *  @param   None.
 *  @returns NOne.
 */
static inline void spi1_cr2_init(void);

// ****************************************************************************
// * Module APIs
// ****************************************************************************

static inline void spi1_gpio_init(void)
{
    // GPIO configuration
    // PA5 -> SPI1_SCK 
    // PA6 -> SPI1_MISO
    // PA7 -> SPI1_MOSI
    // CS is managed by user

    // Enable GPIOA clock
    RCC->AHB1ENR   |= RCC_AHB1ENR_GPIOAEN;

    // Connect PA5, PA6 and PA7 to AF5 (SPI1)
    GPIOA->AFR[0]  &= ~((0xFUL << GPIO_AFRL_AFSEL5_Pos) | \
                        (0xFUL << GPIO_AFRL_AFSEL6_Pos) | \
                        (0xFUL << GPIO_AFRL_AFSEL7_Pos));

    GPIOA->AFR[0]  |=   (0x5UL << GPIO_AFRL_AFSEL5_Pos) | \
                        (0x5UL << GPIO_AFRL_AFSEL6_Pos) | \
                        (0x5UL << GPIO_AFRL_AFSEL7_Pos);

    // Configure PA5, PA6 and PA7 as alternate function I/Os
    GPIOA->MODER   &= ~((0x3UL << GPIO_MODER_MODER5_Pos) | \
                        (0x3UL << GPIO_MODER_MODER6_Pos) | \
                        (0x3UL << GPIO_MODER_MODER7_Pos));

    GPIOA->MODER   |=   (0x2UL << GPIO_MODER_MODER5_Pos) | \
                        (0x2UL << GPIO_MODER_MODER6_Pos) | \
                        (0x2UL << GPIO_MODER_MODER7_Pos);

    // Configure PA5, PA6 and PA7 as very high speed I/Os
    GPIOA->OSPEEDR |=   (0x3UL << GPIO_OSPEEDR_OSPEED5_Pos) | \
                        (0x3UL << GPIO_OSPEEDR_OSPEED6_Pos) | \
                        (0x3UL << GPIO_OSPEEDR_OSPEED7_Pos);

    // Configure PA5, PA6 and PA7 as push pull
    GPIOA->OTYPER  &= ~((0x1UL << GPIO_OTYPER_OT5_Pos) | \
                        (0x1UL << GPIO_OTYPER_OT6_Pos) | \
                        (0x1UL << GPIO_OTYPER_OT7_Pos));

    // Configure PA5, PA6 and PA7 as no pull
    GPIOA->PUPDR   &= ~((0x3UL << GPIO_PUPDR_PUPD5_Pos) | \
                        (0x3UL << GPIO_PUPDR_PUPD6_Pos) | \
                        (0x3UL << GPIO_PUPDR_PUPD7_Pos));        
}

static inline void spi1_cr1_init(void)
{
    // Set SPI baud rate to PCLK/2
    SPI_INSTANCE->CR1 &= ~(0x07UL << SPI_CR1_BR_Pos);
    
    // Configure SPI1 as a master
    // Enable software slave management
    SPI_INSTANCE->CR1 |= (SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI);

    // Set CPOL = 0 and CPHA = 0
    // Select 8-bit data frame format
    // Data is transmitted MSB first
    // Disable hardware CRC 
    // Use SPI1 in full duplex mode
    // Enable 2-line unidirectional data mode
    SPI_INSTANCE->CR1 &= ~(SPI_CR1_CPOL     | SPI_CR1_CPHA  | SPI_CR1_DFF | \
                           SPI_CR1_LSBFIRST | SPI_CR1_CRCEN | SPI_CR1_RXONLY | \
                           SPI_CR1_BIDIMODE);            

    // Enable SPI1
    SPI_INSTANCE->CR1 |= SPI_CR1_SPE;
}

static inline void spi1_cr2_init(void)
{
    // Disable SPI1 interrupts
    SPI_INSTANCE->CR2 &= ~(SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE);

    // Set SPI1 to Motorola mode
    SPI_INSTANCE->CR2 &= ~(SPI_CR2_FRF);

    // Disable SPI1 TX & RX DMA capability
    SPI_INSTANCE->CR2 &= ~(SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
}

void spi1_init(void)
{
    // Enable SPI1 peripheral
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    
    // Initialize associated SPI1 GPIO
    spi1_gpio_init();
    
    // Initialize control register 2 settings
    spi1_cr2_init();

    // Initialize control register 1 settings.
    // This is called last because the last statement
    // of the spi1_cr1_init() function enables the SPI1 module.
    spi1_cr1_init();
}

uint8_t spi1_transceive(uint8_t data)
{
    // Wait for previous transfer to complete
    while ((SPI_INSTANCE->SR & SPI_SR_TXE) == 0);

    // Send data
    SPI_INSTANCE->DR = (uint32_t) data;

    // Wait for data to be received
    while ((SPI_INSTANCE->SR & SPI_SR_RXNE) == 0);

    // Read received data
    uint8_t received_data = (uint8_t) SPI_INSTANCE->DR;

    // Wait for last transfer to complete
    while ((SPI_INSTANCE->SR & SPI_SR_TXE) == 0 || \
           (SPI_INSTANCE->SR & SPI_SR_BSY));

    return received_data;
}


void spi1_transmit_buffer(uint8_t * buffer, uint32_t buffer_size)
{
    for (uint32_t i = 0; i < buffer_size; i++)
    {
        // Wait for previous transfer to complete
        while ((SPI_INSTANCE->SR & SPI_SR_TXE) == 0);

        // Send data
        SPI_INSTANCE->DR = *buffer++;
    }

    // Wait for last transfer to complete
    while ((SPI_INSTANCE->SR & SPI_SR_TXE) == 0 || \
           (SPI_INSTANCE->SR & SPI_SR_BSY));

    // Clear the overrun flag because we don't need to read the DR here.
    // This is done by reading the data register and then the status register.
    uint8_t data __attribute__((unused)) = SPI_INSTANCE->DR;
    data = SPI_INSTANCE->SR;
}

void spi1_receive_buffer(uint8_t * buffer, uint32_t buffer_size)
{
    for (uint32_t i = 0; i < buffer_size; i++)
    {
        // Wait for previous transfer to complete
        while ((SPI_INSTANCE->SR & SPI_SR_TXE) == 0);

        // Send some dummy data - this is needed to generate clock
        // pulses for the slave device to shift out data.
        SPI_INSTANCE->DR = (uint32_t) 0x00;

        // Wait for data to be received
        while ((SPI_INSTANCE->SR & SPI_SR_RXNE) == 0);

        // Read received data
        *buffer++ = (uint8_t) SPI_INSTANCE->DR;
    }

    // Wait for last transfer to complete
    while ((SPI_INSTANCE->SR & SPI_SR_TXE) == 0 || \
           (SPI_INSTANCE->SR & SPI_SR_BSY));
}

void spi1_transceive_buffer(uint8_t * tx_buffer, uint8_t * rx_buffer,
                            uint32_t buffer_size)
{
    for (uint32_t i = 0; i < buffer_size; i++)
    {
        // Wait for previous transfer to complete
        while ((SPI_INSTANCE->SR & SPI_SR_TXE) == 0);

        // Send data
        SPI_INSTANCE->DR = *tx_buffer++;

        // Wait for data to be received
        while ((SPI_INSTANCE->SR & SPI_SR_RXNE) == 0);

        // Read received data
        *rx_buffer++ = (uint8_t) SPI_INSTANCE->DR;
    }

    // Wait for last transfer to complete
    while ((SPI_INSTANCE->SR & SPI_SR_TXE) == 0 || \
           (SPI_INSTANCE->SR & SPI_SR_BSY));
}