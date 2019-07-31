/**
 * @Author
 *   Adom Kwabena
 *
 * @File Name
 *   ssd1322.h
 *
 * @Summary
 *   This module provides APIs for displaying data on ssd1322 OLED displays.
 */

#include <stddef.h>
#include "spi.h"
#include "ssd1322.h"

/**
 * Section: Module Global Variables
 */

const font_t * g_active_font = NULL;

/**
 * Section: Private Function Prototypes
 */

/**
 * @Summary
 *   This function is used to select the SSD1322 GDDRAM address
 *   increment mode and remap of OLED display segments to memory
 *   locations in GDDRAM.
 *
 * @Param
 *   format - the type of remap format required - refer to SSD1322 datasheet
 *   for various remap formats available.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_remap_format(uint8_t format);

/**
 * @Summary
 *   This function specifies a vertical offset by mapping the display
 *   start line to one of the COM0-127 rows.
 *
 * @Param
 *   offset - the vertical offset required.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_display_offset(uint8_t offset);

/**
 * @Summary
 *   This function provides the means to select one of four display
 *   configurations which are:
 *   1. Normal display [0xA4]
 *   2. Set Entire Display ON [0xA5]
 *   3. Set Entire Display OFF [0xA6]
 *   4. Inverse Display [0xA7]- The gray level of display data are swapped
 *                              i.e. GS0->GS15, GS1->GS14, ...
 *
 * @Param
 *   display_mode - this selects one of the four configurations above.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_display_mode(uint8_t display_mode);

/**
 * @Summary
 *   This function displays an area defined by the parameters supplied.
 *
 * @Param
 *   partial_mode - enable or exit partial mode.
 *
 * @Param
 *   row_address_start - initial row address to display.
 *
 * @Param
 *   row_address_end - final row address to display.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_partial_display(uint8_t partial_mode,
                                               uint8_t row_address_start,
                                               uint8_t row_address_end);

/**
 * @Summary
 *   This function is used to enable or disable the VDD regulator.
 *
 * @Param
 *   function - enable or disable the internal VDD regulator.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_function_selection(uint8_t function);

/**
 * @Summary
 *   This function sets the length of phases 1 and 2 of segement waveforms
 *   of the driver.
 *
 * @Param
 *   phase_length - The length of Phase 1 is defined by bits [3:0] from 5 to 31
 *                  in units of 2 display clocks (DCLKs).
 *                  The length of Phase 2 is defined by bits [7:4] from 3 to 15
 *                  in the unit of display clocks (DCLKs).
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_phase_length(uint8_t phase_length);

/**
 * @Summary
 *   This function is used to select the frequency of the SSD1322 and
 *   together with other parameters the frame rate of the display.
 *
 * @Param
 *   display_clock - bits [3:0] define the clock divide ratio by a factor
 *                   from 1 to 16.
 *                   bits [7:4] define the oscillator frequency. There are
 *                   16 different frequency settings with higher values
 *                   providing higher clock frequencies.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_display_clock(uint8_t display_clock);

/**
 * @Summary
 *   This function is used to enhance the display performance.
 *
 * @Param
 *   vsl - this is used to enable or disable external VSL.
 *
 * @Param
 *   gray_scale_quality - this is used to improve the low gray scale quality.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_display_enhancement_a(uint8_t vsl,
        uint8_t gray_scale_quality);

/**
 * @Summary
 *   This function is used to set the states of the GPIO0 and GPIO1 pins.
 *
 * @Param
 *   gpio_mode - selects the states of GPIO0 and GPIO1 pins.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_gpio(uint8_t gpio_mode);

/**
 * @Summary
 *   This function is used to set the phase 3 second pre-charge period.
 *
 * @Param
 *   precharge_period - sets the phase 3 second pre-charge period from 0 to 15
 *                      display clocks (DCLKs).
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_precharge_period(uint8_t precharge_period);

/**
 * @Summary
 *   This function is used to set the first pre-charge (phase 2) level of
 *   segment pins.
 *
 * @Param
 *   precharge_voltage - selects the precharge voltage with refernce to VCC.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_precharge_voltage(uint8_t precharge_voltage);

/**
 * @Summary
 *   This function sets the de-select level of common pins.
 *
 * @Param
 *   vcomh_value - sets the de-select level of common pins with reference to VCC.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_vcomh(uint8_t vcomh_value);

/**
 * @Summary
 *   This function is used to set the brightness of the display by
 *   setting the segment output current.
 *
 * @Param
 *   contrast_current - selects contrast current / display brightness
 *                      from 0 to 255 in linear steps.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_contrast_current(uint8_t contrast_current);

/**
 * @Summary
 *   This function controls the brightness of the display with 16 master
 *   control steps.
 *
 * @Param
 *   master_current - sets the brightness of the display by setting the
 *                    master current. Higher values result in a higher
 *                    current and thus brighter display.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_master_current(uint8_t master_current);

/**
 * @Summary
 *   This function selects the number of common pins (rows) used by the display.
 *
 * @Param
 *   multiplex_ratio - the number of rows of the display.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_multiplex_ratio(uint8_t multiplex_ratio);

/**
 * @Summary
 *   This function is used to enhance display performance.
 *
 * @Param
 *   display_enhancement_b - selects the display enhancement.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_display_enhancement_b(uint8_t display_enhancement_b);

/**
 * @Summary
 *   This function is used to lock the SSD1322 driver chip from accepting
 *   any command apart from the "command lock" command.
 *
 * @Param
 *   command_lock - sets the SSD1322 to command lock or unlock.
 *
 * @Returns
 *   None
 */
