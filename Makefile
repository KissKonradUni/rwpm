# RimWorld Packs Manager (RWPM) - C++ Rewrite
# Makefile for building with Clang++ and MSYS

# Compiler and flags
CXX = clang++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++23 -O2
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

# Default target
all: prepare $(EXEC)

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

.PHONY: all clean clean-all run prepare
