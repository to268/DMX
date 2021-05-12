CC?=gcc

# Basic CFLAGS
CFLAGS_BASE=-Wall -std=c17 -I include/ \
			-fomit-frame-pointer -fstack-protector-strong

# Normal CFLAGS
CFLAGS=$(CFLAGS_BASE) -Og -g3
# Debug CFLAGS
CFLAGS_RELEASE=$(CFLAGS_BASE) -O2

LDFLAGS=-lpthread
BIN=controller

C_FILES=$(shell find ./src -type f -name "*.c")
OBJS=$(C_FILES:.c=.o)

all: debug

debug: $(BIN)

release: $(BIN) check
	CFLAGS=$(CFLAGS_RELEASE)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS)

$(OBJS): $(C_FILES)
	$(CC) $(CFLAGS) -c $*.c -o $@

clean:
	rm -f $(OBJS)

mrproper: clean
	rm -f $(BIN)

.PHONY: all debug release clean mrproper
