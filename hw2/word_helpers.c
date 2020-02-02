/*
 * The file uses the word_count interface to implement the word_helpers
 * interface.
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

#include <ctype.h>
#include <stdio.h>

#include "word_count.h"
#include "word_helpers.h"

/*
 * Reads a word from a stream, skipping initial non-alpha characters, and
 * stores it in a malloc'd buffer. Returns length of the word, or 0 if reached
 * end of file.
 */
static size_t get_word(char **word, FILE *infile) {
  int ch;
  size_t buffer_cap = 16;
  size_t index = 0;
  char *buffer;

  /* Skip initial non-alpha characters. */
  while (!isalpha(ch = fgetc(infile))) {
    if (ch == EOF) {
      return 0;
    }
  }

  /* Allocate buffer on heap. */
  if ((buffer = malloc(buffer_cap * sizeof(char))) == NULL) {
    perror("malloc");
    return 0;
  }

  /* Accumulate word's characters into buffer. */
  do {
    buffer[index++] = tolower(ch);

    /* Expand buffer if full. */
    if (index == buffer_cap) {
      char *new_buffer;
      buffer_cap *= 2;
      if ((new_buffer = realloc(buffer, buffer_cap)) == NULL) {
        perror("realloc");
        free(buffer);
        return 0;
      }
      buffer = new_buffer;
    }
  }
  while (isalpha(ch = fgetc(infile)));
  buffer[index] = '\0';

  *word = buffer;
  return index;
}

void count_words(word_count_list_t *wclist, FILE *infile) {
  /* Extract all words in infile and update word counts for them. */
  char *word;
  size_t len;
  while ((len = get_word(&word, infile)) != 0) {
    if (len == 1) {
      free(word);
    } else if (add_word(wclist, word) == NULL) {
      free(word);
      return;
    }
  }
}

bool less_count(const word_count_t * wc1, const word_count_t * wc2) {
  return (wc1->count < wc2->count) ||
         ((wc1->count == wc2->count) && (strcmp(wc1->word, wc2->word) < 0));
}

bool less_word(const word_count_t * wc1, const word_count_t * wc2) {
  return strcmp(wc1->word, wc2->word) < 0;
}
