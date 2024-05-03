#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "vec.h"

typedef struct {
	char *key;
	void *value;
} key_value_t;

void map_init(map_t *self) { vec_init(&self->key_values); }

void map_deinit(map_t *self) {
	for (int i = 0; i < map_size(self); i++) {
		key_value_t *kv = vec_get(&self->key_values, i);
		free(kv);
	}

	vec_deinit(&self->key_values);
}

int map_size(map_t *self) { return vec_size(&self->key_values); }

void map_keys(map_t *self, char **ret) {
	// TODO: assert ret.size() to be at least map size

	for (int i = 0; i < map_size(self); i++) {
		key_value_t *kv = (key_value_t *)vec_get(&self->key_values, i);

		ret[i] = kv->key;
	}
}

static int find_key_value_index(map_t *self, char *key) {
	int m_size = map_size(self);
	for (int i = 0; i < m_size; i++) {
		key_value_t *kv = vec_get(&self->key_values, i);
		if (strcmp(kv->key, key) == 0)
			return i;
	}

	return -1;
}

static key_value_t *find_key_value(map_t *self, char *key) {
	int index = find_key_value_index(self, key);
	if (index == -1)
		return NULL;

	return (key_value_t *)(vec_get(&self->key_values, index));
}

void map_set(map_t *self, char *key, void *value) {
	key_value_t *found_key_value = find_key_value(self, key);

	if (found_key_value) {
		found_key_value->value = value;
		return;
	}

	found_key_value = malloc(sizeof(key_value_t));
	found_key_value->key = malloc(strlen(key) + 1);
	strcpy(found_key_value->key, key);
	found_key_value->value = value;

	vec_push(&self->key_values, found_key_value);
}

void *map_get(map_t *self, char *key) {
	key_value_t *kv = find_key_value(self, key);

	if (!kv)
		return NULL;

	return kv->value;
}

void map_del(map_t *self, char *key) {
	int index = find_key_value_index(self, key);
	assert(index != -1);

	int v_size = vec_size(&self->key_values);
	for (int i = index; i < v_size - 1; ++i)
		vec_set(&self->key_values, i, vec_get(&self->key_values, i + 1));

	key_value_t *last = vec_pop(&self->key_values);
	free(last);
}

int map_has(map_t *self, char *key) {
	int index = find_key_value_index(self, key);

	return index != -1;
}
