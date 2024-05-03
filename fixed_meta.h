#ifndef FIXED_META_H
#define FIXED_META_H

#include "map.h"
#include "tracker.h"
#include "vec.h"

typedef struct {
	map_t map;
} fixed_meta_t;

void read_fixed_meta(fixed_meta_t *self, resource_t *res);
void write_fixed_meta(fixed_meta_t *self, resource_t *res);

void fixed_meta_init(fixed_meta_t *self);
void fixed_meta_deinit(fixed_meta_t *self);

int fixed_meta_has(fixed_meta_t *self, char *key, char *value);
void fixed_meta_add(fixed_meta_t *self, char *key, char *value);
void fixed_meta_remove(fixed_meta_t *self, char *key, char *value);

#endif // FIXED_META_H
