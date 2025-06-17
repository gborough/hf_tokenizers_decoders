CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -Iinclude -Itests/test_include -Ivendors/STC/include -Ivendors/utf8proc -Ivendors/oniguruma/src
LDFLAGS = -Lvendors/oniguruma/src/.libs -lonig

BUILD_DIR = .build

SRC_FILES := $(shell find src -name '*.c')
OBJ_FILES := $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# === Vendors Non-Header ===
UTF8PROC_SRC := vendors/utf8proc/utf8proc.c
UTF8PROC_OBJ := $(BUILD_DIR)/vendors/utf8proc/utf8proc.o
# === Vendors Non-Header end ===

TEST_SRCS := $(shell find tests -name '*.c')
TEST_OBJS := $(patsubst tests/%.c, $(BUILD_DIR)/tests/%.o, $(TEST_SRCS))
TEST_BIN := $(BUILD_DIR)/test_runner

.PHONY: all clean test oniguruma submodules

all: submodules oniguruma test

$(TEST_BIN): $(OBJ_FILES) $(UTF8PROC_OBJ) $(TEST_OBJS) 
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/tests/%.o: tests/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# === Vendors build ===
$(BUILD_DIR)/vendors/utf8proc/%.o: vendors/utf8proc/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

oniguruma:
	@if [ ! -d vendors/oniguruma/src/.libs ]; then \
		echo ">> Initializing oniguruma submodule..."; \
		git submodule update --init --recursive vendors/oniguruma; \
		cd vendors/oniguruma && autoreconf -fi && ./configure; \
	fi
	@echo ">> Building oniguruma..."
	$(MAKE) -C vendors/oniguruma
# === Vendors build end ===

submodules:
	@echo ">> Initializing git submodules..."
	@git submodule update --init --recursive

$(TEST_OBJ): $(TEST_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(TEST_BIN)
	@echo ">> Running tests..."
	@set -e; \
	output=`./$(TEST_BIN)`; \
	echo "$$output"; \
	echo "$$output" | grep -q "ok" || (echo "Test did not pass!" && exit 1)

clean:
	rm -rf $(BUILD_DIR)