static inline void ssd1322_set_command_lock(uint8_t command_lock);

/**
 * Section: Module APIs
 */

void ssd1322_write_data(uint8_t data)
{
    DATA_COMMAND_HIGH();
    CHIP_SELECT_LOW();
    // Write data
    HAL_SPI_Transmit(&hspi1, &data, 1, 1);
    CHIP_SELECT_HIGH();
    DATA_COMMAND_LOW();
}

void ssd1322_write_command(uint8_t command)
{
    DATA_COMMAND_LOW();
    CHIP_SELECT_LOW();
    // Write command
    HAL_SPI_Transmit(&hspi1, &command, 1, 1);
    CHIP_SELECT_HIGH();
    DATA_COMMAND_HIGH();
}

void ssd1322_set_column_address(uint8_t column_start, uint8_t column_end)
{
    ssd1322_write_command(SET_COLUMN_ADDRESS);
    ssd1322_write_data(column_start);
    ssd1322_write_data(column_end);
}

void ssd1322_set_row_address(uint8_t row_start, uint8_t row_end)
{
    ssd1322_write_command(SET_ROW_ADDRESS);
    ssd1322_write_data(row_start);
    ssd1322_write_data(row_end);
}

void ssd1322_write_ram_enable(void)
{
    // Enable MCU to write into RAM
    ssd1322_write_command(WRITE_RAM);
}

static inline void ssd1322_set_remap_format(uint8_t format)
{
    // Set Re-Map/Dual COM line mode
    ssd1322_write_command(SET_REMAP_DUAL_COM_LINE_MODE);
    // Default => 0x40
    ssd1322_write_data(format);
    // Default => 0x01 (Disable dual COM mode)
    ssd1322_write_data(0x11);
}

void ssd1322_set_start_line(uint8_t start_line)
{
    // Set vertical scroll by RAM
    ssd1322_write_command(SET_DISPLAY_START_LINE);
    //Default => 0x00
    ssd1322_write_data(start_line);
}

static inline void ssd1322_set_display_offset(uint8_t offset)
{
    // Set vertical scroll by ROW
    ssd1322_write_command(SET_DISPLAY_OFFSET);
    // Default => 0x00
    ssd1322_write_data(offset);
}

static inline void ssd1322_set_display_mode(uint8_t display_mode)
{
    ssd1322_write_command(SET_DISPLAY_MODE_MASK | display_mode);
}

static inline void ssd1322_set_partial_display(uint8_t partial_mode,
                                               uint8_t row_address_start,
                                               uint8_t row_address_end)
{
    ssd1322_write_command(PARTIAL_DISPLAY_MASK | partial_mode);

    if (partial_mode == ENABLE_PARTIAL_DISPLAY)
    {
        ssd1322_write_data(row_address_start);
        ssd1322_write_data(row_address_end);
    }
}

static inline void ssd1322_set_function_selection(uint8_t function)
{
    ssd1322_write_command(SET_FUNCTION_SELECTION);
    ssd1322_write_data(function);
}

