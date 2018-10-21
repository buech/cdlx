CFLAGS ?= -O3 -march=native
CFLAGS += -std=c99 -Wall -Wextra -pedantic
LDFLAGS = -L. -lcdlx

vpath %.c examples

SRC = dlx.c print_solution.c
OBJ := $(SRC:.c=.o)
HDR = dlx.h
LIB = libcdlx.a
EXMPLS = simple sudoku-solver

all: lib examples

lib: $(LIB)

examples: $(EXMPLS)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJ)
	ar crus $@ $?

simple: simple.c $(LIB) $(HDR)
	$(CC) $(CFLAGS) -I. $< -o $@ $(LDFLAGS)

sudoku-solver: sudoku-solver.c $(LIB) $(HDR)
	$(CC) $(CFLAGS) -I. $< -o $@ $(LDFLAGS)

clean:
	rm -f $(LIB) *.o $(EXMPLS)

.PHONY: all clean
