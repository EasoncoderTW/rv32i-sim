CC=../riscv-toolchain/bin/riscv32-unknown-elf-gcc
LD=../riscv-toolchain/bin/riscv32-unknown-elf-ld
SIMULATOR=./bin/rv32i-sim
MEMORY_SIZE=4K

SRC_ASM=../example/memory/test1.S
SRC_LINKER=../example/memory/test1.ld

# compile assembly to object file
OBJ=../test1.o
$CC -c $SRC_ASM -o $OBJ
# link object file to create executable
$LD -T $SRC_LINKER $OBJ -o test1.elf
# run the simulator with the executable
$SIMULATOR -M $MEMORY_SIZE test1.elf
