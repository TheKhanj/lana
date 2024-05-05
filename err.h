#ifndef ERR_H
#define ERR_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
	ERROR_no_error,

	ERROR_VEC_out_of_range,

	ERROR_MAP_key_not_found,

	ERROR_INI_section_not_found,
	ERROR_INI_key_not_found_in_section,

	ERROR_max_error
} err_code_t;

typedef long err_t;

#define PREFIX(name) (ERROR_##name)
#define LANA_ERR(name) ((err_t) - (PREFIX(name)))
#define VEC_ERR(name) ((err_t)LANA_ERR(VEC_##name))
#define MAP_ERR(name) ((err_t)LANA_ERR(MAP_##name))
#define INI_ERR(name) ((err_t)LANA_ERR(INI_##name))

static inline bool IS_ERR_VALUE(err_t err) { return err < 0; }
static inline bool IS_ERR(void *ptr) { return IS_ERR_VALUE((err_t)ptr); }
static inline void *ERR_PTR(err_t err) { return (void *)(err); }

#endif // ERR_H
