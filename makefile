CFLAGS = -std=c99 -O3 -flto -Wall -Wextra -pedantic
LDFLAGS = -L.
LDLIBS = -lcdlx

vpath %.c examples

SRC = dlx.c print_solution.c
OBJ := $(SRC:.c=.o)
HDR = dlx.h
LIB = libcdlx.a
EXMPLS = simple

all: lib examples

lib: $(LIB)

examples: $(EXMPLS)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJ)
	ar crus $@ $?

$(EXMPLS): % : %.c $(LIB) $(HDR)
	$(CC) $(CFLAGS) $(LDFLAGS) -I. $< -o $@ $(LDLIBS)

clean:
	rm -f $(LIB) *.o $(EXMPLS)

.PHONY: all clean
