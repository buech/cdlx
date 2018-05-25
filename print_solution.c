#include <stdio.h>

#include "dlx.h"

void print_solution(dlx_t *d) {
   for (sol_t *cur = d->s; cur; cur = cur->next) {
      int i = 0;
      for (node_t *row = cur->row; row != cur->row || !i; row = row->r, i++) {
         printf("%zu ", row->c->n);
      }
      putchar('\n');
   }
}
