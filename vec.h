#ifndef VEC_H
#define VEC_H

#include "err.h"

typedef struct {
	int size;
	int max_size;

	void **data;
} vec_t;

void vec_init(vec_t *self);
void vec_deinit(vec_t *self);

long vec_size(vec_t *self);
void vec_push(vec_t *self, void *data);
void *vec_pop(vec_t *self);
void *vec_get(vec_t *self, long index);
err_t vec_set(vec_t *self, long index, void *data);

#endif // VEC_H
