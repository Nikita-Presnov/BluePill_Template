# Path you your toolchain installation, leave empty if already in system PATH
TOOLCHAIN_ROOT =

# Path to the STM32 codebase, make sure to update the submodule to get the code
PERIPH_LIB = PeripheralLib
CMSIS_ROOT = CMSIS
STARTUP = Startup
###############################################################################

# Project specific
SRC_DIR = Src
INC_DIR = Inc
OUTPUT_DIR = Bin
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
# SRC_ASM_FILES += Startup/startup_stm32f103xb.s
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

# This does an in-source build. An out-of-source build that places all object
# files into a build directory would be a better solution, but the goal was to
# keep this file very simple.
CXX_OBJS = $(patsubst %.c, $(OUTPUT_DIR)/%.o, $(SRC_CXX_FILES))
ASM_OBJS = $(patsubst %.s, $(OUTPUT_DIR)/%.o, $(SRC_ASM_FILES))
ALL_OBJS = $(ASM_OBJS) $(CXX_OBJS)

.PHONY: clean

all: $(OUTPUT_DIR)/$(TARGET)

# Compile
$(CXX_OBJS): $(OUTPUT_DIR)/%.o: %.c
$(ASM_OBJS): $(OUTPUT_DIR)/%.o: %.s
$(ALL_OBJS):
	@echo "[CC] $< -> $@"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

# Link
$(OUTPUT_DIR)/$(TARGET): $(ALL_OBJS)
	@echo "[LD] $@"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(LFLAGS) $(ALL_OBJS) -o $@

# Clean
clean:
	@rm -rfv $(OUTPUT_DIR)
