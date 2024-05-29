#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "err.h"
#include "ini.h"
#include "ini_parser.h"
#include "ini_parser_test.h"

void test_writing() {
	ini_parser_t parser;
	ini_t ini;

	ini_init(&ini);
	ini_set(&ini, "section", "key", "value");
	ini_parser_init(&parser, "test.ini", &ini);

	ini_parser_write(&parser);

	ini_deinit(&ini);
	ini_parser_deinit(&parser);
}

void test_reading() {
	ini_parser_t parser;
	ini_t ini;
	ini_init(&ini);
	ini_parser_init(&parser, "test.ini", &ini);

	err_t err = ini_parser_read(&parser);
	assert(IS_ERR_VALUE(err) == false);
	assert(ini_size(&ini) == 1);
	assert(ini_section_size(&ini, "section") == 1);
	assert(strcmp(ini_get(&ini, "section", "key"), "value") == 0);

	ini_parser_deinit(&parser);
	remove("test.ini");
}

void test_ini_parser() {
	test_writing();
	printf("[ok] ini_parser_test: test writing\n");

	test_reading();
	printf("[ok] ini_parser_test: test reading\n");
}
