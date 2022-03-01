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
PROJECT    = stm32f4_proj_template

# Build directory
BUILD_DIR := build

# Compiler & Linker
CC         = arm-none-eabi-gcc

# Sources
SOURCES   += $(shell find src -name '*.c')

# Objects
OBJECTS   += $(addprefix $(BUILD_DIR)/,$(SOURCES:.c=.o))
OBJECTS   += $(BUILD_DIR)/src/startup/startup.o

# Header file include directories
CFLAGS    += -Iinclude/
CFLAGS    += -Iinclude/cmsis/
CFLAGS    += -Iinclude/system/
CFLAGS    += -Iinclude/core_drivers/

# Processor specific flags
CFLAGS    += -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard

# Compiler flags to compile for code size
CFLAGS    += -O0 -flto -Wall -ffunction-sections -fdata-sections -fno-builtin

# Enable debugging data
CFLAGS    += -g3 -gdwarf-2

# Linker flags to link for code size
LDFLAGS   += -Wl,--gc-sections --specs=nosys.specs -Tlinker_script.ld
# Generate a map file
LDFLAGS   += -Wl,-Map=$@.map

# Dependency flags
DEPFLAGS  += -MMD -MP -MF $@.d

# Include dependencies
include $(shell find . -name "*.d")

###############################################################################
# Compiling and Linking
###############################################################################

# Target
.DEFAULT_GOAL = all
.PHONY: all
all: $(BUILD_DIR)/$(PROJECT).bin

# Create a build directory before generating objects
$(OBJECTS): | $(BUILD_DIR)

# Rule for actually creating the build directory
$(BUILD_DIR):
	@mkdir -p $@

# Assembly source rule
# Create the folder structure for the output file before compiling
$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# C source rule
# Create the folder structure for the output file before compiling
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Invoke linker to generate .elf file
$(BUILD_DIR)/$(PROJECT).elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Generate .bin file
$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	arm-none-eabi-objcopy -O binary $< $@

# Remove compiler outputs
.PHONY: clean
clean:
	@echo "Deleting all build outputs ..."
	@rm -rf $(BUILD_DIR)
	@echo "Done!"


###############################################################################
# Programming
###############################################################################

.PHONY: burn
burn:
	st-flash --reset write $(BUILD_DIR)/$(PROJECT).bin 0x8000000