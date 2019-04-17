#include <string.h>

#include "instructions.h"
#include "rtda.h"

void ins_invalid(Frame *f) {}

void ins_nop(Frame *f) {}

void ins_aconst_null(Frame *f) { operand_stack_push(&f->operand_stack, JNULL); }

void ins_sconst_m1(Frame *f) { operand_stack_push(&f->operand_stack, -1); }

void ins_sconst_0(Frame *f) { operand_stack_push(&f->operand_stack, 0); }

void ins_sconst_1(Frame *f) { operand_stack_push(&f->operand_stack, 1); }

void ins_sconst_2(Frame *f) { operand_stack_push(&f->operand_stack, 2); }

void ins_sconst_3(Frame *f) { operand_stack_push(&f->operand_stack, 3); }

void ins_sconst_4(Frame *f) { operand_stack_push(&f->operand_stack, 4); }

void ins_sconst_5(Frame *f) { operand_stack_push(&f->operand_stack, 5); }

void ins_bspush(Frame *f) {
  jbyte v = bytecode_read_u1();
  operand_stack_push(&f->operand_stack, v);
}

void ins_sspush(Frame *f) {
  jshort v = bytecode_read_u2();
  operand_stack_push(&f->operand_stack, v);
}

void ins_asload(Frame *f) {
  u1 index = bytecode_read_u1();
  jshort v = variable_table_get(&f->variable_table, index);
  operand_stack_push(&f->operand_stack, v);
}

void ins_asload_0(Frame *f) {
  jshort v = variable_table_get(&f->variable_table, 0);
  operand_stack_push(&f->operand_stack, v);
}

void ins_asload_1(Frame *f) {
  jshort v = variable_table_get(&f->variable_table, 1);
  operand_stack_push(&f->operand_stack, v);
}

void ins_asload_2(Frame *f) {
  jshort v = variable_table_get(&f->variable_table, 2);
  operand_stack_push(&f->operand_stack, v);
}

void ins_asload_3(Frame *f) {
  jshort v = variable_table_get(&f->variable_table, 3);
  operand_stack_push(&f->operand_stack, v);
}

void ins_aaload(Frame *f) {
  // TODO
}

void ins_baload(Frame *f) {
  // TODO
}

void ins_saload(Frame *f) {
  // TODO
}

void ins_asstore(Frame *f) {
  u1 index = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, index, v);
}

void ins_asstore_0(Frame *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, 0, v);
}

void ins_asstore_1(Frame *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, 1, v);
}

void ins_asstore_2(Frame *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, 2, v);
}

void ins_asstore_3(Frame *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, 3, v);
}

void ins_aastore(Frame *f) {
  // TODO
}

void ins_bastore(Frame *f) {
  // TODO
}

void ins_sastore(Frame *f) {
  // TODO
}

void ins_pop(Frame *f) { operand_stack_pop(&f->operand_stack); }

void ins_pop2(Frame *f) {
  operand_stack_pop(&f->operand_stack);
  operand_stack_pop(&f->operand_stack);
}

void ins_dup(Frame *f) {
  jshort v = operand_stack_get(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v);
}

void ins_dup2(Frame *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_get(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v2);
  operand_stack_push(&f->operand_stack, v1);
  operand_stack_push(&f->operand_stack, v2);
}

void ins_dup_x(Frame *f) {
  u1 mn = bytecode_read_u1();
  u1 m = (mn >> 4) * 2;
  u1 n = (mn & 0xF) * 2;
  memmove(f->operand_stack.next - n + m, f->operand_stack.next - n, n);
  memcpy(f->operand_stack.next - n, f->operand_stack.next, m);
  f->operand_stack.next += m;
}

void ins_swap_x(Frame *f) {
  u1 buf[2];
  u1 mn = bytecode_read_u1();
  u1 m = (mn >> 4) * 2;
  u1 n = (mn & 0xF) * 2;
  memcpy(buf, f->operand_stack.next - m, m);
  memmove(f->operand_stack.next - n, f->operand_stack.next - n - m, n);
  memcpy(f->operand_stack.next - n - m, buf, m);
}

