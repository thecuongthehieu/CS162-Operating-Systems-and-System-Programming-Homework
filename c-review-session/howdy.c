#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pointers implement writeable function parameters.
void howdy(int *p) {
  *p += 1;
}

// Function for function pointers.
void howdy_world(int num) {
  printf("Howdy World x%d!!\n", num);
}

// Demonstrate storage of C strings.
char *string_howdy(void) {
  char *string_lit = "hello"; // String literals are stored in the Data segment.
  char stack_str[] = "hello"; // Character arrays are stored on the stack.
  char *heap_str = malloc(4); // Malloc'd memory is stored on the heap.

  free(heap_str);
  return string_lit;
}

int main(int argc, char *argv[]) {
  // This integer is stored on the stack of main.
  int p = 69;
  printf("Before: %d\n", p);
  howdy(&p);
  printf("After: %d\n", p);

  // How to declare and define a function pointer.
  void (*function_ptr)(int) = &howdy_world;
  //void (*function_ptr)(int) = howdy_world;

  // How to use a function pointer.
  (*function_ptr)(86);
  //function_ptr(86);

  // Print the string returned from the function.
  printf("%s\n", string_howdy());
}
