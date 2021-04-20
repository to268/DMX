#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

int main(int argc, char **argv) {
    debug("This line is only printed if it's a debug build\n");
    return EXIT_SUCCESS;
}
