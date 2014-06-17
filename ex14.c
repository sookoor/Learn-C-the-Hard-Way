#include <stdio.h>
#include <ctype.h>  // isalpha, isblank
#include <string.h> // strlen

void print_letters(char arg[], int len)
{
  int i = 0;
  
  for(i = 0; i < len; i++) {
    char ch = arg[i];

    if(isalpha(ch) || isblank(ch)) {
      printf("'%c' == %d ", ch, ch);
    }
  }

  printf("\n");
}

int main(int argc, char *argv[])
{
  int i = 0;
  
  for(i = 0; i < argc; i++) {
    char *arg = argv[i];
    print_letters(arg, strlen(arg));
  }

  return 0;
}