void ssd1322_set_display_on_off(uint8_t display_on_off)
{
    ssd1322_write_command(DISPLAY_ON_OFF_MASK | display_on_off);
}

static inline void ssd1322_set_phase_length(uint8_t phase_length)
{
    ssd1322_write_command(SET_PHASE_LENGTH);
    ssd1322_write_data(phase_length);
}

static inline void ssd1322_set_display_clock(uint8_t display_clock)
{
    ssd1322_write_command(SET_FRONT_CLOCK_DIVIDER);
    ssd1322_write_data(display_clock);
}

static inline void ssd1322_set_display_enhancement_a(uint8_t vsl,
        uint8_t gray_scale_quality)
{
    ssd1322_write_command(DISPLAY_ENHANCEMENT_A);
    ssd1322_write_data(0xA0 | vsl);
    ssd1322_write_data(0x05 | gray_scale_quality);
}

static inline void ssd1322_set_gpio(uint8_t gpio_mode)
{
    ssd1322_write_command(SET_GPIO);
    ssd1322_write_data(gpio_mode);
}
static inline void ssd1322_set_precharge_period(uint8_t precharge_period)
{
    ssd1322_write_command(SET_SECOND_PRECHARGE_PERIOD);
    ssd1322_write_data(precharge_period);
}

static inline void ssd1322_set_precharge_voltage(uint8_t precharge_voltage)
{
    ssd1322_write_command(SET_PRECHARGE_VOLTAGE);
    ssd1322_write_data(precharge_voltage);
}

static inline void ssd1322_set_vcomh(uint8_t vcomh_value)
{
    ssd1322_write_command(SET_VCOMH_VOLTAGE);
    ssd1322_write_data(vcomh_value);
}

static inline void ssd1322_set_contrast_current(uint8_t contrast_current)
{
    ssd1322_write_command(SET_CONTRAST_CURRENT);
    ssd1322_write_data(contrast_current);
}

static inline void ssd1322_set_master_current(uint8_t master_current)
{
    ssd1322_write_command(MASTER_CURRENT_CONTROL);
    ssd1322_write_data(master_current);
}

static inline void ssd1322_set_multiplex_ratio(uint8_t multiplex_ratio)
{
    ssd1322_write_command(SET_MULTIPLEX_RATIO);
    ssd1322_write_data(multiplex_ratio);
}

static inline void ssd1322_set_display_enhancement_b(uint8_t display_enhancement_b)
{
    ssd1322_write_command(DISPLAY_ENHANCEMENT_B);
    ssd1322_write_data(0x82 | display_enhancement_b);
    ssd1322_write_data(0x20);
}

static inline void ssd1322_set_linear_gray_scale_table(void)
{
    ssd1322_write_command(SELECT_DEFAULT_LINEAR_GRAY_SCALE_TABLE);
}

static inline void ssd1322_set_command_lock(uint8_t command_lock)
{
    ssd1322_write_command(SET_COMMAND_LOCK);
    ssd1322_write_data(0x12 | command_lock);
}

void ssd1322_fill_ram(uint8_t data)
{
    uint8_t i;
    uint8_t j;

    ssd1322_set_column_address(0x00, 0x77);
    ssd1322_set_row_address(0x00, 0x7F);
    ssd1322_write_ram_enable();

    for (i = 0; i < DISPLAY_HEIGHT; i++)
    {
        for (j = 0; j < DISPLAY_WIDTH; j++)
        {
            ssd1322_write_data(data);
        }
    }
}

