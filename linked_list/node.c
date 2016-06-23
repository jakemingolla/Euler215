#include "stdio.h"
#include "stdlib.h"

typedef struct Node {
    void *data;
    struct Node *next;
} *Node;

Node new_node(void *data) {
    Node n = (Node)malloc(sizeof(struct Node));
    n->data = data;
    n->next = NULL;
    return n;
}

Node append(Node head, Node n) {
    n->next = head;
    return n;
}

void print(Node head) {
    Node cur = head;
    while (cur != NULL) {
        fprintf(stdout, "%ld\n", (long)cur->data);
        cur = cur->next;
    }
}

int main(int argc, char *argv[]) {
    fprintf(stdout, "Hello world!\n");
    Node n = new_node((void *)5);
    Node j = new_node((void *)4);

    Node ll = append(j, n);

    print(ll);
    return 0;
}

