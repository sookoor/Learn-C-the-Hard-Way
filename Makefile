SRC = $(wildcard *.c)
BIN = $(patsubst %.c,%,$(SRC))
SYM = $(patsubst %.c,%.dSYM,$(SRC))
CFLAGS=-I . -Wall -g
CC=gcc

.PHONY: all clean

all: $(BIN)

clean:
	$(RM) -rf $(BIN) $(SYM)
