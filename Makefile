# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -g -fPIC -I$(INC_DIR)

# Directories
BUILD_DIR = build
SRC_DIR = src/controlleur
TST_DIR = test

# Include Directory
INC_DIR = $(SRC_DIR)

# Source and Header Files
SRCS := $(wildcard $(SRC_DIR)/*.c)
DEPS := $(wildcard $(SRC_DIR)/*.h)

# Object Files
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Test Files
TEST_SRCS := $(wildcard $(TST_DIR)/*.c)
TEST_OBJS := $(patsubst $(TST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SRCS))
TEST_EXECUTABLES := $(patsubst $(TST_DIR)/%.c, $(BUILD_DIR)/%, $(TEST_SRCS))



.PHONY: all clean

# Default Target
all: $(TEST_EXECUTABLES)


# Test Executables
$(BUILD_DIR)/%: $(BUILD_DIR)/%.o $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Object Files for Source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Object Files for Tests
$(BUILD_DIR)/%.o: $(TST_DIR)/%.c $(DEPS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean Up
clean:
	rm -rf $(BUILD_DIR)
