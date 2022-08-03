/**
 * @file   ssd1322.c
 * @author Adom Kwabena
 * @brief  This module provides APIs for showing data on ssd1322 OLED displays
 */

#include <stddef.h>
#include "spi1.h"
#include "ssd1322.h"

// ****************************************************************************
// * Module Global Variables
// ****************************************************************************

const font_t *g_active_font = NULL;

// ****************************************************************************
// * Private Functions
// ****************************************************************************

/**
 * @brief   This function is used to select the SSD1322 GDDRAM address
 *          increment mode and remap of OLED display segments to memory
 *          locations in GDDRAM.
 *
 * @param   format: The type of remap format required - refer to the
 *                  SSD1322 datasheet for various remap formats available.
 * @returns None
 */
static inline void ssd1322_set_remap_format(uint8_t format)
{
    // Set Re-Map/Dual COM line mode
    ssd1322_write_command(SET_REMAP_DUAL_COM_LINE_MODE);
    // Default => 0x40
    ssd1322_write_data(format);
    // Default => 0x01 (Disable dual COM mode)
    ssd1322_write_data(0x11);
}

/**
 * @brief   This function specifies a vertical offset by mapping the display
 *          start line to one of the COM0-127 rows.
 * @param   offset: The vertical offset required.
 * @returns None
 */
static inline void ssd1322_set_display_offset(uint8_t offset)
{
    // Set vertical scroll by ROW
    ssd1322_write_command(SET_DISPLAY_OFFSET);
    // Default => 0x00
    ssd1322_write_data(offset);
}

/**
 * @brief   This function provides the means to select one of four display
 *          configurations which are:
 *          1. Normal display [0xA4]
 *          2. Set Entire Display ON [0xA5]
 *          3. Set Entire Display OFF [0xA6]
 *          4. Inverse Display [0xA7]- The gray level of display data are
 *                                     swapped i.e. GS0->GS15, GS1->GS14, ...
 *
 * @param   display_mode: This selects one of the four configurations above.
 * @returns None
 */
static inline void ssd1322_set_display_mode(uint8_t display_mode)
{
    ssd1322_write_command(SET_DISPLAY_MODE_MASK | display_mode);
}

/**
 * @brief   This function displays an area defined by the parameters supplied.
 *
 * @param   partial_mode: Enable or exit partial mode.
 * @param   row_address_start: Initial row address to display.
 * @param   row_address_end: final row address to display.
 *
 * @returns None
 */
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

/**
 * @brief   This function is used to enable or disable the VDD regulator.
 *
 * @param   function: Enable or disable the internal VDD regulator.
 * @returns None
 */
static inline void ssd1322_set_function_selection(uint8_t function)
{
    ssd1322_write_command(SET_FUNCTION_SELECTION);
    ssd1322_write_data(function);
}

/**
 * @brief   This function sets the length of phases 1 and 2 of segement
 *          waveforms of the driver.
 *
 * @param   phase_length - The length of Phase 1 is defined by bits [3:0]
 *                       from 5 to 31 in units of 2 display clocks (DCLKs).
 *                       The length of Phase 2 is defined by bits [7:4]
 *                       from 3 to 15 in the unit of display clocks (DCLKs).
 * @returns None
 */
static inline void ssd1322_set_phase_length(uint8_t phase_length)
{
    ssd1322_write_command(SET_PHASE_LENGTH);
    ssd1322_write_data(phase_length);
}

/**
 * @brief   This function is used to select the frequency of the SSD1322 and
 *          together with other parameters the frame rate of the display.
 *
 * @param   display_clock: Bits [3:0] define the clock divide ratio by a factor
 *                       from 1 to 16.
 *                       bits [7:4] define the oscillator frequency. There are
 *                       16 different frequency settings with higher values
 *                       providing higher clock frequencies.
 * @returns None
 */
static inline void ssd1322_set_display_clock(uint8_t display_clock)
{
    ssd1322_write_command(SET_FRONT_CLOCK_DIVIDER);
    ssd1322_write_data(display_clock);
}

/**
 * @brief   This function is used to enhance the display performance.
 *
 * @param   vsl: This is used to enable or disable external VSL.
 * @param   gray_scale_quality: This is used to improve the low gray
 *                            scale quality.
 * @returns None
 */
static inline void ssd1322_set_display_enhancement_a(uint8_t vsl,
                                                     uint8_t gray_scale_quality)
{
    ssd1322_write_command(DISPLAY_ENHANCEMENT_A);
    ssd1322_write_data(0xA0 | vsl);
    ssd1322_write_data(0x05 | gray_scale_quality);
}

