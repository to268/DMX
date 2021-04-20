#!/bin/sh

CC="$1"
CFLAGS="-Wall -Wextra -std=c17 -I $(pwd)/include/"
LDFLAGS=
#LDFLAGS=-lusb-1.0 -lpthread

SRC_FILES=($(find -name "*.c"))
OBJS_FILES=($(find -name "*.c" | sed 's/.c/.o/g'))
BINS=($(find -name "*.c" | cut -d'.' -f1))

# Ensure to be in the test directory
cd tests/

# Make objs
for i in "${!SRC_FILES[@]}"
do
    $CC $CFLAGS -c "${SRC_FILES[$i]}" -o "${OBJS_FILES[$i]}"
done

# Make bins
for i in "${!OBJS_FILES[@]}"
do
    $CC $CFLAGS -c "${SRC_FILES[$i]}" -o "${OBJS_FILES[$i]}"
	$CC -o "${BIN[$i]}" "${OBJS[$i]}" $LDFLAGS
done

# Execute them
for i in "${BINS[@]}"
do
    ./$i
done