void ins_sadd(Frame *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  jshort result = v1 + v2;
  operand_stack_push(&f->operand_stack, result);
}

void ins_ssub(Frame *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  jshort result = v1 - v2;
  operand_stack_push(&f->operand_stack, result);
}

void ins_smul(Frame *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  jshort result = v1 * v2;
  operand_stack_push(&f->operand_stack, result);
}

void ins_sdiv(Frame *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  jshort result = v1 / v2;
  operand_stack_push(&f->operand_stack, result);
}

void ins_srem(Frame *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  jshort result = v1 % v2;
  operand_stack_push(&f->operand_stack, result);
}

void ins_sneg(Frame *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, -v);
}

void ins_sshl(Frame *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  v2 &= 0x1F;
  operand_stack_push(&f->operand_stack, v1 << v2);
}

void ins_sshr(Frame *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  v2 &= 0x1F;
  operand_stack_push(&f->operand_stack, v1 >> v2);
}

void ins_sushr(Frame *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  v2 &= 0x1F;
  operand_stack_push(&f->operand_stack, (u2)v1 >> v2);
}

void ins_sand(Frame *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v1 & v2);
}

void ins_sor(Frame *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v1 | v2);
}

void ins_sxor(Frame *f) {
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v1 ^ v2);
}

void ins_sinc(Frame *f) {
  u1 index = bytecode_read_u1();
  jbyte v = bytecode_read_u1();
  jshort var = variable_table_get(&f->variable_table, index);
  variable_table_set(&f->variable_table, index, var + v);
}

void ins_s2b(Frame *f) {
  jshort v = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, (jbyte)v);
}

