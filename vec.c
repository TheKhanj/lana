#include <assert.h>
#include <stdlib.h>

#include "vec.h"

void vec_init(vec_t *vec) {
	vec->size = 0;
	vec->max_size = 1;

	vec->data = (void **)malloc(vec->max_size * sizeof(void *));
}

void vec_deinit(vec_t *vec) { free(vec->data); }

int vec_size(vec_t *vec) { return vec->size; }

void vec_push(vec_t *vec, void *data) {
	vec->data[vec->size++] = data;

	if (vec->size == vec->max_size) {
		vec->max_size *= 2;
		vec->data = realloc(vec->data, vec->max_size);
	}
}

void *vec_pop(vec_t *vec) { return vec->data[--vec->size]; }

void *vec_get(vec_t *vec, int index) {
	assert(index >= 0);
	assert(index < vec->size);

	return vec->data[index];
}

void vec_set(vec_t *vec, int index, void *data) {
	assert(index < vec_size(vec));
	assert(index >= 0);

	vec->data[index] = data;
}
