#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "err.h"
#include "ini.h"
#include "map.h"

static void add_section(ini_t *self, char *section_name) {
	map_t *section;

	section = malloc(sizeof(map_t));
	map_init(section);

	map_set(&self->_sections, section_name, section);
}

static void delete_section(ini_t *self, char *section_name) {
	map_t *section = map_get(&self->_sections, section_name);

	map_deinit(section);
	free(section);

	map_del(&self->_sections, section_name);
}

static int has_section(ini_t *self, char *section_name) {
	return map_has(&self->_sections, section_name);
}

void ini_init(ini_t *self) { map_init(&self->_sections); }

void ini_deinit(ini_t *self) { map_deinit(&self->_sections); }

void ini_set(ini_t *self, char *section_name, char *key, char *value) {
	map_t *section;

	if (!has_section(self, section_name))
		add_section(self, section_name);

	section = map_get(&self->_sections, section_name);

	map_set(section, key, value);
}

char *ini_get(ini_t *self, char *section_name, char *key) {
	if (!has_section(self, section_name))
		return CAST_TO_ERR_PTR(INI_ERR(section_not_found));

	map_t *section = map_get(&self->_sections, section_name);

	if (!map_has(section, key))
		return CAST_TO_ERR_PTR(INI_ERR(key_not_found_in_section));

	return map_get(section, key);
}

err_t ini_del(ini_t *self, char *section_name, char *key) {
	if (!has_section(self, section_name))
		return INI_ERR(section_not_found);

	map_t *section = map_get(&self->_sections, section_name);

	if (!map_has(section, key))
		return INI_ERR(key_not_found_in_section);

	map_del(section, key);

	if (map_size(section) == 0)
		delete_section(self, section_name);

	return 0;
}

size_t ini_size(ini_t *self) { return map_size(&self->_sections); }

void ini_sections(ini_t *self, char **sections) {
	map_keys(&self->_sections, sections);
}

long ini_section_size(ini_t *self, char *section_name) {
	if (!has_section(self, section_name))
		return INI_ERR(section_not_found);

	map_t *section = map_get(&self->_sections, section_name);

	return map_size(section);
}

err_t ini_section_keys(ini_t *self, char *section_name, char **keys) {
	if (!has_section(self, section_name))
		return INI_ERR(section_not_found);

	map_t *section = map_get(&self->_sections, section_name);
	map_keys(section, keys);

	return 0;
}
