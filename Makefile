# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -g -fPIC -I$(INC_DIR) 

# Directories
BUILD_DIR = build
SRC_DIR = src/controlleur
TST_DIR = test


# Include Directory
INC_DIR = $(SRC_DIR)

# Output Executable
MAIN = $(BUILD_DIR)/main

# Source and Header Files
SRCS := $(wildcard $(SRC_DIR)/*.c) 
DEPS := $(wildcard $(SRC_DIR)/*.h)	

# Object Files from sources
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Test Sources & Executables
TEST_SRCS := $(wildcard $(TST_DIR)/*.c)
TEST_OBJS := $(patsubst $(TST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SRCS))
TEST_EXECUTABLES := $(patsubst $(TST_DIR)/%.c, $(BUILD_DIR)/%, $(TEST_SRCS))

.PHONY: all clean tests

# ====== Default build ======
all: $(MAIN)

# ====== Main executable ======
$(MAIN): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) -pthread $(CFLAGS)  $^ -o $@

# ====== Build test executables ======
tests: $(TEST_EXECUTABLES)

$(BUILD_DIR)/%: $(BUILD_DIR)/%.o $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS)  $^ -o $@

# ====== Compile object files from source ======
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@mkdir -p $(BUILD_DIR)
	$(CC) -pthread $(CFLAGS)  -c $< -o $@

# ====== Compile object files from tests ======
$(BUILD_DIR)/%.o: $(TST_DIR)/%.c $(DEPS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ====== Clean build directory ======
clean:
	rm -rf $(BUILD_DIR)
