#include <sys/time.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  struct timeval start, stop;

  //  Inicialização


  // Execução

  gettimeofday(&start, NULL);

  //  código


  gettimeofday(&stop, NULL);

  double t_start = ((double)start.tv_sec * 1000.0) + ((double)start.tv_usec / 1000.0);
  double t_stop = ((double)stop.tv_sec * 1000.0) + ((double)stop.tv_usec / 1000.0);

  printf("Tempo total: %f\n", t_stop - t_start);


  return 0;
}
