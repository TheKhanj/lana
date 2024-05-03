#include <assert.h>
#include <stdlib.h>

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

int vec_size(vec_t *self) { return self->size; }

void vec_push(vec_t *self, void *data) {
	self->data[self->size++] = data;

	if (self->size == self->max_size) {
		self->max_size *= 2;
		self->data = realloc(self->data, self->max_size * sizeof(void *));
	}
}

void *vec_pop(vec_t *self) { return self->data[--self->size]; }

void *vec_get(vec_t *self, int index) {
	assert(index >= 0);
	assert(index < self->size);

	return self->data[index];
}

void vec_set(vec_t *self, int index, void *data) {
	assert(index < vec_size(self));
	assert(index >= 0);

	self->data[index] = data;
}
