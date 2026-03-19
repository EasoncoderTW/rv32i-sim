# Makefile for RV32I Simulator

# Compiler and tools
CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -g
LDFLAGS =

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Files
INCLUDES = -Iinclude
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = $(BIN_DIR)/rv32i-sim

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean