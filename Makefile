# Path you your toolchain installation, leave empty if already in system PATH
# TOOLCHAIN_PATH =

# Path to the STM32 codebase, make sure to update the submodule to get the code
PERIPH_LIB = PeripheralLib
CMSIS_ROOT = CMSIS
STARTUP = Startup
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
CC = $(TOOLCHAIN_ROOT)/arm-none-eabi-gcc
AS = $(TOOLCHAIN_ROOT)/arm-none-eabi-gcc -x assembler-with-cpp
CP = $(TOOLCHAIN_ROOT)/arm-none-eabi-objcopy
SZ = $(TOOLCHAIN_ROOT)/arm-none-eabi-size
else
CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc -x assembler-with-cpp
CP = arm-none-eabi-objcopy
SZ = arm-none-eabi-size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

# Project sources
CXX_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.s) $(wildcard $(SRC_DIR)/*/*.s)
ASM_FILES += $(STARTUP)/startup_stm32f103xb.s
CXX_FILES += $(STARTUP)/system_stm32f1xx.c
LD_SCRIPT = STM32F103XB_FLASH.ld

# Project includes
INCLUDES   = -I$(INC_DIR)

# Vendor sources: Note that files in "Templates" are normally copied into project for customization, but that is not necessary for this simple project.
CXX_FILES += $(PERIPH_LIB)/Src/stm32f1xx_ll_gpio.c
CXX_FILES += $(PERIPH_LIB)/Src/stm32f1xx_ll_tim.c

# Vendor includes
INCLUDES += -I$(PERIPH_LIB)/Inc
INCLUDES += -I$(CMSIS_ROOT)/STM32F1xx/Include
INCLUDES += -I$(CMSIS_ROOT)/Include

# Compiler Flags

# GCC
CFLAGS  = -O2 -Wall -Wextra -Warray-bounds
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

# Linker Flags
LFLAGS = -Wl,--gc-sections -Wl,-T$(LD_SCRIPT) --specs=rdimon.specs

###############################################################################

# Unlike the original source, this file throws object files into the correct directory.

CXX_OBJS = $(addprefix $(BUILD_DIR)/$(OBJ_DIR)/,$(notdir $(CXX_FILES:.c=.o)))
ASM_OBJS = $(addprefix $(BUILD_DIR)/$(OBJ_DIR)/,$(notdir $(ASM_FILES:.s=.o)))
DEPENDS  = $(addprefix $(BUILD_DIR)/$(OBJ_DIR)/,$(notdir $(CXX_FILES:.c=.d)))
DEPENDS += $(addprefix $(BUILD_DIR)/$(OBJ_DIR)/,$(notdir $(CXX_FILES:.s=.d)))
ALL_OBJS = $(ASM_OBJS) $(CXX_OBJS)
vpath %.c $(sort $(dir $(CXX_FILES)))
vpath %.s $(sort $(dir $(ASM_FILES)))


.PHONY: clean

all: $(BUILD_DIR)/$(TARGET).elf

-include $(DEPENDS)
# Compile asm
$(BUILD_DIR)/$(OBJ_DIR)/%.o: %.s Makefile | $(BUILD_DIR) 
	@echo "[CC] $<"
	@$(AS) $(CFLAGS) -c $< -o $@

# Compile c
$(BUILD_DIR)/$(OBJ_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@echo "[CC] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Link
$(BUILD_DIR)/$(TARGET).elf: $(ALL_OBJS) 
	@echo "[LD] $@"
	@$(CC) $(CFLAGS) $(LFLAGS) $(ALL_OBJS) -o $@
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
	@rm -rfv $(BUILD_DIR)
