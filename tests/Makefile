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
CXX_SOURCES += $(wildcard $(LIB_DIR)/*.cpp)
C_SOURCES += $(wildcard $(LIB_DIR)/utility/trezor/*.c) \
			$(wildcard $(LIB_DIR)/utility/*.c) \
			$(wildcard $(LIB_DIR)/*.c)

# include lib path, don't use mbed or arduino config (-DUSE_STDONLY)
CFLAGS = -I$(LIB_DIR)
CPPFLAGS = -I$(LIB_DIR) -DUSE_STDONLY

OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/src/%.o, \
		$(patsubst $(LIB_DIR)/%, $(BUILD_DIR)/lib/%.o, \
		$(C_SOURCES) $(CXX_SOURCES)))

vpath %.cpp $(SRC_DIR)
vpath %.cpp $(LIB_DIR)
vpath %.c $(LIB_DIR)

TESTS=$(wildcard $(SRC_DIR)/*.cpp)
TESTOBJS=$(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/test/%.o, $(TESTS))
TESTBINS=$(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.test, $(TESTS))


.PHONY: clean all run

all: $(TESTBINS)

run: $(TESTBINS)
	for test in $(TESTBINS); do echo $$test; ./$$test ; done

# lib c sources
$(BUILD_DIR)/lib/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

# lib cpp sources
$(BUILD_DIR)/lib/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) -c $(CPPFLAGS) $< -o $@

# test cpp sources
$(BUILD_DIR)/test/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) -c $(CPPFLAGS) $< -o $@

$(BUILD_DIR)/%.test: $(BUILD_DIR)/test/%.cpp.o $(OBJS)
	$(CXX) $< $(OBJS) $(CPPFLAGS) -o $@

clean:
	$(RM_R) $(BUILD_DIR)