/**
 * @brief   This function is used to set the states of the GPIO0 and GPIO1 pins.
 * @param   gpio_mode: Selects the states of GPIO0 and GPIO1 pins.
 * @returns None
 */
static inline void ssd1322_set_gpio(uint8_t gpio_mode)
{
    ssd1322_write_command(SET_GPIO);
    ssd1322_write_data(gpio_mode);
}

/**
 * @brief   This function is used to set the phase 3 second pre-charge period.
 *
 * @param   precharge_period: Sets the phase 3 second pre-charge period
 *                          from 0 to 15 display clocks (DCLKs).
 * @returns None
 */
static inline void ssd1322_set_precharge_period(uint8_t precharge_period)
{
    ssd1322_write_command(SET_SECOND_PRECHARGE_PERIOD);
    ssd1322_write_data(precharge_period);
}

/**
 * @brief   This function is used to set the first pre-charge (phase 2) level
 *          of segment pins.
 * @param   precharge_voltage: Selects the precharge voltage with refernce
 *                             to VCC.
 * @returns None
 */
static inline void ssd1322_set_precharge_voltage(uint8_t precharge_voltage)
{
    ssd1322_write_command(SET_PRECHARGE_VOLTAGE);
    ssd1322_write_data(precharge_voltage);
}

/**
 * @brief   This function sets the de-select level of common pins.
 * @param   vcomh_value: Sets the de-select level of common pins with reference to VCC.
 * @returns None
 */
static inline void ssd1322_set_vcomh(uint8_t vcomh_value)
{
    ssd1322_write_command(SET_VCOMH_VOLTAGE);
    ssd1322_write_data(vcomh_value);
}

/**
 * @brief   This function is used to set the brightness of the display by
 *          setting the segment output current.
 *
 * @param   contrast_current: Selects contrast current / display brightness
 *                          from 0 to 255 in linear steps.
 * @returns None
 */
static inline void ssd1322_set_contrast_current(uint8_t contrast_current)
{
    ssd1322_write_command(SET_CONTRAST_CURRENT);
    ssd1322_write_data(contrast_current);
}

/**
 * @brief   This function controls the brightness of the display with 16 master
 *          control steps.
 *
 * @param   master_current: Sets the brightness of the display by setting the
 *                          master current. Higher values result in a higher
 *                          current and thus brighter display.
 * @returns None
 */
static inline void ssd1322_set_master_current(uint8_t master_current)
{
    ssd1322_write_command(MASTER_CURRENT_CONTROL);
    ssd1322_write_data(master_current);
}

/**
 * @brief   This function selects the number of common pins (rows) used by the display.
 * @param   multiplex_ratio: The number of rows of the display.
 * @returns None
 */
static inline void ssd1322_set_multiplex_ratio(uint8_t multiplex_ratio)
{
    ssd1322_write_command(SET_MULTIPLEX_RATIO);
    ssd1322_write_data(multiplex_ratio);
}

/**
 * @brief   This function is used to enhance display performance.
 * @param   display_enhancement_b: Selects the display enhancement.
 * @returns None
 */
static inline void ssd1322_set_display_enhancement_b(uint8_t display_enhancement_b)
{
    ssd1322_write_command(DISPLAY_ENHANCEMENT_B);
    ssd1322_write_data(0x82 | display_enhancement_b);
    ssd1322_write_data(0x20);
}

/**
 *  @brief   Selects a linear gray scale table
 *  @param   None
 *  @returns None
 */
static inline void ssd1322_set_linear_gray_scale_table(void)
{
    ssd1322_write_command(SELECT_DEFAULT_LINEAR_GRAY_SCALE_TABLE);
}

/**
 * @brief   This function is used to lock the SSD1322 driver chip from accepting
 *          any command apart from the "command lock" command.
 * @param   command_lock: sets the SSD1322 to command lock or unlock.
 * @returns None
 */
static inline void ssd1322_set_command_lock(uint8_t command_lock)
{
    ssd1322_write_command(SET_COMMAND_LOCK);
    ssd1322_write_data(0x12 | command_lock);
}

/**
 *  @brief   Initialize GPIOs connected to OLED display.
 *  @param   None.
 *  @returns None.
 */
