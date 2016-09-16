
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "clothing.h"

#define LAVAR_TEMPO     1000000
#define SECAR_TEMPO     5000000
#define GUARDAR_TEMPO   8000000


struct _cl_type {
  cl_status status;
  int       cor;
  int       preco;
  int       tipo;
};



clothing_t *cl_criar(cl_status s)
{
  clothing_t *c = (clothing_t *)malloc(sizeof(clothing_t));
  c->status = s;

  return c;
}


void cl_apagar(clothing_t *c)
{
  assert(c != NULL);
  free(c);
}


void cl_lavar(clothing_t *c)
{
  assert(c->status == CL_SUJA);

  volatile unsigned int i;

  // gasta tempo
  for (i=0; i<LAVAR_TEMPO; i++);

  c->status = CL_LAVADA;
}

void cl_secar(clothing_t *c)
{
  assert(c->status == CL_LAVADA);
  
  volatile unsigned int i;
  // gasta tempo
  for (i=0; i<SECAR_TEMPO; i++);
  
  c->status = CL_SECA;
}

void cl_guardar(clothing_t *c)
{
  assert(c->status == CL_SECA);
  
  volatile unsigned int i;
  // guardar tempo
  for (i=0; i<GUARDAR_TEMPO; i++);
  
  c->status = CL_GUARDADA;
}

int  cl_is_status(clothing_t *c, cl_status s)
{
  return c->status == s;
}

cl_status cl_get_status(clothing_t *c)
{
  return c->status;
}
