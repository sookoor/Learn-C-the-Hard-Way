/* -*- C -*- */
/* stack.c: stack implementation */
#include <stdbool.h>
#include <stdio.h>
#include "stack.h"

#define MAXSTACK 10
#define EMPTYSTACK -1
int top = EMPTYSTACK;
char items[MAXSTACK];

void push(char c) {
  items[++top] = c;
}

char pop() {
  return items[top--];
}

bool full() {
  return top+1 == MAXSTACK; 
}

bool empty() {
  return top == EMPTYSTACK;
}

char peek() {
  return items[top];
}

int main() {
  char ch;

  while ((ch = getchar()) != '\n') {
    if (!full()) {
      push(ch);
    }
  }

  while (!empty()) {
    printf("%c", pop());
  }

  printf("\n");
  return 0;
}
