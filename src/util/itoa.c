/**
 *  @file   itoa.c
 *  @author Adom Kwabena
 *  @brief  This file provides functions to convert an int to a string
 */

// ****************************************************************************
// * Include Files.
// ****************************************************************************

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "ftoa.h"

// ****************************************************************************
// * Module APIs
// ****************************************************************************

void reverse_string(char * string)
{
    // Reverse a string in place
    int j = strlen(string) - 1;
    for (int i = 0; i <= strlen(string); i++, j--)
    {
        if (j > i)
        {
            char tmp = string[i];
            string[i] = string[j];
            string[j] = tmp;
        }
    }
}

void itoa(int number, char * buffer)
{
    bool is_negative = 0;
    
    // Handle negative numbers
    if (number < 0)
    {
        number = -number;
        is_negative = 1;
    }

    int index = 0;

    do
    {
        // Convert last digit to char
        buffer[index++] = (number % 10) + '0';
        // Handle next digit
        number = number / 10;
    } 
    while (number > 0);

    if (is_negative)
    {
        buffer[index++] = '-';
    }
    // Terminate string
    buffer[index] = '\0';
    reverse_string(buffer);
}