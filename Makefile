CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g
LDFLAGS =
BUILD_DIR = build
SRC_DIR = src
OBJ_DIR = $(BUILD_DIR)/obj
BIN = $(BUILD_DIR)/animation
ANI_SRC = $(SRC_DIR)/animation.ani
ANI_DST = $(BUILD_DIR)/animation.ani

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

.PHONY: all clean run

all: $(BIN)

$(BIN): $(OBJECTS)
	@mkdir -p $(dir $@)
	cp $(ANI_SRC) $(ANI_DST)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(BIN)

clean:
	rm -rf $(BUILD_DIR)
