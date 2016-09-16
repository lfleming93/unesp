
#ifndef _BUFFER_H
#define _BUFFER_H

typedef struct buffer_type buffer_t;


buffer_t *buffer_create(int size);
void buffer_destroy(buffer_t *b);

int buffer_is_empty(buffer_t *b);
int buffer_is_full(buffer_t *b);

void buffer_put(buffer_t *b, void *data);
void *buffer_get(buffer_t *b);


#endif
