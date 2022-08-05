/**
 *  @file   itoa.h
 *  @author Adom Kwabena
 *  @brief  This file provides functions to convert an int to a string
 */

// Prevent multiple file inclusion.
#ifndef     __ITOA_INC__
#define     __ITOA_INC__

// ****************************************************************************
// * Include Files.
// ****************************************************************************

#include <stdint.h>

// ****************************************************************************
// * Function Prototypes.
// ****************************************************************************

/**
 *  @brief Reverses the characters of a string in place.
 *  @param buffer: A pointer to the string to be reversed.
 *  @returns: None
 */
void reverse_string(char string[]);

/**
 *  @brief Converts an integer into a string.
 *  @param number: The number to be converted.
 *  @param buffer: A pointer to the array where the result is stored.
 *  @returns: None
 */
void itoa(int number, char buffer[]);


#endif
