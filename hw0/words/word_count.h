/*

Copyright © 2019 University of California, Berkeley

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/*
word_count provides lists of words and associated count

Functional methods take the head of a list as first arg.
Mutators take a reference to a list as first arg.
*/



#ifndef word_count_h
#define word_count_h

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Representation of a word count object.
   Includes next field for constructing singly linked list*/
struct word_count {
    char *word;
    int count;
    struct word_count *next;
};

/* Introduce a type name for the struct */
typedef struct word_count WordCount;

/* Initialize a word count list, updating the reference to the list */
void init_words(WordCount **wclist);

/* Length of a word count list */
size_t len_words(WordCount *wchead);

/* Find a word in a word_count list */
WordCount *find_word(WordCount *wchead, char *word);

/* Insert word with count=1, if not already present; increment count if present. */
void add_word(WordCount **wclist, char *word);

//static int wordcntcmp(const WordCount *wc1, WordCount *wc2);

/* print word counts to a file */
void fprint_words(WordCount *wchead, FILE *ofile);

/* Inserts a word into the list in order. Assumes the existing list is already sorted */
void wordcount_insert_ordered(WordCount **wclist, WordCount *elem, bool less(const WordCount *, const WordCount *));

/* Sort a word count list in place */
void wordcount_sort(WordCount **wclist, bool less(const WordCount *, const WordCount *));

#endif /* word_count_h */


