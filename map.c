#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "vec.h"

typedef struct {
	char *key;
	void *value;
} key_value_t;

void map_init(map_t *map) { vec_init(&map->key_values); }

void map_deinit(map_t *map) {
	for (int i = 0; i < map_size(map); i++) {
		key_value_t *kv = vec_get(&map->key_values, i);
		free(kv);
	}

	vec_deinit(&map->key_values);
}

int map_size(map_t *map) { return vec_size(&map->key_values); }

void map_keys(map_t *map, char **ret) {
	// TODO: assert ret.size() to be at least map size

	for (int i = 0; i < map_size(map); i++) {
		key_value_t *kv = (key_value_t *)vec_get(&map->key_values, i);

		ret[i] = kv->key;
	}
}

static int find_key_value_index(map_t *map, char *key) {
	for (int i = 0; i < map_size(map); i++) {
		key_value_t *kv = vec_get(&map->key_values, i);
		if (strcmp(kv->key, key) == 0)
			return i;
	}

	return -1;
}

static key_value_t *find_key_value(map_t *map, char *key) {
	int index = find_key_value_index(map, key);
	if (index == -1)
		return NULL;

	return (key_value_t *)(vec_get(&map->key_values, index));
}

void map_set(map_t *map, char *key, void *value) {
	key_value_t *found_key = find_key_value(map, key);

	if (found_key) {
		found_key->value = value;
		return;
	}

	found_key = malloc(sizeof(key_value_t));

	vec_push(&map->key_values, found_key);
}

void *map_get(map_t *map, char *key) {
	key_value_t *kv = find_key_value(map, key);

	return kv->value;
}

void map_del(map_t *map, char *key) {
	int index = find_key_value_index(map, key);
	assert(index != -1);

	for (int i = index; i < vec_size(&map->key_values) - 1; ++i) {
		vec_set(&map->key_values, i, vec_get(&map->key_values, i + 1));
	}

	key_value_t *last = vec_pop(&map->key_values);
	free(last);
}

int map_exists(map_t *map, char *key) {
	int index = find_key_value_index(map, key);

	return index != -1;
}
