/**
 * @file   ssd1322.h
 * @author Adom Kwabena
 * @brief  This module provides APIs for showing data on ssd1322 OLED displays
 */

// Prevent multiple file inclusion
#ifndef __SSD1322_INC__
#define __SSD1322_INC__

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include <stdint.h>
#include "delay.h"
#include "stm32f407xx.h"

// ****************************************************************************
// * Definitions and Macros
// ****************************************************************************

// Hardware abstraction for GPIO
// Use PORTA pin 0 as chip select pin
#define CHIP_SELECT_HIGH()                      GPIOA->BSRR = GPIO_BSRR_BS_0
#define CHIP_SELECT_LOW()                       GPIOA->BSRR = GPIO_BSRR_BR_0
// Use PORTA pin 1 as data / command pin
#define DATA_COMMAND_HIGH()                     GPIOA->BSRR = GPIO_BSRR_BS_1
#define DATA_COMMAND_LOW()                      GPIOA->BSRR = GPIO_BSRR_BR_1
// Use PORTA pin 2 as chip reset pin
#define CHIP_RESET_HIGH()                       GPIOA->BSRR = GPIO_BSRR_BS_2
#define CHIP_RESET_LOW()                        GPIOA->BSRR = GPIO_BSRR_BR_2

// SSD1322 commands
#define ENABLE_GRAY_SCALE_TABLE                 0x00
#define SET_COLUMN_ADDRESS                      0x15
#define WRITE_RAM                               0x5C
#define READ_RAM                                0x5D
#define SET_ROW_ADDRESS                         0x75
#define SET_REMAP_DUAL_COM_LINE_MODE            0xA0
#define SET_DISPLAY_START_LINE                  0xA1
#define SET_DISPLAY_OFFSET                      0xA2
#define SET_DISPLAY_MODE_MASK                   0xA4
#define PARTIAL_DISPLAY_MASK                    0xA8
#define SET_FUNCTION_SELECTION                  0xAB
#define DISPLAY_ON_OFF_MASK                     0xAE
#define SET_PHASE_LENGTH                        0xB1
#define SET_FRONT_CLOCK_DIVIDER                 0xB3
#define DISPLAY_ENHANCEMENT_A                   0xB4
#define SET_GPIO                                0xB5
#define SET_SECOND_PRECHARGE_PERIOD             0xB6
#define SET_GRAY_SCALE_TABLE                    0xB8
#define SELECT_DEFAULT_LINEAR_GRAY_SCALE_TABLE  0xB9
#define SET_PRECHARGE_VOLTAGE                   0xBB
#define SET_VCOMH_VOLTAGE                       0xBE
#define SET_CONTRAST_CURRENT                    0xC1
#define MASTER_CURRENT_CONTROL                  0xC7
#define SET_MULTIPLEX_RATIO                     0xCA
#define DISPLAY_ENHANCEMENT_B                   0xD1
#define SET_COMMAND_LOCK                        0xFD

// Display dimensions
// Actual width is 480 pixels but each pixel is 4 bits wide
#define DISPLAY_WIDTH                           240U
#define DISPLAY_HEIGHT                          128U

// Options for turning display on or off
#define DISPLAY_ON                              0x01
#define DISPLAY_OFF                             0x00

// Options for controlling partial display
#define ENABLE_PARTIAL_DISPLAY                  0x00
#define DISABLE_PARTIAL_DISPLAY                 0x01

// Options for controlling VSL selection
#define ENABLE_EXTERNAL_VSL                     0x00
#define ENABLE_INTERNAL_VSL                     0x02

// Options for grayscale quality
#define NORMAL_GRAYSCALE_QUALITY                0xB0
#define ENHANCED_LOW_GRAY_SCALE_QUALITY         0XF8

// Options for display enhancement b
#define NORMAL_ENHANCEMENT                      0x20

// Options for command lock
#define COMMANDS_LOCK                           0x16
#define COMMANDS_UNLOCK                         0x12

// Frame buffer definitions
#define BUFFER_WIDTH                            128U
#define BUFFER_HEIGHT                           64U
#define BUFFER_SIZE                             8192U
#define NO_PIXEL_PADDING                        99U
#define ONE_PIXEL_PADDING                       1U
#define TWO_PIXEL_PADDING                       2U
#define THREE_PIXEL_PADDING                     3U
#define ZERO_PIXEL_PADDING                      0U