static inline void ssd1322_gpio_init(void)
{
    // Enable GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
 
    // Configure PA0, PA1 and PA2 as output I/O.
    GPIOA->MODER &=  ~(0x3UL << GPIO_MODER_MODER0_Pos) |
                     ~(0x3UL << GPIO_MODER_MODER1_Pos) |
                     ~(0x3UL << GPIO_MODER_MODER12_Pos);

    GPIOA->MODER |=   (0x1UL << GPIO_MODER_MODER0_Pos) |
                      (0x1UL << GPIO_MODER_MODER1_Pos) |
                      (0x1UL << GPIO_MODER_MODER2_Pos);

    // Configure PD4, PD12, PD13, PD14 & PD15 as very high speed I/O.
    GPIOA->OSPEEDR |= (0x3UL << GPIO_OSPEEDR_OSPEED0_Pos) |
                      (0x3UL << GPIO_OSPEEDR_OSPEED1_Pos) |
                      (0x3UL << GPIO_OSPEEDR_OSPEED2_Pos);

    // Configure PD4, PD12, PD13, PD14 & PD15 as push pull I/O.
    GPIOA->OTYPER &= ~(0x1UL << GPIO_OTYPER_OT0_Pos) |
                     ~(0x1UL << GPIO_OTYPER_OT1_Pos) |
                     ~(0x1UL << GPIO_OTYPER_OT2_Pos);

    // Disable pull up/pull down functionality on PD4, PD12, PD13, PD14 & PD15.
    GPIOA->PUPDR &=  ~(0x3UL << GPIO_PUPDR_PUPD0_Pos) |
                     ~(0x3UL << GPIO_PUPDR_PUPD1_Pos) |
                     ~(0x3UL << GPIO_PUPDR_PUPD12_Pos);
}

// ****************************************************************************
// * Module APIs
// ****************************************************************************

void ssd1322_write_data(uint8_t data)
{
    DATA_COMMAND_HIGH();
    CHIP_SELECT_LOW();
    // Write data
    spi1_transmit(data);
    CHIP_SELECT_HIGH();
    DATA_COMMAND_LOW();
}

