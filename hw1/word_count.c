/*
 * Implementation of the word_count interface using dedicated lists.
 *
 * You may NOT modify this file. Any changes you make to this file will not
 * be used when grading your submission.
 */

/*
 * Copyright © 2019 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "word_count.h"

void init_words(word_count_list_t *wclist) {
  /* Initialize word count.  */
  *wclist = NULL;
}

size_t len_words(word_count_list_t *wclist) {
  size_t len = 0;
  word_count_t *cur;
  for (cur = *wclist; cur != NULL; cur = cur->next) {
    len++;
  }
  return len;
}

word_count_t *find_word(word_count_list_t *wclist, char *word) {
  /* Return count for word, if it exists. */
  word_count_t *wc = *wclist;
  while ((wc != NULL) && (strcmp(word, wc->word) != 0)) {
    wc = wc->next;
  }
  return wc;
}

word_count_t *add_word(word_count_list_t *wclist, char *word) {
  /*
   * If word is present in word_counts list, increment the count.
   * Otherwise, insert at head of list with count 1.
   */
  word_count_t *wc = find_word(wclist, word);
  if (wc != NULL) {
    wc->count++;
  } else if ((wc = malloc(sizeof(word_count_t))) != NULL) {
    wc->word = word;
    wc->count = 1;
    wc->next = *wclist;
    *wclist = wc;
  } else {
    perror("malloc");
  }
  return wc;
}

void fprint_words(word_count_list_t *wclist, FILE *outfile) {
  word_count_t *wc;
  for (wc = *wclist; wc != NULL; wc = wc->next) {
    fprintf(outfile, "%8d\t%s\n", wc->count, wc->word);
  }
}

void wordcount_insert_ordered(word_count_list_t *wclist, word_count_t *elem,
                              bool less(const word_count_t *,
                                        const word_count_t *)) {
  word_count_t *prev = *wclist;
  if (prev == NULL || less(elem, prev)) {
    elem->next = prev;
    *wclist = elem;
  } else {
    word_count_t *cur = prev->next;
    while (cur != NULL && less(cur, elem)) {
      prev = cur;
      cur = prev->next;
    }
    prev->next = elem;
    elem->next = cur;
  }
}

void wordcount_sort(word_count_list_t *wclist,
                    bool less(const word_count_t *, const word_count_t *)) {
  word_count_t *head = *wclist;
  word_count_list_t sorted;
  init_words(&sorted);
  while (head != NULL) {
    word_count_t *to_insert = head;
    head = head->next;
    to_insert->next = NULL;
    wordcount_insert_ordered(&sorted, to_insert, less);
  }
  *wclist = sorted;
}
