CC = gcc
CFLAGS_DEBUG = -g -ggdb -std=c17 -pedantic -W -Wall -Wextra
CFLAGS_RELEASE = -std=c17 -pedantic -W -Wall -Wextra -Werror

SRC_DIR = src
BUILD_DIR = build
DEBUG_DIR = $(BUILD_DIR)/debug
RELEASE_DIR = $(BUILD_DIR)/release

SRC_FILES = $(SRC_DIR)/dirwalk.c $(SRC_DIR)/main.c
OBJ_FILES_DEBUG = $(patsubst $(SRC_DIR)/%.c, $(DEBUG_DIR)/%.o, $(SRC_FILES))
OBJ_FILES_RELEASE = $(patsubst $(SRC_DIR)/%.c, $(RELEASE_DIR)/%.o, $(SRC_FILES))
EXEC_NAME = dirwalk

all: debug release

debug: $(DEBUG_DIR)/$(EXEC_NAME)

release: $(RELEASE_DIR)/$(EXEC_NAME)

$(DEBUG_DIR)/$(EXEC_NAME): $(OBJ_FILES_DEBUG) | $(DEBUG_DIR)
	$(CC) $^ -o $@

$(RELEASE_DIR)/$(EXEC_NAME): $(OBJ_FILES_RELEASE) | $(RELEASE_DIR)
	$(CC) $^ -o $@

$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.c | $(DEBUG_DIR)
	$(CC) $(CFLAGS_DEBUG) -c $< -o $@

$(RELEASE_DIR)/%.o: $(SRC_DIR)/%.c | $(RELEASE_DIR)
	$(CC) $(CFLAGS_RELEASE) -c $< -o $@

$(DEBUG_DIR) $(RELEASE_DIR):
	@mkdir -p $@

clean:
	rm -fr $(BUILD_DIR)