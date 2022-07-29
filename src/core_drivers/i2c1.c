/** 
 *  @file   i2c1.c
 *  @author Adom Kwabena
 *  @brief  A driver for the I2C1 module of the stm32f407vgt6 microcontroller.
 * 
 *          This driver implements blocking routines for receiving and
 *          transmitting data via the I2C1 peripheral.
 */

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include "i2c1.h"
#include "stm32f407xx.h"

// ****************************************************************************
// * Function Prototypes Of Private Functions
// ****************************************************************************

/**
 *  @brief   Initializes GPIO associated with I2C1 peripheral.
 * 
 *  @param   None.
 * 
 *  @returns None.
 */
static inline void i2c1_gpio_init(void);

// ****************************************************************************
// * Private Functions
// ****************************************************************************

static inline void i2c1_gpio_init(void)
{
    // GPIO Configuration 
    // SCL -> PB6
    // SDA -> PB9

    // Enable GPIOB Clock
    RCC->AHB1ENR   |= RCC_AHB1ENR_GPIOBEN;

    // Connect PB6 to AF4 (I2C1)
    GPIOB->AFR[0]  &= ~(0xFUL << GPIO_AFRL_AFSEL6_Pos);
    GPIOB->AFR[0]  |=  (0x4UL << GPIO_AFRL_AFSEL6_Pos);

    // Connect PB9 to AF4 (I2C1)
    GPIOB->AFR[1]  &= ~(0xFUL << GPIO_AFRH_AFSEL9_Pos);
    GPIOB->AFR[1]  |=  (0x4UL << GPIO_AFRH_AFSEL9_Pos);

    // Configure PB6 and PB9 as alternative function I/Os
    GPIOB->MODER   &= ~(0x3UL << GPIO_MODER_MODE6_Pos) | \
                      ~(0x3UL << GPIO_MODER_MODE9_Pos);

    GPIOB->MODER   |=  (0x2UL << GPIO_MODER_MODE6_Pos) | \
                       (0x2UL << GPIO_MODER_MODE9_Pos);

    // Configure PB6 and PB9 as very high speed I/Os
    GPIOB->MODER   &= ~(0x3UL << GPIO_MODER_MODE6_Pos) | \
                      ~(0x3UL << GPIO_MODER_MODE9_Pos);

    GPIOB->OSPEEDR |=  (0x3UL << GPIO_OSPEEDR_OSPEED6_Pos) | \
                       (0x3UL << GPIO_OSPEEDR_OSPEED9_Pos);

    // Configure PB6 and PB9 as open drain
    GPIOB->OTYPER  |=  (0x1UL << GPIO_OTYPER_OT6_Pos) | \
                       (0x1UL << GPIO_OTYPER_OT9_Pos);

    // Configure PB6 and PB9 as no pull
    GPIOB->PUPDR   &= ~(0x3UL << GPIO_PUPDR_PUPD6_Pos) | \
                      ~(0x3UL << GPIO_PUPDR_PUPD9_Pos);
}

// ****************************************************************************
// * Module APIs
// ****************************************************************************

void i2c1_init(void)
{
    // Enable I2C1 peripheral
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Reset I2C1 peripheral
    RCC->APB1RSTR |=  RCC_APB1RSTR_I2C1RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

    // Initialize associated GPIO
    i2c1_gpio_init();

    // Disable acknowledge and I2C1 peripheral
    I2C_INSTANCE->CR1 &= ~(I2C_CR1_ACK | I2C_CR1_PE);

    // APB1 is set at 10MHz
    I2C_INSTANCE->CR2 |= (0x0AUL << I2C_CR2_FREQ_Pos);

    // Set CCR value to 0x32 (50d).
    // For a 10MHz APB1 clock our period is 100ns,
    // therefore CCR = 5,000ns / 100ns = 50d in order to achieve 100KHz.
    // 5,000ns = Ti2c / 2 which in our case Ti2c = 10,000ns for 1/100KHz.
    I2C_INSTANCE->CCR |= 0x00000032;

    // Set contents of the TRISE register
    // TRISE = 1000ns/TPCLK + 1, in our case TPCLK = 100ns,
    // so TRISE = 1000ns/100ns + 1 = 11
    I2C_INSTANCE->TRISE |= 0x0000000B;

    // Enable I2C1 peripheral
    I2C_INSTANCE->CR1 |= I2C_CR1_PE;
}

void i2c1_transmit(uint8_t address, uint8_t data)
{
    i2c1_transmit_buffer(address, &data, 1);
}

