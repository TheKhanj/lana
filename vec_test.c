#include <assert.h>
#include <stdio.h>

#include "vec.h"

static void test_simple_operations() {
	vec_t v;
	vec_init(&v);

	int data[] = {1, 2, 3};

	assert(vec_size(&v) == 0);

	vec_push(&v, data + 0);
	assert(vec_size(&v) == 1);
	assert(*(int *)vec_get(&v, 0) == 1);

	vec_set(&v, 0, data + 1);
	assert(*(int *)vec_get(&v, 0) == 2);

	int *last = vec_pop(&v);
	assert(*last == 2);

	vec_deinit(&v);
}

static void test_reallocation() {
	vec_t v;
	vec_init(&v);

	int arr[100];
	for (int i = 0; i < 100; i++) {
		arr[i] = i;
		vec_push(&v, arr + i);
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
	test_reallocation();
	printf("[ok] test_vec: reallocation\n");
}