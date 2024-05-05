#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "err.h"
#include "map.h"
#include "map_test.h"

static void test_simple_operations() {
	map_t map;
	map_init(&map);

	assert(map_size(&map) == 0);
	assert(IS_ERR(map_get(&map, "non-existing-key")));
	assert(IS_ERR_VALUE(map_del(&map, "non-existing-key")));

	map_set(&map, "key", "value");
	assert(map_size(&map) == 1);
	assert(strcmp((char *)map_get(&map, "key"), "value") == 0);

	map_set(&map, "key", "new_value");
	assert(map_size(&map) == 1);
	assert(strcmp((char *)map_get(&map, "key"), "new_value") == 0);

	char *keys[1];
	map_keys(&map, keys);
	assert(strcmp(keys[0], "key") == 0);
	assert(map_has(&map, "key"));

	map_del(&map, "key");
	assert(map_size(&map) == 0);
	assert(!map_has(&map, "key"));

	map_deinit(&map);
}

static void test_large_collection() {
	map_t map;
	map_init(&map);

	int data[100];
	for (int i = 0; i < 100; ++i) {
		data[i] = i;

		char key[100];
		sprintf(key, "key-%d", i);

		map_set(&map, key, data + i);
		assert(map_size(&map) == i + 1);
		assert(*(int *)map_get(&map, key) == i);
		assert(map_has(&map, key));
	}

	for (int i = 99; i >= 0; --i) {
		char key[100];
		sprintf(key, "key-%d", i);
		map_del(&map, key);
		assert(!map_has(&map, key));
	}

	map_deinit(&map);
}

void test_map() {
	test_simple_operations();
	printf("[ok] map_test: simple operations\n");
	test_large_collection();
	printf("[ok] map_test: large collection\n");
}
