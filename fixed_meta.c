#include <alloca.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fixed_meta.h"
#include "map.h"
#include "tracker.h"
#include "utils.h"
#include "vec.h"

static void get_meta_dir(char *ret) {
	strcpy(ret, get_lana_dir());
	strcat(ret, "/fixed_meta");
}

static void get_meta_file(char *ret, char *id) {
	get_meta_dir(ret);
	strcat(ret, "/");
	strcat(ret, id);
}

static void parse_values(fixed_meta_t *self, char *line, char *key) {
	int len = strlen(line);

	int size = 0;
	for (int i = 0; i < len; i++) {
		char c = line[i];
		if (c != '\n' && c != ',') {
			size++;
			continue;
		}

		char *str = malloc((size + 1) * sizeof(char));
		char *start = line + i - size;
		start[size] = '\0';
		strcpy(str, start);

		fixed_meta_add(self, key, str);

		size = 0;
	}
}

static void parse_line(fixed_meta_t *self, char *line) {
	int is_comment = line[0] == '#';

	if (is_comment)
		return;

	int len = strlen(line);
	int equal = 0;
	for (int i = 0; i < len; ++i) {
		if (line[i] != '=')
			continue;
		equal = i;
		break;
	}

	assert(equal != len);
	char *key = malloc((equal + 1) * sizeof(char));
	line[equal] = '\0';
	strcpy(key, line);

	parse_values(self, line + equal + 1, key);
}

static void assert_meta_file(resource_t *res) {
	char str[1024];

	char *meta_dir = str;
	get_meta_dir(meta_dir);
	assert_dir(meta_dir);

	char *meta_file = str;
	get_meta_file(str, res->id);
	assert_file(meta_file);
}

void read_fixed_meta(fixed_meta_t *self, resource_t *res) {
	fixed_meta_init(self);

	assert_meta_file(res);
	char meta_file[1024];
	get_meta_file(meta_file, res->id);

	FILE *file = fopen(meta_file, "r");

	if (file == NULL)
		crash("failed opening file for reading");

	char line[1024];
	while (fgets(line, sizeof(line), file))
		parse_line(self, line);
	if (fclose(file) != 0)
		crash("failed closing file after read");
};

void write_fixed_meta(fixed_meta_t *self, resource_t *res) {
	assert_meta_file(res);

	char meta_file[1024];
	get_meta_file(meta_file, res->id);

	FILE *file = fopen(meta_file, "w");

	if (file == NULL)
		crash("failed opening file for writing");

	fprintf(file, "# vi: ft=sh\n");

	int mp_size = map_size(&self->map);
	char *keys[mp_size];
	map_keys(&self->map, keys);

	for (int i = 0; i < mp_size; i++) {
		char *key = keys[i];
		vec_t *vec = map_get(&self->map, key);
		int v_size = vec_size(vec);

		if (v_size == 0)
			continue;

		fprintf(file, "%s=", key);

		for (int j = 0; j < v_size; ++j) {
			char *value = vec_get(vec, j);
			fprintf(file, "%s", value);
			if (j != v_size - 1)
				fprintf(file, ",");
		}

		fprintf(file, "\n");
	}

	if (fclose(file) != 0)
		crash("failed closing file after write");
}

void fixed_meta_init(fixed_meta_t *self) { map_init(&self->map); }

void fixed_meta_deinit(fixed_meta_t *self) {
	int size = map_size(&self->map);

	char *keys[size];
	map_keys(&self->map, keys);
	for (int i = 0; i < size; i++) {
		char *key = keys[i];
		vec_t *values = map_get(&self->map, key);
		int v_size = vec_size(values);
		for (int j = 0; j < v_size; ++j) {
			char *str = vec_get(values, j);
			free(str);
		}
	}
	map_deinit(&self->map);
}

int fixed_meta_has(fixed_meta_t *self, char *key, char *value) {
	if (!map_has(&self->map, key))
		return 0;

	vec_t *v = map_get(&self->map, key);
	int v_size = vec_size(v);

	for (int i = 0; i < v_size; ++i) {
		char *value_i = vec_get(v, i);
		if (strcmp(value_i, value) == 0)
			return 1;
	}

	return 0;
}

static int get_index_in_vector(vec_t *vec, char *value) {
	int v_size = vec_size(vec);
	for (int i = 0; i < v_size; ++i) {
		char *vec_value = vec_get(vec, i);
		int already_exists = strcmp(value, vec_value) == 0;

		if (already_exists)
			return i;
	}

	return -1;
}

void fixed_meta_add(fixed_meta_t *self, char *key, char *value) {
	vec_t *vec = map_get(&self->map, key);

	if (!vec) {
		vec = malloc(sizeof(vec_t));
		vec_init(vec);
		map_set(&self->map, key, vec);
	}

	int exists_in_vector = get_index_in_vector(vec, value) != -1;

	if (exists_in_vector)
		return;

	char *allocated_value = malloc((strlen(value) + 1) * sizeof(char));
	strcpy(allocated_value, value);

	vec_push(vec, allocated_value);
}

void fixed_meta_remove(fixed_meta_t *self, char *key, char *value) {
	vec_t *vec = map_get(&self->map, key);

	int index = get_index_in_vector(vec, value);
	int exists_in_vector = index != -1;

	assert(exists_in_vector);

	int v_size = vec_size(vec);
	for (int i = index; i < v_size - 1; i++)
		vec_set(vec, i, vec_get(vec, i + 1));

	char *last = vec_pop(vec);
	free(last);

	if (vec_size(vec) == 0) {
		map_del(&self->map, key);
		vec_deinit(vec);
	}
}
