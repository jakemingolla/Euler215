#include "Row.h"
#include "Node.h"
#include "stdlib.h"

#include "time.h"

void generate_rows(Node **head, Row current, int len) {
        if (len > WALL_WIDTH) return;
        else if (len == WALL_WIDTH) {
                Node *n = init_node(current);
                (*head) = append(*head, n);
        } else {
                generate_rows(head, add_crack(current, len + 2), len + 2);
                generate_rows(head, add_crack(current, len + 3), len + 3);
        }
}

int generate_stacks(Node *head) {
        int combinations = 0;
        Node *iter_1 = head;
        while (iter_1 != NULL) {
                Node *iter_2 = iter_1->next;
                while(iter_2 != NULL) {
                        Row data_1 = iter_1->data;
                        Row data_2 = iter_2->data;
                        if (stackable(data_1, data_2)) {
                                combinations++;
                                add_stack(iter_1, iter_2);
                                add_stack(iter_2, iter_1);
                        }
                        iter_2 = iter_2->next;
                }
                iter_1 = iter_1->next;
        }
        return combinations;
}

unsigned long long build_wall(Node *current, int row) {
        if (row == (WALL_HEIGHT - 2)) {
                return current->size; 
        } else if (current->solutions[row] != SENTINEL) {
                return current->solutions[row];
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

unsigned long long generate_solutions(Node *head) {
        unsigned long long solutions = 0;
        Node *iter = head;
        while (iter != NULL) {
                solutions += build_wall(iter, 0);
                iter = iter->next;
        }

        return solutions;
}

void destroy(Node *head) {
        Node *trail = head;
        Node *iter = head;

        while (iter != NULL) {
                free(iter->stacks);
                free(iter->solutions);
                trail = iter;
                iter = iter->next;
                free(trail);
        }
}

int main(int argc, char* argv[]) {
        clock_t before = clock();

        Node *head = NULL;
        Row current = init_row();

        generate_rows(&head, current, 0);
        generate_stacks(head);
        unsigned long long solutions = generate_solutions(head);

        fprintf(stdout, "number of solutions = %llu\n", solutions);
        destroy(head);

        clock_t after = clock();
        fprintf(stdout, "Elapsed: %f secondds\n", (double)(after - before) / CLOCKS_PER_SEC);
        return 0;
}
