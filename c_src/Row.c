#include "Row.h"

Row add_crack(Row r, int index) {
    uint32_t mask = 1;
    Row crack = mask << index;
    return r |= crack;
}

Row init_row() {
    return 0;
}

bool stackable(Row first, Row second) {
    return !((first - 1) & (second - 1));
}

void print_row(FILE *f, Row r) {
    Row mask = 1;
    mask = mask << (WALL_WIDTH - 1);
    int i;
    for (i = 0; i < WALL_WIDTH; i++) {
        if (mask & r) fprintf(stdout, "1");
        else fprintf(f, "0");
        mask = mask >> 1;
    }
    fprintf(f, "\n");
}
