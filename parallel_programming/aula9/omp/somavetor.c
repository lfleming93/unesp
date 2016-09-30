#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define N 100000

int vector[N];

int main()
{
  unsigned long i;
  unsigned long phase = 0;
  struct timeval start, stop;

  // inicializa vetor
  for (i=0; i<N; i++)
    vector[i] = N-i;

  gettimeofday(&start, NULL);

  for (int phase = 0; phase < N; phase++)
  {
    if (phase%2 == 0) {
      for (int i = 1; i < N; i+=2)
      {
        if (vector[i-1] > vector[i]) {
          int tmp = vector[i-1];
          vector[i-1] = vector[i];
          vector[i] = tmp;
        }
      }
    } else {
      for (int i = 2; i < N; i+=2)
      {
        if (vector[i-1] > vector[i]) {
          int tmp = vector[i-1];
          vector[i-1] = vector[i];
          vector[i] = tmp;
        }
      }
    }
  }

  gettimeofday(&stop, NULL);

  double tempo = \
    (((double)(stop.tv_sec)*1000.0 + (double)(stop.tv_usec/1000.0)) - \
    ((double)(start.tv_sec)*1000.0 + (double)(start.tv_usec/1000.0)));

  // fprintf(stdout, "Soma do vetor = %lu\n", soma);
  fprintf(stdout, "Tempo total gasto = %g ms\n", tempo);

  return 0;
}