void ins_ifeq(Frame *f) {
  jbyte branch = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v == 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifne(Frame *f) {
  jbyte branch = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v != 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_iflt(Frame *f) {
  jbyte branch = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v < 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifge(Frame *f) {
  jbyte branch = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v >= 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifgt(Frame *f) {
  jbyte branch = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v > 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifle(Frame *f) {
  jbyte branch = bytecode_read_u1();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v <= 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifnull(Frame *f) {
  jbyte branch = bytecode_read_u1();
  u2 v = operand_stack_pop(&f->operand_stack);
  if (v == 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifnonnull(Frame *f) {
  jbyte branch = bytecode_read_u1();
  u2 v = (u2)operand_stack_pop(&f->operand_stack);
  if (v > 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_acmpeq(Frame *f) {
  jbyte branch = bytecode_read_u1();
  u2 v2 = (u2)operand_stack_pop(&f->operand_stack);
  u2 v1 = (u2)operand_stack_pop(&f->operand_stack);
  if (v1 == v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_acmpne(Frame *f) {
  jbyte branch = bytecode_read_u1();
  u2 v2 = (u2)operand_stack_pop(&f->operand_stack);
  u2 v1 = (u2)operand_stack_pop(&f->operand_stack);
  if (v1 != v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpeq(Frame *f) {
  jbyte branch = bytecode_read_u1();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 == v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpne(Frame *f) {
  jbyte branch = bytecode_read_u1();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 != v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmplt(Frame *f) {
  jbyte branch = bytecode_read_u1();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 < v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpge(Frame *f) {
  jbyte branch = bytecode_read_u1();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 >= v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpgt(Frame *f) {
  jbyte branch = bytecode_read_u1();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 > v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmple(Frame *f) {
  jbyte branch = bytecode_read_u1();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 <= v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_goto(Frame *f) {
  jbyte branch = bytecode_read_u1();
  bytecode_jump_offset(branch);
}

void ins_jsr(Frame *f) {
  // TODO
}

void ins_ret(Frame *f) {
  // TODO
}

void ins_stableswitch(Frame *f) {
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

void ins_slookupswitch(Frame *f) {
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

void ins_areturn(Frame *f) {
  // TODO
}

void ins_sreturn(Frame *f) {
  // TODO
}

void ins_return(Frame *f) {
  // TODO
}

void ins_getstatic_abs(Frame *f) {
  u2 index = bytecode_read_u2();
  jshort v = constant_pool_get(index);
  operand_stack_push(&f->operand_stack, v);
}

void ins_putstatic_abs(Frame *f) {
  u2 index = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  constant_pool_set(index, v);
}

void ins_getfield_abs(Frame *f) {
  u1 index = bytecode_read_u1();
  jshort objRef = operand_stack_pop(&f->operand_stack);
  jshort value = object_data_get(objRef, index);
  operand_stack_push(&f->operand_stack, value);
}

void ins_putfield_abs(Frame *f) {
  u1 index = bytecode_read_u1();
  jshort value = operand_stack_pop(&f->operand_stack);
  jshort objRef = operand_stack_pop(&f->operand_stack);
  object_data_set(objRef, index, value);
}

void ins_invokevirtual(Frame *f) {
  // TODO
}

void ins_invokespecial(Frame *f) {
  // TODO
}

void ins_invokestatic(Frame *f) {
  // TODO
}

void ins_invokeinterface(Frame *f) {
  // TODO
}

void ins_new(Frame *f) {
  // TODO
}

void ins_newarray(Frame *f) {
  // TODO
}

void ins_anewarray(Frame *f) {
  // TODO
}

void ins_arraylength(Frame *f) {
  // TODO
}

void ins_athrow(Frame *f) {
  // TODO
}

void ins_checkcast(Frame *f) {
  // TODO
}

void ins_instanceof(Frame *f) {
  // TODO
}

void ins_sinc_w(Frame *f) {
  u1 index = bytecode_read_u1();
  jshort v = bytecode_read_u2();
  jshort var = variable_table_get(&f->variable_table, index);
  variable_table_set(&f->variable_table, index, var + v);
}

void ins_ifeq_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v == 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifne_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v != 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_iflt_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v < 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifge_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v >= 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifgt_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v > 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifle_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  jshort v = operand_stack_pop(&f->operand_stack);
  if (v <= 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifnull_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  u2 v = operand_stack_pop(&f->operand_stack);
  if (v == 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_ifnonnull_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  u2 v = (u2)operand_stack_pop(&f->operand_stack);
  if (v > 0) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_acmpeq_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  u2 v2 = (u2)operand_stack_pop(&f->operand_stack);
  u2 v1 = (u2)operand_stack_pop(&f->operand_stack);
  if (v1 == v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_acmpne_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  u2 v2 = (u2)operand_stack_pop(&f->operand_stack);
  u2 v1 = (u2)operand_stack_pop(&f->operand_stack);
  if (v1 != v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpeq_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 == v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpne_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 != v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmplt_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 < v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpge_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 >= v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmpgt_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 > v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_if_scmple_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  jshort v2 = operand_stack_pop(&f->operand_stack);
  jshort v1 = operand_stack_pop(&f->operand_stack);
  if (v1 <= v2) {
    bytecode_jump_offset(branch);
  }
}

void ins_goto_w(Frame *f) {
  jshort branch = bytecode_read_u2();
  bytecode_jump_offset(branch);
}

void ins_getfield_abs_w(Frame *f) {
  u2 index = bytecode_read_u2();
  jshort objRef = operand_stack_pop(&f->operand_stack);
  jshort value = object_data_get(objRef, index);
  operand_stack_push(&f->operand_stack, value);
}

void ins_getfield_abs_this(Frame *f) {
  u1 index = bytecode_read_u1();
  jshort objRef = variable_table_get(&f->variable_table, 0);
  jshort value = object_data_get(objRef, index);
  operand_stack_push(&f->operand_stack, value);
}

void ins_putfield_abs_w(Frame *f) {
  u2 index = bytecode_read_u2();
  jshort value = operand_stack_pop(&f->operand_stack);
  jshort objRef = operand_stack_pop(&f->operand_stack);
  object_data_set(objRef, index, value);
}

void ins_putfield_abs_this(Frame *f) {
  u1 index = bytecode_read_u1();
  jshort value = operand_stack_pop(&f->operand_stack);
  jshort objRef = variable_table_get(&f->variable_table, 0);
  object_data_set(objRef, index, value);
}

void (*opcodes[256])(Frame *) = {
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
