#ifndef INI_H
#define INI_H

#include <stddef.h>

#include "err.h"
#include "map.h"

typedef struct {
	map_t _sections;
} ini_t;

void ini_init(ini_t *self);
void ini_deinit(ini_t *self);

void ini_set(ini_t *self, char *section, char *key, char *value);
char *ini_get(ini_t *self, char *section, char *key);
err_t ini_del(ini_t *self, char *section, char *key);
size_t ini_size(ini_t *self);
void ini_sections(ini_t *self, char **sections);
long ini_section_size(ini_t *self, char *section);
err_t ini_section_keys(ini_t *self, char *section, char **keys);

#endif // INI_H
