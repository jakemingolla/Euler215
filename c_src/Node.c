#include "Node.h"
#include "stdlib.h"
#include "stdio.h"

Node init_node(Row data) {
    Node n = (Node)malloc(sizeof(struct Node));
    n->data = data;
    n->next = NULL;
    return n;
}

Node append(Node head, Node n) {
    n->next = head;
    return n;
}

void print_node(Node head) {
    Node cur = head;
    while (cur != NULL) {
        print_row(stdout, cur->data);
        cur = cur->next;
    }
}
