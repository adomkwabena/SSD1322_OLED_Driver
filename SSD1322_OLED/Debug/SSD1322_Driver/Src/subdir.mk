################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SSD1322_Driver/Src/IBMPlexMono_Regular.c \
../SSD1322_Driver/Src/Lato_Regular.c \
../SSD1322_Driver/Src/PTM55FT.c \
../SSD1322_Driver/Src/bitmaps.c \
../SSD1322_Driver/Src/delay.c \
../SSD1322_Driver/Src/ssd1322.c 

OBJS += \
./SSD1322_Driver/Src/IBMPlexMono_Regular.o \
./SSD1322_Driver/Src/Lato_Regular.o \
./SSD1322_Driver/Src/PTM55FT.o \
./SSD1322_Driver/Src/bitmaps.o \
./SSD1322_Driver/Src/delay.o \
./SSD1322_Driver/Src/ssd1322.o 

C_DEPS += \
./SSD1322_Driver/Src/IBMPlexMono_Regular.d \
./SSD1322_Driver/Src/Lato_Regular.d \
./SSD1322_Driver/Src/PTM55FT.d \
./SSD1322_Driver/Src/bitmaps.d \
./SSD1322_Driver/Src/delay.d \
./SSD1322_Driver/Src/ssd1322.d 


# Each subdirectory must supply rules for building sources it contributes
SSD1322_Driver/Src/IBMPlexMono_Regular.o: ../SSD1322_Driver/Src/IBMPlexMono_Regular.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Adom_Kwabena/STM32CubeIDE/workspace_1.0.0/SSD1322_OLED/SSD1322_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SSD1322_Driver/Src/IBMPlexMono_Regular.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SSD1322_Driver/Src/Lato_Regular.o: ../SSD1322_Driver/Src/Lato_Regular.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Adom_Kwabena/STM32CubeIDE/workspace_1.0.0/SSD1322_OLED/SSD1322_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SSD1322_Driver/Src/Lato_Regular.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SSD1322_Driver/Src/PTM55FT.o: ../SSD1322_Driver/Src/PTM55FT.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Adom_Kwabena/STM32CubeIDE/workspace_1.0.0/SSD1322_OLED/SSD1322_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SSD1322_Driver/Src/PTM55FT.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SSD1322_Driver/Src/bitmaps.o: ../SSD1322_Driver/Src/bitmaps.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Adom_Kwabena/STM32CubeIDE/workspace_1.0.0/SSD1322_OLED/SSD1322_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SSD1322_Driver/Src/bitmaps.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SSD1322_Driver/Src/delay.o: ../SSD1322_Driver/Src/delay.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Adom_Kwabena/STM32CubeIDE/workspace_1.0.0/SSD1322_OLED/SSD1322_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SSD1322_Driver/Src/delay.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SSD1322_Driver/Src/ssd1322.o: ../SSD1322_Driver/Src/ssd1322.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Adom_Kwabena/STM32CubeIDE/workspace_1.0.0/SSD1322_OLED/SSD1322_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SSD1322_Driver/Src/ssd1322.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

