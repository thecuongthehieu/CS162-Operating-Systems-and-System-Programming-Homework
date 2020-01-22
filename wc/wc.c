/*
 * wc.c
 * CS 162
 * Author: Roger Chen
 */

#include <stdio.h>
#include <ctype.h>

void wc(FILE *ofile, FILE *infile, char *inname) {

  int WHITESPACE_UNSET = 0,
      WHITESPACE_WORD = 1,
      WHITESPACE_WHITESPACE = 2;

  unsigned char byte_read;
  int newlines = 0, words = 0, bytes = 0;
  int whitespace_mode = WHITESPACE_UNSET;
  while ((byte_read = getc(infile)), !feof(infile)) {
    if ((byte_read >= 0 && byte_read <= 8) ||
        (byte_read >= 14 && byte_read <= 31) ||
        (byte_read >= 127 && byte_read <= 159)) {
      // Neither a "word character" nor or whitespace
    } else if ((byte_read >= 9 && byte_read <= 13) ||
        byte_read == 32) {
      // Whitespace
      if (byte_read == '\n') {
        newlines++;
      }
      if (whitespace_mode == WHITESPACE_WORD) {
        words++;
      }
      whitespace_mode = WHITESPACE_WHITESPACE;
    } else {
      // Standard character
      whitespace_mode = WHITESPACE_WORD;
    }
    bytes += 1;
  }

  // Handle trailing word
  if (whitespace_mode == WHITESPACE_WORD) {
    words++;
  }

  fprintf(ofile, "%7d %7d %7d %s\n", newlines, words, bytes, inname);

}

int main (int argc, char *argv[]) {
  FILE *infile;
  FILE *ofile;
  char *inname;

  if (argc >= 3) {
    ofile = fopen(argv[2], "w");
  } else {
    ofile = stdout;
  }

  if (argc >= 2) {
    infile = fopen(argv[1], "r");
    inname = argv[1];
  } else {
    infile = stdin;
    inname = "";
  }

  wc(ofile, infile, inname);

  fclose(infile);
  fclose(ofile);

  return 0;
}
