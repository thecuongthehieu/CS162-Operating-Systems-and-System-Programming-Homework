/*
 * You may NOT modify this file. Any changes you make to this file will not
 * be used when grading your submission.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUM_THREADS     4
int common = 162;
char *somethingshared;

void *threadfun(void *threadid) {
  long tid;
  tid = (long)threadid;
  printf("Thread #%lx stack: %lx common: %lx (%d) tptr: %lx\n", tid,
	 (unsigned long) &tid,
	 (unsigned long) &common, common++,
	 (unsigned long) threadid);
  printf("%lx: %s\n", (unsigned long) somethingshared, somethingshared+tid);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  int rc;
  long t;
  int nthreads = NUM_THREADS;
  char *targs = strcpy(malloc(100),"I am on the heap.");

  if (argc > 1) {
    nthreads = atoi(argv[1]);
  }

  pthread_t threads[nthreads];

  printf("Main stack: %lx, common: %lx (%d)\n",
	 (unsigned long) &t,
	 (unsigned long) &common, common);
  puts(targs);
  somethingshared = targs;
  for(t = 0; t < nthreads; t++) {
    printf("main: creating thread %ld\n", t);
    rc = pthread_create(&threads[t], NULL, threadfun, (void *)t);
    if (rc) {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }

  /* Last thing that main() should do */
  pthread_exit(NULL);
}

