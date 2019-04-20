#include <string.h>
#include <vm.h>

#include "context.h"
#include "instructions.h"
#include "rtda.h"

void ins_invalid(frame_t *f) {}

void ins_nop(frame_t *f) {}

void ins_aconst_null(frame_t *f) {
  operand_stack_push(&f->operand_stack, JNULL);
}

void ins_sconst_m1(frame_t *f) { operand_stack_push(&f->operand_stack, -1); }

void ins_sconst_0(frame_t *f) { operand_stack_push(&f->operand_stack, 0); }

void ins_sconst_1(frame_t *f) { operand_stack_push(&f->operand_stack, 1); }

void ins_sconst_2(frame_t *f) { operand_stack_push(&f->operand_stack, 2); }

void ins_sconst_3(frame_t *f) { operand_stack_push(&f->operand_stack, 3); }

void ins_sconst_4(frame_t *f) { operand_stack_push(&f->operand_stack, 4); }

void ins_sconst_5(frame_t *f) { operand_stack_push(&f->operand_stack, 5); }

void ins_bspush(frame_t *f) {
  jbyte v = bytecode_read_u1();
  operand_stack_push(&f->operand_stack, v);
}

void ins_sspush(frame_t *f) {
  jshort v = bytecode_read_u2();
  operand_stack_push(&f->operand_stack, v);
}

void ins_asload(frame_t *f) {
  u1 index = bytecode_read_u1();
  jshort v = variable_table_get(&f->variable_table, index);
  operand_stack_push(&f->operand_stack, v);
}

void ins_asload_0(frame_t *f) {
  jshort v = variable_table_get(&f->variable_table, 0);
  operand_stack_push(&f->operand_stack, v);
}

void ins_asload_1(frame_t *f) {
  jshort v = variable_table_get(&f->variable_table, 1);
  operand_stack_push(&f->operand_stack, v);
}

void ins_asload_2(frame_t *f) {
  jshort v = variable_table_get(&f->variable_table, 2);
  operand_stack_push(&f->operand_stack, v);
}

void ins_asload_3(frame_t *f) {
  jshort v = variable_table_get(&f->variable_table, 3);
  operand_stack_push(&f->operand_stack, v);
}

void ins_aaload(frame_t *f) {
  jshort index = operand_stack_pop(&f->operand_stack);
  jshort ref = operand_stack_pop(&f->operand_stack);
  jshort v;
  context_read_array(&current_package, ref, ARRAY_T_REFERENCE, index, (u1 *)&v);
  operand_stack_push(&f->operand_stack, v);
}

void ins_baload(frame_t *f) {
  jshort index = operand_stack_pop(&f->operand_stack);
  jshort ref = operand_stack_pop(&f->operand_stack);
  jbyte v;
  context_read_array(&current_package, ref, ARRAY_T_BYTE, index, (u1 *)&v);
  operand_stack_push(&f->operand_stack, v);
}

void ins_saload(frame_t *f) {
  jshort index = operand_stack_pop(&f->operand_stack);
  jshort ref = operand_stack_pop(&f->operand_stack);
  jshort v;
  context_read_array(&current_package, ref, ARRAY_T_SHORT, index, (u1 *)&v);
  operand_stack_push(&f->operand_stack, v);
}

void ins_asstore(frame_t *f) {
  u1 index = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, index, v);
}

void ins_asstore_0(frame_t *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, 0, v);
}

void ins_asstore_1(frame_t *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, 1, v);
}

void ins_asstore_2(frame_t *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, 2, v);
}

void ins_asstore_3(frame_t *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, 3, v);
}

void ins_aastore(frame_t *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  jshort index = operand_stack_pop(&f->operand_stack);
  jshort ref = operand_stack_pop(&f->operand_stack);
  context_write_array(&current_package, ref, ARRAY_T_REFERENCE, index, v);
}

