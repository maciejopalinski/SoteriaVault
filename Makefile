TARGET := ./bin/soteria-vault.o
CC := gcc
CFLAGS := -DSQLITE_ENABLE_DESERIALIZE
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wno-unused-parameter -Wno-unused-result -g
LDFLAGS := -lncurses -lcdk -lcrypto -lpthread -ldl

SRC_DIR := ./src
BIN_DIR := ./bin
OBJ_DIR := ./obj

C_SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.c')
C_OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SRC_FILES))
CPP_SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.cpp')
CPP_OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CPP_SRC_FILES))

$(TARGET): $(CPP_OBJ_FILES) $(C_OBJ_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(TARGET)

run: $(TARGET)
	$(TARGET)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)