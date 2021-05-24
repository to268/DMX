CC?=gcc

# Basic CFLAGS
CFLAGS_BASE=-Wall -std=c17 -I include/ \
			-fomit-frame-pointer -fstack-protector-strong

# Normal CFLAGS
CFLAGS=$(CFLAGS_BASE) -Og -g3
# Debug CFLAGS
CFLAGS_RELEASE=$(CFLAGS_BASE) -Werror -fanalyzer -Wno-stringop-truncation -O2 -DRELEASE

LDFLAGS=-lpthread -lpigpio
BUILD_TYPE=Debug
BIN=controller

C_FILES=$(shell find ./src -type f -name "*.c")
OBJS=$(C_FILES:.c=.o)

all: debug

debug: $(BIN)

$(BIN): $(OBJS)
	echo "   LD        $(BIN)"
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS)

$(OBJS): $(C_FILES)
	echo "   CC        $*.o"
ifeq ($(BUILD_TYPE),Release)
	$(CC) $(CFLAGS_RELEASE) -c $*.c -o $@
else
	$(CC) $(CFLAGS) -c $*.c -o $@
endif

clean:
	echo "   RM        *.o"
	rm -f $(OBJS)

mrproper: clean
	echo "   RM        $(BIN)"
	rm -f $(BIN)

.SILENT:
.PHONY: all debug clean mrproper
