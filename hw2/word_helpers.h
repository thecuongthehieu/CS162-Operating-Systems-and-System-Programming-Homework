/*
 * The word_helpers interface provides functions that are useful to count
 * words in a file and sort the resulting word count list.
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

#ifndef WORD_HELPERS_H
#define WORD_HELPERS_H

#include <ctype.h>
#include <stdio.h>

#include "word_count.h"

/*
 * Reads all words from a stream and updates a word count list with their
 * counts.
 */
void count_words(word_count_list_t *wclist, FILE *infile);

/*
 * Returns true if the first entry has a lower count than the second entry,
 * breaking ties according to alphabetical order.
 */
bool less_count(const word_count_t *wc1, const word_count_t *wc2);

/*
 * Returns true if the first entry comes before the second entry in
 * alphabetical order.
 */
bool less_word(const word_count_t *wc1, const word_count_t *wc2);

#endif /* WORD_HELPERS_H */
