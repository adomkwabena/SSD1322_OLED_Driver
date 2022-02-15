###############################################################################
# A template Makefile for ST's Cortex-M4 MCUs      
#
# References: 
# https://github.com/vkottler/diymore-stm32f407/blob/master/Makefile   
# https://interrupt.memfault.com/blog/gnu-make-guidelines                        
###############################################################################

###############################################################################
# Definitions
###############################################################################

# Project
PROJECT   = stm32f4_proj_template

# Compiler & Linker
CC        = arm-none-eabi-gcc

# Sources
SOURCES  += $(shell find src -name '*.c')

# Objects
OBJECTS  += $(SOURCES:.c=.o)
OBJECTS  += src/startup/startup.o

# Header file includes
CFLAGS   += -Iinclude/
CFLAGS   += -Iinclude/cmsis/
CFLAGS   += -Iinclude/system/
CFLAGS   += -Iinclude/core_drivers/rcc/
CFLAGS   += -Iinclude/core_drivers/spi1/
CFLAGS   += -Iinclude/core_drivers/usart1/
CFLAGS   += -Iinclude/core_drivers/i2c1/
CFLAGS   += -Iinclude/core_drivers/gpio/
CFLAGS   += -Iinclude/core_drivers/usart2/

# Processor specific flags
CFLAGS   += -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard

# Compiler flags to compile for code size
CFLAGS   += -O0 -flto -Wall -ffunction-sections -fdata-sections -fno-builtin

# Enable debugging data
CFLAGS   += -g3 -gdwarf-2

# Linker flags to link for code size
LDFLAGS  += -Wl,--gc-sections --specs=nosys.specs -Tlinker_script.ld

# Dependency flags
DEPFLAGS += -MMD -MP -MF $<.d

# Include dependencies
include $(shell find . -name "*.d")

###############################################################################
# Compiling and Linking
###############################################################################

# Target
.DEFAULT_GOAL = all
.PHONY: all
all: $(PROJECT).bin

# Assembly source rule
%.o: %.S
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

# C source rule
%.o: %.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

# Linker - generate .elf file
$(PROJECT).elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Generate .bin file
$(PROJECT).bin: $(PROJECT).elf
	arm-none-eabi-objcopy -O binary $< $@

.PHONY: clean
clean:
	@echo "Deleting all build outputs ..."
	@rm -f *.bin *.elf
	@find . -name "*.o" -delete
	@find . -name "*.d*" -delete
	@echo "Done!"


###############################################################################
# Programming
###############################################################################

.PHONY: burn
burn:
	st-flash --reset write $(PROJECT).bin 0x8000000