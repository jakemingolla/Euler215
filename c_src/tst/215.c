// Crack free walls 32 wide by 10 high

#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

#define WALL_WIDE	32
#define WALL_TALL	10

#define MAX_CRACKS 	(WALL_WIDE/2)

typedef struct Wall_row {
	int cracks[MAX_CRACKS];
	long long wallsbuilt[WALL_TALL]; // caches results from build_wall()
	linked_list list;
} *wall_row;

wall_row new_wall_row(int[]);
void gen_rows(wall_row, int, int[], int);
int interlace_rows (wall_row allrows);
int can_stack(wall_row first, wall_row second);
long long build_wall(wall_row row, int layer);

main() {
	printf("Generating rows... ");
	fflush(stdout);
	int buf[MAX_CRACKS] = {0};
	wall_row allrows = new_wall_row(buf);
    fprintf(stdout, "MAX_CRACKS = %d\n", MAX_CRACKS);
	gen_rows(allrows, 0, buf, 0);
	printf("%d rows generated\n", ll_listlen(allrows->list));
    return 0;

	printf("Linking rows... ");
	fflush(stdout);
	int i = interlace_rows(allrows);
	printf("%d links found\n", i);

	printf("Building walls... ");
	fflush(stdout);
	long long totalcount = 0;
	list_iterator thisit = ll_iterate_new(allrows->list);
	wall_row nextrow;
	ITERATE(nextrow, thisit) {
		totalcount += build_wall(nextrow, 1);
	}
	printf("%llu walls built\n",totalcount);
	return;
}

// Take an array of all the cracks and build a new row node
// Note: The crackbuf is temp, must copy
wall_row new_wall_row(int crackbuf[]) {
	int i;
	wall_row newrow = (wall_row) malloc (sizeof(struct Wall_row));
	for (i=0; i<MAX_CRACKS; i++) {
		newrow->cracks[i] = crackbuf[i];
	}
	for (i=0; i<WALL_TALL; i++) {
		newrow->wallsbuilt[i] = -1;
	}
	newrow->list = ll_init(NULL);
	return newrow;
}

void gen_rows(wall_row allrows, int spacefilled, 
		int crackbuf[], int crackindex) {
	if (spacefilled == WALL_WIDE) { 
		// row perfectly filled, save it
		int i;
		wall_row newrow;
		// zero out 32 crack and garbage in buffer
		for (i=crackindex-1; i<MAX_CRACKS; i++) {
			crackbuf[i] = 0;
		}
		newrow = new_wall_row(crackbuf);
		ll_add(allrows->list, (void*)newrow);
		return;
	} else if (spacefilled > WALL_WIDE) {
		// doesn't fit, don't save
		return;
	}

	// row isn't full yet, try putting a 2 block in it.
	crackbuf[crackindex] = spacefilled+2;
	gen_rows(allrows, spacefilled+2, crackbuf, crackindex+1);

	// and try a 3 block
	crackbuf[crackindex] = spacefilled+3;
	gen_rows(allrows, spacefilled+3, crackbuf, crackindex+1);

	// if either of these worked, they were saved in the call.
	return;
}

int interlace_rows (wall_row allrows) {
	int linkcount = 0;
	wall_row first, second;

	list_iterator firstit = ll_iterate_new(allrows->list);
	ITERATE(first, firstit) {
		list_iterator secondit = ll_iterate_copy(firstit);
		ITERATE(second, secondit) {
			if (can_stack(first, second)) {
				ll_add(first->list, second);
				ll_add(second->list, first);
				linkcount++;
			}
		}
		free(secondit);
	}
	free(firstit);

	return linkcount;
}

int can_stack(wall_row first, wall_row second) {
	int fi = 0, si = 0; // first/second index
	while (first->cracks[fi] && second->cracks[si] &&
			fi < MAX_CRACKS && si < MAX_CRACKS) {
		if (first->cracks[fi] == second->cracks[si]) {
			// They have a crack at the same place
			return 0;
		}
		if (first->cracks[fi] > second->cracks[si]) {
			si++;
		} else {
			fi++;
		}
	}
	return 1;
}

long long build_wall(wall_row row, int layer) {
	if (layer == WALL_TALL-1) {
		// no point in looking at top row, nothing on top of it
		return ll_listlen(row->list);
	}

	if (row->wallsbuilt[layer] != -1) {
		return row->wallsbuilt[layer];
	}

	long long totalcount = 0;
	list_iterator thisit = ll_iterate_new(row->list);
	wall_row nextrow;
	ITERATE(nextrow, thisit) {
		totalcount += build_wall(nextrow, layer+1);
	}
	free(thisit);
	// cache result
	row->wallsbuilt[layer] = totalcount;
	return totalcount;
}
