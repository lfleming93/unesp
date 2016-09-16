

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include "clothing.h"





/*
 * Input parameters:
 *
 *  <#items>
 */
int main(int argc, char *argv[])
{
  struct timeval start, end;
  float elapsed_time;
  void **roupas;
  unsigned int i;

  if (argc <= 1) {
    fprintf(stderr, "Invalid parameter number: use <#items>\n");
    exit(-1);
  }

  unsigned int num_roupas = strtol(argv[1], NULL, 10);

  fprintf(stdout, "Numero de roupas = %u\n", num_roupas);


  // gera roupas sujas com base em parametro passado pelo usuario
  roupas = (void **)malloc(sizeof(void **)*num_roupas);

  for (i=0; i<num_roupas; i++)
    roupas[i] = (void *)cl_criar(CL_SUJA);
    


  gettimeofday(&start, NULL);
  

  // processa as roupas: lava, seca, guarda  -- sequencial
  for (i=0; i<num_roupas; i++) {
    cl_lavar(roupas[i]);
    cl_secar(roupas[i]);
    cl_guardar(roupas[i]);
  }
  
  gettimeofday(&end, NULL);


  // checa se roupas estão guardadas
  for (i=0; i<num_roupas; i++)
    if (!cl_is_status(roupas[i], CL_GUARDADA))
      fprintf(stderr, "Roupa [%d] não foi guardada\n", i);

  // desaloca roupas
  for (i=0; i<num_roupas; i++)
    cl_apagar(roupas[i]);

  elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
  fprintf(stdout, "Elapsed time: %g s\n", elapsed_time); 

  return 0;
}
