#include "Row.h"
#include "Node.h"
#include "stdlib.h"

Row add_c(Row r, int index) {
        return r |= (1ULL << index);
}

int main(int argc, char *argv[]) {
        Row r = init_row();
        for (i = 0; i < 64; i++) {
                r = add_c(init_row(), i);
                printf("%lu\n", r);
        }
}
