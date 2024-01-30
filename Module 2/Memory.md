# Memory in CPP

## Allocating on the HEAP
1. int* p = new int;
2. delete[] p and delete p;

How does the system know how many bytes to return to the free store in each case?

## Issues with Memory Management
- There is overhead for every invocation of the new operator
- To lose some of this overhead, we can allocate space for many
objects in a single invocation of the new operator. (We did this for
Program 1.)
- More important, can we reuse spaces that were previously used,
even if they are released (via delete) in random order?
- How can we do our own “mini-heap management”?