void ssd1322_initialize(void)
{
    // Initialize DWT delay
    delay_initialize();

    // SSD1322 Power on sequence
    CHIP_RESET_LOW();
    delay_ms(500);
    CHIP_RESET_HIGH();
    delay_ms(1000);

    // Initialization sequence
    ssd1322_set_command_lock(COMMANDS_UNLOCK);
    ssd1322_set_display_on_off(DISPLAY_OFF);
    ssd1322_set_column_address(0x1C, 0x5B);
    ssd1322_set_row_address(0x00, 0x3F);
    // Set clock at 80 frames per second
    ssd1322_set_display_clock(0xF1);
    // Set multiplex ratio to 1/64
    ssd1322_set_multiplex_ratio(0x3F);
    ssd1322_set_display_offset(0x00);
    ssd1322_set_start_line(0x00);
    // Column address 0 mapped to SEG0
    // Disable nibble remap
    // Scan from COM[N-1] to C0M0
    // Disable COM split between odd and even
    // Enable dual COM line mode
    ssd1322_set_remap_format(0x14);
    // Disable GPIO pins input
    ssd1322_set_gpio(0x00);
    // Enable internal VDD regulator
    ssd1322_set_function_selection(0x01);
    // Enable external VSL
    ssd1322_set_display_enhancement_a(ENABLE_EXTERNAL_VSL,
                                      ENHANCED_LOW_GRAY_SCALE_QUALITY);
    // Set segment output current
    ssd1322_set_contrast_current(0x9F);
    // Set scale factor of segment output current control
    ssd1322_set_master_current(0x0F);
    // Set default linear gray scale table
    ssd1322_set_linear_gray_scale_table();
    // Set phase 1 as 5 clocks and phase 2 as 14 clocks
    ssd1322_set_phase_length(0xE2);
    // Enhance driving scheme capability
    ssd1322_set_display_enhancement_b(NORMAL_ENHANCEMENT);
    // Set pre-charge voltage level as 0.60 * VCC
    ssd1322_set_precharge_voltage(0x1F);
    // Set second pre-charge period as 8 clocks
    ssd1322_set_precharge_period(0x08);
    // Set common pin deselect voltage as 0.86 * VCC
    ssd1322_set_vcomh(0x07);
    // Normal display mode - 0x02
    ssd1322_set_display_mode(0x02);
    ssd1322_set_partial_display(DISABLE_PARTIAL_DISPLAY, 0x00, 0x00);
    ssd1322_set_display_on_off(DISPLAY_ON);
}

void ssd1322_set_address(uint8_t x, uint8_t y)
{
    // There is a horizontal offset of 28 (pixels start from segment 112)
    ssd1322_set_column_address((x + 0x1C), 0x5B);
    ssd1322_set_row_address(y, 0x3F);
    ssd1322_write_ram_enable();
}

void ssd1322_put_horizontal_line(uint8_t x, uint8_t y, uint8_t length)
{
    uint8_t i;

    ssd1322_set_address(x, y);
    delay_ms(1);

    for (i = 0; i < length; i++)
    {
        ssd1322_write_data(0xFF);
        ssd1322_write_data(0xFF);
    }
}

void ssd1322_put_vertical_line(uint8_t x,
        uint8_t y,
        uint8_t length,
        uint8_t align)
{
    uint8_t i;

    if (align == ALIGN_LEFT)
    {
        for (i = 0; i < length; i++)
        {
            ssd1322_set_address(x, y + i);
            delay_ms(1);
            ssd1322_write_data(0xF0);
            ssd1322_write_data(0x00);
        }
    }
    else if (align == ALIGN_RIGHT)
    {
        for (i = 0; i < length; i++)
        {
            ssd1322_set_address(x, y + i);
            delay_ms(1);
            ssd1322_write_data(0x00);
            ssd1322_write_data(0x0F);
        }
    }
}

void ssd1322_put_rectangle(uint8_t x_1, uint8_t y_1, uint8_t x_2, uint8_t y_2)
{
    ssd1322_put_vertical_line(x_1, y_1, (y_2 - y_1 + 1), ALIGN_LEFT);
    delay_ms(1);
    ssd1322_put_vertical_line(x_2, y_1, (y_2 - y_1 + 1), ALIGN_RIGHT);
    delay_ms(1);
    ssd1322_put_horizontal_line(x_1, y_1, (x_2 - x_1 + 1));
    delay_ms(1);
    ssd1322_put_horizontal_line(x_1, y_2, (x_2 - x_1 + 1));
    delay_ms(1);

}

void ssd1322_put_bitmap(uint8_t x, uint8_t y, const bitmap_t * bitmap)
{
    uint8_t rows = bitmap->height;
    uint8_t columns = bitmap->width;
    const uint8_t * bitmap_address = bitmap->address;

    uint8_t i;
    uint8_t j;

    for (i = 0; i < rows; i ++)
    {
        ssd1322_set_address(x, y + i);
        delay_ms(1);
        // Display image on screen 4 pixels at a time
        for (j = 0; j < columns; j++)
        {
            ssd1322_write_data(*bitmap_address++);
            ssd1322_write_data(*bitmap_address++);
            delay_us(100);
        }
    }
}

void ssd1322_set_font(const font_t * font)
{
    // Set active font
    g_active_font = font;
}

