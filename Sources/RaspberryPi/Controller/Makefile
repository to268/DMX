CC?=gcc

# Basic CFLAGS
CFLAGS_BASE=-Wall -Wextra -std=c17 -I include/ \
			-fomit-frame-pointer -fstack-protector-strong

# Normal CFLAGS
CFLAGS=$(CFLAGS_BASE) -Og -g3 -D DEBUG
# Debug CFLAGS
CFLAGS_RELEASE=$(CFLAGS_BASE) -O2

LDFLAGS=
#LDFLAGS=-lusb-1.0 -lpthread

BIN=controller

C_FILES=$(shell find ./src -type f -name "*.c")
OBJS=$(C_FILES:.c=.o)

TEST_FILES=$(wildcard tests/*.c)
TEST_OBJS=$(TEST_FILES:.c=.o)

all: debug

debug: $(BIN)

release: $(BIN) check
	CFLAGS=$(CFLAGS_RELEASE)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS)

$(OBJS): $(C_FILES)
	$(CC) $(CFLAGS) -c $*.c -o $@

check: $(TEST_BINS)
	sh tests/test.sh $(CC)

clean:
	rm -f $(OBJS) $(TEST_OBJS)

mrproper: clean
	rm -f $(BIN)

.PHONY: all debug release check clean mrproper
