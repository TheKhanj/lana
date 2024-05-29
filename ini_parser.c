#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "err.h"
#include "ini.h"
#include "ini_parser.h"

static bool check_line_length(ini_parser_t *self) {
	if (self->_line_length < INI_MAX_COL)
		return true;

	self->_parsing_err.column = 0;
	self->_parsing_err.message = "line length is more that maximum allowed";

	return false;
}

static bool is_section(char *line, col_t n) { return n >= 1 && line[0] == '['; }

static bool get_section_name(ini_parser_t *self) {
	if (self->_line[self->_line_length - 1] != ']') {
		self->_parsing_err.column = self->_line_length - 1;
		self->_parsing_err.message = "section line does not end with ]";

		return false;
	}

	for (col_t i = 1; i < self->_line_length - 1; i++) {
		char c = self->_line[i];

		if (!isalnum(c)) {
			self->_parsing_err.column = i;
			self->_parsing_err.message = "section name must be alphanumeric";

			return false;
		}
	}

	self->_line[self->_line_length - 1] = '\0';
	strcpy(self->_section, self->_line + 1);

	return true;
}

static bool validate_key(ini_parser_t *self) {
	col_t n = strlen(self->_key);

	for (col_t i = 0; i < n; ++i) {
		char c = self->_key[i];
		if (isalnum(c))
			continue;

		self->_parsing_err.column = i;
		self->_parsing_err.message = "key must be alphanumeric";
		return false;
	}

	return true;
}

static bool get_equal_sign(ini_parser_t *self) {
	col_t equal = self->_line_length;

	for (col_t i = 0; i < self->_line_length; i++)
		if (self->_line[i] == '=') {
			equal = i;
			break;
		}

	if (equal == self->_line_length) {
		self->_parsing_err.column = 0;
		self->_parsing_err.message = "equal sign not found in line";

		return false;
	}

	self->_equal = equal;
	return true;
}

static bool get_key(ini_parser_t *self) {
	self->_line[self->_equal] = '\0';
	strcpy(self->_key, self->_line);
	if (!validate_key(self))
		return false;

	return true;
}

static bool is_all_whitespace(char *line, col_t n) {
	for (col_t i = 0; i < n; i++)
		if (!isspace(line[i]))
			return false;

	return true;
}

static bool get_value(ini_parser_t *self) {
	for (col_t i = self->_equal + 1; i < self->_line_length; ++i) {
		char c = self->_line[i];
		if (c != '=')
			continue;

		self->_parsing_err.column = i;
		self->_parsing_err.message = "equal signs are not allowed in value";

		return false;
	}

	strcpy(self->_value, self->_line + self->_equal + 1);

	return true;
}

static void write_section(ini_parser_t *self, char *section) {
	fprintf(self->_file, "[%s]\n", section);

	long key_count = ini_section_size(self->ini, section);
	char *keys[key_count];

	ini_section_keys(self->ini, section, keys);

	for (long i = 0; i < key_count; ++i) {
		char *key = keys[i];
		char *value = ini_get(self->ini, section, key);

		fprintf(self->_file, "%s=%s\n", key, value);
	}
}

void ini_parser_init(ini_parser_t *self, char *file, ini_t *ini) {
	self->file_path = file;
	self->ini = ini;
	self->_parsing_err.message = NULL;
}

void ini_parser_deinit(ini_parser_t *self) {}

err_t ini_parser_read(ini_parser_t *self) {
	err_t ret = 0;
	self->_file = fopen(self->file_path, "r");

	if (!self->_file) {
		ret = INI_PARSER_ERR(failed_openning_file);
		goto defer;
	}

	col_t line_no = 0;
	self->_parsing_err.line = line_no;

	while (fgets(self->_line, sizeof(self->_line), self->_file)) {
		col_t n = strlen(self->_line);

		if (self->_line[n - 1] == '\n')
			n--;
		if (self->_line[n - 1] == '\r')
			n--;
		self->_line[n] = '\0';
		self->_line_length = n;

		if (!check_line_length(self)) {
			ret = INI_PARSER_ERR(parsing_failed);
			goto defer;
		}

		if (is_all_whitespace(self->_line, n))
			continue;

		if (is_section(self->_line, n)) {
			if (get_section_name(self))
				continue;

			ret = INI_PARSER_ERR(parsing_failed);
			goto defer;
		}

		if (!get_equal_sign(self)) {
			ret = INI_PARSER_ERR(parsing_failed);
			goto defer;
		}
		if (!get_key(self)) {
			ret = INI_PARSER_ERR(parsing_failed);
			goto defer;
		}
		if (!get_value(self)) {
			ret = INI_PARSER_ERR(parsing_failed);
			goto defer;
		}

		ini_set(self->ini, self->_section, self->_key, self->_value);

		self->_parsing_err.line = ++line_no;
	}

defer:
	if (self->_file)
		fclose(self->_file);

	return ret;
}

err_t ini_parser_write(ini_parser_t *self) {
	self->_file = fopen(self->file_path, "w");

	err_t ret = 0;

	long section_count = ini_size(self->ini);
	char *sections[section_count];

	if (!self->_file) {
		ret = INI_PARSER_ERR(failed_openning_file);
		goto defer;
	}

	if (IS_ERR_VALUE(section_count)) {
		ret = CAST_TO_ERR_VALUE(section_count);
		goto defer;
	}

	ini_sections(self->ini, sections);

	for (long i = 0; i < section_count; ++i) {
		char *section = sections[i];
		write_section(self, section);

		if (i != section_count - 1)
			fprintf(self->_file, "\n");
	}

defer:
	if (self->_file)
		fclose(self->_file);

	return ret;
}
