/*
 * Jake Mingolla
 * June 24th, 2016
 * Euler Problem 216
 *
 * --------------------
 *
 * Consider the problem of building a wall out of 2×1 and 3×1 bricks 
 * (horizontal×vertical dimensions) such that, for extra strength, 
 * the gaps between horizontally-adjacent bricks never line up in 
 * consecutive layers, i.e. never form a "running crack".
 *
 * There are eight ways of forming a crack-free 9×3 wall, written W(9,3) = 8.
 *
 * Calculate W(32,10).
 *
 * --------------------
 *
 * This program calculates W(32, 10) = 806,844,323,190,414 in 50 - 100 ms. This
 * solution is verified using other solutions online.
 *
 * To get this number, the program keeps track of a linked list of Node objects.
 * Each Node keeps track of a specific Row object as well as a dynamic array
 * of all rows that can stack ontop of it for every height layer.
 *
 * To begin, the program calculates all possible combinations of 2 and 3 length
 * blocks that can make a row and stores them in the linked list. Then,
 * we perform a O(n ^ 2) operation and find all the rows that can stack upon
 * all other rows and store that in the "stacks" dynamic array, expanding as
 * necessary. We cache whether or not two rows stack ontop of eachother
 * in order to ignore all impossible rows during computation.
 *
 * To find the final solution, we build all possible combinations of the walls
 * and cache each row's number of solutions stacked atop it for each layer. This
 * heavily impacts the runtime, as the number of solutions for a given row
 * at a given layer in the wall will always be the same and does not need
 * to be recalcuated. Finally, we return the result and destroy all dynamically
 * allocated memory.
*/


#include "Row.h"
#include "Node.h"
#include "stdlib.h"


/*
 * Generate all possible rows and store them in the linked list specfied
 * by the pointer to a pointer to a Node object. At each step, we have
 * a currently building Row object as well as the length of the Row we
 * are at right now.
 */
void generate_rows(Node **head, Row current, int len) {
        // If we have created a row that is too far over the wall,
        // we discard it as it is by definition invalid.
        if (len > WALL_WIDTH) return;

        // If we are exactly at the wall width, we create a new Node
        // object and append it to the head of the linked list.
        else if (len == WALL_WIDTH) {
                Node *n = init_node(current);
                (*head) = append(*head, n);

        // Otherwise, we continue to generate rows using the blocks
        // of size 2 or 3.
        } else {
                generate_rows(head, add_crack(current, len + 2), len + 2);
                generate_rows(head, add_crack(current, len + 3), len + 3);
        }
}

/*
 * Generate all possible combinations of the Rows specified at
 * the linked list Node pointer head and stores the stackability
 * records in each respective Node.
 */
void generate_stacks(Node *head) {

        // Outer iterator, continues until it reaches the end of the
        // linked list.
        Node *iter_1 = head;
        while (iter_1 != NULL) {
                
                // Inner iterator, continues until it reaches the end
                // of the linked list. Starts at the outer iterator's
                // next pointer, which we know cannot by NULL since the
                // outer iterator cannot be NULL inside the loop.
                Node *iter_2 = iter_1->next;
                while(iter_2 != NULL) {

                        // Extract the Row data from each iterator.
                        Row data_1 = iter_1->data;
                        Row data_2 = iter_2->data;

                        // If the two Rows are stackable,
                        // add a stackability record to the Nodes specified
                        // by each iterator.
                        if (stackable(data_1, data_2)) {
                                add_stack(iter_1, iter_2);
                                add_stack(iter_2, iter_1);
                        }

                        // Continue the inner iterator.
                        iter_2 = iter_2->next;
                }
                
                // Continue the outer iterator.
                iter_1 = iter_1->next;
        }
}

/*
 * Returns the number of solutions that can be generated from
 * the current Node if we are at the row specified (starting at 0
 * for the base row). Returns an unsigned long long datatype to 
 * avoid overflow.
 *
 * TODO: Add an arbitrarily large data type to avoid overflow.
 */
unsigned long long build_wall(Node *current, int row) {
        // If we are at the penultimate row of the wall (since we 
        // start at 0), there can be only one more row stacked ontop
        // of us, and we know how many there are based on the
        // size of the stackability records dynamic array.
        if (row == (WALL_HEIGHT - 2)) {
                return current->size; 

        // And, if we have already cached the number of solutions
        // at this row, return the number of solutions previously 
        // calculated.
        } else if (current->solutions[row] != SENTINEL) {
                return current->solutions[row];

        // Otherwise, we continue to calculate the number of solutions
        // at this row through summing the recurisve calls to the
        // build_wall function through all of the stackability
        // record's Nodes.
        } else {
                unsigned long long count = 0;
                int i;
                int len = current->size;
                for (i = 0; i < len; i++) {
                        count += build_wall(current->stacks[i], row+1);
                }
                current->solutions[row] = count;
                return count;
        }
}

/*
 * Given a linked list specified by the Node pointed to by head,
 * returns the total number of solutions by iterating through all
 * Nodes and summing the number of solutions as if the Row object
 * associated with each Node is the base.
 */
unsigned long long generate_solutions(Node *head) {

        unsigned long long solutions = 0;
        // Iterate through all Nodes until we reach the end of the
        // linked list.
        Node *iter = head;
        while (iter != NULL) {
                // Use this Node as the base of the wall and
                // add it to the sum.
                solutions += build_wall(iter, 0);
                iter = iter->next;
        }

        return solutions;
}

/*
 * Destroys all allocated dynamic memory associated with
 * the linked list specified by the Node pointer head.
 */
void destroy(Node *head) {
        Node *trail = head;
        Node *iter = head;

        // Uses a trailing pointer to destroy each node and its
        // dynamic array of solutions and stackability records
        // untik all Nodes have been deleted.
        while (iter != NULL) {
                free(iter->stacks);
                free(iter->solutions);

                trail = iter;
                iter = iter->next;

                free(trail);
        }
}

/*
 * Handles the correct ordering of all methods and prints the result to
 * standard out.
 */
int main(int argc, char* argv[]) {
        Node *head = NULL;
        Row current = init_row();

        generate_rows(&head, current, 0);
        generate_stacks(head);
        unsigned long long solutions = generate_solutions(head);

        fprintf(stdout, "number of solutions = %llu\n", solutions);
        destroy(head);

        return 0;
}
