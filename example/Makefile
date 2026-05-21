# Makefile for building the RISC-V firmware example
# Usage: make TARGET_DIR=path/to/target/dir TARGET_NAME=target_name

ifndef TARGET_DIR
$(info TARGET_DIR is not set. Usage: make TARGET_DIR=path/to/target/dir TARGET_NAME=target_name)
$(error TARGET_DIR is required)
endif

TARGET_NAME ?= $(notdir $(patsubst %/,%,$(TARGET_DIR)))

#################################################################
TOOLCHAIN_BIN = /home/yoyo/work/Code/bereise/ISS/rv32i-sim/riscv-toolchain/bin
CC = $(TOOLCHAIN_BIN)/riscv32-unknown-elf-gcc
OBJDUMP = $(TOOLCHAIN_BIN)/riscv32-unknown-elf-objdump
OBJCOPY = $(TOOLCHAIN_BIN)/riscv32-unknown-elf-objcopy

COMMON_DIR = common

COMMON_FLAGS = -march=rv32i_zicsr -mabi=ilp32
CFLAGS = $(COMMON_FLAGS) -ffreestanding -O0 -ffunction-sections -fdata-sections -I$(COMMON_DIR)
LDFLAGS = $(COMMON_FLAGS) -T $(COMMON_DIR)/linker.ld -nostartfiles -nostdlib -static -specs=nano.specs -specs=nosys.specs -Wl,--gc-sections -Wl,-Map=$(TARGET:.elf=.map)
LIBS = -lc -lgcc -lnosys

TARGET_SRC_C = $(wildcard $(TARGET_DIR)/*.c)
COMMON_SRC_C = $(wildcard $(COMMON_DIR)/*.c)
TARGET_SRC_S = $(wildcard $(TARGET_DIR)/*.S)
COMMON_SRC_S = $(wildcard $(COMMON_DIR)/*.S)
SRC_C = $(TARGET_SRC_C) $(COMMON_SRC_C)
SRC_S = $(TARGET_SRC_S) $(COMMON_SRC_S)

SOURCES = $(SRC_C) $(SRC_S)

BUILD_DIR = $(TARGET_DIR)/build

TARGET_OBJ_C = $(patsubst $(TARGET_DIR)/%.c,$(BUILD_DIR)/%.o,$(TARGET_SRC_C))
COMMON_OBJ_C = $(patsubst $(COMMON_DIR)/%.c,$(BUILD_DIR)/common-%.o,$(COMMON_SRC_C))
TARGET_OBJ_S = $(patsubst $(TARGET_DIR)/%.S,$(BUILD_DIR)/%.o,$(TARGET_SRC_S))
COMMON_OBJ_S = $(patsubst $(COMMON_DIR)/%.S,$(BUILD_DIR)/common-%.o,$(COMMON_SRC_S))
OBJECTS = $(TARGET_OBJ_C) $(COMMON_OBJ_C) $(TARGET_OBJ_S) $(COMMON_OBJ_S)

TARGET = $(BUILD_DIR)/$(TARGET_NAME).elf
BIN = $(BUILD_DIR)/$(TARGET_NAME).bin

SIMULATOR = ../simulator/bin/rv32i-sim
SIM_FLAGS = -M 512K

all: $(BIN)

$(BIN): $(TARGET)
	@mkdir -p $(dir $@)
	$(OBJCOPY) -O binary $< $@

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(TARGET_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TARGET_DIR)/%.S
	@mkdir -p $(dir $@)
	$(CC) $(COMMON_FLAGS) -c $< -o $@

$(BUILD_DIR)/common-%.o: $(COMMON_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/common-%.o: $(COMMON_DIR)/%.S
	@mkdir -p $(dir $@)
	$(CC) $(COMMON_FLAGS) -c $< -o $@

dump: $(TARGET)
	$(OBJDUMP) -D $< > $(TARGET:.elf=.dump)

dump_%: $(TARGET)
	$(OBJDUMP) --disassemble=$* $< > $(TARGET:.elf=.dump)

clean:
	rm -rf $(BUILD_DIR)

sim:
	@echo "Running simulation for $(TARGET_NAME)..."
	@$(SIMULATOR) $(SIM_FLAGS) $(TARGET)

.PHONY: all clean sim dump dump_%
