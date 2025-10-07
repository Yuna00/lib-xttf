#include "../inc/load.h"
#include "../inc/util.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// clang-format off
#define ERR_CK(condition, ...)                                                      \
  if ((condition)) { printf(__VA_ARGS__); goto ERR; }
// clang-format on

static u16 numTables;

static inline uint16_t getu16(int fd, u16 *u, uint32_t offset) {
  pread(fd, u, 2, offset);
  return ((uint16_t)u->bytes[0] << 8 | (uint16_t)u->bytes[1]);
}

static inline uint32_t getu32(int fd, u32 *u, uint32_t offset) {
  pread(fd, u, 4, offset);

  return ((uint32_t)u->bytes[0] << 24 | (uint32_t)u->bytes[1] << 16 |
          (uint32_t)u->bytes[2] << 8 | (uint32_t)u->bytes[3]);
}

static void stack_table_offset(int fd, XTTF_TDir table[]) {
  int j = 0;
  for (int i = HEADER_SZ; i < numTables.val * TABLE_SZ; i += TABLE_SZ) {
    table[j].tag.val = getu32(fd, &table->tag, i);
    printf("TAG: %c%c%c%c\n", table->tag.bytes[0], table->tag.bytes[1],
           table->tag.bytes[2], table->tag.bytes[3]);
    table[j].offset.val = getu32(fd, &table->tag, i + 8);
    printf("OFFSET: %u\n", table[j].offset.val);
    j++;
  }
}
XTTF_Font *load_font(char *fn) {
  int fd;

  // allocate and zero out struct
  XTTF_Font *f = calloc(1, sizeof *f);
  ERR_CK(!f, "calloc l33");
  struct stat info;
  // get file size
  ERR_CK((fd = open(fn, O_RDONLY)) < 0, "fd l37");
  ERR_CK((fstat(fd, &info) < 0), "fstat l38");
  f->sz = info.st_size;
  //  ERR_CK(!(f->mem = (uint32_t *)malloc(f->sz)), "malloc l38")
  printf("f->sz = %lu\n", f->sz);
  //  tables
  u32 head, hhea, ScalerType;
  ERR_CK((is_safe_offset(f, 0, 12) == 0), "l45 offset")
  ScalerType.val = getu32(fd, &ScalerType, 0);

  printf("scaler %u", ScalerType.val);

  //  sfnt version has to be one of these numbers according to microsoft docs
  //  otherwise the font file is not following the same specification that
  //  my code expects
  //  this error check also ensures that my byte rearrangement converts to the
  //  correct endianness
  ERR_CK(((ScalerType.val != 0x00010000 && ScalerType.val != 0x4F54544F)),
         "scaler = %u l57\n", ScalerType.val)

  numTables.val = getu16(fd, &numTables, 4);
  printf("numTables: %d\n", (int)numTables.val);
  ERR_CK(is_safe_offset(f, 12, numTables.val * TABLE_SZ) == 0,
         "l62\n f->sz - offset: %d\n margin: %d", (uint32_t)f->sz - 12,
         ((uint32_t)(numTables.val * TABLE_SZ)))

  // empty scope because gotos dont like VLAs
  {
    XTTF_TDir table[numTables.val];

    // we allocate table offsets on the stack
    stack_table_offset(fd, table);
  }

  return f;
ERR:
  printf("\nERR");
  close(fd);
  free(f);
  return NULL;
}
