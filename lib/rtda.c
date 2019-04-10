#include "rtda.h"

static u1 *pc;
static u2 *data;

jshort OSPop(OperandStack *s) { return *(--s->next); }

void OSPush(OperandStack *s, jshort val) { *s->next++ = val; }

jshort OSGet(OperandStack *s) { return *(s->next - 1); }

jshort VTGet(VariableTable *t, u1 index) { return t->base[index]; }

void VTSet(VariableTable *t, u1 index, jshort val) { t->base[index] = val; }

u1 BCReadU1(void) { return *pc++; }

u2 BCReadU2(void) {
  u1 v1 = *pc++;
  u1 v2 = *pc++;
  return (v1 << 8) | v2;
}

void PCSetOffset(int16_t offset) { pc += offset; }

u1 *PCGet(void) { return pc; }

void PCSet(u1 *new_pc) { pc = new_pc; }

u2 CPGetData(u2 index) { return data[index]; }

void CPSetData(u2 index, u2 val) { data[index] = val; }

jshort ODGet(jshort objRef, jshort index) { return 0; }

void ODSet(jshort objRef, jshort index, jshort value) {}