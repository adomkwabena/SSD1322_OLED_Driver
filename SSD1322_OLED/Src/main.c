/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#include <stdlib.h>
#include <stddef.h>
#include "main.h"
#include "rcc.h"
#include "spi.h"
#include "gpio.h"
#include "delay.h"
#include "ssd1322.h"
#include "bitmaps.h"
#include "PTM55FT.h"
#include "Lato_Regular.h"
#include "IBMPlexMono_Regular.h"


int main(void)
{

    // Reset of all peripherals, Initializes the Flash interface and the Systick.
    HAL_Init();
    // Configure the system clock.
    SystemClock_Config();
    // Initialize all configured peripherals.
    MX_GPIO_Init();
    MX_SPI1_Init();

    // Allocate memory for a frame buffer
    uint8_t *frame_buffer = (uint8_t *) calloc(8192, sizeof(uint8_t));

    delay_initialize();
    ssd1322_initialize();
    // Clear SSD1322 RAM
    ssd1322_fill_ram(0x00);

    if (frame_buffer != NULL)
    {
        ssd1322_fill_fb(frame_buffer, 0xFF);
        ssd1322_display_fb(frame_buffer);
        delay_ms(40000);
        ssd1322_fill_fb(frame_buffer, 0x00);
        ssd1322_display_fb(frame_buffer);

        ssd1322_put_rectangle_fb(frame_buffer, 0, 0, 127, 63);
        ssd1322_put_rectangle_fb(frame_buffer, 10, 10, 117, 53);
        ssd1322_display_fb(frame_buffer);
        delay_ms(20000);

        // Display images on screen
        ssd1322_fill_fb(frame_buffer, 0x00);
        ssd1322_put_bitmap_fb(frame_buffer, 0,  0, (const bitmap_t *) &einstein_0);
        ssd1322_put_bitmap_fb(frame_buffer, 33, 0, (const bitmap_t *) &einstein_1);
        ssd1322_put_bitmap_fb(frame_buffer, 60, 0, (const bitmap_t *) &einstein_2);
        ssd1322_put_bitmap_fb(frame_buffer, 91, 0, (const bitmap_t *) &einstein_3);
        ssd1322_display_fb(frame_buffer);
        delay_ms(20000);

        // Display text on screen
        ssd1322_set_font((const font_t *) &PTM55FT);
        ssd1322_fill_fb(frame_buffer, 0x00);
        ssd1322_put_string_fb(frame_buffer, 0, 0,  "SSD1322 OLED Fun:");
        ssd1322_put_string_fb(frame_buffer, 0, 32, "Part Two");
        ssd1322_display_fb(frame_buffer);
        delay_ms(20000);

        // Display final image on screen
        ssd1322_fill_fb(frame_buffer, 0x00);
        ssd1322_put_bitmap_fb(frame_buffer, 34, 0, (const bitmap_t *) &CN);
        ssd1322_display_fb(frame_buffer);

    }

    while (1)
    {

    }
}

void Error_Handler(void)
{
    while (1);
}
