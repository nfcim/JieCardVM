#include "interpreter.h"
#include "types.h"
#include <stdint.h>

static u1 *pc;

u1 ByteCodeReadU1(void) { return *pc++; }

u2 ByteCodeReadU2(void) {
  u1 v1 = *pc++;
  u1 v2 = *pc++;
  return (v1 << 8) | v2;
}

u4 ByteCodeReadU4(void) {
  u1 v1 = *pc++;
  u1 v2 = *pc++;
  u1 v3 = *pc++;
  u1 v4 = *pc++;
  return (v1 << 24 | (v2 << 16) | (v3 << 8) | v4);
}

void ByteCodeOffset(int16_t offset) { pc += offset; }

u1 *ByteCodeGetPC(void) { return pc; }

void ByteCodeSetPC(u1 *new_pc) { pc = new_pc; }
