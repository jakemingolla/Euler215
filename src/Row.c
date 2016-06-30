/*
 * Jake Mingolla
 * June 24th, 2016
 * Euler Problem 216
 *
 * --------------------
 *
 * Contains all of the implementation of the Row methods.
 */
 
 
#include "Row.h"

const uint64_t WALL_MASK = 1ULL << (WALL_WIDTH - 1);

/*
 * Adds a crack to the given row at the specified index.
 */
Row add_crack(Row r, int index) {
        return r |= (1ULL << index);
}

/*
 * Initializes a given row. Not too complicated.
 */
Row init_row() {
        return 0;
}

/*
 * Returns true if the given rows are stackable.
 *
 * Note that this can be done using a bit-wise AND
 * operation on the two Row's bitmaps minus the ending
 * 1 (which they all share as they are all exactly
 * WALL_WIDTH units long.
 */
bool stackable(Row first, Row second) {
        return !((first - WALL_MASK) & (second - WALL_MASK));
        /*
        return !((first - mask) & (second - mask));
        return !((first - 1) & (second - 1));
        return !((first - 1) & (second - 1));
        */
}
