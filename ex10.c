#include <stdio.h>

int main(int argc, char *argv[])
{
  int i = 0;
  
  // go through each string in argv
  // why am I skipping argv[0]?
  for(i = 1; i < argc; i++) {
    printf("arg %d: %s\n", i, argv[i]);
  }

  // let's make our own array of strings
  char *states[] = {
    "California", "Oregon",
    "Washington", "Texas", NULL
  };
  int num_states = 5;

  for(i = 0; i < num_states; i++) {
    printf("state %d: %s\n", i, states[i]);
  }

  states[0] = argv[0];
  printf("argv valued assigned to states: %s\n", argv[0]);

  argv[1] = states[1];
  printf("states valued assigned to argv: %s\n", argv[1]);


  return 0;
}
