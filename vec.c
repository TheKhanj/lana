#include <stdbool.h>
#include <stdlib.h>

#include "err.h"
#include "vec.h"

void vec_init(vec_t *self) {
	self->size = 0;
	self->max_size = 1;

	self->data = (void **)malloc(self->max_size * sizeof(void *));
}

void vec_deinit(vec_t *self) {
	free(self->data);
	self->data = NULL;
}

long vec_size(vec_t *self) { return self->size; }

void vec_push(vec_t *self, void *data) {
	self->data[self->size++] = data;

	if (self->size == self->max_size) {
		self->max_size *= 2;
		self->data = realloc(self->data, self->max_size * sizeof(void *));
	}
}

void *vec_pop(vec_t *self) { return self->data[--self->size]; }

static bool is_out_of_range(vec_t *v, long index) {
	return index < 0 || index >= v->size;
}

void *vec_get(vec_t *self, long index) {
	if (is_out_of_range(self, index))
		return CAST_TO_ERR_PTR(VEC_ERR(out_of_range));

	return self->data[index];
}

err_t vec_set(vec_t *self, long index, void *data) {
	if (is_out_of_range(self, index))
		return VEC_ERR(out_of_range);

	self->data[index] = data;

	return 0;
}
