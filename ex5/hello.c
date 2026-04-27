#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;

void * hello(void * rank) {
   long my_rank = (long)rank;
   printf("Hello from thread %ld out of %d threads\n", my_rank, thread_count);
   return NULL;
}

int main(int argc, char* argv[]) {
   long thread;
   pthread_t* thread_handles;
   if (argc != 2) {
       fprintf(stderr, "Usage: ./hello <number of threads>\n");
       return 1;
   }
   thread_count = strtol(argv[1], NULL, 10);
   thread_handles = malloc(thread_count * sizeof(pthread_t));
   printf("\n**Pthread program to print hello**\n\n");
   for(thread = 0; thread < thread_count; thread++) {
      pthread_create(&thread_handles[thread], NULL, hello, (void*)thread);
   }
   for(thread = 0; thread < thread_count; thread++) {
      pthread_join(thread_handles[thread], NULL);
   }
   free(thread_handles);
   return 0;
}
