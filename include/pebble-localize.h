#pragma once
#include "hash.h"

#define _(str) localize_str(HASH_DJB2(str))

void localize_init(uint32_t resource_locale);

void localize_deinit();

void localize_set_cache_size(uint16_t size);

const char *localize_str(int hashval);
