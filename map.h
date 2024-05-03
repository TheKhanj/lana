#ifndef MAP_H
#define MAP_H

#include "vec.h"

typedef struct {
	vec_t key_values;
} map_t;

void map_init(map_t *self);
void map_deinit(map_t *self);

int map_size(map_t *self);
void map_keys(map_t *self, char **ret);
void map_set(map_t *self, char *key, void *value);
void *map_get(map_t *self, char *key);
void map_del(map_t *self, char *key);
int map_has(map_t *self, char *key);

#endif // MAP_H
