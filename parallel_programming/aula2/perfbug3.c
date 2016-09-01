#include <sys/time.h>
#include <stdio.h>


#define SIZE 20000


int A[SIZE][SIZE];
int X[SIZE];
int S[SIZE];

int main(int argc, char const *argv[]) {
  int i, j, k;
  struct timeval start, stop;

  //  Inicialização
  for (i = 0; i < SIZE; ++i) {
    X[i] = i;
    S[i] = 0;
    for (j = 0; j < SIZE; ++j) {
      A[i][j] = i+j;
    }
  }

  // Multiplicação de A por X, gerando S
  gettimeofday(&start, NULL);
  for (i = 0; i < SIZE; ++i) {
    for (j = 0; j < SIZE; ++j) {
      S[i] += A[i][j] * X[j];
    }
  }

  for (int i = 0; i < SIZE; ++i)
  {
    fprintf(stdout, "%d\n", S[i]);
  }

  gettimeofday(&stop, NULL);

  double t_start = ((double)start.tv_sec * 1000.0) + ((double)start.tv_usec / 1000.0);
  double t_stop = ((double)stop.tv_sec * 1000.0) + ((double)stop.tv_usec / 1000.0);

  printf("Tempo total: %f\n", t_stop - t_start);


  return 0;
}
