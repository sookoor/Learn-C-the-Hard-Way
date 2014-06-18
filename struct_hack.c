#include <stdlib.h>
#include <stddef.h>
#include <stdio.h> // printf

/*
 * C struct hack - the last member of the struct is of variable length
 *
 * C99 however introduces the concept of a flexible array member,
 * which allows the size of an array to be omitted if it is the last
 * member in a structure
 */

struct s 
{
  int n;
  char a[1]; // some compilers would allow a[0] here
};

int main(void)
{
  struct s *p;
  int i, n = 100;
  size_t size;

  size = offsetof(struct s, a) + n * sizeof(p->a[0]);
  /* size = sizeof(*p) - 1 + n * sizeof(p->a[0]); */

  p = malloc(size);
  p->n = n;

  for(i = 0; i < n; i++) {
    p->a[i] = 'A' + (i % 26);
  }

  for(i = 0; i < n; i++) {
    printf("%c\n", p->a[i]);
  }

  return 0;
}
