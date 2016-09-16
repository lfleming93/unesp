#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


#define N 987654321UL

int vector[N], thread_count = 3;
unsigned long soma = 0;
int flag = 0;

void* SumVector(void* rank);

int main()
{
  long thread;
  pthread_t* thread_handles;
  struct timeval start, stop;

  // inicializa vetor
  for (unsigned long i = 0; i < N; i++)
    vector[i] = i;

  thread_handles = malloc (thread_count*sizeof(pthread_t));

  gettimeofday(&start, NULL);

  for (thread = 0; thread < thread_count; thread++)
    pthread_create(&thread_handles[thread], NULL, SumVector, (void*) thread);


  for (thread = 0; thread < thread_count; thread++)
    pthread_join(thread_handles[thread], NULL);

  gettimeofday(&stop, NULL);

  free(thread_handles);


  double tempo = \
    (((double)(stop.tv_sec)*1000.0 + (double)(stop.tv_usec/1000.0)) - \
    ((double)(start.tv_sec)*1000.0 + (double)(start.tv_usec/1000.0)));

  fprintf(stdout, "Soma do vetor = %lu\n", soma);
  fprintf(stdout, "Tempo total gasto = %g ms\n", tempo);

  return 0;
}

void* SumVector(void* rank) {
  long my_rank = (long) rank;
  unsigned long i, my_sum;
  unsigned long local_N = N / thread_count;
  unsigned long first_i = my_rank * local_N;
  unsigned long last_i = (my_rank + 1) * local_N - 1;

  for (i = first_i; i <= last_i; i++)
    my_sum += vector[i];

  while (flag != my_rank);
  soma += my_sum;
  flag = (flag + 1) % thread_count;

  return NULL;
}
