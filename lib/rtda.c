#include "rtda.h"

static bytecode_t byteCode;

jshort operand_stack_pop(operand_stack_t *s) { return *(--s->next); }

void operand_stack_push(operand_stack_t *s, jshort val) { *s->next++ = val; }

jshort operand_stack_get(operand_stack_t *s) { return *(s->next - 1); }

jshort variable_table_get(variable_table_t *t, u1 index) { return t->base[index]; }

void variable_table_set(variable_table_t *t, u1 index, jshort val) { t->base[index] = val; }

u1 bytecode_read_u1(void) {
  if (byteCode.index < MAX_BC_INDEX)
    return byteCode.base[byteCode.index++];
  return 0; // TODO
}

u2 bytecode_read_u2(void) {
  u1 v1 = bytecode_read_u1();
  u1 v2 = bytecode_read_u1();
  return (v1 << 8) | v2;
}

void bytecode_jump_offset(int16_t offset) { byteCode.index += offset; }

void bytecode_set(u1 *base) {
  byteCode.base = base;
  byteCode.index = 0;
}

u2 constant_pool_get(u2 index) { return 0; }

void constant_pool_set(u2 index, u2 val) {}

jshort object_data_get(jshort objRef, jshort index) { return 0; }

void object_data_set(jshort objRef, jshort index, jshort value) {}