void i2c1_transmit_buffer(uint8_t address, uint8_t * buffer, uint32_t buffer_size)
{
    // Generate a start condition
    I2C_INSTANCE->CR1 |= I2C_CR1_START;

    // Wait until start bit is set
    while ((I2C_INSTANCE->SR1 & I2C_SR1_SB) == 0);

    // Read SR1 and send device address - this clears SB
    uint32_t register_data __attribute__((unused)) = I2C_INSTANCE->SR1;
    I2C_INSTANCE->DR = address;

    // Wait until address flag is set
    while ((I2C_INSTANCE->SR1 & I2C_SR1_ADDR) == 0);

    // Clear address flag
    register_data = I2C_INSTANCE->SR1;
    register_data = I2C_INSTANCE->SR2;

    for (uint32_t i = 0; i < buffer_size; i++)
    {
        // Ensure TX buffer is empty and write data
        while ((I2C_INSTANCE->SR1 & I2C_SR1_TXE) == 0);
        I2C_INSTANCE->DR = (uint32_t) *buffer++;
    }

    // Request for a stop condition to be sent
    // after transmission of data - i.e TX when buffer is empty.
    while ((I2C_INSTANCE->SR1 & I2C_SR1_TXE) == 0);
    I2C_INSTANCE->CR1 |= I2C_CR1_STOP;
}

uint8_t i2c1_receive(uint8_t address)
{
    uint8_t data;
    i2c1_receive_buffer(address, &data, 1);
    return data;
}

void i2c1_receive_buffer(uint8_t address, uint8_t * buffer, \
                         uint32_t buffer_size)
{
    // Enable acknowlegements
    I2C_INSTANCE->CR1 |= I2C_CR1_ACK;
    // Clear POS bit
    I2C_INSTANCE->CR1 &= ~I2C_CR1_POS;
    
    // Generate a start condition
    I2C_INSTANCE->CR1 |= I2C_CR1_START;

    // Wait until start bit is set
    while ((I2C_INSTANCE->SR1 & I2C_SR1_SB) == 0);

    // Read SR1 and send device address - this clears SB
    uint32_t register_data __attribute__((unused)) = I2C_INSTANCE->SR1;
    I2C_INSTANCE->DR = address | 0x01;

    // Wait until address flag is set
    while ((I2C_INSTANCE->SR1 & I2C_SR1_ADDR) == 0);

    if (buffer_size == 1)
    {
        // Clear ACK bit
        I2C_INSTANCE->CR1 &= ~I2C_CR1_ACK; 
        // Clear address flag
        register_data = I2C_INSTANCE->SR1;
        register_data = I2C_INSTANCE->SR2;

        // Generate I2C stop condition
        I2C_INSTANCE->CR1 |= I2C_CR1_STOP;

        // Wait until data is received
        while ((I2C_INSTANCE->SR1 & I2C_SR1_RXNE) == 0);
        // Read received data
        *buffer = (uint8_t) I2C_INSTANCE->DR;
    }
    else if (buffer_size == 2)
    {
        // Clear ACK bit
        I2C_INSTANCE->CR1 &= ~I2C_CR1_ACK;
        // Set POS bit
        I2C_INSTANCE->CR1 |= I2C_CR1_POS;

        // Clear address flag
        register_data = I2C_INSTANCE->SR1;
        register_data = I2C_INSTANCE->SR2;

        // Wait until BTF is set and set stop bit
        while ((I2C_INSTANCE->SR1 & I2C_SR1_BTF) == 0);
        I2C_INSTANCE->CR1 |= I2C_CR1_STOP;
        // Read data
        *buffer++ = (uint8_t) I2C_INSTANCE->DR;
        *buffer   = (uint8_t) I2C_INSTANCE->DR;
    }
    else if (buffer_size > 2)
    {
        // Clear address flag
        register_data = I2C_INSTANCE->SR1;
        register_data = I2C_INSTANCE->SR2;

        for (uint32_t i = buffer_size; i > 0; i--)
        {
            if (i > 3)
            {
                // Wait until data is received
                while ((I2C_INSTANCE->SR1 & I2C_SR1_RXNE) == 0);
                // Read received data
                *buffer++ = (uint8_t) I2C_INSTANCE->DR;
            }
            else if (i == 3)
            {
                // Wait until BTF is set and clear ACK bit
                while ((I2C_INSTANCE->SR1 & I2C_SR1_BTF) == 0);
                I2C_INSTANCE->CR1 &= ~I2C_CR1_ACK;
                // Read data N-2
                *buffer++ = (uint8_t) I2C_INSTANCE->DR;

                // Wait until BTF is set and set stop bit
                while ((I2C_INSTANCE->SR1 & I2C_SR1_BTF) == 0);
                I2C_INSTANCE->CR1 |= I2C_CR1_STOP;

                //Read data N-1 and N
                *buffer++ = (uint8_t) I2C_INSTANCE->DR;
                *buffer   = (uint8_t) I2C_INSTANCE->DR;

                // Exit loop
                break;
            }
        }
    }
}
