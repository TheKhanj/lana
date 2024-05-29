#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "err.h"
#include "ini.h"

typedef unsigned short col_t;
typedef unsigned short line_t;

#define INI_MAX_COL ((col_t)(~0))
#define INI_MAX_LINE ((line_t)(~0))

typedef struct {
	char *message;
	line_t line;
	col_t column;
} ini_parse_err_t;

typedef struct {
	char *file_path;
	ini_t *ini;

	FILE *_file;

	ini_parse_err_t _parsing_err;

	col_t _line_length;
	char _line[INI_MAX_COL];
	col_t _equal;
	char _section[INI_MAX_COL], _key[INI_MAX_COL], _value[INI_MAX_COL];
} ini_parser_t;

void ini_parser_init(ini_parser_t *self, char *file, ini_t *ini);
void ini_parser_deinit(ini_parser_t *self);

err_t ini_parser_read(ini_parser_t *self);
err_t ini_parser_write(ini_parser_t *self);

#endif // INI_PARSER_H
