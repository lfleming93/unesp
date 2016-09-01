#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#define N 987654321UL

int vector[N];

int main()
{
  unsigned long i;
  unsigned long soma = 0;
  struct timeval start, stop;

  // inicializa vetor
  for (i=0; i<N; i++)
    vector[i] = i;

  gettimeofday(&start, NULL);
  // Realiza a soma
  for (i=0; i<N; i++)
    soma += vector[i];
  gettimeofday(&stop, NULL);

  double tempo = \
    (((double)(stop.tv_sec)*1000.0 + (double)(stop.tv_usec/1000.0)) - \
    ((double)(start.tv_sec)*1000.0 + (double)(start.tv_usec/1000.0)));

  fprintf(stdout, "Soma do vetor = %lu\n", soma);
  fprintf(stdout, "Tempo total gasto = %g ms\n", tempo);

  return 0;
}
