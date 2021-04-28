CFLAGS=-Wall -Wextra -O2 -std=c17 -I $(PWD)/include/

C_FILES=$(wildcard *.c)
OBJS=$(C_FILES:.c=.o)
BINS=$(C_FILES:.c=)

all: $(BINS) run

$(BINS): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS)

$(OBJS): $(C_FILES)
	$(CC) $(CFLAGS) -c $*.c -o $@

run:
	$(foreach BIN,$(BINS),./$(BIN);)

.PHONY: all run