// Drawing definitions
#define ALIGN_RIGHT                             0U
#define ALIGN_LEFT                              1U
#define CHAR_SPACING                            2U


// ****************************************************************************
// * Module Data Structures
// ****************************************************************************

// Font table entry data structure
typedef struct
{
    uint16_t glyph_location;
    uint8_t  glyph_width;
    uint8_t  glyph_height;
    uint8_t  glyph_baseline;
    uint8_t  dummy_added;
} font_table_entry_t;

// Font data structure
typedef struct
{
    const uint8_t * address;
    const font_table_entry_t * font_table;
    uint8_t height;
    uint8_t max_descent;
} font_t;

// Bitmap data structure
typedef struct
{
    const uint8_t * address;
    uint8_t width;
    uint8_t height;
} bitmap_t;

// ****************************************************************************
// * Module APIs
// ****************************************************************************

/**
 * @brief   Writes a single byte of data to the SSD1322 chip.
 * @param   data: The data to be written. 
 * @returns None
 */
void ssd1322_write_data(uint8_t data);

/**
 * @brief   Writes a single byte command to the SSD1322 chip.
 * @param   command: The command to be written.
 * @returns None
 */
void ssd1322_write_command(uint8_t command);

/**
 * @brief   This function sets the column address of the SSD1322 GDDRAM.
 *          The column address is set as a range from initial address to final address.
 *
 * @param   column_start: The initial column address.
 * @param   column_end: The final column address.
 * @returns None
 */
void ssd1322_set_column_address(uint8_t column_start, uint8_t column_end);

/**
 * @brief   This function sets the row address of the SSD1322 GDDRAM.
 *          The row address is set as a range from initial address to final address.
 *
 * @param   row_start: The initial row address.
 * @param   row_end: The final row address.
 * @returns None
 */
void ssd1322_set_row_address(uint8_t row_start, uint8_t row_end);

/**
 * @brief   This function enables the GDDRAM of the SSD1322 to be accessed.
 * @param   None
 * @returns None
 */
void ssd1322_write_ram_enable(void);

/**
 * @brief   This function selects the row (line) of the GDDRAM from which
 *          the ssd1322 will begin scanning the display.
 *
 * @param   start_line: The address of the row to start scanning from.
 * @returns None
 */
void ssd1322_set_start_line(uint8_t start_line);

/**
 * @brief   This function enables the display to turned on or off.
 * @param   display_on_off: This parameter turns the display on or off.
 * @returns None
 */
void ssd1322_set_display_on_off(uint8_t display_on_off);

/**
 * @brief   This function fills the entire SSD1322 GDDRAM with user data.
 * @param   data - the data used to fill the SSD1322 GDDRAM.
 * @returns None
 */
void ssd1322_fill_ram(uint8_t data);

/**
 * @brief   This function initializes the SSD1322 chip.
 * @param   None
 * @returns None
 */
void ssd1322_initialize(void);

/**
 * @brief   This function selects the address of the SSD1322 GDDRAM.
 *          The function automatically selects final x and y coordinates
 *          to enable writing to the entire display.
 *
 * @param   x: The initial x coordinate of the GDDRAM.
 * @param   y: The initial y coordinate of the GDDRAM.
 * @returns None
 */
void ssd1322_set_address(uint8_t x, uint8_t y);

/**
 * @brief   This function allows the selection of a font to be used for all
 *          text drawing functions.
 *
 * @param   font: A pointer to the selected font.
 * @returns None
 */
void ssd1322_set_font(const font_t * font);

/**
 * @brief   This function draws a horizontal line into a frame buffer.
 *
 * @param   fb: A pointer to the frame buffer to draw the horizontal line into.
 * @param   x: The x coordinate to begin drawing the horizontal line.
 * @param   y: The y coordinate to begin drawing the horizontal line.
 * @param   length: The length of the horizontal line. 1 length = 2 pixels.
 *
 * @returns None
 */
void ssd1322_put_horizontal_line_fb(uint8_t * fb,
                                    uint8_t x,
                                    uint8_t y,
                                    uint8_t length);

