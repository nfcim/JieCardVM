#include "rtda.h"

static ByteCode byteCode;

jshort OSPop(OperandStack *s) { return *(--s->next); }

void OSPush(OperandStack *s, jshort val) { *s->next++ = val; }

jshort OSGet(OperandStack *s) { return *(s->next - 1); }

jshort VTGet(VariableTable *t, u1 index) { return t->base[index]; }

void VTSet(VariableTable *t, u1 index, jshort val) { t->base[index] = val; }

u1 BCReadU1(void) {
  if (byteCode.index < MAX_BC_INDEX)
    return byteCode.base[byteCode.index++];
  return 0; // TODO
}

u2 BCReadU2(void) {
  u1 v1 = BCReadU1();
  u1 v2 = BCReadU1();
  return (v1 << 8) | v2;
}

void BCJump(int16_t offset) { byteCode.index += offset; }

void BCSet(u1 *base) {
  byteCode.base = base;
  byteCode.index = 0;
}

u2 CPGetData(u2 index) { return 0; }

void CPSetData(u2 index, u2 val) {}

jshort ODGet(jshort objRef, jshort index) { return 0; }

void ODSet(jshort objRef, jshort index, jshort value) {}