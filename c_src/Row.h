#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define WALL_WIDTH 32
typedef uint32_t Row;

void print_row(FILE *stream, Row r);

Row add_crack(Row r, int index);
bool stackable(Row first, Row second);
Row init_row();


