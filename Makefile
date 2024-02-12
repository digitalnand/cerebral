CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic -Werror -O2

SRC_DIR := src
BIN_DIR := bin
INCLUDE_DIR := include

SRCS := $(SRC_DIR)/main.cpp $(SRC_DIR)/cerebral.cpp
TARGET := $(BIN_DIR)/crbl

all: $(TARGET)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(TARGET): $(SRCS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) $^ -o $@

clean:
	@rm -rf $(BIN_DIR)

.PHONY: all clean
