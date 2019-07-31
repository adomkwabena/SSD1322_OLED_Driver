################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include SSD1322_Driver/Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
SSD1322_OLED.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
SSD1322_OLED.list \


# All Target
all: SSD1322_OLED.elf secondary-outputs

# Tool invocations
SSD1322_OLED.elf: $(OBJS) $(USER_OBJS) C:\Users\Adom_Kwabena\STM32CubeIDE\workspace_1.0.0\SSD1322_OLED\STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "SSD1322_OLED.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Adom_Kwabena\STM32CubeIDE\workspace_1.0.0\SSD1322_OLED\STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="SSD1322_OLED.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

SSD1322_OLED.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "SSD1322_OLED.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
