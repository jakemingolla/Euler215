/*
 * Jake Mingolla
 * June 24th, 2016
 * Euler Problem 216
 *
 * --------------------
 *
 * Contains all of the information necessary for a Node within the
 * Euler 216 problem. The Node's internals consist of:
 *
 *      size            - Stores the number of stackability records within
 *                        the dynamic array.
 *
 *      capacity        - Stores the current maximum capacity of the
 *                        stackability record dynamic array.
 *
 *      data            - Stores the Row object associated with this Node.
 *
 *      next            - Stores a pointer to the next Node within the linked
 *                        list of all Nodes. May be NULL to indicate the
 *                        terminal end of the linked list.
 *
 *      solutions       - Stores an array of the cached solutions that
 *                        represents the number of solutions using the Row
 *                        associated with this row at the indicated level.
 *
 *      stacks          - Stores a dynamic array of pointers to Node objects
 *                        that the Row associated with this Node can stack
 *                        upon.
 *
 * To try and minimize the segmentation of the struct across memory blocks,
 * the stackability dynamic array is placed at the end of the struct. This attempts
 * to minimize performance-reducing memory allignment issues of the struct.
 */

#ifndef __NODE_H__
#define __NODE_H__

#include "Row.h"

// Used to indicate the base size of the dynamic array of the stackability
// records.
#define BASE_CAPACITY 64

// The value that represents an empty cache record. It is impossible for there
// to be a negative number of solutions at a given level of the wall.
#define SENTINEL -1

typedef struct Node {
        int size;
        int capacity;
        Row data;
        struct Node *next;
        unsigned long long *solutions;
        struct Node **stacks;

} Node;

Node *init_node(Row data);

Node *append(Node *head, Node *n);

void add_stack(Node *current, Node *stack);
void expand(Node *current);

#endif

