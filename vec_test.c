#include <assert.h>
#include <stdio.h>

#include "err.h"
#include "vec.h"

static void test_simple_operations() {
	vec_t v;
	vec_init(&v);

	int data[] = {1, 2, 3};

	assert(vec_size(&v) == 0);
	const long out_of_range_index = 123;
	assert(IS_ERR_PTR(vec_get(&v, out_of_range_index)));
	assert(IS_ERR_VALUE(vec_set(&v, out_of_range_index, "")));

	vec_push(&v, data + 0);
	assert(vec_size(&v) == 1);
	assert(*(int *)vec_get(&v, 0) == 1);

	vec_set(&v, 0, data + 1);
	assert(*(int *)vec_get(&v, 0) == 2);

	int *last = vec_pop(&v);
	assert(*last == 2);

	vec_deinit(&v);
}

static void test_large_collection() {
	vec_t v;
	vec_init(&v);

	int arr[100];
	for (int i = 0; i < 100; i++) {
		arr[i] = i;
		vec_push(&v, arr + i);
		assert(vec_size(&v) == i + 1);
		assert(*(int *)vec_get(&v, i) == i);
	}

	for (int i = 99; i >= 0; i--) {
		int *last = vec_pop(&v);
		assert(*last == i);
	}

	vec_deinit(&v);
}

void test_vec() {
	test_simple_operations();
	printf("[ok] test_vec: simple operations\n");
	test_large_collection();
	printf("[ok] test_vec: large collection\n");
}
