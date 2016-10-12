#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include "clothing.h"
#include "buffer.h"

unsigned int num_roupas, num_lava, num_seca, num_guarda;

buffer_t *buffer_lava, *buffer_seca, *buffer_guarda, *buffer_done;

pthread_mutex_t m_lava = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c_seca = PTHREAD_COND_INITIALIZER;

pthread_mutex_t m_seca = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c_guarda = PTHREAD_COND_INITIALIZER;

pthread_mutex_t m_guarda = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_done = PTHREAD_MUTEX_INITIALIZER;

void* LavaRoupa(void* rank) {
  unsigned long i;
  unsigned long local_num_roupas = num_roupas / num_lava;
  void *roupa;

  for (i = 0; i < local_num_roupas; i++) {

    pthread_mutex_lock(&m_lava);
    roupa = buffer_get(buffer_lava);
    pthread_mutex_unlock(&m_lava);

    cl_lavar(roupa);

    pthread_mutex_lock(&m_seca);
    buffer_put(buffer_seca, roupa);
    pthread_mutex_unlock(&m_seca);

    pthread_cond_signal(&c_seca);
  }

  return NULL;
}

void* SecaRoupa(void* rank) {

  unsigned long i;
  unsigned long local_num_roupas = num_roupas / num_seca;
  void *roupa;

  for (i = 0; i < local_num_roupas; i++) {
    pthread_mutex_lock(&m_seca);
    while (buffer_is_empty(buffer_seca))
      pthread_cond_wait(&c_seca, &m_seca);

    roupa = buffer_get(buffer_seca);
    pthread_mutex_unlock(&m_seca);

    cl_secar(roupa);

    pthread_mutex_lock(&m_guarda);
    buffer_put(buffer_guarda, roupa);
    pthread_mutex_unlock(&m_guarda);

    pthread_cond_signal(&c_guarda);
  }

  return NULL;
}

void* GuardaRoupa(void* rank) {
  unsigned long i;
  unsigned long local_num_roupas = num_roupas / num_guarda;
  void *roupa;

  for (i = 0; i < local_num_roupas; i++) {
    pthread_mutex_lock(&m_guarda);
    while (buffer_is_empty(buffer_guarda))
      pthread_cond_wait(&c_guarda, &m_guarda);

    roupa = buffer_get(buffer_guarda);
    pthread_mutex_unlock(&m_guarda);

    cl_guardar(roupa);

    pthread_mutex_lock(&m_done);
    buffer_put(buffer_done, roupa);
    pthread_mutex_unlock(&m_done);

  }

  return NULL;
}

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
  unsigned int i, thread_count = 0;
  pthread_t* thread_handles;
  long thread;

  if (argc <= 4) {
    fprintf(stderr, "Invalid parameter number: use <#items>\n");
    exit(-1);
  }

  num_roupas = strtol(argv[1], NULL, 10);
  num_lava = strtol(argv[2], NULL, 10);
  num_seca = strtol(argv[3], NULL, 10);
  num_guarda = strtol(argv[4], NULL, 10);


  fprintf(stdout, "Numero de roupas = %u\n", num_roupas);

  thread_handles = malloc ((num_lava + num_seca + num_guarda)*sizeof(pthread_t));


  buffer_lava = buffer_create(num_roupas);
  buffer_seca = buffer_create(num_roupas);
  buffer_guarda = buffer_create(num_roupas);
  buffer_done = buffer_create(num_roupas);


  // gera roupas sujas com base em parametro secado pelo usuario
  for (i=0; i < num_roupas; i++) {
    buffer_put(buffer_lava,  (void *)cl_criar(CL_SUJA));
  }


  gettimeofday(&start, NULL);

  for (thread = 0; thread < num_lava; thread++)
    pthread_create(&thread_handles[thread_count++], NULL, LavaRoupa, (void*) thread);

  for (thread = 0; thread < num_seca; thread++)
    pthread_create(&thread_handles[thread_count++], NULL, SecaRoupa, (void*) thread);

  for (thread = 0; thread < num_guarda; thread++)
    pthread_create(&thread_handles[thread_count++], NULL, GuardaRoupa, (void*) thread);

  for (thread = 0; thread < thread_count; thread++)
    pthread_join(thread_handles[thread], NULL);

  gettimeofday(&end, NULL);


  // checa se roupas estão guardadas
  for (i=0; i<num_roupas; i++)
    if (!cl_is_status(buffer_get(buffer_done), CL_GUARDADA))
      fprintf(stderr, "Roupa [%d] não foi guardada\n", i);


  elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
  fprintf(stdout, "Elapsed time: %g s\n", elapsed_time);

  return 0;
}
