#ifndef MAP_H
#define MAP_H

#include "vec.h"

typedef struct {
	vec_t key_values;
} map_t;

void map_init(map_t *map);
void map_deinit(map_t *map);

int map_size(map_t *map);
void map_keys(map_t *map, char **ret);
void map_set(map_t *map, char *key, void *value);
void *map_get(map_t *map, char *key);
void map_del(map_t* map, char * key);
int map_exists(map_t *map, char *key);

#endif // MAP_H