void ins_bastore(frame_t *f) {
  jbyte v = operand_stack_pop(&f->operand_stack);
  jshort index = operand_stack_pop(&f->operand_stack);
  jshort ref = operand_stack_pop(&f->operand_stack);
  context_write_array(&current_package, ref, ARRAY_T_BYTE, index, v);
}

void ins_sastore(frame_t *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  jshort index = operand_stack_pop(&f->operand_stack);
  jshort ref = operand_stack_pop(&f->operand_stack);
  context_write_array(&current_package, ref, ARRAY_T_SHORT, index, v);
}

void ins_pop(frame_t *f) { operand_stack_pop(&f->operand_stack); }

void ins_pop2(frame_t *f) {
  operand_stack_pop(&f->operand_stack);
  operand_stack_pop(&f->operand_stack);
}

void ins_dup(frame_t *f) {
  jshort v = operand_stack_get(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v);
}

void ins_dup2(frame_t *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_get(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v2);
  operand_stack_push(&f->operand_stack, v1);
  operand_stack_push(&f->operand_stack, v2);
}

void ins_dup_x(frame_t *f) {
  // TODO: check m & n
  u1 mn = bytecode_read_u1();
  u1 m = mn >> 4u;
  u1 n = mn & 0xFu;
  memmove(f->operand_stack.base + f->operand_stack.index - n + m,
          f->operand_stack.base + f->operand_stack.index - n, n * 2);
  memcpy(f->operand_stack.base + f->operand_stack.index - n,
         f->operand_stack.base + f->operand_stack.index, m * 2);
  f->operand_stack.index += m;
}

void ins_swap_x(frame_t *f) {
  // TODO: check m & n
  u1 buf[2];
  u1 mn = bytecode_read_u1();
  u1 m = mn >> 4u;
  u1 n = mn & 0xFu;
  memcpy(buf, f->operand_stack.base + f->operand_stack.index - m, m * 2);
  memmove(f->operand_stack.base + f->operand_stack.index - n,
          f->operand_stack.base + f->operand_stack.index - n - m, n * 2);
  memcpy(f->operand_stack.base + f->operand_stack.index - n - m, buf, m * 2);
}

void ins_sadd(frame_t *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  jshort result = v1 + v2;
  operand_stack_push(&f->operand_stack, result);
}

void ins_ssub(frame_t *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  jshort result = v1 - v2;
  operand_stack_push(&f->operand_stack, result);
}

void ins_smul(frame_t *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  jshort result = v1 * v2;
  operand_stack_push(&f->operand_stack, result);
}

void ins_sdiv(frame_t *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  jshort result = v1 / v2;
  operand_stack_push(&f->operand_stack, result);
}

void ins_srem(frame_t *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  jshort result = v1 % v2;
  operand_stack_push(&f->operand_stack, result);
}

void ins_sneg(frame_t *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, -v);
}

void ins_sshl(frame_t *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  v2 &= 0x1F;
  operand_stack_push(&f->operand_stack, v1 << v2);
}

void ins_sshr(frame_t *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  v2 &= 0x1F;
  operand_stack_push(&f->operand_stack, v1 >> v2);
}

void ins_sushr(frame_t *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  v2 &= 0x1F;
  operand_stack_push(&f->operand_stack, (u2)v1 >> v2);
}

void ins_sand(frame_t *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v1 & v2);
}

void ins_sor(frame_t *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v1 | v2);
}

void ins_sxor(frame_t *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v1 ^ v2);
}

void ins_sinc(frame_t *f) {
  u1 index = bytecode_read_u1();
  jbyte v = bytecode_read_u1();
  jshort var = variable_table_get(&f->variable_table, index);
  variable_table_set(&f->variable_table, index, var + v);
}

void ins_s2b(frame_t *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, (jbyte)v);
}

