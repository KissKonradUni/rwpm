# RimWorld Packs Manager (RWPM) - C++ Rewrite
# Makefile for building with Clang++ and MSYS

# Compiler and flags
CXX = clang++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++23
LDFLAGS = 

# Include directories
INCLUDES = -I./include

# Libraries
LIBS =

# Source and object files
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
FILE = rwpm
EXEC = $(BIN_DIR)/$(FILE)

# Build modes
RELEASE_FLAGS = -O2
DEBUG_FLAGS = -g -O0

# Static link stdlib for release
RELEASE_LDFLAGS = -static-libstdc++ -static-libgcc
DEBUG_LDFLAGS =

# Default target: release
all: release

release: CXXFLAGS += $(RELEASE_FLAGS)
release: LDFLAGS += $(RELEASE_LDFLAGS)
release: prepare $(EXEC)

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: LDFLAGS += $(DEBUG_LDFLAGS)
debug: prepare $(EXEC)

# Prepare directories
prepare:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Linking
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

# Compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean project
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Clean everything including libraries
clean-all: clean
	rm -rf lib

# Run the executable
run: all
	cd $(BIN_DIR) && ./$(FILE)

.PHONY: all clean clean-all run prepare release debug
