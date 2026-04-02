TOOLCHAIN_BIN = /home/yoyo/work/Code/bereise/ISS/rv32i-sim/riscv-toolchain/bin
CC = $(TOOLCHAIN_BIN)/riscv32-unknown-elf-gcc
OBJCOPY = $(TOOLCHAIN_BIN)/riscv32-unknown-elf-objcopy

COMMON_FLAGS = -march=rv32i -mabi=ilp32
CFLAGS = $(COMMON_FLAGS) -ffreestanding -O0 -ffunction-sections -fdata-sections
LDFLAGS = $(COMMON_FLAGS) -T linker.ld -nostartfiles -nostdlib -static -specs=nano.specs -specs=nosys.specs -Wl,--gc-sections -Wl,-Map=$(TARGET:.elf=.map)
LIBS = -lc -lgcc -lnosys

SRC_C = $(wildcard *.c)
SRC_S = $(wildcard *.S)
SOURCES = $(SRC_C) $(SRC_S)
OBJ_C = $(SRC_C:.c=.o)
OBJ_S = $(SRC_S:.S=.o)
OBJECTS = $(OBJ_C) $(OBJ_S)
TARGET = uart_print.elf
BIN = uart_print.bin

all: $(BIN)

$(BIN): $(TARGET)
	$(OBJCOPY) -O binary $< $@

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(COMMON_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(BIN)

.PHONY: all clean
