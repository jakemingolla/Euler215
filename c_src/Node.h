
#ifndef __NODE_H__
#define __NODE_H__

#include "Row.h"

#define BASE_CAPACITY 64

typedef struct Node {
                int size;
                int capacity;
                Row data;
                struct Node *next;
                struct Node **stacks;

} Node;

Node *init_node(Row data);

int len(Node *head);

Node *append(Node *head, Node *n);

void add_stack(Node *current, Node *stack);
void expand(Node *current);
int stack_len(Node *current);

void print_node(Node *head);

#endif

