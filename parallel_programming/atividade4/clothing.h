/*
 *
 *
 *
 *
 */

#ifndef _CLOTHING_H
#define _CLOTHING_H

typedef enum {CL_SUJA, CL_LAVADA, CL_SECA, CL_GUARDADA} cl_status;

typedef struct _cl_type clothing_t;

clothing_t *cl_criar(cl_status);

void cl_apagar(clothing_t *c);

void cl_lavar(clothing_t *c);

void cl_secar(clothing_t *c);

void cl_guardar(clothing_t *c);

int  cl_is_status(clothing_t *c, cl_status s);

cl_status cl_get_status(clothing_t *c);

#endif
