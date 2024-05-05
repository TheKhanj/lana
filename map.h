#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#include "err.h"
#include "vec.h"

typedef struct {
	vec_t key_values;
} map_t;

void map_init(map_t *self);
void map_deinit(map_t *self);

long map_size(map_t *self);
void map_keys(map_t *self, char **ret);
void map_set(map_t *self, char *key, void *value);
void *map_get(map_t *self, char *key);
err_t map_del(map_t *self, char *key);
bool map_has(map_t *self, char *key);

#endif // MAP_H
