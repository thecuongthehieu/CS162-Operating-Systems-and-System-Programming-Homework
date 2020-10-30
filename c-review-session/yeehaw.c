#include <stdio.h>
#include <stdlib.h>

// Global array stored in the read-write portion of the data segment.
static int winning_nums[4] = {162, 152, 186, 70};

int main(int argc, char *argv[]){
  int lottery_nums[8]; // Stored on the stack of main.
  lottery_nums[0] = 79;
  lottery_nums[1] = 62;

  printf("Use square-bracket syntax to access elements in the array.\n");
  printf("lottery_nums[0]: %u\n", lottery_nums[0]);
  printf("lottery_nums[1]: %u\n", lottery_nums[1]);
  printf("\n");

  printf("The following is the equivalent pointer arithmetic:\n");
  printf("*(lottery_nums+0): %u\n", *(lottery_nums+0));
  printf("*(lottery_nums+1): %u\n", *(lottery_nums+1));
  printf("\n");

  printf("Use the following syntax to find the address of each item in the array:\n");
  printf("&lottery_nums[0]: %p\n", &lottery_nums[0]);
  printf("&lottery_nums[1]: %p\n", &lottery_nums[1]);
  printf("The high addresses indicate that these are stored on the stack.\n");
  printf("\n");

  printf("The following is the equivalent pointer arithmetic:\n");
  printf("(lottery_nums+0): %p\n", (lottery_nums+0));
  printf("(lottery_nums+1): %p\n", (lottery_nums+1));
  printf("Note how the compiler automatically multiplies the offset with the sizeof(int)!\n");
  printf("\n");

  printf("########################################################\n");
  printf("                         Yeehaw\n");
  printf("########################################################\n");
  printf("\n");

  char *lucky_nums = malloc(8*sizeof(char)); // Malloc'd memory is stored on the heap.
  lucky_nums[0] = 'c';
  lucky_nums[1] = 'h';

  printf("Square brackets can be used on generic pointers as well.\n");
  printf("lucky_nums[0]: %c\n", lucky_nums[0]);
  printf("lucky_nums[1]: %c\n", lucky_nums[1]);
  printf("\n");

  printf("Under the hood, square-bracket syntax just uses pointer arithmetic.\n");
  printf("*(lucky_nums+0): %c\n", *(lucky_nums+0));
  printf("*(lucky_nums+1): %c\n", *(lucky_nums+1));
  printf("\n");

  printf("Let's see the addresses of each character from the malloc'd array:\n");
  printf("&lucky_nums[0]: %p\n", &lucky_nums[0]);
  printf("&lucky_nums[1]: %p\n", &lucky_nums[1]);
  printf("\n");

  printf("Coolio, the compiler knows the data type of the array and correctly multiplies the offsets by sizeof(char).\n");
  printf("(lucky_nums+0): %p\n", (lucky_nums+0));
  printf("(lucky_nums+1): %p\n", (lucky_nums+1));
  printf("\n");
  
  free(lucky_nums);

  printf("\n");
  printf("The following are all equivalent:\n");
  printf("winning_nums: %p\n", winning_nums);
  printf("&winning_nums: %p\n", &winning_nums);
  printf("&winning_nums[0]: %p\n", &winning_nums[0]);
  printf("The winning_nums array is a global variable and is stored below the heap.\n");
}
