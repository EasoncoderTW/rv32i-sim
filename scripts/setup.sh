# 1. download and install riscv toolchain
PROJECT_ROOT=$(pwd)

RISCV_TOOLCHAIN_URL="https://github.com/riscv-collab/riscv-gnu-toolchain.git"

if [ ! -d "$PROJECT_ROOT/riscv-gnu-toolchain" ]; then
    git clone $RISCV_TOOLCHAIN_URL
    cd riscv-gnu-toolchain
    ./configure --prefix=$PROJECT_ROOT/riscv-toolchain --with-arch=rv32i --with-abi=ilp32
    make -j$(nproc)
    cd $PROJECT_ROOT
else
    echo "riscv-gnu-toolchain already exists, skipping download and installation."
fi