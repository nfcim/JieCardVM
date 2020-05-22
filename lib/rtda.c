#include "rtda.h"
#include "context.h"
#include "vm.h"
#include "instructions.h"
#include "utils.h"

static bytecode_t bytecode;
static frame_t frames[TOTAL_FRAMES];

jshort operand_stack_pop(operand_stack_t *s) { return s->base[--s->index]; }

void operand_stack_push(operand_stack_t *s, jshort val) {
  s->base[s->index++] = val;
}

jshort operand_stack_get(operand_stack_t *s) { return s->base[s->index - 1]; }

jshort variable_table_get(variable_table_t *t, u1 index) {
  return t->base[index];
}

void variable_table_set(variable_table_t *t, u1 index, jshort val) {
  t->base[index] = val;
}

u1 bytecode_read_u1(void) {
  if (bytecode.is_external) {
    return bytecode.external.buffer[bytecode.index++];
  } else {
    if (bytecode.index >= BYTECODE_WINDOW_SIZE) {
      // go to next window
      context_read_method(&current_package, bytecode.method.buffer,
                          bytecode.method.method_offset + bytecode.index,
                          sizeof(bytecode.method.buffer));
      bytecode.method.method_offset += bytecode.index;
      bytecode.index = 0;
    }
    return bytecode.method.buffer[bytecode.index++];
  }
}

u2 bytecode_read_u2(void) {
  u1 v1 = bytecode_read_u1();
  u1 v2 = bytecode_read_u1();
  return (v1 << 8) | v2;
}

void bytecode_jump_offset(int16_t offset) { bytecode.index += offset; }

void bytecode_set_method(u2 method_offset) {
  bytecode.is_external = 0;
  context_read_method(&current_package, bytecode.method.buffer, method_offset,
                      sizeof(bytecode.method.buffer));
  bytecode.index = 0;
  bytecode.method.method_offset = method_offset;
}

void bytecode_set_buffer(u1 *buffer) {
  bytecode.is_external = 1;
  bytecode.external.buffer = buffer;
  bytecode.index = 0;
}

u2 _bytecode_get_index(void) { return bytecode.index; }

u2 constant_pool_get(u2 index) { return 0; }

void constant_pool_set(u2 index, u2 val) {}

jshort object_data_get(jshort objRef, jshort index) { return 0; }

void object_data_set(jshort objRef, jshort index, jshort value) {}

void run() {
  for (int i = 0;i < 1000;i++) {
    u1 opcode = bytecode_read_u1();
    DBG_MSG("Opcode %02x\n", opcode);
    opcodes[opcode](&frames[0]);
  }
}