void ins_ifeq(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v == 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifne(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v != 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_iflt(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v < 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifge(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v >= 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifgt(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v > 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifle(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v <= 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifnull(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  u2 v = operand_stack_pop(&f->operand_stack);
  if (v == 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifnonnull(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  u2 v = (u2)operand_stack_pop(&f->operand_stack);
  if (v > 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_acmpeq(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  u2 v2 = (u2)operand_stack_pop(&f->operand_stack);
  u2 v1 = (u2)operand_stack_pop(&f->operand_stack);
  if (v1 == v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_acmpne(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  u2 v2 = (u2)operand_stack_pop(&f->operand_stack);
  u2 v1 = (u2)operand_stack_pop(&f->operand_stack);
  if (v1 != v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpeq(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 == v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpne(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 != v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmplt(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 < v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpge(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 >= v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpgt(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 > v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmple(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 <= v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_goto(frame_t *f) {
  jbyte branch = bytecode_read_u1();
  bytecode_jump_offset(branch);
}

void ins_jsr(frame_t *f) {
  // TODO
}

void ins_ret(frame_t *f) {
  // TODO
}

void ins_stableswitch(frame_t *f) {
  jshort index = operand_stack_pop(&f->operand_stack);
  jshort defaultVal = bytecode_read_u2();
  jshort low = bytecode_read_u2();
  jshort high = bytecode_read_u2();
  if (index < low || index > high) {
    bytecode_jump_offset(defaultVal - 6);
  } else {
    bytecode_jump_offset((index - low) * 2);
    u2 offset = bytecode_read_u2();
    bytecode_jump_offset(offset - 6 - (index - low) * 2);
  }
}

void ins_slookupswitch(frame_t *f) {
  jshort key = operand_stack_pop(&f->operand_stack);
  jshort defaultVal = bytecode_read_u2();
  jshort nPairs = bytecode_read_u2();
  for (jshort i = 0; i < nPairs; ++i) {
    jshort match = bytecode_read_u2();
    jshort offset = bytecode_read_u2();
    if (key == match) {
      bytecode_jump_offset(offset - 4 - 4 * i);
      return;
    }
  }
  bytecode_jump_offset(defaultVal - 4 - 4 * nPairs);
}

void ins_areturn(frame_t *f) {
  // TODO
}

void ins_sreturn(frame_t *f) {
  // TODO
}

void ins_return(frame_t *f) {
  // TODO
}

void ins_getstatic_abs(frame_t *f) {
  u2 index = bytecode_read_u2();
  jshort v = constant_pool_get(index);
  operand_stack_push(&f->operand_stack, v);
}

void ins_putstatic_abs(frame_t *f) {
  u2 index = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  constant_pool_set(index, v);
}

void ins_getfield_abs(frame_t *f) {
  u1 index = bytecode_read_u1();
  jshort objRef = operand_stack_pop(&f->operand_stack);
  jshort value = object_data_get(objRef, index);
  operand_stack_push(&f->operand_stack, value);
}

void ins_putfield_abs(frame_t *f) {
  u1 index = bytecode_read_u1();
  jshort value = operand_stack_pop(&f->operand_stack);
  jshort objRef = operand_stack_pop(&f->operand_stack);
  object_data_set(objRef, index, value);
}

void ins_invokevirtual(frame_t *f) {
  // TODO
}

void ins_invokespecial(frame_t *f) {
  // TODO
}

void ins_invokestatic(frame_t *f) {
  // TODO
}

void ins_invokeinterface(frame_t *f) {
  // TODO
}

void ins_new(frame_t *f) {
  // TODO
}

void ins_newarray(frame_t *f) {
  u1 atype = bytecode_read_u1();
  jshort count = operand_stack_pop(&f->operand_stack);
  int ref = context_create_array(&current_package, atype, 0, count);
  if (ref > 0)
    operand_stack_push(&f->operand_stack, ref);
  else {
    // TODO: error
  }
}

void ins_anewarray(frame_t *f) {
  // TODO
}

void ins_arraylength(frame_t *f) {
  jshort ref = operand_stack_pop(&f->operand_stack);
  array_metadata_t metadata;
  context_array_meta(&current_package, ref, &metadata);
  operand_stack_push(&f->operand_stack, metadata.length);
}

void ins_athrow(frame_t *f) {
  // TODO
}

void ins_checkcast(frame_t *f) {
  // TODO
}

void ins_instanceof(frame_t *f) {
  // TODO
}

void ins_sinc_w(frame_t *f) {
  u1 index = bytecode_read_u1();
  jshort v = bytecode_read_u2();
  jshort var = variable_table_get(&f->variable_table, index);
  variable_table_set(&f->variable_table, index, var + v);
}

void ins_ifeq_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v == 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifne_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v != 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_iflt_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v < 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifge_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v >= 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifgt_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v > 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifle_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v <= 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifnull_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  u2 v = operand_stack_pop(&f->operand_stack);
  if (v == 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifnonnull_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  u2 v = (u2)operand_stack_pop(&f->operand_stack);
  if (v > 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_acmpeq_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  u2 v2 = (u2)operand_stack_pop(&f->operand_stack);
  u2 v1 = (u2)operand_stack_pop(&f->operand_stack);
  if (v1 == v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_acmpne_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  u2 v2 = (u2)operand_stack_pop(&f->operand_stack);
  u2 v1 = (u2)operand_stack_pop(&f->operand_stack);
  if (v1 != v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpeq_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 == v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpne_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 != v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmplt_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 < v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpge_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 >= v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpgt_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 > v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmple_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 <= v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_goto_w(frame_t *f) {
  jshort branch = bytecode_read_u2();
  bytecode_jump_offset(branch);
}

void ins_getfield_abs_w(frame_t *f) {
  u2 index = bytecode_read_u2();
  jshort objRef = operand_stack_pop(&f->operand_stack);
  jshort value = object_data_get(objRef, index);
  operand_stack_push(&f->operand_stack, value);
}

void ins_getfield_abs_this(frame_t *f) {
  u1 index = bytecode_read_u1();
  jshort objRef = variable_table_get(&f->variable_table, 0);
  jshort value = object_data_get(objRef, index);
  operand_stack_push(&f->operand_stack, value);
}

void ins_putfield_abs_w(frame_t *f) {
  u2 index = bytecode_read_u2();
  jshort value = operand_stack_pop(&f->operand_stack);
  jshort objRef = operand_stack_pop(&f->operand_stack);
  object_data_set(objRef, index, value);
}

void ins_putfield_abs_this(frame_t *f) {
  u1 index = bytecode_read_u1();
  jshort value = operand_stack_pop(&f->operand_stack);
  jshort objRef = variable_table_get(&f->variable_table, 0);
  object_data_set(objRef, index, value);
}

void (*opcodes[256])(frame_t *) = {
    ins_nop,
    ins_aconst_null,
    ins_sconst_m1,
    ins_sconst_0,
    ins_sconst_1,
    ins_sconst_2,
    ins_sconst_3,
    ins_sconst_4,
    ins_sconst_5,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_bspush,
    ins_sspush,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_asload, // aload
    ins_asload, // sload
    ins_invalid,
    ins_asload_0, // aload_0
    ins_asload_1, // aload_1
    ins_asload_2, // aload_2
    ins_asload_3, // aload_3
    ins_asload_0, // sload_0
    ins_asload_1, // sload_1
    ins_asload_2, // sload_2
    ins_asload_3, // sload_3
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_aaload,
    ins_baload,
    ins_saload,
    ins_invalid,
    ins_asstore, // astore
    ins_asstore, // sstore
    ins_invalid,
    ins_asstore_0, // astore_0
    ins_asstore_1, // astore_1
    ins_asstore_2, // astore_2
    ins_asstore_3, // astore_3
    ins_asstore_0, // sstore_0
    ins_asstore_1, // sstore_1
    ins_asstore_2, // sstore_2
    ins_asstore_3, // sstore_3
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_aastore,
    ins_bastore,
    ins_sastore,
    ins_invalid,
    ins_pop,
    ins_pop2,
    ins_dup,
    ins_dup2,
    ins_dup_x,
    ins_swap_x,
    ins_sadd,
    ins_invalid,
    ins_ssub,
    ins_invalid,
    ins_smul,
    ins_invalid,
    ins_sdiv,
    ins_invalid,
    ins_srem,
    ins_invalid,
    ins_sneg,
    ins_invalid,
    ins_sshl,
    ins_invalid,
    ins_sshr,
    ins_invalid,
    ins_sushr,
    ins_invalid,
    ins_sand,
    ins_invalid,
    ins_sor,
    ins_invalid,
    ins_sxor,
    ins_invalid,
    ins_sinc,
    ins_invalid,
    ins_s2b,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_ifeq,
    ins_ifne,
    ins_iflt,
    ins_ifge,
    ins_ifgt,
    ins_ifle,
    ins_ifnull,
    ins_ifnonnull,
    ins_if_acmpeq,
    ins_if_acmpne,
    ins_if_scmpeq,
    ins_if_scmpne,
    ins_if_scmplt,
    ins_if_scmpge,
    ins_if_scmpgt,
    ins_if_scmple,
    ins_goto,
    ins_jsr,
    ins_ret,
    ins_stableswitch,
    ins_invalid,
    ins_slookupswitch,
    ins_invalid,
    ins_areturn,
    ins_sreturn,
    ins_invalid,
    ins_return,
    ins_getstatic_abs, // getstatic_a
    ins_getstatic_abs, // getstatic_b
    ins_getstatic_abs, // getstatic_s
    ins_invalid,
    ins_putstatic_abs, // putstatic_a
    ins_putstatic_abs, // putstatic_b
    ins_putstatic_abs, // putstatic_s
    ins_invalid,
    ins_getfield_abs, // getfield_a
    ins_getfield_abs, // getfield_b
    ins_getfield_abs, // getfield_s
    ins_invalid,
    ins_putfield_abs, // putfield_a
    ins_putfield_abs, // putfield_b
    ins_putfield_abs, // putfield_s
    ins_invalid,
    ins_invokevirtual,
    ins_invokespecial,
    ins_invokestatic,
    ins_invokeinterface,
    ins_new,
    ins_newarray,
    ins_anewarray,
    ins_arraylength,
    ins_athrow,
    ins_checkcast,
    ins_instanceof,
    ins_sinc_w,
    ins_invalid,
    ins_ifeq_w,
    ins_ifne_w,
    ins_iflt_w,
    ins_ifge_w,
    ins_ifgt_w,
    ins_ifle_w,
    ins_ifnull_w,
    ins_ifnonnull_w,
    ins_if_acmpeq_w,
    ins_if_acmpne_w,
    ins_if_scmpeq_w,
    ins_if_scmpne_w,
    ins_if_scmplt_w,
    ins_if_scmpge_w,
    ins_if_scmpgt_w,
    ins_if_scmple_w,
    ins_goto_w,
    ins_getfield_abs_w, // getfield_a_w
    ins_getfield_abs_w, // getfield_b_w
    ins_getfield_abs_w, // getfield_s_w
    ins_invalid,
    ins_getfield_abs_this, // getfield_a_this
    ins_getfield_abs_this, // getfield_b_this
    ins_getfield_abs_this, // getfield_s_this
    ins_invalid,
    ins_putfield_abs_w, // putfield_a_w
    ins_putfield_abs_w, // putfield_b_w
    ins_putfield_abs_w, // putfield_s_w
    ins_invalid,
    ins_putfield_abs_this, // putfield_a_this
    ins_putfield_abs_this, // putfield_b_this
    ins_putfield_abs_this, // putfield_s_this
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
};
