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
#include "spi1.h"
#include "spi1_test.h"
#include "usart2.h"
#include "i2c1.h"
#include "i2c1_test.h"
#include "delay.h"
#include "ssd1322.h"
#include "itoa.h"
#include "ftoa.h"
#include "hdc1000.h"
#include "UbuntuMono_Regular_15.h"
#include "UbuntuMono_Regular_20.h"
#include "UbuntuMono_Regular_30.h"
#include "UbuntuMono_Regular_40.h"
#include "UbuntuMono_Regular_50.h"
#include "UbuntuMono_Regular_60.h"
#include "rcc.h"

// ****************************************************************************
// * Definitions
// ****************************************************************************

#define SYSTICK_CLKSOURCE_HCLK_DIV8    0x00000000U
#define SYSTICK_CLKSOURCE_HCLK         0x00000004U

// ****************************************************************************
// * Global Variables.
// ****************************************************************************

uint8_t frame_buffer[8192] __attribute__((section(".ccm"))) = {};
volatile uint32_t frames = 0;
volatile uint32_t fps = 0;

// ****************************************************************************
// * Application Implementation.
// ****************************************************************************

int main(void)
{
    gpio_init();
    spi1_test_init();
    i2c1_test_init();
    usart2_init();
    delay_init();
    // Set acquisition mode to enable either a temperature or humidity conversion.
    // This is a precondition for calling hdc1000_get_temperature() and
    // hdc1000_get_humidity() functions.
    hdc1000_init(MODE_TEMP_OR_HUMID | TRES_14 | HRES_14);

    // Initialize systick to trigger every 500ms
    SysTick_Config(10000000);
    // Set systick to use (HCLK (160MHz) / 8) = 20MHz
    SysTick->CTRL &= ~SYSTICK_CLKSOURCE_HCLK;

    // TODO: Put ADC code into a driver
    // Initialize ADC
    // Enable ADC1
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // Convert only 1 ADC channel
    ADC1->SQR1   &= ~ADC_SQR1_L;
    // Connect ADC1_IN16 to SQ1
    ADC1->SQR3   |= (16U << ADC_SQR3_SQ1_Pos);
    // Set sampling time for IN16 & IN17
    ADC1->SMPR1  |= (8U << ADC_SMPR1_SMP16_Pos);
    ADC1->SMPR1  |= (8U << ADC_SMPR1_SMP17_Pos);
    
    // Set ADC prescaler
    ADC123_COMMON->CCR |= (ADC_CCR_ADCPRE_Pos);
    // Set TSVREFE bit
    ADC123_COMMON->CCR |= ADC_CCR_TSVREFE;
    // Turn ADC1 on
    ADC1->CR2    |= ADC_CR2_ADON;


    while (1)
    {
        // Turn all LEDs on
        RED_LED_ON();
        BLUE_LED_ON();
        GREEN_LED_ON();
        ORANGE_LED_ON();

        // Wait for 200ms
        delay_ms(200);

        // Run demos of the communication protocols.
        // All - waveforms can be viewed using a logic analyzer.

        // Run the spi1 demo
        // Pinout information is in the "spi1_gpio_init" function.
        spi1_demo();
        // Run the i2c1 demo - ensure the right peripheral is connected,
        // otherwise the application will hang.
        // Pinout information is in the "i2c1_gpio_init" function.
        i2c1_demo();
        // Run usart2 demo
        // Pinout information is in the "usart2_gpio_init" function.
        // USART1 is not used here because of a GPIO conflict with i2c1
        usart2_put_string("It is that time! ");

        // Turn all LEDs off
        RED_LED_OFF();
        BLUE_LED_OFF();
        GREEN_LED_OFF();
        ORANGE_LED_OFF();

        // Wait for 200ms
        delay_ms(200);

        ssd1322_initialize();
        // Clear ssd1322 RAM
        ssd1322_fill_ram(0x00);

        ssd1322_fill_fb(frame_buffer, 0xFF);
        ssd1322_display_fb(frame_buffer);
        delay_ms(500);
        ssd1322_fill_fb(frame_buffer, 0x00);
        ssd1322_display_fb(frame_buffer);

        ssd1322_put_rectangle_fb(frame_buffer, 0, 0, 127, 63);
        ssd1322_put_rectangle_fb(frame_buffer, 10, 10, 117, 53);
        ssd1322_display_fb(frame_buffer);
        delay_ms(500);

        // Display text on screen
        ssd1322_set_font((const font_t *)&UbuntuMono_Regular_30);
        ssd1322_fill_fb(frame_buffer, 0x00);
        ssd1322_put_string_fb(frame_buffer, 1, 0, "Welcome to");
        ssd1322_put_string_fb(frame_buffer, 0, 32, "Everyone!");
        ssd1322_display_fb(frame_buffer);
        delay_ms(1000);

        // Display text on screen
        ssd1322_set_font((const font_t *)&UbuntuMono_Regular_15);
        ssd1322_fill_fb(frame_buffer, 0x00);
        ssd1322_put_string_fb(frame_buffer, 1, 0, "Hello!");
        ssd1322_display_fb(frame_buffer);
        delay_ms(1000);

        // Display text on screen
        ssd1322_set_font((const font_t *)&UbuntuMono_Regular_20);
        ssd1322_fill_fb(frame_buffer, 0x00);
        ssd1322_put_string_fb(frame_buffer, 1, 0, "Hello!");
        ssd1322_display_fb(frame_buffer);
        delay_ms(1000);

        // Display text on screen
        ssd1322_set_font((const font_t *)&UbuntuMono_Regular_30);
        ssd1322_fill_fb(frame_buffer, 0x00);
        ssd1322_put_string_fb(frame_buffer, 1, 0, "Hello!");
        ssd1322_display_fb(frame_buffer);
        delay_ms(1000);

        // Display text on screen
        ssd1322_set_font((const font_t *)&UbuntuMono_Regular_40);
        ssd1322_fill_fb(frame_buffer, 0x00);
        ssd1322_put_string_fb(frame_buffer, 1, 0, "Hello!");
        ssd1322_display_fb(frame_buffer);
        delay_ms(1000);

        // Display text on screen
        ssd1322_set_font((const font_t *)&UbuntuMono_Regular_50);
        ssd1322_fill_fb(frame_buffer, 0x00);
        ssd1322_put_string_fb(frame_buffer, 1, 0, "Hello!");
        ssd1322_display_fb(frame_buffer);
        delay_ms(1000);

        // Display text on screen
        ssd1322_set_font((const font_t *)&UbuntuMono_Regular_60);
        ssd1322_fill_fb(frame_buffer, 0x00);
        ssd1322_put_string_fb(frame_buffer, 1, 0, "Hello!");
        ssd1322_display_fb(frame_buffer);
        delay_ms(1000);

        int32_t counter = 0;

        char string_1[32] = {};
        char string_2[32] = {};
        char string_3[32] = {};
        char string_4[32] = {};
        char string_5[32] = {};
        char string_6[32] = {};

        uint16_t temperature_raw = 0;
        uint16_t humidity_raw = 0;
        float temperature;
        float humidity;

        uint8_t x_coord = 0;

        uint16_t adc_value_raw = 0;
        float temp = 0;
        float v_sense = 0;
        float v_refint = 0;

        while (1)
        {
            // Connect ADC1_IN16 to SQ1
            ADC1->SQR3 &= ~(0x1F << ADC_SQR3_SQ1_Pos);
            ADC1->SQR3 |= (16U << ADC_SQR3_SQ1_Pos);
            // Start ADC conversion
            ADC1->CR2 |= ADC_CR2_SWSTART;
            // Wait for conversion to complete
            while ((ADC1->SR & ADC_SR_EOC) == 0);
            // Get raw ADC value
            adc_value_raw = ADC1->DR;

            // Convert internal temperature into degrees Celsius
            v_sense = (adc_value_raw / 4095.0) * 3.0;
            temp =  ((v_sense - 0.76) / 2.5) + 25.0;
            ftoa(temp, string_5);

            // Connect ADC1_IN17 (VREFINT) to SQ1
            ADC1->SQR3 &= ~(0x1F << ADC_SQR3_SQ1_Pos);
            ADC1->SQR3 |= (17U << ADC_SQR3_SQ1_Pos);
            // Start ADC conversion
            ADC1->CR2 |= ADC_CR2_SWSTART;
            // Wait for conversion to complete
            while ((ADC1->SR & ADC_SR_EOC) == 0);
            // Get raw ADC value
            adc_value_raw = ADC1->DR;

            // Convert VREFINT into a voltage representation
            v_refint = (adc_value_raw / 4095.0) * 3.0;
            ftoa(v_refint, string_6);

            // Get raw temperature and humidity data
            temperature_raw = hdc1000_get_temperature();
            humidity_raw = hdc1000_get_humidity();

            // Convert temperature into degrees Celsius
            temperature = ((temperature_raw / 65536.0) * 165.0) - 40.0;
            // Convert relative humidity into a percentage
            humidity = (humidity_raw / 65536.0) * 100.0;

            ftoa(humidity, string_1);
            ftoa(temperature, string_2);
            itoa(counter, string_3);
            ssd1322_fill_fb(frame_buffer, 0x00);
            ssd1322_set_font((const font_t *)&UbuntuMono_Regular_30);
            x_coord = ssd1322_put_string_fb(frame_buffer, 0, 0, "H:");
            x_coord = ssd1322_put_string_fb(frame_buffer, x_coord, 0, string_1);
            ssd1322_put_char_fb(frame_buffer, x_coord, 0, '%');

            x_coord = ssd1322_put_string_fb(frame_buffer, 0, 32, "T:");
            x_coord = ssd1322_put_string_fb(frame_buffer, x_coord, 32, string_2);
            x_coord += ssd1322_put_char_fb(frame_buffer, x_coord, 32, (const char) 127);
            ssd1322_put_char_fb(frame_buffer, x_coord, 32, 'C');

            ssd1322_set_font((const font_t *)&UbuntuMono_Regular_15);
            x_coord = ssd1322_put_string_fb(frame_buffer, 149, 0, "Frames:");
            ssd1322_put_string_fb(frame_buffer, x_coord, 0, string_3);
            // We are counting the frames every 1/2 a second so fps is
            // multiplied by 2 to get the actual fps.
            itoa((fps * 2), string_4);
            x_coord = ssd1322_put_string_fb(frame_buffer, 149, 16, "FPS:");
            ssd1322_put_string_fb(frame_buffer, x_coord, 16, string_4);

            // Display internal chip temperature
            x_coord = ssd1322_put_string_fb(frame_buffer, 149, 32, "Temp:");
            x_coord = ssd1322_put_string_fb(frame_buffer, x_coord, 32, string_5);
            x_coord += ssd1322_put_char_fb(frame_buffer, x_coord, 32, (const char) 127);
            ssd1322_put_char_fb(frame_buffer, x_coord, 32, 'C');
            
            // Display internal chip temperature
            x_coord = ssd1322_put_string_fb(frame_buffer, 149, 48, "Vref:");
            x_coord = ssd1322_put_string_fb(frame_buffer, x_coord, 48, string_6);
            ssd1322_put_char_fb(frame_buffer, x_coord, 48, 'V');

            // Toggle bit 14 to indicate fps, where fps = freq at which 
            // orange LED toggles.
            ORANGE_LED_ON();
            ssd1322_display_fb(frame_buffer);
            ORANGE_LED_OFF();
            counter++;
            frames++;
        }
    }
}

// ****************************************************************************
// * Interrupt Handlers
// ****************************************************************************

void SysTick_Handler(void)
{
    // Toggle LED On GPIOD pin 15
    GPIOD->ODR ^= GPIO_ODR_OD15;
    // Get the number of frames displayed so far
    fps = frames;
    frames = 0;
}