#include "rtdata.h"

jshort OSPop(OperandStack *s) { return *(--s->next); }

void OSPush(OperandStack *s, jshort val) { *s->next++ = val; }

jshort OSGet(OperandStack *s) { return *(s->next - 1); }

jshort VTGet(VariableTable *t, u1 index) { return t->base[index]; }

void VTSet(VariableTable *t, u1 index, jshort val) {
  t->base[index] = val;
}
