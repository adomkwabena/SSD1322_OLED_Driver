/**
 *  @file   ftoa.h
 *  @author Adom Kwabena
 *  @brief  This file provides functions to convert a float to a string
 */

// Prevent multiple file inclusion.
#ifndef     __FTOA_INC__
#define     __FTOA_INC__

// ****************************************************************************
// * Definitions and Macros
// ****************************************************************************

// Define the precision in terms of decimal places (rounding of is not performed)
// For example 2.17843 with a precision of 2 is 2.17
#define PRECISION       2

/**
 *  @brief Converts a floating point number into a string.
 *  @param number: The number to be converted.
 *  @param buffer: A pointer to the array where the result is stored.
 *  @returns: None
 */
void ftoa(float number, char * buffer);

#endif