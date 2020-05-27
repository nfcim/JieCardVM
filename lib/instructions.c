#include <assert.h>
#include <string.h>
#include <vm.h>

#include "context.h"
#include "instructions.h"
#include "rtda.h"
#include "utils.h"

void ins_invalid(frame_t *f) { assert(false); }

// 0x00 Do nothing
void ins_nop(frame_t *f) {}

// 0x01 Push null
void ins_aconst_null(frame_t *f) {
  operand_stack_push(&f->operand_stack, JNULL);
}

// 0x02 Push short constant
void ins_sconst_m1(frame_t *f) { operand_stack_push(&f->operand_stack, -1); }

// 0x03 Push short constant
void ins_sconst_0(frame_t *f) { operand_stack_push(&f->operand_stack, 0); }

// 0x04 Push short constant
void ins_sconst_1(frame_t *f) { operand_stack_push(&f->operand_stack, 1); }

// 0x05 Push short constant
void ins_sconst_2(frame_t *f) { operand_stack_push(&f->operand_stack, 2); }

// 0x06 Push short constant
void ins_sconst_3(frame_t *f) { operand_stack_push(&f->operand_stack, 3); }

// 0x07 Push short constant
void ins_sconst_4(frame_t *f) { operand_stack_push(&f->operand_stack, 4); }

// 0x08 Push short constant
void ins_sconst_5(frame_t *f) { operand_stack_push(&f->operand_stack, 5); }

// 0x10 push byte
void ins_bspush(frame_t *f) {
  i1 v = bytecode_read_u1();
  operand_stack_push(&f->operand_stack, v);
}

// 0x11 push short
void ins_sspush(frame_t *f) {
  i2 v = bytecode_read_u2();
  operand_stack_push(&f->operand_stack, v);
}

// 0x15 Load reference from local variable
// 0x16 Load short from local variable
void ins_asload(frame_t *f) {
  u1 index = bytecode_read_u1();
  i2 v = variable_table_get(&f->variable_table, index);
  operand_stack_push(&f->operand_stack, v);
}

// 0x18 Load reference from local variable
// 0x1c Load short from local variable
void ins_asload_0(frame_t *f) {
  i2 v = variable_table_get(&f->variable_table, 0);
  operand_stack_push(&f->operand_stack, v);
}

// 0x19 Load reference from local variable
// 0x1d Load short from local variable
void ins_asload_1(frame_t *f) {
  i2 v = variable_table_get(&f->variable_table, 1);
  operand_stack_push(&f->operand_stack, v);
}

// 0x1a Load reference from local variable
// 0x1e Load short from local variable
void ins_asload_2(frame_t *f) {
  i2 v = variable_table_get(&f->variable_table, 2);
  operand_stack_push(&f->operand_stack, v);
}

// 0x1b Load reference from local variable
// 0x1f Load short from local variable
void ins_asload_3(frame_t *f) {
  i2 v = variable_table_get(&f->variable_table, 3);
  operand_stack_push(&f->operand_stack, v);
}

// 0x24 Load reference from array
void ins_aaload(frame_t *f) {
  i2 index = operand_stack_pop(&f->operand_stack);
  i2 ref = operand_stack_pop(&f->operand_stack);
  i2 v;
  context_read_array(&current_package, ref, ARRAY_T_REFERENCE, index, (u1 *)&v);
  operand_stack_push(&f->operand_stack, v);
}

// 0x25 Load byte or boolean from array
void ins_baload(frame_t *f) {
  i2 index = operand_stack_pop(&f->operand_stack);
  i2 ref = operand_stack_pop(&f->operand_stack);
  i1 v;
  context_read_array(&current_package, ref, ARRAY_T_BYTE, index, (u1 *)&v);
  operand_stack_push(&f->operand_stack, v);
}

// 0x26 Load short from array
void ins_saload(frame_t *f) {
  i2 index = operand_stack_pop(&f->operand_stack);
  i2 ref = operand_stack_pop(&f->operand_stack);
  i2 v;
  context_read_array(&current_package, ref, ARRAY_T_SHORT, index, (u1 *)&v);
  operand_stack_push(&f->operand_stack, v);
}

// 0x28 Store reference into local variable
// 0x29 Store short into local variable
void ins_asstore(frame_t *f) {
  u1 index = bytecode_read_u1();
  i2 v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, index, v);
}

