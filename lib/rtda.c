#include "rtda.h"
#include "common.h"
#include "context.h"
#include "instructions.h"
#include "utils.h"
#include "vm.h"
#include <assert.h>

static bytecode_t bytecode;
static u2 stack_buffer[128];
static i2 variable_buffer[128];
frame_t frames[TOTAL_FRAMES];
int current_frame;
int running;

i2 operand_stack_pop(operand_stack_t *s) { return s->base[--s->index]; }

void operand_stack_push(operand_stack_t *s, i2 val) {
  s->base[s->index++] = val;
}

i2 operand_stack_get(operand_stack_t *s) { return s->base[s->index - 1]; }

i2 variable_table_get(variable_table_t *t, u1 index) { return t->base[index]; }

void variable_table_set(variable_table_t *t, u1 index, i2 val) {
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

i2 object_data_get(i2 objRef, i2 index) { return 0; }

void object_data_set(i2 objRef, i2 index, i2 value) {}

void run() {
  current_frame = 0;
  running = 1;
  while (running) {
    u2 pc = bytecode.method.method_offset + bytecode.index -
            frames[current_frame].method_offset - 2;
    u1 opcode = bytecode_read_u1();
    DBG_MSG("Frame %d Opcode %02x pc %d stack %d:", current_frame, opcode, pc,
            frames[current_frame].operand_stack.index);
    for (int i = 0; i < frames[current_frame].operand_stack.index; i++) {
      DBG_PRINT(" %04X", frames[current_frame].operand_stack.base[i]);
    }
    DBG_PRINT("\n");
    opcodes[opcode](&frames[current_frame]);
  }
}

int push_frame(u2 method_offset) {
  if (current_frame >= 0) {
    // save return address
    frames[current_frame].bytecode_offset =
        bytecode.method.method_offset + bytecode.index;
  }

  current_frame++;
  frames[current_frame].method_offset = method_offset;

  method_header_info header;
  int res = context_read_method(&current_package, (u1 *)&header, method_offset,
                                sizeof(header));
  if (res != sizeof(header))
    return res;
  DBG_MSG("Method flags:%x max_stack:%d nargs:%d max_locals:%d\n", header.flags,
          header.max_locals, header.nargs, header.max_locals);

  // bytecode offset
  bytecode_set_method(method_offset + 2);

  // set frame info
  frames[current_frame].operand_stack.max_stack = header.max_stack;
  if (current_frame >= 1) {
    frames[current_frame].operand_stack.base =
        frames[current_frame - 1].operand_stack.base +
        frames[current_frame - 1].operand_stack.max_stack;
  } else {
    frames[current_frame].operand_stack.base = stack_buffer;
  }
  frames[current_frame].operand_stack.index = 0;
  frames[current_frame].variable_table.max_locals = header.max_locals;
  if (current_frame >= 1) {
    frames[current_frame].variable_table.base =
        frames[current_frame - 1].variable_table.base +
        frames[current_frame - 1].variable_table.max_locals;
  } else {
    frames[current_frame].variable_table.base = variable_buffer;
  }
  frames[current_frame].info = header;
  return 0;
}

int init_frame(u2 method_offset) {
  current_frame = -1;
  return push_frame(method_offset);
}

frame_t *pop_frame() {
  assert(current_frame > 0);

  current_frame--;

  DBG_MSG("Return to frame %d\n", current_frame);

  // bytecode offset
  bytecode_set_method(frames[current_frame].bytecode_offset);
  return &frames[current_frame];
}