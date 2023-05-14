# Path you your toolchain installation, leave empty if already in system PATH
# TOOLCHAIN_PATH =

# Path to the codebase, make sure to update the submodule to get the code
# PERIPH_LIB = PeripheralLib
CMSIS_ROOT = CMSIS
USB = USBLib
###############################################################################

# Project specific
SRC_DIR = Src
INC_DIR = Inc
BUILD_DIR = Build
DEBUG_DIR = Debug
RELEASE_DIR = Release
OBJ_DIR = Obj
TARGET = firmware
MODEL = STM32F103xB
MODEL_CORE = cortex-m3

# Toolchain
ifdef TOOLCHAIN_PATH
CC = $(TOOLCHAIN_PATH)/arm-none-eabi-gcc
AS = $(TOOLCHAIN_PATH)/arm-none-eabi-gcc -x assembler-with-cpp
CP = $(TOOLCHAIN_PATH)/arm-none-eabi-objcopy
SZ = $(TOOLCHAIN_PATH)/arm-none-eabi-size -d -G
else
CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc -x assembler-with-cpp
CP = arm-none-eabi-objcopy
SZ = arm-none-eabi-size -d -G
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

# Project sources
CXX_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.s) $(wildcard $(SRC_DIR)/*/*.s)
# TMP_CXX_FILES := $(CXX_FILES)
# CXX_FILES = $(filter-out $(SRC_DIR)/system_stm32f1xx.c, $(TMP_CXX_FILES))
LD_SCRIPT = STM32F103XB_FLASH.ld

# Project includes
INCLUDES   = -I$(INC_DIR)

ifdef PERIPH_LIB
CXX_FILES += $(PERIPH_LIB)/Src/stm32f1xx_ll_gpio.c
CXX_FILES += $(PERIPH_LIB)/Src/stm32f1xx_ll_tim.c
CXX_FILES += $(PERIPH_LIB)/Src/stm32f1xx_ll_rcc.c
CXX_FILES += $(PERIPH_LIB)/Src/stm32f1xx_ll_usart.c
INCLUDES += -I$(PERIPH_LIB)/Inc
endif

ifdef CMSIS_ROOT
INCLUDES += -I$(CMSIS_ROOT)/STM32F1xx/Include
INCLUDES += -I$(CMSIS_ROOT)/Include
endif

ifdef USB
CXX_FILES += $(wildcard $(USB)/*.c)
INCLUDES += -I$(USB)
endif
# Compiler Flags

# GCC

ifdef DEBUG
# $(info [info] debug mode)
CFLAGS  = -g -Og 
BUILD_MODE = $(DEBUG_DIR)
else
# $(info [info] release mode)
CFLAGS  = -O2
BUILD_MODE = $(RELEASE_DIR)
endif
CFLAGS += -Wall -Wextra -Warray-bounds -std=c99 -lc -lgcc -lm
CFLAGS += -std=c11
CFLAGS += -lc -lgcc -lm
# Generate dependency information
CFLAGS += -MMD -MP
#  -MF"$(@:%.o=%.d)"
# ARM
CFLAGS += -mthumb -mlittle-endian -mthumb-interwork
# CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 

# PROJECT
CFLAGS += -mcpu=$(MODEL_CORE) 
CFLAGS += -D$(MODEL)
CFLAGS += $(INCLUDES)
CFLAGS += -DUSE_FULL_LL_DRIVER

# Linker Flags
LFLAGS = -Wl,--gc-sections -Wl,-T$(LD_SCRIPT) --specs=rdimon.specs

###############################################################################

# Unlike the original source, this file throws object files into the correct directory.
OBJECTS  = $(addprefix $(BUILD_DIR)/$(BUILD_MODE)/$(OBJ_DIR)/,$(notdir $(CXX_FILES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/$(BUILD_MODE)/$(OBJ_DIR)/,$(notdir $(ASM_FILES:.s=.o)))
DEPENDS  = $(addprefix $(BUILD_DIR)/$(BUILD_MODE)/$(OBJ_DIR)/,$(notdir $(CXX_FILES:.c=.d)))
DEPENDS += $(addprefix $(BUILD_DIR)/$(BUILD_MODE)/$(OBJ_DIR)/,$(notdir $(ASM_FILES:.s=.d)))
.PHONY: all clean flash
vpath %.c $(sort $(dir $(CXX_FILES)))
vpath %.s $(sort $(dir $(ASM_FILES)))
# .PHONY: clean

all: $(BUILD_DIR)/$(BUILD_MODE)/$(TARGET).elf
# all: make 
debug:
	@echo "[info] debug mode"
	@DEBUG=1 make

release:
	@echo "[info] release mode"
	@make
	
-include $(DEPENDS)
# Compile asm
$(BUILD_DIR)/$(BUILD_MODE)/$(OBJ_DIR)/%.o: %.s Makefile | $(BUILD_DIR)/$(BUILD_MODE)
	@echo "[AS] $<"
	@$(AS) -c $< -o $@ $(CFLAGS)

# Compile c
$(BUILD_DIR)/$(BUILD_MODE)/$(OBJ_DIR)/%.o: %.c Makefile | $(BUILD_DIR)/$(BUILD_MODE)
	@echo "[CC] $<"
	@$(CC) -c $< -o $@ $(CFLAGS)

# Link
$(BUILD_DIR)/$(BUILD_MODE)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo "[LD] $@"
	@$(CC) -o $@ $(CFLAGS) $(LFLAGS) $(OBJECTS) 
	@$(SZ) $@

$(BUILD_DIR)/$(BUILD_MODE)/$(TARGET).hex: $(BUILD_DIR)/$(BUILD_MODE)/$(TARGET).elf
	@echo "[HEX] $@"
	@$(HEX) $< $@
	@$(SZ) $@

$(BUILD_DIR)/$(BUILD_MODE)/$(TARGET).bin: $(BUILD_DIR)/$(BUILD_MODE)/$(TARGET).elf
	@echo "[BIN] $@"
	@$(BIN) $< $@

intel-hex: $(BUILD_DIR)/$(BUILD_MODE)/$(TARGET).hex

binary: $(BUILD_DIR)/$(BUILD_MODE)/$(TARGET).bin

flash: $(BUILD_DIR)/$(BUILD_MODE)/$(TARGET).elf
	openocd -f ./openocd.cfg -c "init; reset halt; flash write_image erase $<; reset; exit"

# Make directory
$(BUILD_DIR)/$(BUILD_MODE): $(BUILD_DIR)
	@mkdir $(BUILD_DIR)/$(BUILD_MODE)
	@mkdir $(BUILD_DIR)/$(BUILD_MODE)/$(OBJ_DIR)

$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

# Clean
clean:
	@rm -rfv $(BUILD_DIR)
# @rm $(BUILD_DIR)/$(DEBUG_DIR)/$(TARGET)* \
# 	$(BUILD_DIR)/$(RELEASE_DIR)/$(TARGET)* \
# 	$(BUILD_DIR)/$(DEBUG_DIR)/$(OBJ_DIR)/* \
# 	$(BUILD_DIR)/$(RELEASE_DIR)/$(OBJ_DIR)/* 