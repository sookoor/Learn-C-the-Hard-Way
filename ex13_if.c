#include <stdio.h>

int main(int argc, char *argv[])
{
  if(argc < 2) {
    printf("ERROR: You need at least one argument. \n");
    // this is how you abort a program
    return 1;
  }

  int j;
  for(j = 1; j < argc; j++) {
    int i;
    char letter;
    for(i = 0, letter = argv[j][i]; argv[j][i] != '\0'; i++, letter = argv[j][i]) {
      
      if(letter >= 'A' && letter <= 'Z') {
        letter = 'a' + (letter - 'A');
      }
      
      if('a' == letter) {
        printf("%d: 'a'\n", i);
      } 
      else if('e' == letter) {
        printf("%d: 'e'\n", i);
      } 
      else if('i' == letter) {
        printf("%d: 'i'\n", i);
      } 
      else if('o' == letter) {
        printf("%d: 'o'\n", i);
      } 
      else if('u' == letter) {
        printf("%d: 'u'\n", i);
      } 
      else if('y' == letter) {
        if(i > 2) {
          // it's only sometimes Y
          printf("%d: 'y'\n", i);
        }
        else {
          printf("%d: %c is not a vowel\n", i, letter);
        }
      } 
      else {
        printf("%d: %c is not a vowel\n", i, letter);
      }
    }
  }
  return 0;
}
