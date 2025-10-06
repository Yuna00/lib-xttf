#ifndef LOAD_H_
#define LOAD_H_
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
// clang-format off
#define is_safe_offset(f, offset, margin)                                      \
  (((uint32_t)f->sz - (uint32_t)offset) > (uint32_t)margin)
/*
#define getu16(f, offset) \
	((uint32_t)((uint8_t*)(f)->mem)[(offset)] << 8 | \
	(uint32_t)((uint8_t*)(f)->mem)[(offset) + 1])

#define getu32(f, offset) \
((uint32_t)((uint8_t*)(f)->mem)[(offset)] << 24 | \
	(uint32_t)((uint8_t*)(f)->mem)[(offset) + 1] << 16 | \
	(uint32_t)((uint8_t*)(f)->mem)[(offset) + 2] << 8 | \
	(uint32_t)((uint8_t*)(f)->mem)[(offset) + 3])

#define geti16(f, offset) (int16_t)getu16(f, offset)
#define geti32(f, offset) (int32_t)getu32(f, offset)
*/
#define HEADER_SZ 12
#define TABLE_SZ 16

typedef struct {
  uint32_t tag;
  uint32_t checksum; // might implement in the future so its a placeholder for now
  uint32_t offset;
  uint32_t length;
} XTTF_TDir;

typedef struct {
  size_t sz;
  uint32_t *mem;
} XTTF_Font;


XTTF_Font *load_font(char *fn);

#endif // !LOAD_H_
