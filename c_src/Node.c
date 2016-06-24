#include "Node.h"
#include "stdlib.h"
#include "stdio.h"

Node *init_node(Row data) {
    Node *n = (Node *)malloc(sizeof(struct Node));
    n->data = data;
    n->next = NULL;
    n->stacks = (Node **)malloc(sizeof(Node *) * BASE_CAPACITY);
    n->capacity = BASE_CAPACITY;
    n->size = 0;
    n->solutions = (unsigned long long *)malloc(sizeof(unsigned long long) * WALL_HEIGHT);
    int i;
    for(i = 0; i < WALL_HEIGHT; i++) {
            n->solutions[i] = SENTINEL;
    }
    return n;
}

Node *append(Node *head, Node *n) {
    n->next = head;
    return n;
}

void add_stack(Node *current, Node *stack) {
        if (current->size >= current->capacity) {
                expand(current);
        }
        current->stacks[current->size] = stack;
        current->size++;
}

void expand(Node *current) {
        int old_capacity = current->capacity;
        Node **copy = (Node **)malloc(sizeof(Node *) * old_capacity * 2);
        int i;
        for (i = 0; i < old_capacity; i++) {
                copy[i] = current->stacks[i];
        }
        free(current->stacks);
        current->stacks = copy;
        current->capacity = old_capacity * 2;
}

int stack_len(Node *current) {
        return current->size;
}

int len(Node *head) {
    Node *cur = head;
    int count = 0;
    while (cur != NULL) {
        count++;
        cur = cur->next;
    }
    return count;
}

void print_node(Node *head) {
    Node *cur = head;
    while (cur != NULL) {
        print_row(stdout, cur->data);
        cur = cur->next;
    }
}
