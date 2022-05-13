# Path you your toolchain installation, leave empty if already in system PATH
# But don't forget to add / or \ at the end 
TOOLCHAIN_ROOT =

# Path to the STM32 codebase, make sure to update the submodule to get the code
PERIPH_LIB = PeripheralLib
CMSIS_ROOT = CMSIS
STARTUP = Startup
###############################################################################

# Project specific
SRC_DIR = Src
INC_DIR = Inc
BUILD_DIR = Bin
OBJ_DIR = Obj
TARGET ?= firmware.elf
# Toolchain
CC = $(TOOLCHAIN_ROOT)arm-none-eabi-gcc
DB = $(TOOLCHAIN_ROOT)gdb-multiarch

# Project sources
SRC_CXX_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
SRC_ASM_FILES = $(wildcard $(SRC_DIR)/*.s) $(wildcard $(SRC_DIR)/*/*.s)
LD_SCRIPT = STM32F103XB_FLASH.ld

# Project includes
INCLUDES   = -I$(INC_DIR)

# Vendor sources: Note that files in "Templates" are normally copied into project for customization,
# but that is not necessary for this simple project.
SRC_ASM_FILES += $(STARTUP)/startup_stm32f103xb.s
SRC_CXX_FILES += $(STARTUP)/system_stm32f1xx.c
SRC_CXX_FILES += $(PERIPH_LIB)/Src/stm32f1xx_ll_gpio.c
SRC_CXX_FILES += $(PERIPH_LIB)/Src/stm32f1xx_ll_tim.c

# Vendor includes
INCLUDES += -I$(PERIPH_LIB)/Inc
INCLUDES += -I$(CMSIS_ROOT)/STM32F1xx/Include
INCLUDES += -I$(CMSIS_ROOT)/Include

# Compiler Flags
CFLAGS  = -g -O0 -Wall -Wextra -Warray-bounds -Wno-unused-parameter
CFLAGS += -mcpu=cortex-m3 
# CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb -mlittle-endian -mthumb-interwork
CFLAGS += -DSTM32F103xB
CFLAGS += $(INCLUDES)

# Linker Flags
LFLAGS = -Wl,--gc-sections -Wl,-T$(LD_SCRIPT) --specs=rdimon.specs

###############################################################################

# Unlike the original source, this file throws object files into the correct directory.

CXX_OBJS = $(addprefix $(BUILD_DIR)/$(OBJ_DIR)/,$(notdir $(SRC_CXX_FILES:.c=.o)))
ASM_OBJS = $(addprefix $(BUILD_DIR)/$(OBJ_DIR)/,$(notdir $(SRC_ASM_FILES:.s=.o)))
ALL_OBJS = $(ASM_OBJS) $(CXX_OBJS)
vpath %.c $(sort $(dir $(SRC_CXX_FILES)))
vpath %.s $(sort $(dir $(SRC_ASM_FILES)))


.PHONY: clean

all: $(BUILD_DIR)/$(TARGET) 

# Compile asm
$(BUILD_DIR)/$(OBJ_DIR)/%.o: %.s | $(BUILD_DIR) 
	@echo "[CC] $^ -> $@"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile c
$(BUILD_DIR)/$(OBJ_DIR)/%.o: %.c | $(BUILD_DIR) 
	@echo "[CC] $^ -> $@"
	@$(CC) $(CFLAGS) -c $< -o $@

# Link
$(BUILD_DIR)/$(TARGET): $(ALL_OBJS) 
	@echo "[LD] $@"
	@$(CC) $(CFLAGS) $(LFLAGS) $(ALL_OBJS) -o $@

# Make directory
$(BUILD_DIR):
	@mkdir $@
	@mkdir $@/$(OBJ_DIR)
# Clean
clean:
	@rm -rfv $(BUILD_DIR)
