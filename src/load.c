#include "../inc/load.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// clang-format off
#define ERR_CK(condition, ...)                                                      \
  if ((condition)) { printf(__VA_ARGS__); goto ERR; }
// clang-format on

static uint16_t numTables;
static uint8_t u16[2];
static uint8_t u32[4];

// could make this a macro but typechecking is a nice sanity check
static inline uint16_t getu16(int fd, uint32_t offset) {
  pread(fd, &u16, 2, offset);
  return ((uint16_t)u16[0] << 8 | (uint16_t)u16[1]);
}
static inline uint32_t getu32(int fd, uint32_t offset) {
  pread(fd, &u32, 4, offset);
  return ((uint32_t)u32[0] << 24 | (uint32_t)u32[1] << 16 |
          (uint32_t)u32[2] << 8 | (uint32_t)u32[3]);
}

static void stack_table_offset(int fd, XTTF_Font *f, XTTF_TDir table[]) {
  for (int i = HEADER_SZ; i < numTables * TABLE_SZ; i += TABLE_SZ) {

    table[i].tag = getu32(fd, i);
    table[i].offset = getu32(fd, i + 8);
    printf("TAG: %c%c%c%c\n", u32[0], u32[1], u32[2], u32[3]);
    printf("OFFSET: %u\n", table[i].offset);
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
  // ERR_CK(!(f->mem = (uint32_t *)malloc(f->sz)), "malloc l38")
  printf("f->sz = %lu\n", f->sz);
  //  tables
  uint32_t head, hhea, ScalerType;
  // ERR_CK((is_safe_offset(f, 0, 12)), "l45 offset")
  ScalerType = getu32(fd, 0);

  printf("scaler %d", ScalerType);

  //  sfnt version has to be one of these numbers according to microsoft docs
  //  otherwise the font file is not following the same specification that
  //  my code expects
  //  this error check also ensures that my byte rearrangement converts to the
  //  correct endianness
  ERR_CK(((ScalerType != 0x00010000 && ScalerType != 0x4F54544F)),
         "scaler = %u l57\n", ScalerType)

  numTables = getu16(fd, 4);
  printf("numTables: %d\n", (int)numTables);
  ERR_CK(is_safe_offset(f, 12, numTables * TABLE_SZ) == 0,
         "l62\n f->sz - offset: %d\n margin: %d", (uint32_t)f->sz - 12,
         ((uint32_t)(numTables * TABLE_SZ)))

  goto CONTINUE;

ERR:
  printf("\nERR");
  close(fd);
  free(f);
  return NULL;
CONTINUE:

  XTTF_TDir table[numTables];
  stack_table_offset(fd, f, table);

  //   printf("%u", table[1]->tag);
  //   ERR_CK((gettable(f, "head", &head) < 0));

  /*  ERR_CK(!is_safe_offset(f, head, 54));
    f->unitsPerEm = getu16(f, head + 18);
    f->locaFormat = geti16(f, head + 50);

    ERR_CK((gettable(f, "hhea", &hhea) < 0));
    ERR_CK((!is_safe_offset(f, hhea, 36)));
    f->numLongHmtx = getu16(f, hhea + 34);*/

  return f;
}
