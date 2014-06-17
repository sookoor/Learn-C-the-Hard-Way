#include <stdio.h>

void print_name_age(char **names, int *ages, int count) {
  int i = 0;
  while(i < count) {
        printf("%s is %d years old.\n",
               names[i], ages[i]);
        i++;
  }
}

int main(int argc, char *argv[])
{
  // create two arrays we care about
  int ages[] = {23, 43, 12, 89, 2};
  char *names[] = {
    "Alan", "Frank",
    "Mary", "John", "Lisa"
  };

  // safely get the size of ages
  int count = sizeof(ages) / sizeof(int);
  int i = count - 1;

  // first way using indexing
  while(i >= 0) {
    printf("%s has %d years alive.\n",
           *(names + i), *(ages + i));
    i--;
  }

  printf("---\n");

  // setup the pointers to the start of the arrays
  int *cur_age = ages;
  char **cur_name = names;

  // second way using pointers
  for(i = 0; i < count; i++) {
    printf("%s is %d years old.\n", 
           cur_name[i], cur_age[i]);
  }

  printf("---\n");

  // third way, pointers are just arrays
  for(i = count - 1; i >= 0; i--) {
    printf("%s is %d years old again.\n",
           *(cur_name+i), *(cur_age+i));
  }

  printf("---\n");

  // fourth way with pointers in a stupid complex way
  for(cur_name = names, cur_age = ages;
      (cur_age - ages) < count;
      cur_name++, cur_age++)
    {
      printf("%s lived %d years so far.\n",
             *cur_name, *cur_age);
    }

  printf("---\n");

  for(i = 0; i < count; i++) {
    printf("%s is stored at address %p==%p.\n", names[i], &(names[i]), names+i);
    printf("%d is stored at address %p==%p.\n", *(ages+i), ages+i, &(ages[i]));
  }

  printf("---\n");

  print_name_age(names, ages, count);

  return 0;
}
