/**
 *  @file   rcc.c
 *  @author Adom Kwabena
 *  @brief  A basic RCC driver for the stm32f407vgt6 microcontroller.
 */

#include "rcc.h"
#include "stm32f407xx.h"

// ****************************************************************************
// * Function Prototypes
// ****************************************************************************

/**
 *  @brief   Initializes the MCO1 pin as a clock output.
 *  @param   None. 
 *  @returns None.
 */
static inline void mco1_init(void);

/**
 *  @brief   Initializes the MCO2 pin as a clock output.
 *  @param   None.
 *  @returns None.
 */
static inline void mco2_init(void);

/**
 *  @brief   Initializes the MCO1 GPIO pin as an alternative function output.
 *  @param   None.
 *  @returns None.
 */
static inline void mco1_gpio_config(void);

/**
 *  @brief   Initializes the MCO2 GPIO pin as an alternative function output.
 *  @param   None.
 *  @returns None.
 */
static inline void mco2_gpio_config(void);

/**
 *  @brief   Initializes bus and peripheral clocks.
 *  @param   None.
 *  @returns None.
 */
static inline void clock_tree_init(void);

// ****************************************************************************
// * Module APIs.
// ****************************************************************************

void system_init(void)
{
    // Enable FPU by setting full access on CP10 and CP11
    // CP10 and CP11 are the FPU co-processors
    // SCB (System Control Block)
    // CPACR (CoProcessor Access Control Register)
    // set CP10 and CP11 Full Access - these are two bit fields (0b11 - full access)
    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));

    /* Reset the RCC clock configuration to the default reset state */
    /* Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001;

    /* Reset CFGR register */
    RCC->CFGR = 0x00000000;

    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t)0xFEF6FFFF;

    /* Reset PLLCFGR register */
    RCC->PLLCFGR = 0x24003010;

    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFFFBFFFF;

    /* Disable all interrupts */
    RCC->CIR = 0x00000000;

    // Initialize system clocks
    clock_tree_init();
}

static inline void mco1_gpio_config(void)
{
    // Enable GPIOA clock
    RCC->AHB1ENR   |= RCC_AHB1ENR_GPIOAEN;
    
    // Connect PA8 to AF0 (MCO1)
    GPIOA->AFR[1]  |=  (0x0UL << GPIO_AFRH_AFSEL8_Pos);
    // Configure PA8 to alternate function mode
    GPIOA->MODER   |=  (0x2UL << GPIO_MODER_MODER8_Pos);
    // Configure PA8 as a very high speed I/O
    GPIOA->OSPEEDR |=  (0x3UL << GPIO_OSPEEDR_OSPEED8_Pos);
    // Configure PA8 as push pull
    GPIOA->OTYPER  |=  (0x0UL << GPIO_OTYPER_OT8_Pos);
    // Configure PA8 as no pull
    GPIOA->PUPDR   |=  (0x0UL << GPIO_PUPDR_PUPD8_Pos);
}

static inline void mco2_gpio_config(void)
{
    // Enable GPIOC clock
    RCC->AHB1ENR   |= RCC_AHB1ENR_GPIOCEN;

    // Connect PC9 to AF0 (MCO2)
    GPIOC->AFR[1]  |=  (0x0UL << GPIO_AFRH_AFSEL9_Pos);
    // Configure PC9 to alternative function mode
    GPIOC->MODER   |=  (0x2UL << GPIO_MODER_MODER9_Pos);
    // Configure PC9 as a very high speed I/O
    GPIOC->OSPEEDR |=  (0x3UL << GPIO_OSPEEDR_OSPEED9_Pos);
    // Configure PC9 as push-pull
    GPIOC->OTYPER  |=  (0x0UL << GPIO_OTYPER_OT9_Pos);
    // Configure PC9 as no pull
    GPIOC->PUPDR   |=  (0x0UL << GPIO_PUPDR_PUPD9_Pos);
}

static inline void mco1_init(void)
{
    // Config PA8 as MCO1 output
    mco1_gpio_config();

    // Set MCO1 prescaler value
    RCC->CFGR |= (0x7UL << RCC_CFGR_MCO1PRE_Pos);

    // Set MCO1 clock source - HSE
    RCC->CFGR |= (0x02UL << RCC_CFGR_MCO1_Pos);
}

static inline void mco2_init(void)
{
    // Config PC9 as MCO2 output
    mco2_gpio_config();

    // Set MCO2 prescaler value
    RCC->CFGR |= (0x07UL << RCC_CFGR_MCO2PRE_Pos);

    // Set MCO2 clock source - SYSCLK
    RCC->CFGR |= (0x00UL << RCC_CFGR_MCO2_Pos);
}

static inline void clock_tree_init(void)
{
    // Enable MCO1 and MCO2 outputs
    mco1_init();
    mco2_init();

    // Program wait states based on CPU frequency
    FLASH->ACR |= FLASH_ACR_LATENCY_6WS;
    // Enable Data cache, instruction cache and instruction prefetch
    FLASH->ACR |= FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN;

    // Enable HSE oscillator
    RCC->CR |= RCC_CR_HSEON;

    // Wait for HSE clock to be ready
    while ((RCC->CR & RCC_CR_HSERDY_Msk) == 0);

    // Reset PLLCFGR PLLM, PLLN and PLLP values
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP);

    // Set PLL input clock source as HSE
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
    
    // Set PLLP value to /2
    RCC->PLLCFGR |= (0x00UL << RCC_PLLCFGR_PLLP_Pos);

    // Set PLLN value to /160
    RCC->PLLCFGR |= (0x00A0L << RCC_PLLCFGR_PLLN_Pos);

    // Set PLLM value to /4
    RCC->PLLCFGR |= (0x04UL << RCC_PLLCFGR_PLLM_Pos);

    // Set AHB prescaler value to /1
    // AHB clock = (HSE/PLLM) * (PLLN/PLLP) = (8/4) * (160/2) = 160MHz
    RCC->CFGR |= (0x0000UL << RCC_CFGR_HPRE_Pos);

    // Set APB1 prescaler value to /16
    // APB1 clock = (AHB clock / APB1PRE) = 160/16 = 10MHz
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV16;

    // Set APB2 prescaler value to /8
    // APB2 clock = (AHB clock / APB2PRE) = 160/8 = 20MHz
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV8;

    // Turn PLL On
    RCC->CR |= RCC_CR_PLLON;

    // Wait for the PLL to become locked
    while ((RCC->CR & RCC_CR_PLLRDY_Msk) == 0);
    
    // Select PLL as clock source
    RCC->CFGR |=  RCC_CFGR_SW_PLL;
}
