#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dlx.h"

void print_board(int *board) {
   for (int i = 0; i < 9; i++) {
      if (!(i % 3) && i)
         putchar('\n');
      for (int k = 9 * i; k < 9 * (i+1); k++) {
         if (!(k % 3))
            putchar(' ');
         board[k] ? printf("%2i", board[k]) : printf(" .");
      }
      putchar('\n');
   }
   putchar('\n');
}

void usage(char *program_name) {
   printf(
"usage: %s [-h] SUDOKU\n\n", program_name);
   puts(
"Sudoku solver in C using Donald Knuth's DLX\n\n"

"positional arguments:\n"
"  SUDOKU        Sudoku in the form of a continuous string read from\n"
"                left to right with 0 for empty cells, must have a\n"
"                length of at least 81 characters, additional\n"
"                characters are ignored\n"
"optional arguments:\n"
"  -h, --help    show this help message and exit"
   );
}

void print_sol(dlx_t *d) {
   int board[81];
   int min, val;
   node_t *rmin, *row;
   sol_t *cur;
   for (cur = d->s; cur; cur = cur->next) {
      rmin = cur->row;
      min = rmin->c->n;
      for (row = rmin->r; row != rmin; row = row->r) {
         val = row->c->n;
         if (val < min) {
            min = val;
            rmin = row;
         }
      }
      board[9 * (rmin->c->n / 9) + (rmin->c->n % 9)] = (rmin->r->c->n) % 9 + 1;
   }

   d->nsol > 1 ? printf("Solution %zu:\n", d->nsol) : puts("Solution:");
   print_board(board);
}

int main(int argc, char *argv[]) {

   int board[81];

   if (argc == 2) {
      if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
         usage(argv[0]);
         return EXIT_SUCCESS;
      } else {
         if (strlen(argv[1]) < 81) {
            fputs("Error: The sudoku has too few fields\n", stderr);
            usage(argv[0]);
            return EXIT_FAILURE;
         } else {
            if (strlen(argv[1]) > 81)
               fputs("Warning: The sudoku has too many fields. Additional fields will be ignored.\n", stderr);
            for (int i = 0; i < 81; i++)
               board[i] = (argv[1])[i] >= '1' && (argv[1])[i] <= '9' ? (argv[1])[i]-'0' : 0;
         }
      }
   } else if (argc > 2) {
      fputs("Error: Too many arguments\n", stderr);
      usage(argv[0]);
      return EXIT_FAILURE;
   } else {
      fputs("Error: Too few arguments\n", stderr);
      usage(argv[0]);
      return EXIT_FAILURE;
   }

   puts("Input:");
   print_board(board);

   dlx_t *d = new_dlx(324);

   for (int row = 0, i = 0; row < 9; row++) {
      for (int col = 0; col < 9; col++, i++) {
         int block = row / 3 * 3 + col / 3;
         int n = board[i];
         if (n) {
            int con[324] = {0};
            con[i] = 1;
            con[ 81 + 9 * row   + n - 1] = 1;
            con[162 + 9 * col   + n - 1] = 1;
            con[243 + 9 * block + n - 1] = 1;
            add_row(d, 324, con);
         } else {
            for (n = 0; n < 9; n++) {
               int con[324] = {0};
               con[i] = 1;
               con[ 81 + 9 * row   + n] = 1;
               con[162 + 9 * col   + n] = 1;
               con[243 + 9 * block + n] = 1;
               add_row(d, 324, con);
            }
         }
      }
   }

   solve(d, print_sol);

   if (!d->nsol)
      puts("The Sudoku is not solvable!");

   free_dlx(d);

   return EXIT_SUCCESS;
}
