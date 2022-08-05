/**
 *  @file   ftoa.c
 *  @author Adom Kwabena
 *  @brief  This file implements a very basic float to string converter.
 * 
 *  Burrowed some ideas from: https://github.com/mpaland/printf/blob/master/printf.c
 */

// ****************************************************************************
// * Included Files.
// ****************************************************************************

#include "itoa.h"
#include "ftoa.h"
#include <string.h>
#include <stdbool.h>

// ****************************************************************************
// * Module APIs
// ****************************************************************************

void ftoa(float number, char * buffer)
{
    // Get the integer portion of this number.
    int integer_part = (int) number;

    // Powers of 10
    static const double pow10[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

    double tmp = (number - integer_part) * pow10[PRECISION];
    int fractional_part = (int) tmp;

    char fractional_string[32] = {0};

    // Convert integer to string
    itoa(number, buffer);
    itoa(fractional_part, fractional_string);

    int integer_len = strlen(buffer);
    
    // Insert decimal point in number
    buffer[integer_len] = '.';
    buffer[integer_len + 1] = '\0';

    strcat(buffer, fractional_string);
}