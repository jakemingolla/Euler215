/*
 * Jake Mingolla
 * June 24th, 2016
 * Euler Problem 216
 *
 * --------------------
 *
 * Contains all of the implementation of the Node methods.
 */
 
#include "Node.h"
#include "stdlib.h"
#include "stdio.h"


/*
 * Initializes a Node given a specified Row.
 */
Node *init_node(Row data) {
        Node *n = (Node *)malloc(sizeof(struct Node));
        n->data = data;
        n->next = NULL;
        // Stacks starts at the initial capacity.
        n->stacks = (Node **)malloc(sizeof(Node *) * BASE_CAPACITY);
        n->capacity = BASE_CAPACITY;
        n->size = 0;
        n->solutions = (unsigned long long *)malloc(sizeof(unsigned long long) * WALL_HEIGHT);
        int i;
         
        // Initialize solutiosn cache to the sentinel value to
        // indicate we need to caclulate all levels
        for(i = 0; i < WALL_HEIGHT; i++) {
                 n->solutions[i] = SENTINEL;
        }
        return n;
}

/*
 * Appends a Node pointer n to the linked list specified
 * by the head Node pointer.
 */
Node *append(Node *head, Node *n) {
        n->next = head;
        return n;
}

/* 
 * Adds a stackability record to the given Node. If necessary,
 * the stackability records dynamic array will be resized.
 */
void add_stack(Node *current, Node *stack) {
        if (current->size >= current->capacity) {
                expand(current);
        }
        current->stacks[current->size] = stack;
        current->size++;
}

/*
 * Expands the given stackability records dynamic array and updates
 * the capacity as necessary. Performs a deep copy of the original
 * stackability records array into the new array, then deletes the
 * old one.
 */
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
