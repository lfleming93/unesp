#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

#define N 10000

int vector[N];

double f(int i){

  int j, start = i*(i+1)/2, finish = start+i;
  double return_val = 0.0;

  for(j = start; j<=finish; j++) {
    return_val += sin(j);
  }

  return return_val;
}


int main()
{
  unsigned long i;
  double sum = 0.0;
  struct timeval start, stop;

  // inicializa vetor
  // for (i=0; i<N; i++)
    // vector[i] = N-i;

  gettimeofday(&start, NULL);

  for  (i=0; i<N; i++){
    sum += f(i);
  }

  gettimeofday(&stop, NULL);

  double tempo = \
    (((double)(stop.tv_sec)*1000.0 + (double)(stop.tv_usec/1000.0)) - \
    ((double)(start.tv_sec)*1000.0 + (double)(start.tv_usec/1000.0)));

  // fprintf(stdout, "Soma do vetor = %lu\n", soma);
  fprintf(stdout, "Soma = %f \n", sum);
  fprintf(stdout, "Tempo total gasto = %g ms\n", tempo);

  return 0;
}
