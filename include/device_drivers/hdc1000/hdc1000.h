/** 
 *  @file   hdc1000.h
 *  @author Adom Kwabena
 *  @brief  This driver provides APIs for interfacing with the HDC1000 temperature
 *          and humidity sensor from TI.
 */

#ifndef __HDC1000_INC__
#define	__HDC1000_INC__

// ****************************************************************************
// * Included Files
// ****************************************************************************

#include <stdint.h>

// ****************************************************************************
// * Definitions and Macros
// ****************************************************************************

// HDC1000 I2C and register addresses
#define HDC1000_ADDRESS         0x80
#define TEMPERATURE_REGISTER    0x00
#define HUMIDITY_REGISTER       0X01
#define CONFIG_REGISTER         0X02
#define SERIAL_ID_1             0xFB
#define SERIAL_ID_2             0xFC
#define SERIAL_ID_3             0xFD
#define MANUFACTURER_ID         0xFE
#define DEVICE_ID               0xFF

// Useful for setting and clearing bits of the configuration register
#define HEATER_ON              0x2000
#define HEATER_OFF             0x0000
#define MODE_TEMP_OR_HUMID     0x0000
#define MODE_TEMP_AND_HUMID    0x1000              
#define BATTERY_STATUS_MASK    0x0800      
#define TRES_11                0x0400
#define TRES_14                0x0000
#define HRES_8                 0x0200
#define HRES_11                0x0100
#define HRES_14                0x0000     

// ****************************************************************************
// * Module Data Structures
// ****************************************************************************

// This data structure stores raw and converted data from the HDC1000 chip
typedef struct
{
    uint16_t temperature_raw;
    uint16_t humidity_raw;
    float temperature;
    float humidity;
} hdc1000_data_t;

// ****************************************************************************
// * Function Prototypes
// ****************************************************************************

/**
 * @brief This function initializes the hdc1000 chip.
 * 
 * @param acquisition_mode: Set the format and resolution of read data
 * 
 * @returns None
 */
void hdc1000_init(uint16_t acquisition_mode);

/**
 * @brief This function abstracts I2C read transactions with the HDC1000 chip
 *   
 * @param length: The number of bytes to be sent to the HDC1000 chip
 * @param data_ptr: A pointer to an array into which data will be stored
 * 
 * @returns None
 */
void hdc1000_read(uint8_t length, uint8_t * data_ptr);

/**
 * @brief This function abstracts I2C write transactions with the HDC1000 chip
 *   
 * @param length: The number of bytes to be sent to the HDC1000 chip
 * @param data_ptr: A pointer to an array from which data will be sent
 * 
 * @returns None
 */
void hdc1000_write(uint8_t length, uint8_t * data_ptr);

/**
 * @brief This function provides a means to edit the values of a register
 * 
 * @param Address: The address of the register to be modified
 * @param Value: The new contents of the register
 * 
 * @returns None
 */
void hdc1000_set_reg(uint8_t address, uint16_t value);

/**
 * @brief This function reads the data in a specified register
 * 
 * @param address: The address of the register to be read
 * 
 * @returns The contents of the specified register
 */
uint16_t hdc1000_get_reg(uint8_t address);

/**
 * @brief This function triggers a temperature conversion and returns the result.
 * 
 * @pre Initialize the contents of the status register before calling this function.
 *      The function hdc1000_set_reg(CONFIG_REGISTER, mode) has to be called, where
 *      the mode should enable measuring temperature or humidity.
 * 
 * @param None.
 * 
 * @returns The result of the temperature conversion.
 */
uint16_t hdc1000_get_temperature(void);

/**
 * @brief This function triggers a humidity conversion and returns the result.
 *  
 * @pre Initialize the contents of the status register before calling this function.
 *       The function hdc1000_set_reg(CONFIG_REGISTER, mode) has to be called, where
 *       the mode should enable measuring temperature or humidity.
 * 
 * @param None.
 * 
 * @returns The result of the humidity conversion.
 */
uint16_t hdc1000_get_humidity(void);

/**
 * @brief This function triggers a temperature and humidity conversion and 
 *        and converts the result from raw data to scientific units.
 * 
 * @pre Initialize the contents of the status register before calling this 
 *      function. The function hdc1000_set_reg(CONFIG_REGISTER, mode) has 
 *      to be called, where the mode should enable measuring 
 *      temperature and humidity at a go.
 * 
 * @param values: A pointer to a structure to store the read data into.
 * 
 * @returns None
 */
void hdc1000_get_everything(hdc1000_data_t * values);

#endif
