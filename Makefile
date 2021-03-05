TARGET := ./bin/soteria-vault.o
CC := g++
CXXFLAGS := -std=c++11 -g
LDFLAGS := -lncurses -lcdk -lcrypto

SRC_DIR := ./src
BIN_DIR := ./bin
OBJ_DIR := ./obj

SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

$(TARGET): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -c -o $@ $<
	
run: $(TARGET)
	$(TARGET)

.PHONY: clean
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)