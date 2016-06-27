# Project Euler # 215

** Jake Mingolla **

** June 2016 **

** Category: General Backend Programming **

** Languages: C **

### About

This program solves the [Project Euler #215](https://projecteuler.net/problem=215) problem. Here is the problem
directly from the website:

> Consider the problem of building a wall out of 2×1 and 3×1 bricks (horizontal×vertical dimensions) such that, for extra strength, the gaps between horizontally-adjacent bricks never line up in consecutive layers, i.e. never form a "running crack".

>For example, the following 9×3 wall is not acceptable due to the running crack shown in red:

>![](https://projecteuler.net/project/images/p215_crackfree.gif)


>There are eight ways of forming a crack-free 9×3 wall, written W(9,3) = 8.

>Calculate W(32,10).

### Introduction

I solve the problem by representing each row of 32 unit length as a 32
bit unsigned integer which we declare as the Row type. The bitmap
within a Row can be a 1 to represent a crack and a 0 to represent
a space occupied by a brick. Figuring out whether two Rows can stack
upon each other requires only a simple BITWISE AND operation.

In addition to the Row objects, we use a linked list of Nodes to store
the data associated with each unique Row we find. After finding all
possible legal Rows (i.e. ones that are exactly 32 units long) and
arranging them in a linked list, we do a `O(n^2)` iteration through
the linked list. As we iterate through all possible pairs of Nodes
within the linked list, if the Row objects associated with each Node
can stack, we provide each Node a pointer to the other. We store
these pointers in a dynamic array, resizing as necessary, which
represent the stackability records for each Node.

Finally, we calculate the total number of walls that can be built
by using all of the unique Rows in the linked list of Nodes as the
base and recursively calculating all possible arrangements of 32 x 10
unit walls by iterating through all possible stackability records for
each Node.

In addition, it is vital to note the existence of a cache within each
Node that stores the number of possible ways to arrange Rows above
the Row corresponding to the Node for every level of the wall. For
example, if one round of computation finds that a given Row has
21 solutions above it if it is placed at the 3rd layer of the wall,
the next time we attempt to place that Row at the 3rd layer of the wall
we can avoid the expensive computation and simply use the cached
result.

### Runtime

My program outputs W(32, 10) = 806,844,323,190,414 in 50 - 100 ms. This
solution is verified using other solutions online.

In addition, my program handles memory management by destroying all
dynamically allocated blocks after computation. This is verified
using valgrind.

### TO-DO's

- Handle arbitrarily large walls, i.e. not just 32 x 10. While modifying
the height is currently supported by the program, we would need to
modify the internal data structure of the Row to by not just a 32 bit
unsigned integer to handle larger rows.
