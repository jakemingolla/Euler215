#include "Row.h"
#include "Node.h"

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

int main(int argc, char* argv[]) {
        Node *head = NULL;
        Row current = init_row();
        generate_rows(&head, current, 0);
        fprintf(stdout, "number of rows = %d\n", len(head));
        fprintf(stdout, "generating stacks...\n");
        int combinations = generate_stacks(head);
        fprintf(stdout, "Done generating stacks\n");
        fprintf(stdout, "number of combos = %d\n", combinations);

        Node *iter = head;
        while (iter != NULL) {
                fprintf(stdout, "found row: ");
                print_row(stdout, iter->data);
                int h = iter->size;
                fprintf(stdout, "it can stack with %d rows\n", h);
                int i;
                for (i = 0; i < h; i++) {
                        print_row(stdout, iter->stacks[i]->data);
                }
                fprintf(stdout, "\n");
                iter = iter->next;
        }
        return 0;
}
