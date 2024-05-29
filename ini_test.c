#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "err.h"
#include "ini.h"
#include "ini_test.h"

static void test_basic_operations() {
	ini_t ini;
	ini_init(&ini);

	assert(ini_size(&ini) == 0);
	assert(IS_ERR_VALUE(ini_section_size(&ini, "section-1")));

	ini_set(&ini, "section-1", "key", "value");
	assert(ini_size(&ini) == 1);
	assert(ini_section_size(&ini, "section-1") == 1);
	char *keys[1];
	ini_section_keys(&ini, "section-1", keys);
	assert(strcmp(keys[0], "key") == 0);

	char *value = ini_get(&ini, "section-1", "key");
	assert(IS_ERR_PTR(value) == false);
	assert(strcmp(value, "value") == 0);

	ini_del(&ini, "section-1", "key");
	assert(ini_size(&ini) == 0);
	assert(IS_ERR_VALUE(ini_section_size(&ini, "section-1")));

	ini_deinit(&ini);
}

void test_ini() {
	test_basic_operations();
	printf("[ok] ini_test: test basic operations\n");
}
