/* pengyhash v0.3 - GNU GPLv3, see LICENSE file */

#include "pengyhash.h"

#define V64(p) ( \
    (uint64_t)(p)[0] | (uint64_t)(p)[1]<<8 | (uint64_t)(p)[2]<<16 | (uint64_t)(p)[3]<<24 | \
	(uint64_t)(p)[4]<<32 | (uint64_t)(p)[5]<<40 | (uint64_t)(p)[6]<<48 | (uint64_t)(p)[7]<<56 \
)


uint64_t pengyhash(const void *buf, size_t size, uint64_t seed)
{
	const uint8_t *p = (const uint8_t*) buf;
	uint64_t s[4] = { 0 };
	uint64_t f[4] = { 0 };
	uint8_t i;

	for(*s = size; size >= 32; size -= 32, p += 32) {
		s[1] = (s[0] += (s[1] += V64(p+8)) + V64(p)) ^ (s[1] << 14 | s[1] >> 50);
		s[3] = (s[2] += (s[3] += V64(p+24)) + V64(p+16)) ^ (s[3] << 23 | s[3] >> 41);
		s[3] = (s[0] += (s[3] += V64(p+24)) + V64(p)) ^ (s[3] << 11 | s[3] >> 53);
		s[1] = (s[2] += (s[1] += V64(p+8)) + V64(p+16)) ^ (s[1] << 40 | s[1] >> 24);
	}

	for(i = 0; i + 8 < size; i += 8, p += 8) f[i/8] = V64(p);
	for(; i < size; i++) f[i/8] |= (uint64_t) p[i%8] << i%8*8;

	for(i = 0; i < 6; i++) {
		s[1] = (s[0] += (s[1] += f[1] + seed) + f[0]) ^ (s[1] << 14 | s[1] >> 50);
		s[3] = (s[2] += (s[3] += f[3]) + f[2]) ^ (s[3] << 23 | s[3] >> 41);
		s[3] = (s[0] += (s[3] += f[3]) + f[0]) ^ (s[3] << 9 | s[3] >> 55);
		s[1] = (s[2] += (s[1] += f[1]) + f[2]) ^ (s[1] << 40 | s[1] >> 24);
	}

	return s[0] + s[1] + s[2] + s[3];
}
