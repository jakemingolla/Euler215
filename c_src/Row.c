#include "Row.h"

Row add_crack(Row r, int index) {
        return r |= (1 << index);
}

Row init_row() {
        return 0;
}

bool stackable(Row first, Row second) {
        return !((first - 1) & (second - 1));
}
