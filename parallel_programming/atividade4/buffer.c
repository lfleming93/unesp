#include <stdlib.h>
#include <assert.h>


#include "buffer.h"

struct buffer_type {
  void **data;
  int size;

  unsigned int head, tail;

  //int space_available;
};


buffer_t *buffer_create(int size)
{
  buffer_t *b = (buffer_t *)malloc(sizeof(buffer_t));

  // this is a hack to workaround detecting full/empty conditions
  // we always used an extra slot
  size = size+1;

  b->data = (void **)malloc(sizeof(void *)*size);

  b->size = size;

  b->head = 0;
  b->tail = 0;

  return b;
}

void buffer_destroy(buffer_t *b)
{
  assert(b->data != NULL);
  free(b->data);
  assert(b != NULL);
  free(b);
}

int buffer_is_empty(buffer_t *b)
{
  return b->head == b->tail;
}

int buffer_is_full(buffer_t *b)
{
  return (((b->head+1) % (b->size)) == b->tail);
}

void buffer_put(buffer_t *b, void *data)
{
  int next = (b->head+1) % (b->size);

  assert(next != b->tail);

  b->data[b->head] = data;
  //printf("[%x] head: %d --- status put %d\n", b, b->head, cl_get_status((clothing_t *)data));
  b->head = next;

}

void *buffer_get(buffer_t *b)
{
  assert(b->head != b->tail);

  void *data = b->data[b->tail];
  //printf("[%x] tail: %d --- status get %d\n", b, b->tail, cl_get_status((clothing_t *)data));
  b->tail = (b->tail+1) % (b->size);

  return data;
}



#ifdef TEST_BUFFER
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main()
{
  buffer_t *buffer = buffer_create(2);

  fprintf(stdout, "adding 1\n");
  buffer_put(buffer, (void *)1);

  fprintf(stdout, "adding 2\n");
  buffer_put(buffer, (void *)2);

  fprintf(stdout, "removing %d\n", (int)buffer_get(buffer));
  
  fprintf(stdout, "adding 3\n");
  buffer_put(buffer, (void *)3);
  
  fprintf(stdout, "removing %d\n", (int)buffer_get(buffer));
  
  fprintf(stdout, "removing %d\n", (int)buffer_get(buffer));
  
  fprintf(stdout, "removing %d\n", (int)buffer_get(buffer));

  return 0;
}

#endif
