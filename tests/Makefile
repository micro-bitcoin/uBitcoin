# Paths
BUILD_DIR = build
SRC_DIR = .
# uBitcoin library
LIB_DIR = ../src

# Tools
ifeq ($(OS),Windows_NT)
TOOLCHAIN_PREFIX ?= x86_64-w64-mingw32-
MKDIR_P = mkdir
RM_R = rmdir /s /q
else
TOOLCHAIN_PREFIX ?= 
MKDIR_P = mkdir -p
RM_R = rm -r
endif

# compilers
CC := $(TOOLCHAIN_PREFIX)gcc
CXX := $(TOOLCHAIN_PREFIX)g++

# uBitcoin sources
CXX_SOURCES=$(wildcard $(LIB_DIR)/*.cpp)
C_SOURCES=$(wildcard $(LIB_DIR)/utility/trezor/*.c)
C_SOURCES+=$(wildcard $(LIB_DIR)/utility/*.c)

# include lib path, don't use mbed or arduino config (-DUSE_STDONLY)
CFLAGS=-I$(LIB_DIR) -DUSE_STDONLY

OBJS=$(patsubst $(LIB_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
OBJS+=$(patsubst $(LIB_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CXX_SOURCES))

TESTS=$(wildcard $(SRC_DIR)/*.cpp)
TESTBINS=$(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%, $(TESTS))


.PHONY: clean all run

all: $(TESTBINS)

run: $(TESTBINS)
	for test in $(TESTBINS); do echo $$test; ./$$test ; done

clean:
	$(RM_R) $(BUILD_DIR)

# c source
$(BUILD_DIR)/%.o: $(LIB_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

# cpp source
$(BUILD_DIR)/%.o: $(LIB_DIR)/%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) -c $(CFLAGS) $< -o $@

# cpp source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%: $(SRC_DIR)/%.cpp $(OBJS)
	$(CXX) $< $(OBJS) $(CFLAGS) -o $@
