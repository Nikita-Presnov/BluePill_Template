# Path you your toolchain installation, leave empty if already in system PATH
# TOOLCHAIN_PATH =

# Path to the STM32 codebase, make sure to update the submodule to get the code
PERIPH_LIB = PeripheralLib
CMSIS_ROOT = CMSIS
###############################################################################

# Project specific
SRC_DIR = Src
INC_DIR = Inc
BUILD_DIR = Build
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
LD_SCRIPT = STM32F103XB_FLASH.ld

# Project includes
INCLUDES   = -I$(INC_DIR)

# Vendor sources
CXX_FILES += $(PERIPH_LIB)/Src/stm32f1xx_ll_gpio.c
CXX_FILES += $(PERIPH_LIB)/Src/stm32f1xx_ll_tim.c

# Vendor includes
INCLUDES += -I$(PERIPH_LIB)/Inc
INCLUDES += -I$(CMSIS_ROOT)/STM32F1xx/Include
INCLUDES += -I$(CMSIS_ROOT)/Include

# Compiler Flags

# GCC
CFLAGS  = -O2 -Wall -Wextra -Warray-bounds -std=c99
ifdef DEBUG
CFLAGS += -g
endif
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
# CFLAGS += -DUSE_FULL_LL_DRIVER

# Linker Flags
LFLAGS = -Wl,--gc-sections -Wl,-T$(LD_SCRIPT) --specs=rdimon.specs

###############################################################################

# Unlike the original source, this file throws object files into the correct directory.
OBJECTS  = $(addprefix $(BUILD_DIR)/$(OBJ_DIR)/,$(notdir $(CXX_FILES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/$(OBJ_DIR)/,$(notdir $(ASM_FILES:.s=.o)))
DEPENDS  = $(addprefix $(BUILD_DIR)/$(OBJ_DIR)/,$(notdir $(CXX_FILES:.c=.d)))
DEPENDS += $(addprefix $(BUILD_DIR)/$(OBJ_DIR)/,$(notdir $(ASM_FILES:.s=.d)))
vpath %.c $(sort $(dir $(CXX_FILES)))
vpath %.s $(sort $(dir $(ASM_FILES)))


.PHONY: clean

all: $(BUILD_DIR)/$(TARGET).elf

-include $(DEPENDS)
# Compile asm
$(BUILD_DIR)/$(OBJ_DIR)/%.o: %.s Makefile | $(BUILD_DIR) 
	@echo "[AS] $<"
	@$(AS) $(CFLAGS) -c $< -o $@

# Compile c
$(BUILD_DIR)/$(OBJ_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@echo "[CC] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Link
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo "[LD] $@"
	@$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) -o $@
	@$(SZ) $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	@echo "[HEX] $@"
	@$(HEX) $< $@
	@$(SZ) $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	@echo "[BIN] $@"
	@$(BIN) $< $@

intel-hex: $(BUILD_DIR)/$(TARGET).hex

binary: $(BUILD_DIR)/$(TARGET).bin

# Make directory
$(BUILD_DIR):
	@mkdir $@
	@mkdir $@/$(OBJ_DIR)

# Clean
clean:
#	@rm -rfv $(BUILD_DIR)
	@rm $(BUILD_DIR)/$(TARGET).* $(OBJECTS) $(DEPENDS)
