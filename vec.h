#ifndef VEC_H
#define VEC_H

typedef struct {
	int size;
	int max_size;

	void **data;
} vec_t;

void vec_init(vec_t *vec);
void vec_deinit(vec_t *vec);

int vec_size(vec_t *vec);
void vec_push(vec_t *vec, void *data);
void *vec_pop(vec_t *vec);
void *vec_get(vec_t *vec, int index);
void vec_set(vec_t *vec, int index, void *data);

#endif // VEC_H
