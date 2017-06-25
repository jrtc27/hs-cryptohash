#ifndef ALIGN_H
#define ALIGN_H

#include "bitfn.h"

#if (defined(__i386__))
# define UNALIGNED_ACCESS_OK
#elif defined(__x86_64__)
# define UNALIGNED_ACCESS_OK
#else
# define UNALIGNED_ACCESS_FAULT
#endif

/* n need to be power of 2.
 * IS_ALIGNED(p,8) */
#define IS_ALIGNED(p,alignment) (((uintptr_t) (p)) & ((alignment)-1))

#ifdef WITH_ASSERT_ALIGNMENT
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
# define ASSERT_ALIGNMENT(up, alignment) \
	do { if (IS_ALIGNED(up, alignment)) \
	{ printf("ALIGNMENT-ASSERT-FAILURE: %s:%d: ptr=%p alignment=%d\n", __FILE__, __LINE__, (void *) up, (alignment)); \
	  exit(99); \
	}; } while (0)
#else
# define ASSERT_ALIGNMENT(p, n) do {} while (0)
#endif

#ifdef UNALIGNED_ACCESS_OK
#define need_alignment(p,n) (0)
#else
#define need_alignment(p,n) IS_ALIGNED(p,n)
#endif

static inline uint32_t load_be32_aligned(const uint8_t *p)
{
	return be32_to_cpu(*((uint32_t *) p));
}

static inline uint64_t load_be64_aligned(const uint8_t *p)
{
	return be64_to_cpu(*((uint64_t *) p));
}

static inline void store_be32_aligned(uint8_t *p, uint32_t val)
{
	*((uint32_t *) p) = cpu_to_be32(val);
}

static inline void store_be64_aligned(uint8_t *p, uint64_t val)
{
	*((uint64_t *) p) = cpu_to_be64(val);
}

static inline uint32_t load_le32_aligned(const uint8_t *p)
{
	return le32_to_cpu(*((uint32_t *) p));
}

static inline uint64_t load_le64_aligned(const uint8_t *p)
{
	return le64_to_cpu(*((uint64_t *) p));
}

static inline void store_le32_aligned(uint8_t *p, uint32_t val)
{
	*((uint32_t *) p) = cpu_to_le32(val);
}

static inline void store_le64_aligned(uint8_t *p, uint64_t val)
{
	*((uint64_t *) p) = cpu_to_le64(val);
}

#ifdef UNALIGNED_ACCESS_OK

#define load_be32(p) load_be32_aligned(p)
#define load_be64(p) load_be64_aligned(p)

#define store_be32(p, v) store_be32_aligned((p), (v))
#define store_be64(p, v) store_be64_aligned((p), (v))

#define load_le32(p) load_le32_aligned(p)
#define load_le64(p) load_le64_aligned(p)

#define store_le32(p, v) store_le32_aligned((p), (v))
#define store_le64(p, v) store_le64_aligned((p), (v))

#else

static inline uint32_t load_be32(const uint8_t *p)
{
	return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] <<  8) | ((uint32_t)p[3]);
}

static inline uint64_t load_be64(const uint8_t *p)
{
	return ((uint64_t)p[0] << 56) | ((uint64_t)p[1] << 48) | ((uint64_t)p[2] << 40) | ((uint64_t)p[3] << 32) |
	       ((uint64_t)p[4] << 24) | ((uint64_t)p[5] << 16) | ((uint64_t)p[6] <<  8) | ((uint64_t)p[7]);
}

static inline void store_be32(uint8_t *p, uint32_t val)
{
	p[0] = (val >> 24);
	p[1] = (val >> 16) & 0xFF;
	p[2] = (val >>  8) & 0xFF;
	p[3] = (val      ) & 0xFF;
}

static inline void store_be64(uint8_t *p, uint64_t val)
{
	p[0] = (val >> 56);
	p[1] = (val >> 48) & 0xFF;
	p[2] = (val >> 40) & 0xFF;
	p[3] = (val >> 32) & 0xFF;
	p[4] = (val >> 24) & 0xFF;
	p[5] = (val >> 16) & 0xFF;
	p[6] = (val >>  8) & 0xFF;
	p[7] = (val      ) & 0xFF;
}

static inline uint32_t load_le32(const uint8_t *p)
{
	return ((uint32_t)p[0]) | ((uint32_t)p[1] <<  8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static inline uint64_t load_le64(const uint8_t *p)
{
	return ((uint64_t)p[0]) | ((uint64_t)p[1] <<  8) | ((uint64_t)p[2] << 16) | ((uint64_t)p[3] << 24) |
	       ((uint64_t)p[4] << 32) | ((uint64_t)p[5] << 40) | ((uint64_t)p[6] << 48) | ((uint64_t)p[7] << 56);
}

static inline void store_le32(uint8_t *p, uint32_t val)
{
	p[0] = (val      ) & 0xFF;
	p[1] = (val >>  8) & 0xFF;
	p[2] = (val >> 16) & 0xFF;
	p[3] = (val >> 24);
}

static inline void store_le64(uint8_t *p, uint64_t val)
{
	p[0] = (val      ) & 0xFF;
	p[1] = (val >>  8) & 0xFF;
	p[2] = (val >> 16) & 0xFF;
	p[3] = (val >> 24) & 0xFF;
	p[4] = (val >> 32) & 0xFF;
	p[5] = (val >> 40) & 0xFF;
	p[6] = (val >> 48) & 0xFF;
	p[7] = (val >> 56);
}

#endif

#endif