void ssd1322_write_command(uint8_t command)
{
    DATA_COMMAND_LOW();
    CHIP_SELECT_LOW();
    // Write command
    spi1_transmit(command);
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

void ssd1322_set_start_line(uint8_t start_line)
{
    // Set vertical scroll by RAM
    ssd1322_write_command(SET_DISPLAY_START_LINE);
    // Default => 0x00
    ssd1322_write_data(start_line);
}

void ssd1322_set_display_on_off(uint8_t display_on_off)
{
    ssd1322_write_command(DISPLAY_ON_OFF_MASK | display_on_off);
}

void ssd1322_fill_ram(uint8_t data)
{
    ssd1322_set_column_address(0x00, 0x77);
    ssd1322_set_row_address(0x00, 0x7F);
    ssd1322_write_ram_enable();

    for (uint8_t i = 0; i < GDDRAM_HEIGHT; i++)
    {
        for (uint8_t j = 0; j < GDDRAM_WIDTH; j++)
        {
            ssd1322_write_data(data);
        }
    }
}

void ssd1322_initialize(void)
{
    // Initialize GPIO & SPI
    ssd1322_gpio_init();
    spi1_init(SPI_MODE_3);

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

void ssd1322_set_font(const font_t *font)
{
    // Set active font
    g_active_font = font;
}

void ssd1322_put_pixel_fb(uint8_t *fb, uint8_t x_virtual, uint8_t y)
{
    // Convert x from a virtual address to a physical address
    // This is done by dividing by 2
    uint8_t x_physical = x_virtual >> 1;

    // Check if the virtual address is odd or even.
    // Two virtual addresses would provide the same physical address,
    // so we'll need to determine which nibble to set.
    // [0, 1] [2, 3] [4, 5]  ----> Virtual Address space
    //    |      |      |
    //    v      v      v
    //    0      1      2    ----> Physical Address space

    if (x_virtual & 0x01)
    {
        // If the virtual address is odd we want to set the right nibble
        fb[(y * BUFFER_WIDTH) + x_physical] |= 0x0F;
    }
    else
    {
        // If the virtual address is even we want to set the left nibble
        fb[(y * BUFFER_WIDTH) + x_physical] |= 0xF0;
    }
}

void ssd1322_put_horizontal_line_fb(uint8_t *fb,
                                    uint8_t x,
                                    uint8_t y,
                                    uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        fb[(y * BUFFER_WIDTH) + x + i] = 0xFF;
    }
}

void ssd1322_put_vertical_line_fb(uint8_t *fb,
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

void ssd1322_put_rectangle_fb(uint8_t *fb,
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

void ssd1322_put_resource_fb(uint8_t *fb,
                             uint8_t x_virtual,
                             uint8_t y,
                             uint8_t rows,
                             uint8_t columns,
                             const uint8_t *resource_ptr)
{
    uint8_t x_physical = x_virtual >> 1;

    // Check if there is enough space to draw the requested resource
    if (((x_physical + columns) >= BUFFER_WIDTH) || ((y + rows) >= BUFFER_HEIGHT))
    {
        // Exit if there is not enough space
        return;
    }

    // Check if input pointers are valid
    if (resource_ptr == NULL || fb == NULL)
    {
        // Exit if a pointer is invalid
        return;
    }

    // Check if the virtual address is even
    if (!(x_virtual & 0x01))
    {
        // Display incoming pixels at the current physical x coordinate
        for (uint8_t i = 0; i < rows; i++)
        {
            for (uint8_t j = 0; j < columns; j++)
            {
                fb[((y + i) * BUFFER_WIDTH) + x_physical + j] = *resource_ptr++;
            }
        }
    }
    else
    {
        // If the virtual address is odd, do a pixel by pixel draw of the data
        for (uint8_t i = 0; i < rows; i++)
        {
            // The iterator j is in bytes. We have to convert it into nibbles
            // Since 1 byte = 2 nibbles, we multiply by 2.
            for (uint8_t j = 0; j < columns; j++)
            {
                // Read data from resource
                uint8_t data = *resource_ptr++;

                // We write two pixels for one colum address because 
                // a pixel is 4 bits and a column address represents 1 byte.
                
                // If there is data in the left nibble, display it at an odd address
                if (data & 0xF0)
                {
                    // Initially the address is odd ...
                    ssd1322_put_pixel_fb(fb, x_virtual + (j * 2), y + i);
                }
                // If there is data in the right nibble, display it at an even address
                if (data & 0x0F)
                {
                    // ... then the address becomes even, and the cycle continues
                    ssd1322_put_pixel_fb(fb, x_virtual + (j * 2) + 1, y + i);
                }
            }
        }
    }
}

void ssd1322_put_bitmap_fb(uint8_t *fb,
                           uint8_t x_virtual,
                           uint8_t y,
                           const bitmap_t *bmp)
{
    // Display bitmap
    ssd1322_put_resource_fb(fb, x_virtual, y, bmp->height, bmp->width, bmp->address);
}

uint8_t ssd1322_put_char_fb(uint8_t *fb, uint8_t x_virtual, uint8_t y, const char c)
{
    // Check if character is valid or not
    // Note: Character 127 is a special character we've added
    // The actual representation for this character is 176
    if (!(c >= 32 && c <= 127))
    {
        // Exit if the character does not exist
        return 0;
    }

    // Fetch glyph metadata
    uint16_t glyph_offset = g_active_font->font_table[c - ' '].glyph_location;
    uint8_t columns       = g_active_font->font_table[c - ' '].glyph_width;
    uint8_t rows          = g_active_font->font_table[c - ' '].glyph_height;
    uint8_t baseline      = g_active_font->font_table[c - ' '].glyph_baseline;
    uint8_t advance_width = g_active_font->font_table[c - ' '].glyph_advance_width;
    // Get location of glyph in font
    const uint8_t *glyph_address = g_active_font->address + glyph_offset;
    // Calculate correct glyph baseline
    y += baseline;
    // Display glyph
    ssd1322_put_resource_fb(fb, x_virtual, y, rows, columns, glyph_address);
    // Return the current x coordinate of the frame buffer
    return advance_width;
}

uint8_t ssd1322_put_string_fb(uint8_t *fb,
                              uint8_t x_virtual,
                              uint8_t y,
                              const char *string)
{
    while (*string)
    {
        x_virtual += ssd1322_put_char_fb(fb, x_virtual, y, *string++);
    }

    // Return the current x coordinate of the frame buffer
    return x_virtual;
}

void ssd1322_fill_fb(uint8_t *fb, uint8_t data)
{
    for (uint8_t i = 0; i < BUFFER_HEIGHT; i++)
    {
        for (uint8_t j = 0; j < BUFFER_WIDTH; j++)
        {
            fb[(i * BUFFER_WIDTH) + j] = data;
        }
    }
}

void ssd1322_display_fb(uint8_t *fb)
{
    ssd1322_set_address(0, 0);
    // Send entire frame buffer to ssd1322
    DATA_COMMAND_HIGH();
    CHIP_SELECT_LOW();
    spi1_transmit_buffer(fb, BUFFER_SIZE);
    DATA_COMMAND_HIGH();
    CHIP_SELECT_HIGH();
}