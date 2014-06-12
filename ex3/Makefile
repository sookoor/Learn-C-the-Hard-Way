CC=gcc
CFLAGS=-Wall -g -I.
OBJ = ex3.o
OUT = ex3

all: $(OBJ)
	$(CC) -o $(OUT) $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -rf $(OUT) *.o *.dSYM
