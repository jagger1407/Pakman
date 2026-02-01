SRC_DIR = src
BIN_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.c)

CC = gcc

all: debug release

debug: CFLAGS = -g -O0
debug: $(BIN_DIR)/pakman_debug

release: CFLAGS = -O2
release: $(BIN_DIR)/pakman

$(BIN_DIR)/obj/debug/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)/obj/debug
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/obj/release/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)/obj/release
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/pakman_debug: $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/obj/debug/%.o,$(SOURCES)) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/pakman: $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/obj/release/%.o,$(SOURCES)) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR) $(BIN_DIR)/obj/debug $(BIN_DIR)/obj/release:
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR)/*

help:
	@echo "Available targets:"
	@echo "  make              - Build both debug and release"
	@echo "  make all          - Same as above"
	@echo "  make debug        - Build debug version"
	@echo "  make release      - Build release version"
	@echo "  make clean        - Remove all build files"
	@echo "  make help         - Show this message"

.PHONY: all debug release clean help
