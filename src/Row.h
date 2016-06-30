/*
 * Jake Mingolla
 * June 24th, 2016
 * Euler Problem 216
 *
 * --------------------
 *
 * Contains all of the information necessary to represent
 * a Row of the brick wall for the Euler 216 problem.
 *
 * Because of the convience of a 32 foot long wall, we abstract
 * an unsigned 32-bit integer to represent the wall. A 1 in
 * the bitmap represents a crack, and a 0 a space occupied by a brick.
 *
 */

#ifndef __ROW_H__
#define __ROW_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Default width and heights of the brick wall.
#define WALL_WIDTH 32
#define WALL_HEIGHT 10
typedef uint64_t Row;

Row add_crack(Row r, int index);
bool stackable(Row first, Row second);
Row init_row();


#endif

