#ifndef DLX_H
#define DLX_H

typedef struct node_t node_t;
typedef struct sol_t sol_t;
typedef struct dlx_t dlx_t;

struct node_t {
   node_t *l, *r, *u, *d, *c;
   size_t len;
   size_t n;
};

struct sol_t {
   sol_t *next;
   void *row;
};

struct dlx_t {
   node_t *h;
   size_t size;
   sol_t *s;
   size_t nsol;
};

dlx_t *new_dlx(size_t);
void free_dlx(dlx_t *);
void add_row(dlx_t *, size_t, int *);
void solve(dlx_t *, void (*)(dlx_t *));
void print_solution(dlx_t *);

#endif
