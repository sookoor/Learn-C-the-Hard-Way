#include <stdio.h> // printf
#include <assert.h>
#include <stdlib.h> // malloc, free
#include <string.h> // strdup

// A structure with 4 elements to describe a person 
// typedef allows the declaration of instances without using the
// keyword "struct"
typedef struct{
  char *name;
  int age;
  int height;
  int weight;
} Person; /* note semi-colon here */

// Create person structs by initializing each field using the x->y
// syntax
Person Person_create(char *name, int age, int height, int weight)
{

  Person who; // automatic allocation, all fields placed on stack

  // Duplicate the string for the name, to make sure the structure
  // actually owns it
  who.name = strdup(name);  // strdup allocates memory and copies
                            // string to it
  who.age = age;
  who.height = height;
  who.weight = weight;

  return who;
}

// Destroys Person structs
void Person_destroy(Person who)
{ 
  // Return the memory obtained with strdup
  free(who.name);
}

// Print out people.
void Person_print(Person who)
{
  printf("Name: %s\n", who.name);
  printf("\tAge: %d\n", who.age);
  printf("\tHeight: %d\n", who.height);
  printf("\tWeight: %d\n", who.weight);
}

int main(int argc, char *argv[])
{
  // make two people structures
  Person joe = Person_create("Joe Alex", 32, 64, 140);

  Person frank = Person_create("Frank Blank", 20, 72, 180);

  // print them out and where they are in memory
  printf("Joe is at memory location %p:\n", &joe);
  Person_print(joe);

  printf("Frank is at memory location %p:\n", &frank);
  Person_print(frank);

  // make everyone age 20 years and print them again
  joe.age += 20;
  joe.height -= 2;
  joe.weight += 40;
  Person_print(joe);

  frank.age += 20;
  frank.weight += 20;
  Person_print(frank);

  // destroy them both so we clean up
  Person_destroy(joe);
  Person_destroy(frank);

  return 0;
}
