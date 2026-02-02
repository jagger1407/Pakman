SRC_DIR = src

BIN_DIR = build/linux
WBIN_DIR = build/win

SOURCES = $(wildcard $(SRC_DIR)/*.c)

OBJ_DEBUG = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/obj/debug/%.o,$(SOURCES))
OBJ_REL = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/obj/release/%.o,$(SOURCES))
WINOBJ_DEBUG = $(patsubst $(SRC_DIR)/%.c,$(WBIN_DIR)/obj/debug/%.o,$(SOURCES))
WINOBJ_REL = $(patsubst $(SRC_DIR)/%.c,$(WBIN_DIR)/obj/release/%.o,$(SOURCES))

CC = gcc
MINGWCC = x86_64-w64-mingw32-gcc

all: debug release
winall: windebug winrelease

debug: CFLAGS = -g -O0
debug: $(BIN_DIR)/pakman_debug

windebug: CFLAGS = -g -O0
windebug: $(WBIN_DIR)/pakman_debug

release: CFLAGS = -O2
release: $(BIN_DIR)/pakman

winrelease: CFLAGS = -O2
winrelease: $(WBIN_DIR)/pakman

$(BIN_DIR)/obj/debug/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)/obj/debug
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/obj/release/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)/obj/release
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/pakman_debug: $(OBJ_DEBUG) | $(BIN_DIR)
	@echo "Linking final executable..."
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "Program '$@' created."

$(BIN_DIR)/pakman: $(OBJ_REL) | $(BIN_DIR)
	@echo "Linking final executable..."
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "Program '$@' created."

$(WBIN_DIR)/obj/debug/%.o: $(SRC_DIR)/%.c | $(WBIN_DIR)/obj/debug
	@echo "Compiling $@..."
	@$(MINGWCC) $(CFLAGS) -c $< -o $@

$(WBIN_DIR)/obj/release/%.o: $(SRC_DIR)/%.c | $(WBIN_DIR)/obj/release
	@echo "Compiling $@..."
	@$(MINGWCC) $(CFLAGS) -c $< -o $@

$(WBIN_DIR)/pakman_debug: $(WINOBJ_DEBUG) | $(WBIN_DIR)
	@echo "Linking final executable..."
	@$(MINGWCC) $(CFLAGS) $^ -o $@
	@echo "Program '$@.exe' created."

$(WBIN_DIR)/pakman: $(WINOBJ_REL) | $(WBIN_DIR)
	@echo "Linking final executable..."
	@$(MINGWCC) $(CFLAGS) $^ -o $@
	@echo "Program '$@.exe' created."


$(BIN_DIR) $(BIN_DIR)/obj/debug $(BIN_DIR)/obj/release:
	@mkdir -p $@

$(WBIN_DIR) $(WBIN_DIR)/obj/debug $(WBIN_DIR)/obj/release:
	@mkdir -p $@

clean:
	@rm -rf $(BIN_DIR)/*
	@rm -rf $(WBIN_DIR)/*

help:
	@echo "Available targets:"
	@echo "  make              - Build both debug and release"
	@echo "  make all          - Same as above"
	@echo "  make debug        - Build debug version"
	@echo "  make release      - Build release version"
	@echo "  make winall       - Build both debug and release (Windows Version)"
	@echo "  make windebug     - Build debug version (Windows Version)"
	@echo "  make winrelease   - Build release version (Windows Version)"
	@echo "  make clean        - Remove all build files"
	@echo "  make help         - Show this message"

.PHONY: all debug release winall windebug winrelease clean help
