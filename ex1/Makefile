CC=gcc
CFLAGS=-Wall -g -I.
OBJ = ex1.o
OUT = ex1

all: $(OBJ)
	$(CC) -o $(OUT) $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -rf $(OUT) *.o *.dSYM
