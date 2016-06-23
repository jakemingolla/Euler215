#ifndef _LIB_LINKED_LIST
#define _LIB_LINKED_LIST

#include <stdio.h>
#include <stdlib.h>

typedef struct List_node {
	void *node;
	struct List_node *nextnode;
} *list_node;

typedef struct _list_iterator {
	list_node next;
} *list_iterator;

typedef struct _linked_list {
	unsigned int listlen;
	list_node firstnode;
	list_node lastnode;
	int (*comparer)(const void*, const void*);
} *linked_list;

// comparer is function that returns 0 on equality
linked_list ll_init(int (*comparer)(const void*, const void*));

void ll_add(linked_list, void *);
void ll_add_front(linked_list, void *);
void ll_add_dedup(linked_list, void *);

unsigned int ll_listlen(linked_list);
int ll_contains(linked_list, void *);

list_iterator ll_iterate_new(linked_list);
void *ll_iterate(list_iterator);
list_iterator ll_iterate_copy(list_iterator);

void *ll_remove(linked_list, void *);
void *ll_pop(linked_list);

#define ITERATE(var, iter) while (var = ll_iterate(iter))

list_node ll_new_node(void *newitem) {
	list_node newnode = (list_node)
	       malloc(sizeof (struct List_node));
	if (newnode == NULL) {
		fprintf(stderr, "ll_new_node: Malloc failed\n");
		exit(EXIT_FAILURE);
	}
	newnode->node = newitem;
	newnode->nextnode = NULL;
	return newnode;
}	

linked_list ll_init(int (*comparer)(const void*, const void*)) {
	linked_list newll = (linked_list) 
		malloc(sizeof (struct _linked_list));
	newll->listlen = 0;
	newll->firstnode = NULL;
	newll->lastnode = NULL;
	newll->comparer = comparer;
	return newll;
}

// Adds new item to back of list
void ll_add(linked_list ll, void *newitem) {
	list_node newnode = ll_new_node(newitem);

	if (ll->listlen == 0) {
		ll->listlen = 1;
		ll->firstnode = newnode;
		ll->lastnode = newnode;
		return;
	}

	ll->listlen++;
	ll->lastnode->nextnode = newnode;
	ll->lastnode = newnode;
	return;
}

void ll_add_front(linked_list ll, void *newitem) {
	list_node newnode = ll_new_node(newitem);

	if (ll->listlen == 0) {
		ll->listlen = 1;
		ll->firstnode = newnode;
		ll->lastnode = newnode;
		return;
	}

	ll->listlen++;
	newnode->nextnode = ll->firstnode;
	ll->firstnode = newnode;
	return;
}

void ll_add_dedup(linked_list ll, void *newitem) {
	list_iterator it = ll_iterate_new(ll);
	void *current;

	ITERATE(current, it) {
		if ((ll->comparer)(current, newitem)) {
			free(it);
			return;
		}
	}
	ll_add(ll, newitem);
	free(it);
	return;
}

unsigned int ll_listlen(linked_list ll) {
	return ll->listlen;
}

int ll_contains(linked_list ll, void *item) {
	list_iterator it = ll_iterate_new(ll);
	void *current;

	ITERATE(current, it) {
		if ((ll->comparer)(current,item)==0) {
			// It's in the list
			free(it);
			return 1;
		}
	}
	free(it);
	return 0;
}

list_iterator ll_iterate_new(linked_list ll){
	list_iterator newit = (list_iterator)
		malloc(sizeof(struct _list_iterator));
	newit->next = ll->firstnode;
	return;
}

// MUST CALL ll_iterate_reset BEFORE USE!!!
// Will return NULL when list is exausted
void *ll_iterate(list_iterator it) {
	if (it->next == NULL) {
		return NULL;
	}
	void *nextitem = it->next->node;
	it->next = it->next->nextnode;
	return nextitem;
}

list_iterator ll_iterate_copy(list_iterator oldit) {
	list_iterator newit = (list_iterator)
		malloc(sizeof(struct _list_iterator));
	newit->next = oldit->next;
	return newit;
}

void *ll_remove(linked_list ll, void *olditem) {
	////////////////////////////////////////////
	fprintf(stderr, "ll_remove() not implemented yet\n");
	exit(EXIT_FAILURE);
	return NULL;
}

void *ll_pop(linked_list ll) {
	if (ll->firstnode == NULL) {
		return NULL;
	}
	void *olditem = ll->firstnode->node;
	list_node oldfirst = ll->firstnode;
	ll->firstnode = ll->firstnode->nextnode;
	free (oldfirst);
	return olditem;
}

#endif
