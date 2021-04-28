CC?=gcc

# Basic CFLAGS
CFLAGS_BASE=-Wall -I include/ \
			-fomit-frame-pointer -fstack-protector-strong

# Normal CFLAGS
CFLAGS=$(CFLAGS_BASE) -Og -g3
# Debug CFLAGS
CFLAGS_RELEASE=$(CFLAGS_BASE) -O2

LDFLAGS=-lusb-1.0 -lpthread
BIN=controller

C_FILES=$(shell find ./src -type f -name "*.c")
OBJS=$(C_FILES:.c=.o)

# Tests files
TESTS_FILES=$(wildcard tests/*.c)
TESTS_OBJS=$(TEST_FILES:.c=.o)
TESTS_BINS=$(TEST_FILES:.c=)

all: debug

debug: $(BIN)

release: $(BIN) check
	CFLAGS=$(CFLAGS_RELEASE)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS)

$(OBJS): $(C_FILES)
	$(CC) $(CFLAGS) -c $*.c -o $@

check: $(TESTS_BINS)
	$(MAKE) -C tests/

clean:
	rm -f $(OBJS) $(TESTS_OBJS) $(TEST_BINS)

mrproper: clean
	rm -f $(BIN)

.PHONY: all debug release check clean mrproper
