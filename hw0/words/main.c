/*

  Word Count using dedicated lists

*/
#include <assert.h>
#include <ctype.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "word_count.h"

/* Global data structure tracking the words encountered */

WordCount *word_counts = NULL;

size_t get_word(FILE *infile, char *buf, size_t buflen) {
  /*
    Stateless input parser - extract next word from a input stream, skipping initial non-alpha
    returns strlen(buf)
  */
  return 0;
}

void count_words(WordCount **wclist, FILE *infile) {
}

int num_words(FILE* infile) {
  return 0;
}

int main(int argc, char **argv) {
    int index = 0;
    bool count_arg = false;
    int total_words = 0;
    static struct option long_options[] =
    {
        {"count", no_argument, 0, 'c'},
        {0, 0, 0, 0}
    };
    while ((index = getopt_long(argc, argv, "c", long_options, NULL)) != -1) {
        switch (index) {
            case 'c':
                count_arg = true;
                break;
        }
    }

    /* These lines are for your reference. They can be removed! */
    printf("The command line arguments are as follows: \n");
    for (index = optind; index < argc; index++) {
        printf("%s\n", argv[index]);
    }

    if(count_arg)
      printf("The total number of words is: %i\n", total_words);
    else
      printf("The frequencies of each word are: \n");
    return 0;
}
