#include "word_count.h"

void init_words(WordCount **wclist) {
  /* Initialize word count.  */
  *wclist = NULL;
}

size_t len_words(WordCount *wchead) {
  return 0;
}

WordCount *find_word(WordCount *wchead, char *word) {
  return NULL;
}

void add_word(WordCount **wclist, char *word) {
}

void fprint_words(WordCount *wchead, FILE *ofile) {
  /* print word counts to a file */
  WordCount *wc;
  for (wc = wchead; wc; wc = wc->next) {
    fprintf(ofile, "%i\t%s\n", wc->count, wc->word);
  }
}

/*
sort_words is provided for you in the object file wc_sort.o
If you want to do the *stretch* implement it here and comment
out the wc_sort library in the Makefile

void sort_words(word_count **words, int (*comparator)(word_count *, word_count *)) {

}
*/
