#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "dlx.h"

static void insert_right(node_t *cur, node_t *new) {
   new->r = cur->r;
   new->l = cur;
   cur->r->l = new;
   cur->r = new;
}

static void insert_above(node_t *cur, node_t *new) {
   new->u = cur->u;
   new->d = cur;
   cur->u->d = new;
   cur->u = new;
}

dlx_t *new_dlx(size_t ncols) {
   dlx_t *d = malloc(sizeof(dlx_t));
   d->size = ncols;
   d->s = NULL;
   d->nsol = 0;

   node_t *root = malloc(sizeof(node_t));
   root->l = root;
   root->r = root;
   root->u = NULL;
   root->d = NULL;
   root->c = NULL;
   root->len = 0;
   root->n = -1;

   node_t *cur = root;
   for (size_t i = 0; i < ncols; ++i, cur = cur->r) {
      node_t *new = malloc(sizeof(node_t));
      insert_right(cur, new);
      new->u = new;
      new->d = new;
      new->c = new;
      new->len = 0;
      new->n = cur->n + 1;
   }

   d->h = root;

   return d;
}

void free_dlx(dlx_t *d) {
   node_t *h = d->h;

   node_t *cnext;
   for (node_t *c = h->r; c != h; c = cnext) {
      cnext = c->r;
      node_t *rnext;
      for (node_t *r = c->d; r != c; r = rnext) {
         rnext = r->d;
         free(r);
      }
      free(c);
   }

   free(d->h);
   free(d);
}

void add_row(dlx_t *d, size_t len, int *row) {
   node_t *cur = d->h->r;
   node_t *prev = NULL;
   for (size_t i = 0; i < len; ++i, cur = cur->r) {
      if (row[i]) {
         node_t *new = malloc(sizeof(node_t));
         new->c = cur;
         insert_above(cur, new);
         if (!prev) {
            new->l = new;
            new->r = new;
         } else {
            insert_right(prev, new);
         }
         new->c->len++;
         prev = new;
      }
   }
}

static void cover(node_t *c) {
   c->r->l = c->l;
   c->l->r = c->r;
   for (node_t *i = c->d; i != c; i = i->d) {
      for (node_t *j = i->r; j != i; j = j->r) {
         j->d->u = j->u;
         j->u->d = j->d;
         j->c->len--;
      }
   }
}

static void uncover(node_t *c) {
   for (node_t *i = c->u; i != c; i = i->u) {
      for (node_t *j = i->l; j != i; j = j->l) {
         j->c->len++;
         j->d->u = j;
         j->u->d = j;
      }
   }
   c->r->l = c;
   c->l->r = c;
}

static node_t *choose_col(node_t *h) {
   size_t s = SIZE_MAX;
   node_t *c = h->r;

   for (node_t *j = h->r; j != h; j = j->r) {
      if (j->len < s) {
         c = j;
         s = j->len;
      }
   }

   return c;
}

static void push(sol_t **s, node_t *row) {
   sol_t *new = malloc(sizeof(sol_t));

   new->row = row;
   new->next = *s;
   *s = new;
}

static node_t *pop(sol_t **s) {
   if (!(*s))
      return NULL;

   sol_t *next = (*s)->next;
   node_t *row = (*s)->row;
   free(*s);
   *s = next;

   return row;
}

void solve(dlx_t *d, void (*cb)(dlx_t *d)) {
   node_t *h = d->h;

   if (h->r == h) {
      d->nsol++;
      cb(d);
      return;
   }

   node_t *c = choose_col(h);
   cover(c);

   for (node_t *r = c->d; r != c; r = r->d) {
      push(&d->s, r);

      for (node_t *j = r->r; j != r; j = j->r)
         cover(j->c);

      solve(d, cb);

      r = pop(&d->s);

      c = r->c;
      for (node_t *j = r->l; j != r; j = j->l)
         uncover(j->c);
   }
   uncover(c);
}
