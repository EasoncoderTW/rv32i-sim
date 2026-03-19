TOOLCHAIN_BIN = /home/yoyo/work/Code/bereise/ISS/rv32i-sim/riscv-toolchain/bin
CC = $(TOOLCHAIN_BIN)/riscv32-unknown-elf-gcc
LD = $(TOOLCHAIN_BIN)/riscv32-unknown-elf-ld
OBJCOPY = $(TOOLCHAIN_BIN)/riscv32-unknown-elf-objcopy

CFLAGS = -march=rv32i -mabi=ilp32 -nostdlib -nostartfiles
LDFLAGS = -T linker.ld

SRC_C = $(wildcard *.c)
SRC_S = $(wildcard *.S)
SOURCES = $(SRC_C) $(SRC_S)
OBJ_C = $(SRC_C:.c=.o)
OBJ_S = $(SRC_S:.S=.o)
OBJECTS = $(OBJ_C) $(OBJ_S)
TARGET = helloworld.elf
BIN = helloworld.bin

all: $(BIN)

$(BIN): $(TARGET)
	$(OBJCOPY) -O binary $< $@

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(BIN)

.PHONY: all clean
