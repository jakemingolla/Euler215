#ifndef __ROW_H__
#define __ROW_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define WALL_WIDTH 32
#define WALL_HEIGHT 10
typedef uint32_t Row;

Row add_crack(Row r, int index);
bool stackable(Row first, Row second);
Row init_row();


#endif

