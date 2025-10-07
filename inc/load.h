#ifndef LOAD_H_
#define LOAD_H_
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
// clang-format off
#define is_safe_offset(f, offset, margin)                                      \
  (((uint32_t)f->sz - (uint32_t)offset) > (uint32_t)margin)

#define HEADER_SZ 12
#define TABLE_SZ 16

typedef union {
  uint8_t bytes[2];
  uint16_t val;
} u16;

typedef union {
  uint8_t bytes[4];
  uint32_t val;
} u32;

typedef struct {
  u32 tag;
  u32 checksum; // might implement in the future so its a placeholder for now
  u32 offset;
  u32 length;
} XTTF_TDir;

typedef struct {
  size_t sz;
  uint32_t *mem;
} XTTF_Font;


XTTF_Font *load_font(char *fn);

#endif // !LOAD_H_