uint8_t ssd1322_put_char(uint8_t x, uint8_t y, const char c)
{
    // Fetch glyph metadata
    uint16_t glyph_offset  = g_active_font->font_table[c - ' '].glyph_location;
    uint8_t  columns       = g_active_font->font_table[c - ' '].glyph_width;
    uint8_t  rows          = g_active_font->font_table[c - ' '].glyph_height;
    uint8_t  baseline      = g_active_font->font_table[c - ' '].glyph_baseline;

    // Get location of glyph in font
    const uint8_t * glyph_address = g_active_font->address + glyph_offset;
    // Pack glyph into a bitmap structure
    bitmap_t glyph_bitmap = {glyph_address, columns, rows};

    // Calculate correct glyph baseline
    y += baseline;
    // Display glyph
    ssd1322_put_bitmap(x, y, (const bitmap_t *) &glyph_bitmap);

    // Return the width of the character displayed
    return columns;
}

void ssd1322_put_string(uint8_t x, uint8_t y, const char * string)
{
    while (*string)
    {
        x += ssd1322_put_char(x, y, *string++);
    }
}

void ssd1322_put_horizontal_line_fb(uint8_t * fb,
                                    uint8_t x,
                                    uint8_t y,
                                    uint8_t length)
{
    uint8_t i;

    for (i = 0; i < length; i++)
    {
        fb[(y * BUFFER_WIDTH) + x + i] = 0xFF;
    }
}

void ssd1322_put_vertical_line_fb(uint8_t * fb,
                                  uint8_t x,
                                  uint8_t y,
                                  uint8_t height,
                                  uint8_t align)
{
    uint8_t i;

    if (align == ALIGN_LEFT)
    {
        for (i = 0; i < height; i++)
        {
            fb[((y + i) * BUFFER_WIDTH) + x] = 0xF0;
        }
    }
    else if (align == ALIGN_RIGHT)
    {
        for (i = 0; i < height; i++)
        {
            fb[((y + i) * BUFFER_WIDTH + x)] = 0x0F;
        }
    }
}

void ssd1322_put_rectangle_fb(uint8_t * fb,
                              uint8_t x_1,
                              uint8_t y_1,
                              uint8_t x_2,
                              uint8_t y_2)
{
    ssd1322_put_vertical_line_fb(fb, x_1, y_1, (y_2 - y_1 + 1), ALIGN_LEFT);
    ssd1322_put_vertical_line_fb(fb, x_2, y_1, (y_2 - y_1 + 1), ALIGN_RIGHT);
    ssd1322_put_horizontal_line_fb(fb, x_1, y_1, (x_2 - x_1 + 1));
    ssd1322_put_horizontal_line_fb(fb, x_1, y_2, (x_2 - x_1 + 1));
}

