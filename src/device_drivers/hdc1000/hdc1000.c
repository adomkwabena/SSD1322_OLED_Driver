/** 
 *  @file   hdc1000.c
 *  @author Adom Kwabena
 *  @brief  This driver provides APIs for interfacing with the HDC1000 temperature
 *          and humidity sensor from TI.
 */

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include "delay.h"
#include "hdc1000.h"
#include "i2c2.h"

// ****************************************************************************
// * Module APIs
// ****************************************************************************

void hdc1000_init(uint16_t acquisition_mode)
{
    i2c2_init();

    // Set the acquisition mode of the chip
    hdc1000_set_reg(CONFIG_REGISTER, acquisition_mode);
}

void hdc1000_read(uint8_t length, uint8_t* data_ptr)
{
    i2c2_receive_buffer(HDC1000_ADDRESS, data_ptr, length);
}

void hdc1000_write(uint8_t length, uint8_t* data_ptr)
{
    i2c2_transmit_buffer(HDC1000_ADDRESS, data_ptr, length);
}

void hdc1000_set_reg(uint8_t address, uint16_t value)
{
    uint8_t data[3] = {0, 0, 0};
    data[0] = address;
    // Get MSB of value
    data[1] = value >> 8;
    // Get LSB of value
    data[2] = value;
    hdc1000_write(3, &data[0]);
}

uint16_t hdc1000_get_reg(uint8_t address)
{
    uint8_t data[2] = {0, 0};
    data[0] = address;
    // Send address of register
    hdc1000_write(1, &data[0]);
    // Wait for conversion to complete
    delay_ms(8);
    // Read data from specified address
    hdc1000_read(2, &data[0]);
    return (data[0] << 8) + data[1];
}

uint16_t hdc1000_get_temperature(void)
{
    return hdc1000_get_reg(TEMPERATURE_REGISTER);
}

uint16_t hdc1000_get_humidity(void)
{
    return hdc1000_get_reg(HUMIDITY_REGISTER);
}

void hdc1000_get_everything(hdc1000_data_t * values)
{
    // Temperature(address 0x00) is read first before humidity(address 0x01)
    uint8_t data[4] = {TEMPERATURE_REGISTER, 0, 0, 0};
    hdc1000_write(1, &data[0]);
    // Wait for conversion to complete
    delay_ms(16);
    hdc1000_read(4, &data[0]);
    // Temperature data
    values->temperature_raw = (data[0] << 8) + data[1];
    // Humidity data
    values->humidity_raw = (data[2] << 8) + data[3];
    // Convert temperature into degrees celsius
    values->temperature = ((values->temperature_raw / 65536.0) * 165.0) - 40.0;
    // Convert humidity into a percentage
    values->humidity = (values->humidity_raw / 65536.0) * 100.0;
}