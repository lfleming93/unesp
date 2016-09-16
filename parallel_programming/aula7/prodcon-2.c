#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

#include "buffer.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/* producer thread */
void *producer(void *arg)
{
  long int i;
  long int loops = (long int)arg;
  for (i=0; i<loops; i++) {
    pthread_mutex_lock(&mutex);

      if (buffer_is_full())
        pthread_cond_wait(&cond, &mutex);

    buffer_put(i);

    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}

/* consumer thread */
void *consumer(void *arg)
{
  long int i;
  long int loops = (long int)arg;
  for (i=0; i<loops; i++) {
    pthread_mutex_lock(&mutex);

      if (buffer_is_empty())
        pthread_cond_wait(&cond, &mutex);

    int tmp = buffer_get();

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    fprintf(stdout, "%d\n", tmp);
  }

  return NULL;
}


/*
 * Input parameters:
 *
 *  <#producers> <#consumers> <#items>
 */
int main(int argc, char *argv[])
{
  int i;
  pthread_t *thread_handles;
  struct timeval start, end;
  float elapsed_time;

  long int num_producers, num_consumers, items;

  if (argc <= 3) {
    fprintf(stderr, "Invalid parameter number: use <#producers> <#consumers> <#items>\n");
    exit(-1);
  }

  num_producers = strtol(argv[1], NULL, 10);
  num_consumers = strtol(argv[2], NULL, 10);
  items = strtol(argv[3], NULL, 10);


  thread_handles = malloc((num_producers+num_consumers)*sizeof(pthread_t));

  fprintf(stdout, "Number of producers = %ld\n", num_producers);
  fprintf(stdout, "Number of consumers = %ld\n", num_consumers);
  fprintf(stdout, "Number of items = %ld\n", items);


  gettimeofday(&start, NULL);

  // Launch producers
  for (i=0; i<num_producers; i++)
    if (pthread_create(&thread_handles[i], NULL, producer, (void *) (items/num_producers))) {
      fprintf(stderr, "Error spawning thread\n");
      exit(-1);
    }

  // Launch consumers
  for (i=num_producers; i<num_producers+num_consumers; i++)
    if (pthread_create(&thread_handles[i], NULL, consumer, (void *) (items/num_consumers))) {
      fprintf(stderr, "Error spawning thread\n");
      exit(-1);
    }

  for (i=0; i<num_producers+num_consumers; i++) {
    pthread_join(thread_handles[i], NULL);
  }

  gettimeofday(&end, NULL);


  elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
  fprintf(stdout, "Elapsed time: %g s\n", elapsed_time);

  free(thread_handles);

  return 0;
}
