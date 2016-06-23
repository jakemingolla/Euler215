#include "Row.h"
#include "Node.h"

int main(int argc, char* argv[]) {
    Row j = init_row();
    j = add_crack(j, 1);
    j = add_crack(j, 4);
    print_row(stdout, j);

    Row i = init_row();
    i = add_crack(i, 3);
    print_row(stdout, i);
    fprintf(stdout, "Stackable? = %d\n", stackable(j, i));

    Node n = init_node(j);
    Node m = init_node(i);
    Node head;
    head = append(head, n);
    head = append(head, m);
    print_node(head);
    print_node(head);

    return 0;
}