// 0x2b Store reference into local variable
// 0x2f Store short into local variable
void ins_asstore_0(frame_t *f) {
  i2 v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, 0, v);
}

// 0x2c Store reference into local variable
// 0x30 Store short into local variable
void ins_asstore_1(frame_t *f) {
  i2 v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, 1, v);
}

// 0x2d Store reference into local variable
// 0x31 Store short into local variable
void ins_asstore_2(frame_t *f) {
  i2 v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, 2, v);
}

// 0x2e Store reference into local variable
// 0x32 Store short into local variable
void ins_asstore_3(frame_t *f) {
  i2 v = operand_stack_pop(&f->operand_stack);
  variable_table_set(&f->variable_table, 3, v);
}

// 0x37 Store into reference array or reference array view
void ins_aastore(frame_t *f) {
  i2 v = operand_stack_pop(&f->operand_stack);
  i2 index = operand_stack_pop(&f->operand_stack);
  i2 ref = operand_stack_pop(&f->operand_stack);
  context_write_array(&current_package, ref, ARRAY_T_REFERENCE, index, v);
}

// 0x38 Store into byte or boolean array
void ins_bastore(frame_t *f) {
  i1 v = operand_stack_pop(&f->operand_stack);
  i2 index = operand_stack_pop(&f->operand_stack);
  i2 ref = operand_stack_pop(&f->operand_stack);
  DBG_MSG("Store byte array %d[%d] = %d\n", ref, index, v);
  context_write_array(&current_package, ref, ARRAY_T_BYTE, index, v);
}

// 0x39 Store into short array
void ins_sastore(frame_t *f) {
  i2 v = operand_stack_pop(&f->operand_stack);
  i2 index = operand_stack_pop(&f->operand_stack);
  i2 ref = operand_stack_pop(&f->operand_stack);
  context_write_array(&current_package, ref, ARRAY_T_SHORT, index, v);
}

// 0x3b Pop top operand stack word
void ins_pop(frame_t *f) { operand_stack_pop(&f->operand_stack); }

// 0x3c Pop top two operand stack words
void ins_pop2(frame_t *f) {
  operand_stack_pop(&f->operand_stack);
  operand_stack_pop(&f->operand_stack);
}

// 0x3d Duplicate top operand stack word
void ins_dup(frame_t *f) {
  i2 v = operand_stack_get(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v);
}

// 0x3e Duplicate top two operand stack words
void ins_dup2(frame_t *f) {
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_get(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v2);
  operand_stack_push(&f->operand_stack, v1);
  operand_stack_push(&f->operand_stack, v2);
}

// 0x3f Duplicate top operand stack words and insert below
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

// 0x40 Swap top two operand stack words
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

// 0x41 Add short
void ins_sadd(frame_t *f) {
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  i2 result = v1 + v2;
  operand_stack_push(&f->operand_stack, result);
}

// 0x43 Subtract short
void ins_ssub(frame_t *f) {
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  i2 result = v1 - v2;
  operand_stack_push(&f->operand_stack, result);
}

// 0x45 Multiply short
void ins_smul(frame_t *f) {
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  i2 result = v1 * v2;
  operand_stack_push(&f->operand_stack, result);
}

// 0x47 Divide short
void ins_sdiv(frame_t *f) {
  // TODO: divisor is 0
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  i2 result = v1 / v2;
  operand_stack_push(&f->operand_stack, result);
}

// 0x49 Remainder short
void ins_srem(frame_t *f) {
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  i2 result = v1 % v2;
  operand_stack_push(&f->operand_stack, result);
}

// 0x4b Negative short
void ins_sneg(frame_t *f) {
  i2 v = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, -v);
}

// 0x4d Shift left short
void ins_sshl(frame_t *f) {
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  v2 &= 0x1F;
  operand_stack_push(&f->operand_stack, v1 << v2);
}

// 0x4f Arithmetic shift right short
void ins_sshr(frame_t *f) {
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  v2 &= 0x1F;
  operand_stack_push(&f->operand_stack, v1 >> v2);
}

// 0x51 Logical shift right short
void ins_sushr(frame_t *f) {
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  v2 &= 0x1F;
  operand_stack_push(&f->operand_stack, (u2)v1 >> v2);
}

