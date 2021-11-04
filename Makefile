###############################################################################
# A template Makefile for ST's Cortex-M4 MCUs      
# Reference: https://github.com/vkottler/diymore-stm32f407/blob/master/Makefile                  
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
SOURCES  += $(shell find src -name '*.s')
#SOURCES += ./src/main.c
#SOURCES += ./src/core_drivers/rcc/rcc.c

# Objects
OBJECTS  += ./src/main.o
OBJECTS  += ./src/startup/startup.o
OBJECTS  += ./src/core_drivers/rcc/rcc.o
OBJECTS  += ./src/core_drivers/spi1/spi1.o
#OBJECTS += $(SOURCES:.c,.s=.o)

# Header file includes
CFLAGS   += -I ./include
CFLAGS   += -I ./include/cmsis
CFLAGS   += -I ./include/system
CFLAGS   += -I ./include/core_drivers/rcc
CFLAGS   += -I ./include/core_drivers/spi1

# Processor specific flags
CFLAGS   += -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard

# Compiler flags to compile for code size
CFLAGS   += -O0 -flto -Wall -ffunction-sections -fdata-sections -fno-builtin

# Linker flags to link for code size
LFLAGS   += -Wl,--gc-sections --specs=nosys.specs -Tlinker_script.ld


###############################################################################
# Compiling and Linking
###############################################################################

# Target
.DEFAULT_GOAL = all
.PHONY: all
all: $(PROJECT).bin

# Automatic prerequisite generation
%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# Assembly source rule
%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

# C source rule
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Linker - generate .elf file
$(PROJECT).elf: $(OBJECTS)
	$(CC) $(LFLAGS) $^ $(CFLAGS) -o $@

# Generate .bin file
$(PROJECT).bin: $(PROJECT).elf
	arm-none-eabi-objcopy -O binary $< $@

include $(SOURCES:.c=.d)

.PHONY: clean
clean:
	@echo "Deleting all build outputs ..."
	@rm -f *.bin *.elf
	@find . -name "*.o" -delete
	@find . -name "*.d*" -delete
	@echo "Done."


###############################################################################
# Programming
###############################################################################

.PHONY: burn
burn:
	st-flash --reset write $(PROJECT).bin 0x8000000