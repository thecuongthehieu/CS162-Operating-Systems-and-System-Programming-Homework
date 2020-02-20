/*
 * The word_count interface provides lists of words and associated counts.
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

#ifndef WORD_COUNT_H
#define WORD_COUNT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
 * Representation of a word count object and word count list object.
 * PINTOS_LIST and/or PTHREADS are #define'd prior to #include to select the
 * representations.
 */

#ifdef PINTOS_LIST
#include "list.h"
typedef struct word_count {
  char *word;
  int count;
  struct list_elem elem;
} word_count_t;

#ifdef PTHREADS
#include <pthread.h>
typedef struct word_count_list {
  struct list lst;
  pthread_mutex_t lock;
} word_count_list_t;
#else /* PTHREADS */
typedef struct list word_count_list_t;
#endif /* PTHREADS */

#else /* PINTOS_LIST */

typedef struct word_count {
  char *word;
  int count;
  struct word_count *next;
} word_count_t;

typedef word_count_t *word_count_list_t;
#endif /* PINTOS_LIST */

/* Initialize a word count list. */
void init_words(word_count_list_t *wclist);

/* Get length of a word count list. */
size_t len_words(word_count_list_t *wclist);

/* Find a word in a word_count list. */
word_count_t *find_word(word_count_list_t *wclist, char *word);

/*
 * Insert word with count=1, if not already present; increment count if
 * present. Takes ownership of word.
 */
word_count_t *add_word(word_count_list_t *wclist, char *word);

/*
 * Insert word with count, if not already present; increment count if present.
 * Takes ownership of word.
 */
word_count_t *add_word_with_count(word_count_list_t *wclist, char *word,
                                  int count);

/* Print word counts to a file. */
void fprint_words(word_count_list_t *wclist, FILE *outfile);

/* Sort a word count list using the provided comparator function. */
void wordcount_sort(word_count_list_t *wclist,
                    bool less(const word_count_t *, const word_count_t *));

#endif /* WORD_COUNT_H */