// 0x53 Boolean AND short
void ins_sand(frame_t *f) {
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v1 & v2);
}

// 0x55 Boolean OR short
void ins_sor(frame_t *f) {
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v1 | v2);
}

// 0x57 Boolean XOR short
void ins_sxor(frame_t *f) {
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, v1 ^ v2);
}

// 0x59 Increment local short variable by constant
void ins_sinc(frame_t *f) {
  u1 index = bytecode_read_u1();
  i1 v = bytecode_read_u1();
  i2 var = variable_table_get(&f->variable_table, index);
  variable_table_set(&f->variable_table, index, var + v);
}

// 0x5b Convert short to byte
void ins_s2b(frame_t *f) {
  i2 v = operand_stack_pop(&f->operand_stack);
  operand_stack_push(&f->operand_stack, (i1)v);
}

// 0x60 Branch if short comparison with zero succeeds
void ins_ifeq(frame_t *f) {
  i1 branch = bytecode_read_u1();
  i2 v = operand_stack_pop(&f->operand_stack);
  if (v == 0) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x61 Branch if short comparison with zero succeeds
void ins_ifne(frame_t *f) {
  i1 branch = bytecode_read_u1();
  i2 v = operand_stack_pop(&f->operand_stack);
  if (v != 0) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x62 Branch if short comparison with zero succeeds
void ins_iflt(frame_t *f) {
  i1 branch = bytecode_read_u1();
  i2 v = operand_stack_pop(&f->operand_stack);
  if (v < 0) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x63 Branch if short comparison with zero succeeds
void ins_ifge(frame_t *f) {
  i1 branch = bytecode_read_u1();
  i2 v = operand_stack_pop(&f->operand_stack);
  if (v >= 0) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x64 Branch if short comparison with zero succeeds
void ins_ifgt(frame_t *f) {
  i1 branch = bytecode_read_u1();
  i2 v = operand_stack_pop(&f->operand_stack);
  if (v > 0) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x65 Branch if short comparison with zero succeeds
void ins_ifle(frame_t *f) {
  i1 branch = bytecode_read_u1();
  i2 v = operand_stack_pop(&f->operand_stack);
  if (v <= 0) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x64 Branch if short comparison with zero succeeds
void ins_ifnull(frame_t *f) {
  i1 branch = bytecode_read_u1();
  u2 v = operand_stack_pop(&f->operand_stack);
  if (v == 0) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x66 Branch if reference is null
void ins_ifnonnull(frame_t *f) {
  i1 branch = bytecode_read_u1();
  u2 v = (u2)operand_stack_pop(&f->operand_stack);
  if (v > 0) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x68 Branch if reference comparison succeeds
// 0x6a Branch if short comparison succeeds
void ins_if_ascmpeq(frame_t *f) {
  i1 branch = bytecode_read_u1();
  i2 v2 = (u2)operand_stack_pop(&f->operand_stack);
  i2 v1 = (u2)operand_stack_pop(&f->operand_stack);
  if (v1 == v2) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x69 Branch if reference comparison succeeds
// 0x6b Branch if short comparison succeeds
void ins_if_ascmpne(frame_t *f) {
  i1 branch = bytecode_read_u1();
  i2 v2 = (u2)operand_stack_pop(&f->operand_stack);
  i2 v1 = (u2)operand_stack_pop(&f->operand_stack);
  if (v1 != v2) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x6c Branch if short comparison succeeds
void ins_if_scmplt(frame_t *f) {
  i1 branch = bytecode_read_u1();
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  if (v1 < v2) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x6d Branch if short comparison succeeds
void ins_if_scmpge(frame_t *f) {
  i1 branch = bytecode_read_u1();
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  if (v1 >= v2) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x6e Branch if short comparison succeeds
void ins_if_scmpgt(frame_t *f) {
  i1 branch = bytecode_read_u1();
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  if (v1 > v2) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x6f Branch if short comparison succeeds
void ins_if_scmple(frame_t *f) {
  i1 branch = bytecode_read_u1();
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  if (v1 <= v2) {
    bytecode_jump_offset(branch - 1);
  }
}

// 0x70 Branch always
void ins_goto(frame_t *f) {
  i1 branch = bytecode_read_u1();
  bytecode_jump_offset(branch - 1);
}

// 0x71 Jump subroutine
void ins_jsr(frame_t *f) {
  // TODO
}

// 0x72 Return from subroutine
void ins_ret(frame_t *f) {
  // TODO
}

// 0x73 Access jump table by short index and jump
void ins_stableswitch(frame_t *f) {
  i2 index = operand_stack_pop(&f->operand_stack);
  i2 defaultVal = bytecode_read_u2();
  i2 low = bytecode_read_u2();
  i2 high = bytecode_read_u2();
  if (index < low || index > high) {
    bytecode_jump_offset(defaultVal - 6);
  } else {
    bytecode_jump_offset((index - low) * 2);
    u2 offset = bytecode_read_u2();
    bytecode_jump_offset(offset - 8 - (index - low) * 2);
  }
}

// 0x75 Access jump table by key match and jump
void ins_slookupswitch(frame_t *f) {
  i2 key = operand_stack_pop(&f->operand_stack);
  i2 defaultVal = bytecode_read_u2();
  i2 nPairs = bytecode_read_u2();
  for (i2 i = 0; i < nPairs; ++i) {
    i2 match = bytecode_read_u2();
    i2 offset = bytecode_read_u2();
    if (key == match) {
      bytecode_jump_offset(offset - 4 - 4 * (i + 1));
      return;
    }
  }
  bytecode_jump_offset(defaultVal - 4 - 4 * nPairs);
}

// 0x77 Return reference from method
void ins_areturn(frame_t *f) {
  // TODO
}

// 0x78 Return short from method
void ins_sreturn(frame_t *f) {
  // returning from root frame
  if (current_frame == 0) {
    running = 0;
  } else {
    u2 ret = operand_stack_pop(&f->operand_stack);
    f = pop_frame();
    operand_stack_push(&f->operand_stack, ret);
  }
}

// 0x77 Return void from method
void ins_return(frame_t *f) {
  // returning from root frame
  if (current_frame == 0) {
    running = 0;
  } else {
    pop_frame();
  }
}

// 0x7b/0x7c/0x7d Get static field from class
void ins_getstatic_abs(frame_t *f) {
  u2 index = bytecode_read_u2();
  i2 v = constant_pool_get(index);
  operand_stack_push(&f->operand_stack, v);
}

// 0x7f/0x80/0x81 Set static field from class
void ins_putstatic_abs(frame_t *f) {
  u2 index = bytecode_read_u2();
  u2 v = operand_stack_pop(&f->operand_stack);
  constant_pool_set(index, v);
}

// 0x83/0x84/0x85 Fetch field from object
void ins_getfield_abs(frame_t *f) {
  u1 index = bytecode_read_u1();
  i2 objRef = operand_stack_pop(&f->operand_stack);
  i2 value = object_data_get(objRef, index);
  operand_stack_push(&f->operand_stack, value);
}

// 0x87/0x88/0x89 Set field in object
void ins_putfield_abs(frame_t *f) {
  u1 index = bytecode_read_u1();
  i2 value = operand_stack_pop(&f->operand_stack);
  i2 objRef = operand_stack_pop(&f->operand_stack);
  DBG_MSG("Object %d field %d = %d\n", objRef, index, value);
  object_data_set(objRef, index, value);
}

// 0x8b Invoke instance method, dispatch based on class
void ins_invokevirtual(frame_t *f) {
  u2 index = bytecode_read_u2();
  DBG_MSG("Invoke virtual %d\n", index);
  cp_info cp;
  context_read_constant(&current_package, index, (u1 *)&cp, sizeof(cp));
  DBG_MSG("Constant type %d\n", cp.tag);
  if (cp.tag == CONSTANT_VIRTUAL_METHOD_REF) {
  }
}

// 0x8c Invoke instance method; special handling for superclass, private and
// instance initialization method invocations
void ins_invokespecial(frame_t *f) {
  u2 index = bytecode_read_u2();
  DBG_MSG("Invoke special %d\n", index);
  cp_info cp;
  context_read_constant(&current_package, index, (u1 *)&cp, sizeof(cp));
  DBG_MSG("Constant type %d\n", cp.tag);
  if (cp.tag == CONSTANT_STATIC_METHOD_REF) {
    if (cp.static_elem.external_ref.package_token & 0x80) {
      // external
      u1 package = cp.static_elem.external_ref.package_token & 0x7F;
      DBG_MSG("External static method: package %d class %d token %d\n", package,
              cp.static_elem.external_ref.class_token,
              cp.static_elem.external_ref.token);
      package_info info;
      context_read_import(&current_package, (u1 *)&info, package,
                          sizeof(package_info));
      u1 aid_buffer[64];
      context_read_import(&current_package, (u1 *)aid_buffer, package + 3,
                          sizeof(aid_buffer));
      DBG_MSG("Imported from AID");
      for (u1 i = 0; i < info.aid_length;i++) {
        DBG_PRINT(" %02X", aid_buffer[i]);
      }
      DBG_PRINT(", version %d.%d\n", info.major_version, info.minor_version);
    } else {
      u2 offset = htobe16(cp.static_elem.internal_ref.offset);
      DBG_MSG("Internal static method: Method offset %d\n", offset);
      push_frame(offset);
      // copy args from previous frame
      for (int i = 0; i < frames[current_frame].info.nargs - 1; i++) {
        u2 data = operand_stack_pop(&frames[current_frame - 1].operand_stack);
        variable_table_set(&frames[current_frame].variable_table, i, data);
      }
      // set this pointer
      u2 ref = operand_stack_pop(&frames[current_frame - 1].operand_stack);
      frames[current_frame].this_ref = ref;
    }
  }
}

// 0x8d Invoke a class (static) method
void ins_invokestatic(frame_t *f) {
  u2 index = bytecode_read_u2();
  DBG_MSG("Invoke static at %d\n", index);
  cp_info cp;
  context_read_constant(&current_package, index, (u1 *)&cp, sizeof(cp));
  DBG_MSG("Constant type %d\n", cp.tag);
  if (cp.tag == CONSTANT_STATIC_METHOD_REF) {
    if (cp.static_elem.external_ref.package_token & 0x80) {
      // external
      DBG_MSG("Package token %d\n", cp.static_elem.external_ref.package_token);
    } else {
      u2 offset = htobe16(cp.static_elem.internal_ref.offset);
      DBG_MSG("Method offset %d\n", offset);
      push_frame(offset);
    }
  }
}

// 0x8e Invoke interface method
void ins_invokeinterface(frame_t *f) {
  // TODO
}

// 0x8f Create new object
void ins_new(frame_t *f) {
  u2 index = bytecode_read_u2();
  cp_info info;
  context_read_constant(&current_package, index, (u1 *)&info, sizeof(info));
  u2 ref = htobe16(info.klass.internal_ref);
  u2 res = context_create_object(&current_package, ref);
  operand_stack_push(&f->operand_stack, res);
  DBG_MSG("Created new object at index=%d\n", res);
}

// 0x90 Create new array
void ins_newarray(frame_t *f) {
  u1 atype = bytecode_read_u1();
  i2 count = operand_stack_pop(&f->operand_stack);
  int ref = context_create_array(&current_package, atype, 0, count);
  if (ref > 0)
    operand_stack_push(&f->operand_stack, ref);
  else {
    // TODO: error
  }
}

// 0x91 Create new array of reference
void ins_anewarray(frame_t *f) {
  // TODO
}

// 0x92 Get length of array
void ins_arraylength(frame_t *f) {
  i2 ref = operand_stack_pop(&f->operand_stack);
  array_metadata_t metadata;
  context_array_meta(&current_package, ref, &metadata);
  operand_stack_push(&f->operand_stack, metadata.length);
}

// 0x93 Throw exception or error
void ins_athrow(frame_t *f) {
  // TODO
}

// 0x94 Check whether objects is of given type
void ins_checkcast(frame_t *f) {
  // TODO
}

// 0x95 Determine if object is of given type
void ins_instanceof(frame_t *f) {
  // TODO
}

// 0x96 Increment local short variable by constant
void ins_sinc_w(frame_t *f) {
  u1 index = bytecode_read_u1();
  i2 v = bytecode_read_u2();
  i2 var = variable_table_get(&f->variable_table, index);
  variable_table_set(&f->variable_table, index, var + v);
}

// 0x98 Branch if short comparison with zero succeeds (wide index)
void ins_ifeq_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  i2 v = operand_stack_pop(&f->operand_stack);
  if (v == 0) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0x99 Branch if short comparison with zero succeeds (wide index)
void ins_ifne_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  i2 v = operand_stack_pop(&f->operand_stack);
  if (v != 0) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0x9a Branch if short comparison with zero succeeds (wide index)
void ins_iflt_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  i2 v = operand_stack_pop(&f->operand_stack);
  if (v < 0) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0x9b Branch if short comparison with zero succeeds (wide index)
void ins_ifge_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  i2 v = operand_stack_pop(&f->operand_stack);
  if (v >= 0) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0x9c Branch if short comparison with zero succeeds (wide index)
void ins_ifgt_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  i2 v = operand_stack_pop(&f->operand_stack);
  if (v > 0) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0x9d Branch if short comparison with zero succeeds (wide index)
void ins_ifle_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  i2 v = operand_stack_pop(&f->operand_stack);
  if (v <= 0) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0x9e Branch if reference is null (wide index)
void ins_ifnull_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  u2 v = operand_stack_pop(&f->operand_stack);
  if (v == 0) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0x9f Branch if reference is null (wide index)
void ins_ifnonnull_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  u2 v = (u2)operand_stack_pop(&f->operand_stack);
  if (v > 0) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0xa0 Branch if reference comparison succeeds (wide index)
// 0xa2 Branch if short comparison succeeds (wide index)
void ins_if_ascmpeq_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  i2 v2 = (u2)operand_stack_pop(&f->operand_stack);
  i2 v1 = (u2)operand_stack_pop(&f->operand_stack);
  if (v1 == v2) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0xa1 Branch if reference comparison succeeds (wide index)
// 0xa3 Branch if short comparison succeeds (wide index)
void ins_if_ascmpne_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  i2 v2 = (u2)operand_stack_pop(&f->operand_stack);
  i2 v1 = (u2)operand_stack_pop(&f->operand_stack);
  if (v1 != v2) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0xa4 Branch if short comparison succeeds (wide index)
void ins_if_scmplt_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  if (v1 < v2) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0xa5 Branch if short comparison succeeds (wide index)
void ins_if_scmpge_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  if (v1 >= v2) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0xa6 Branch if short comparison succeeds (wide index)
void ins_if_scmpgt_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  if (v1 > v2) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0xa7 Branch if short comparison succeeds (wide index)
void ins_if_scmple_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  i2 v2 = operand_stack_pop(&f->operand_stack);
  i2 v1 = operand_stack_pop(&f->operand_stack);
  if (v1 <= v2) {
    bytecode_jump_offset(branch - 2);
  }
}

// 0xa8 Branch always (wide index)
void ins_goto_w(frame_t *f) {
  i2 branch = bytecode_read_u2();
  bytecode_jump_offset(branch - 2);
}

// 0xa9/0xaa/0xab Fetch field from object (wide index)
void ins_getfield_abs_w(frame_t *f) {
  u2 index = bytecode_read_u2();
  i2 objRef = operand_stack_pop(&f->operand_stack);
  i2 value = object_data_get(objRef, index);
  operand_stack_push(&f->operand_stack, value);
}

// 0xad/0xae/0xaf Fetch field from current object
void ins_getfield_abs_this(frame_t *f) {
  u1 index = bytecode_read_u1();
  i2 objRef = variable_table_get(&f->variable_table, 0);
  i2 value = object_data_get(objRef, index);
  operand_stack_push(&f->operand_stack, value);
}

// 0xb1/0xb2/0xb3 Set field in object (wide index)
void ins_putfield_abs_w(frame_t *f) {
  u2 index = bytecode_read_u2();
  i2 value = operand_stack_pop(&f->operand_stack);
  i2 objRef = operand_stack_pop(&f->operand_stack);
  object_data_set(objRef, index, value);
}

// 0xb5/0xb6/0xb7 Set field in current object
void ins_putfield_abs_this(frame_t *f) {
  u1 index = bytecode_read_u1();
  i2 value = operand_stack_pop(&f->operand_stack);
  i2 objRef = variable_table_get(&f->variable_table, 0);
  object_data_set(objRef, index, value);
}

void (*opcodes[256])(frame_t *) = {
    // 0x00
    ins_nop, ins_aconst_null, ins_sconst_m1, ins_sconst_0, ins_sconst_1,
    ins_sconst_2, ins_sconst_3, ins_sconst_4, ins_sconst_5, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid,
    // 0x10
    ins_bspush, ins_sspush, ins_invalid, ins_invalid, ins_invalid,
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
    // 0x20
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_aaload, ins_baload,
    ins_saload, ins_invalid,
    ins_asstore, // astore
    ins_asstore, // sstore
    ins_invalid,
    ins_asstore_0, // astore_0
    ins_asstore_1, // astore_1
    ins_asstore_2, // astore_2
    ins_asstore_3, // astore_3
    // 0x30
    ins_asstore_0, // sstore_0
    ins_asstore_1, // sstore_1
    ins_asstore_2, // sstore_2
    ins_asstore_3, // sstore_3
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_aastore,
    ins_bastore, ins_sastore, ins_invalid, ins_pop, ins_pop2, ins_dup, ins_dup2,
    ins_dup_x,
    // 0x40
    ins_swap_x, ins_sadd, ins_invalid, ins_ssub, ins_invalid, ins_smul,
    ins_invalid, ins_sdiv, ins_invalid, ins_srem, ins_invalid, ins_sneg,
    ins_invalid, ins_sshl, ins_invalid, ins_sshr,
    // 0x50
    ins_invalid, ins_sushr, ins_invalid, ins_sand, ins_invalid, ins_sor,
    ins_invalid, ins_sxor, ins_invalid, ins_sinc, ins_invalid, ins_s2b,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    // 0x60
    ins_ifeq, ins_ifne, ins_iflt, ins_ifge, ins_ifgt, ins_ifle, ins_ifnull,
    ins_ifnonnull,
    ins_if_ascmpeq, // if_acmpeq
    ins_if_ascmpne, // if_acmpne
    ins_if_ascmpeq, // if_scmpeq
    ins_if_ascmpne, // if_scmpne
    ins_if_scmplt, ins_if_scmpge, ins_if_scmpgt, ins_if_scmple,
    // 0x70
    ins_goto, ins_jsr, ins_ret, ins_stableswitch, ins_invalid,
    ins_slookupswitch, ins_invalid, ins_areturn, ins_sreturn, ins_invalid,
    ins_return,
    ins_getstatic_abs, // getstatic_a
    ins_getstatic_abs, // getstatic_b
    ins_getstatic_abs, // getstatic_s
    ins_invalid,
    ins_putstatic_abs, // putstatic_a
    // 0x80
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
    ins_invalid, ins_invokevirtual, ins_invokespecial, ins_invokestatic,
    ins_invokeinterface, ins_new,
    // 0x90
    ins_newarray, ins_anewarray, ins_arraylength, ins_athrow, ins_checkcast,
    ins_instanceof, ins_sinc_w, ins_invalid, ins_ifeq_w, ins_ifne_w, ins_iflt_w,
    ins_ifge_w, ins_ifgt_w, ins_ifle_w, ins_ifnull_w, ins_ifnonnull_w,
    // 0xa0
    ins_if_ascmpeq_w, // ins_if_acmpeq_w
    ins_if_ascmpne_w, // ins_if_acmpne_w
    ins_if_ascmpeq_w, // ins_if_scmpeq_w
    ins_if_ascmpne_w, // ins_if_scmpne_w
    ins_if_scmplt_w, ins_if_scmpge_w, ins_if_scmpgt_w, ins_if_scmple_w,
    ins_goto_w,
    ins_getfield_abs_w, // getfield_a_w
    ins_getfield_abs_w, // getfield_b_w
    ins_getfield_abs_w, // getfield_s_w
    ins_invalid,
    ins_getfield_abs_this, // getfield_a_this
    ins_getfield_abs_this, // getfield_b_this
    ins_getfield_abs_this, // getfield_s_this
    // 0xb0
    ins_invalid,
    ins_putfield_abs_w, // putfield_a_w
    ins_putfield_abs_w, // putfield_b_w
    ins_putfield_abs_w, // putfield_s_w
    ins_invalid,
    ins_putfield_abs_this, // putfield_a_this
    ins_putfield_abs_this, // putfield_b_this
    ins_putfield_abs_this, // putfield_s_this
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, ins_invalid, ins_invalid, ins_invalid, ins_invalid,
    ins_invalid, // impdep
    ins_invalid, // impdep2
};
