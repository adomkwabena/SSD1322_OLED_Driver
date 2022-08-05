/** 
 *  @file   delay.c
 *  @author Adom Kwabena
 *  @brief  Implements a blocking delay routine using the DWT module.
 * 
 *  References: 
 *  https://www.carminenoviello.com/2015/09/04/precisely-measure-microseconds-stm32/
 *  https://developer.arm.com/documentation/ddi0337/e/CEGHJDCF
 */

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include "delay.h"
#include "rcc.h"
#include "stm32f407xx.h"



// ****************************************************************************
// * Module APIs
// ****************************************************************************

void delay_init(void)
{
    // Enable trace and blocks - this must be enabled before enabling the DWT
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    // Enable counter
    DWT->CTRL        |= DWT_CTRL_CYCCNTENA_Msk;

}

// Store previous GCC options
#pragma GCC push_options
// Always optimize this function
#pragma GCC optimize ("03")

void __delay_us(uint32_t period)
{
    // Calculate number of cycles required
    uint32_t cycles = SYSTEM_CORE_CLOCK * period;
    // Reset counter
    DWT->CYCCNT = 0UL;

    // Wait
    while (DWT->CYCCNT < cycles);
}

// Restore previos GCC options
#pragma GCC pop_options