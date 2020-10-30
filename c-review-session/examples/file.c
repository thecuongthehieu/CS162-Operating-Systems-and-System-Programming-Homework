#include <stdio.h>

int write_char(char *infile) {
    FILE *f = fopen(infile, "r+");
    if (f != NULL) {
      char buf[1];
      size_t chars_read = fread(buf, 1, 1, f);
      printf("The character is: %c\n", buf[0]);
      size_t chars_written = fwrite(buf, 1, 1, f);
      return fclose(f);
    }
    return 1;
}

int main() {
  char *file_name = "hello.txt";
  int status_code = write_char(file_name);
  return status_code;
}
