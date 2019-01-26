# CDLX

A very simple C-implementation of Donald Knuth's DLX algorithm to solve exact
cover problems efficiently, closely following [his
paper](https://arxiv.org/abs/cs/0011047v1).

## Compilation
Compile only the DLX library:
```
$ make lib
```
This creates the library `libcdlx.a` that can be linked with.

Compile the examples:
```
$ make examples
```

Compile everything:
```
$ make
```

## Usage

Create a new DLX object with `new_dlx`, add constraint rows to it with `add_row`
and solve for all possible solutions with `solve`. See `examples/` for
examples:
- `simple.c`: the simple 6x7 matrix example from Knuth's paper

## Documentation
### Functions

---
```c
dlx_t *new_dlx(size_t ncols)
```
- create a pointer to a DLX object with `ncols` columns

---
```c
void free_dlx(dlx_t *d)
```
- delete DLX object pointed to by `d` and release memory allocated for all nodes

---
```c
void add_row(dlx_t *d, size_t len, int *row)
```
- add a constraint row in the form of an integer array `row` of length `len`
  containing 0s and 1s to DLX object pointed to by `d`
- `len` must be equal to the number of columns of the DLX object

---
```c
void solve(dlx_t *d, void (*cb)(dlx_t *d))
```
- solve DLX object pointed to by `d` for all possible solutions and call
  callback function `cb` when a solution was found

---
```c
void print_solution(dlx_t *d)
```
- print the column index of node in each solution row, can be used as a default
  callback function in `solve`

### Structs and Types

---
```
node_t:
   node_t *l, *r, *u, *d, *c
   size_t len
   size_t n
```
- one node of the four-way cyclic linked list of DLX
- `*l, *r, *u, *d`: pointers to left, right, upper and lower node from current
  node
- `*c`: pointer to header column of current node
- `len`: length of column, only used for column header nodes
- `n`: number of column the node lies in

---
```
sol_t:
   sol_t *next
   void *row
```
- one row of a solution
- `*next`: pointer to next solution row
- `*row`: pointer to first node of the solution row

---
```
dlx_t:
   node_t *h
   size_t size
   sol_t *s
   size_t nsol
```
- DLX object
- `*h`: pointer to header node of the DLX linked list
- `size`: number of columns
- `*s`: pointer to first solution row
- `nsol`: number of solutions, gets incremented before callback function is
  called