uint8_t ssd1322_put_resource_fb(uint8_t * fb,
                                uint8_t x,
                                uint8_t y,
                                uint8_t rows,
                                uint8_t columns,
                                uint8_t padding,
                                const uint8_t * resource_ptr)
{
    // Variables for manipulating bytes
    uint8_t temp_byte 	        = 0x00;
    uint8_t new_pixel_byte 	= 0x00;
    uint8_t old_pixel_byte 	= 0x00;

    // Iterators
    uint8_t i;
    uint8_t j;

    // Ignore pixel padding upon first function call
    static uint8_t previous_padding = NO_PIXEL_PADDING;
    // Temporary padding - for manipulating padding info
    uint8_t temp_padding = 0;

    // Perform character spacing with "padding compensation"
    if (x == 0 || padding == NO_PIXEL_PADDING)
    {
        previous_padding = NO_PIXEL_PADDING;
    }

    if (previous_padding == THREE_PIXEL_PADDING)
    {
        // Move the x coordinate one step back and create the appearance
        // that the previous_padding was one.
        x--;
        previous_padding = ONE_PIXEL_PADDING;
    }

    if (previous_padding == ONE_PIXEL_PADDING)
    {
        // Move to the space just after the previously displayed character
        // prior to shifting incoming pixels. This creates a character
        // spacing of 2 pixels since there is a space of 1 pixel after
        // the previous character (padding of 1).
        x++;
        // Perform a global shift of incoming pixels by one pixel to the right,
        // hence adding a pixel of spacing to the previous spacing (padding of 1)
        // This automatically increases the padding of the incoming character by one.
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                new_pixel_byte = *resource_ptr++;
                temp_byte = ((new_pixel_byte >> 4) & 0x0F) | old_pixel_byte;
                fb[((y + i) * BUFFER_WIDTH) + x + j] = temp_byte;
                old_pixel_byte = (new_pixel_byte << 4) & 0xF0;
            }

            fb[((y + i) * BUFFER_WIDTH) + x + j] = old_pixel_byte;
            old_pixel_byte = 0x00;
        }

        x += columns;
        // Increase padding due to the global shift of incoming character
        // pixels to the right.
        temp_padding = padding++;
        // Padding should not be allowed to exceed 3
        if (temp_padding > 3)
        {
            // Padding immediately greater than 3 is 4
            // Move x coordinate two steps back to make it look like
            // a padding of 0 was added to current character
            x -= 2;
            temp_padding = ZERO_PIXEL_PADDING;
        }
    }

    if (previous_padding == TWO_PIXEL_PADDING)
    {
        // Display character right after the previous character since there is
        // a spacing of 2 pixels after the previous character (padding of 2)
        x++;
        previous_padding = NO_PIXEL_PADDING;
    }

    if (previous_padding == ZERO_PIXEL_PADDING)
    {
        // Leave a space of 1 byte (2 pixels) after the previously displayed
        // character since there is no space after the character (padding of 0)
        x += 2;
        previous_padding = NO_PIXEL_PADDING;
    }

    if (previous_padding == NO_PIXEL_PADDING)
    {
        // Display incoming pixels at the current x coordinate
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                fb[((y + i) * BUFFER_WIDTH) + x + j] = *resource_ptr++;
            }
        }

        x += columns - 1;
        temp_padding = padding;
    }

    // Update previous padding
    previous_padding = temp_padding;

    // Return the current x coordinate of the frame buffer
    return x;
}

uint8_t ssd1322_put_bitmap_fb(uint8_t * fb,
                              uint8_t x,
                              uint8_t y,
                              const bitmap_t * bmp)
{
    // Display bitmap
    x = ssd1322_put_resource_fb(fb, x, y, bmp->height, (bmp->width * 2), \
                                NO_PIXEL_PADDING, bmp->address);

    // Return the current x coordinate of the frame buffer
    return x;
}

uint8_t ssd1322_put_char_fb(uint8_t * fb, uint8_t x, uint8_t y, const char c)
{
    // Fetch glyph metadata
    uint16_t glyph_offset  = g_active_font->font_table[c - ' '].glyph_location;
    uint8_t  columns       = g_active_font->font_table[c - ' '].glyph_width;
    uint8_t  rows          = g_active_font->font_table[c - ' '].glyph_height;
    uint8_t  baseline      = g_active_font->font_table[c - ' '].glyph_baseline;
    uint8_t  padding       = g_active_font->font_table[c - ' '].dummy_added;

    // Get location of glyph in font
    const uint8_t * glyph_address = g_active_font->address + glyph_offset;
    // Calculate correct glyph baseline
    y += baseline;
    // Display glyph
    x = ssd1322_put_resource_fb(fb, x, y, rows, (columns * 2), \
                                padding, glyph_address);

    // Return the current x coordinate of the frame buffer
    return x;
}

uint8_t ssd1322_put_string_fb(uint8_t * fb,
                              uint8_t x,
                              uint8_t y,
                              const char * string)
{
    while (*string)
    {
        x = ssd1322_put_char_fb(fb, x, y, *string++);
    }

    // Return the current x coordinate of the frame buffer
    return x;
}

void ssd1322_fill_fb(uint8_t * fb, uint8_t data)
{
    uint8_t i;
    uint8_t j;

    for (i = 0; i < BUFFER_HEIGHT; i++)
    {
        for (j = 0; j < BUFFER_WIDTH; j++)
        {
            fb[(i * BUFFER_WIDTH) + j] = data;
        }
    }
}

void ssd1322_display_fb(uint8_t * fb)
{
    uint8_t i;
    uint8_t j;

    ssd1322_set_address(0, 0);

    for (i = 0; i < BUFFER_HEIGHT; i++)
    {
        for (j = 0; j < BUFFER_WIDTH; j++)
        {
            ssd1322_write_data(fb[(i * BUFFER_WIDTH) + j]);
        }
    }
}
