#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "fixed_meta.h"
#include "fixed_meta_test.h"
#include "init.h"
#include "map.h"
#include "vec.h"

static void test_simple_operations() {
	fixed_meta_t meta;
	fixed_meta_init(&meta);

	assert(fixed_meta_has(&meta, "key", "value-1") == 0);

	fixed_meta_add(&meta, "key", "value-1");
	assert(fixed_meta_has(&meta, "key", "value-1") == 1);
	assert(fixed_meta_has(&meta, "key", "value-2") == 0);

	fixed_meta_add(&meta, "key", "value-2");
	assert(fixed_meta_has(&meta, "key", "value-1") == 1);
	assert(fixed_meta_has(&meta, "key", "value-2") == 1);

	fixed_meta_remove(&meta, "key", "value-2");
	assert(fixed_meta_has(&meta, "key", "value-1") == 1);
	assert(fixed_meta_has(&meta, "key", "value-2") == 0);

	fixed_meta_remove(&meta, "key", "value-1");
	assert(fixed_meta_has(&meta, "key", "value-1") == 0);
	assert(fixed_meta_has(&meta, "key", "value-2") == 0);

	fixed_meta_deinit(&meta);
}

static void assert_same_meta(fixed_meta_t *m1, fixed_meta_t *m2) {
	assert(map_size(&m1->map) == map_size(&m2->map));

	char *keys[map_size(&m1->map)];
	map_keys(&m1->map, keys);
	char *key = keys[0];

	vec_t *v1 = map_get(&m1->map, key);
	vec_t *v2 = map_get(&m2->map, key);

	assert(vec_size(v1) == vec_size(v2));

	for (int i = 0; i < vec_size(v1); i++) {
		char *value1 = vec_get(v1, i);
		char *value2 = vec_get(v2, i);

		assert(strcmp(value1, value2) == 0);
	}
}

static void test_read_and_write() {
	lana_init(".");
	fixed_meta_t meta;
	fixed_meta_init(&meta);
	fixed_meta_add(&meta, "key", "value-1");
	fixed_meta_add(&meta, "key", "value-2");

	resource_t res;
	res.path = "test_file";
	res.id = "test_id";

	write_fixed_meta(&meta, &res);

	fixed_meta_t reading_meta;
	read_fixed_meta(&reading_meta, &res);

	assert_same_meta(&meta, &reading_meta);

	fixed_meta_deinit(&meta);
}

void test_fixed_meta() {
	test_simple_operations();
	printf("[ok] fixed_meta_test: simple operations\n");
	test_read_and_write();
	printf("[ok] fixed_meta_test: read and write\n");
}
