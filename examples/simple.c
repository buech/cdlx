#include <stdlib.h>
#include <stdio.h>

#include "dlx.h"

int main() {
   // Define input matrix
   int matrix[6][7] = {
      {0,0,1,0,1,1,0},
      {1,0,0,1,0,0,1},
      {0,1,1,0,0,1,0},
      {1,0,0,1,0,0,0},
      {0,1,0,0,0,0,1},
      {0,0,0,1,1,0,1}
   };

   // Print input matrix
   puts("Input matrix:");
   for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 7; j++)
         printf("%2d", matrix[i][j]);
      putchar('\n');
   }
   putchar('\n');

   // Create a DLX object with 7 columns
   dlx_t *d = new_dlx(7);

   // Add the 6 length-7 matrix rows to the DLX object
   for (int i = 0; i < 6; i++)
      add_row(d, 7, matrix[i]);

   /* Solve and print the solution with the default callback function:
      - will print the column numbers of the elements of the 3 solution rows
        (rows 1, 4 and 5 in Knuth's paper, here 0, 3, 4)

      row 4:   0 1 0 0 0 0 1
                 ^         ^
                 1         6

      row 0:   0 0 1 0 1 1 0
                   ^   ^ ^
                   2   4 5

      row 3:   1 0 0 1 0 0 0
               ^     ^
               0     3

      Note that the elements are not in order. The Sudoku solver gives an
      example of how to order the elements to extract the solution in that case.
   */
   puts("Solution:");
   solve(d, print_solution);

   free_dlx(d);

   return EXIT_SUCCESS;
}
