#include "Row.h"

typedef struct Node {
    Row data;
    struct Node *next;
} *Node;

Node init_node(Row data);

int len(Node head);

Node append(Node head, Node n);

void print_node(Node head);