/**
 * @brief   This function draws a vertical line into a frame buffer.
 * 
 * @param   fb: A pointer to the frame_buffer to draw the vertical line into.
 * @param   x: The x coordinate to begin drawing the vertical line.
 * @param   y: The y coordinate to begin drawing the vertical line.
 * @param   height: The height of the vertical line. 1 height = 1 pixel.
 * @param   align: Selects vertical line to be drawn to the left or right of the address.
 *
 * @returns None
 */
void ssd1322_put_vertical_line_fb(uint8_t * fb,
                                  uint8_t x,
                                  uint8_t y,
                                  uint8_t height,
                                  uint8_t align);

/**
 * @brief   This function draws a rectangle using user supplied coordinates.
 * 
 * @param   fb: A pointer to the frame buffer to draw the rectangle into.
 * @param   x_1: The x coordinate of the upper left corner of the rectangle.
 * @param   y_1: The y coordinate of the upper left corner of the rectangle.
 * @param   x_2: The x coordinate of the bottom right corner of the rectangle.
 * @param   y_2: The y coordinate of the bottom right corner of the rectangle.
 *
 * @returns None
 */
void ssd1322_put_rectangle_fb(uint8_t * fb,
                              uint8_t x_1,
                              uint8_t y_1,
                              uint8_t x_2,
                              uint8_t y_2);

/**
 * @brief   This function draws a resource (character or bitmap) starting from
 *          the user selected coordinates into a frame buffer.
 *
 * @param   fb: A pointer to the frame buffer to draw the resource into.
 * @param   x: The x coordinate to begin drawing the resource.
 * @param   y: The y coordinate to begin drawing the resource.
 * @param   rows: The height of the resource in pixels.
 * @param   columns: The width of the resource in pixels.
 * @param   padding: The number of dummy pixels added to the resource.
 * @param   resource_ptr: A pointer to the resource to be drawn into the frame buffer.
 *
 * @returns The current x coordinate of the frame buffer.
 */
uint8_t ssd1322_put_resource_fb(uint8_t * fb,
                                uint8_t x,
                                uint8_t y,
                                uint8_t rows,
                                uint8_t columns,
                                uint8_t padding,
                                const uint8_t * resource_ptr);

/**
 * @brief   Displays a bitmap on the screen.
 *
 * @param   fb: A pointer to the frame buffer to draw the bitmap into.
 * @param   x:  The x coordinate to begin drawing the bitmap.
 * @param   y: The y coordinate to begin drawing the bitmap.
 * @param   bmp: A pointer to the bitmap to be drawn into the frame buffer.
 *
 * @returns The current x coordinate of the frame buffer.
 */
uint8_t ssd1322_put_bitmap_fb(uint8_t * fb,
                              uint8_t x,
                              uint8_t y,
                              const bitmap_t * bmp);

/**
 * @brief   This function draws a character into a frame buffer starting from the
 *          supplied coordinates.
 *
 * @param   fb: A pointer to the frame buffer to draw the character into.
 * @param   x: The x coordinate to begin drawing the character.
 * @param   y: The y coordinate to begin drawing the character.
 * @param   c: The character to be drawn into the frame buffer.
 *
 * @returns The current x coordinate of the frame buffer.
 */
uint8_t ssd1322_put_char_fb(uint8_t * fb, uint8_t x, uint8_t y, const char c);

/**
 * @brief   This function draws a string into a frame buffer starting from
 *          the supplied coordinates.
 *
 * @param   fb: A pointer to the frame buffer to draw the string into.
 * @param   x: The x coordinate to begin drawing the string.
 * @param   y: The y coordinate to begin drawing the string.
 * @param   string: The string to be displayed.
 *
 * @returns The current x coordinate of the frame buffer.
 */
uint8_t ssd1322_put_string_fb(uint8_t * fb,
                              uint8_t x,
                              uint8_t y,
                              const char * string);

/**
 * @brief   This function fills a frame buffer with the provided data.
 *
 * @param   fb: A pointer to the frame buffer to fill.
 * @param   data: The data to fill the frame buffer with.
 * @returns None
 */
void ssd1322_fill_fb(uint8_t * fb, uint8_t data);

/**
 * @brief   This function dumps the contents of a frame buffer to a section of
 *          the ssd1322 GDDRAM for display.
 *
 * @param   fb: A pointer to the frame buffer whose contents is to be displayed.
 * @returns None
 */
void ssd1322_display_fb(uint8_t * fb);

#endif /* INC_SSD1322_H_ */
