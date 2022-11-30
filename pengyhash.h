#ifndef _PENGYHASH_H
#define _PENGYHASH_H

#include <stdint.h>
#include <stddef.h>

uint64_t pengyhash(const void *p, size_t size, uint64_t seed);

#endif
