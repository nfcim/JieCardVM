#include <string.h>

#include "rtda.h"

void ins_nop(Frame *f) {}

void ins_aconst_null(Frame *f) { OSPush(&f->operandStack, JNULL); }

void ins_sconst_m1(Frame *f) { OSPush(&f->operandStack, -1); }

void ins_sconst_0(Frame *f) { OSPush(&f->operandStack, 0); }

void ins_sconst_1(Frame *f) { OSPush(&f->operandStack, 1); }

void ins_sconst_2(Frame *f) { OSPush(&f->operandStack, 2); }

void ins_sconst_3(Frame *f) { OSPush(&f->operandStack, 3); }

void ins_sconst_4(Frame *f) { OSPush(&f->operandStack, 4); }

void ins_sconst_5(Frame *f) { OSPush(&f->operandStack, 5); }

void ins_bspush(Frame *f) {
  jbyte v = BCReadU1();
  OSPush(&f->operandStack, v);
}

void ins_sspush(Frame *f) {
  jshort v = BCReadU2();
  OSPush(&f->operandStack, v);
}

void ins_asload(Frame *f) {
  u1 index = BCReadU1();
  jshort v = VTGet(&f->variableTable, index);
  OSPush(&f->operandStack, v);
}

void ins_asload_0(Frame *f) {
  jshort v = VTGet(&f->variableTable, 0);
  OSPush(&f->operandStack, v);
}

void ins_asload_1(Frame *f) {
  jshort v = VTGet(&f->variableTable, 1);
  OSPush(&f->operandStack, v);
}

void ins_asload_2(Frame *f) {
  jshort v = VTGet(&f->variableTable, 2);
  OSPush(&f->operandStack, v);
}

void ins_asload_3(Frame *f) {
  jshort v = VTGet(&f->variableTable, 3);
  OSPush(&f->operandStack, v);
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
  u1 index = BCReadU1();
  jshort v = OSPop(&f->operandStack);
  VTSet(&f->variableTable, index, v);
}

void ins_asstore_0(Frame *f) {
  jshort v = OSPop(&f->operandStack);
  VTSet(&f->variableTable, 0, v);
}

void ins_asstore_1(Frame *f) {
  jshort v = OSPop(&f->operandStack);
  VTSet(&f->variableTable, 1, v);
}

void ins_asstore_2(Frame *f) {
  jshort v = OSPop(&f->operandStack);
  VTSet(&f->variableTable, 2, v);
}

void ins_asstore_3(Frame *f) {
  jshort v = OSPop(&f->operandStack);
  VTSet(&f->variableTable, 3, v);
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

void ins_pop(Frame *f) { OSPop(&f->operandStack); }

void ins_pop2(Frame *f) {
  OSPop(&f->operandStack);
  OSPop(&f->operandStack);
}

void ins_dup(Frame *f) {
  jshort v = OSGet(&f->operandStack);
  OSPush(&f->operandStack, v);
}

void ins_dup2(Frame *f) {
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSGet(&f->operandStack);
  OSPush(&f->operandStack, v2);
  OSPush(&f->operandStack, v1);
  OSPush(&f->operandStack, v2);
}

void ins_dup_x(Frame *f) {
  u1 mn = BCReadU1();
  u1 m = (mn >> 4) * 2;
  u1 n = (mn & 0xF) * 2;
  memmove(f->operandStack.next - n + m, f->operandStack.next - n, n);
  memcpy(f->operandStack.next - n, f->operandStack.next, m);
  f->operandStack.next += m;
}

void ins_swap_x(Frame *f) {
  u1 buf[2];
  u1 mn = BCReadU1();
  u1 m = (mn >> 4) * 2;
  u1 n = (mn & 0xF) * 2;
  memcpy(buf, f->operandStack.next - m, m);
  memmove(f->operandStack.next - n, f->operandStack.next - n - m, n);
  memcpy(f->operandStack.next - n - m, buf, m);
}

void ins_sadd(Frame *f) {
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  jshort result = v1 + v2;
  OSPush(&f->operandStack, result);
}

void ins_ssub(Frame *f) {
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  jshort result = v1 - v2;
  OSPush(&f->operandStack, result);
}

void ins_smul(Frame *f) {
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  jshort result = v1 * v2;
  OSPush(&f->operandStack, result);
}

void ins_sdiv(Frame *f) {
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  jshort result = v1 / v2;
  OSPush(&f->operandStack, result);
}

void ins_srem(Frame *f) {
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  jshort result = v1 % v2;
  OSPush(&f->operandStack, result);
}

void ins_sneg(Frame *f) {
  jshort v = OSPop(&f->operandStack);
  OSPush(&f->operandStack, -v);
}

void ins_sshl(Frame *f) {
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  v2 &= 0x1F;
  OSPush(&f->operandStack, v1 << v2);
}

void ins_sshr(Frame *f) {
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  v2 &= 0x1F;
  OSPush(&f->operandStack, v1 >> v2);
}

void ins_sushr(Frame *f) {
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  v2 &= 0x1F;
  OSPush(&f->operandStack, (u2)v1 >> v2);
}

void ins_sand(Frame *f) {
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  OSPush(&f->operandStack, v1 & v2);
}

void ins_sor(Frame *f) {
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  OSPush(&f->operandStack, v1 | v2);
}

void ins_sxor(Frame *f) {
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  OSPush(&f->operandStack, v1 ^ v2);
}

void ins_sinc(Frame *f) {
  u1 index = BCReadU1();
  jbyte v = BCReadU1();
  jshort var = VTGet(&f->variableTable, index);
  VTSet(&f->variableTable, index, var + v);
}

void ins_s2b(Frame *f) {
  jshort v = OSPop(&f->operandStack);
  OSPush(&f->operandStack, (jbyte)v);
}

void ins_ifeq(Frame *f) {
  jbyte branch = BCReadU1();
  jshort v = OSPop(&f->operandStack);
  if (v == 0) {
    PCSetOffset(branch);
  }
}

void ins_ifne(Frame *f) {
  jbyte branch = BCReadU1();
  jshort v = OSPop(&f->operandStack);
  if (v != 0) {
    PCSetOffset(branch);
  }
}

void ins_iflt(Frame *f) {
  jbyte branch = BCReadU1();
  jshort v = OSPop(&f->operandStack);
  if (v < 0) {
    PCSetOffset(branch);
  }
}

void ins_ifge(Frame *f) {
  jbyte branch = BCReadU1();
  jshort v = OSPop(&f->operandStack);
  if (v >= 0) {
    PCSetOffset(branch);
  }
}

void ins_ifgt(Frame *f) {
  jbyte branch = BCReadU1();
  jshort v = OSPop(&f->operandStack);
  if (v > 0) {
    PCSetOffset(branch);
  }
}

void ins_ifle(Frame *f) {
  jbyte branch = BCReadU1();
  jshort v = OSPop(&f->operandStack);
  if (v <= 0) {
    PCSetOffset(branch);
  }
}

void ins_ifnull(Frame *f) {
  jbyte branch = BCReadU1();
  u2 v = OSPop(&f->operandStack);
  if (v == 0) {
    PCSetOffset(branch);
  }
}

void ins_ifnonnull(Frame *f) {
  jbyte branch = BCReadU1();
  u2 v = (u2)OSPop(&f->operandStack);
  if (v > 0) {
    PCSetOffset(branch);
  }
}

void ins_if_acmpeq(Frame *f) {
  jbyte branch = BCReadU1();
  u2 v2 = (u2)OSPop(&f->operandStack);
  u2 v1 = (u2)OSPop(&f->operandStack);
  if (v1 == v2) {
    PCSetOffset(branch);
  }
}

void ins_if_acmpne(Frame *f) {
  jbyte branch = BCReadU1();
  u2 v2 = (u2)OSPop(&f->operandStack);
  u2 v1 = (u2)OSPop(&f->operandStack);
  if (v1 != v2) {
    PCSetOffset(branch);
  }
}

void ins_if_scmpeq(Frame *f) {
  jbyte branch = BCReadU1();
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  if (v1 == v2) {
    PCSetOffset(branch);
  }
}

void ins_if_scmpne(Frame *f) {
  jbyte branch = BCReadU1();
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  if (v1 != v2) {
    PCSetOffset(branch);
  }
}

void ins_if_scmplt(Frame *f) {
  jbyte branch = BCReadU1();
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  if (v1 < v2) {
    PCSetOffset(branch);
  }
}

void ins_if_scmpge(Frame *f) {
  jbyte branch = BCReadU1();
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  if (v1 >= v2) {
    PCSetOffset(branch);
  }
}

void ins_if_scmpgt(Frame *f) {
  jbyte branch = BCReadU1();
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  if (v1 > v2) {
    PCSetOffset(branch);
  }
}

void ins_if_scmple(Frame *f) {
  jbyte branch = BCReadU1();
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  if (v1 <= v2) {
    PCSetOffset(branch);
  }
}

void ins_goto(Frame *f) {
  jbyte branch = BCReadU1();
  PCSetOffset(branch);
}

void ins_jsr(Frame *f) {
  // TODO
}

void ins_ret(Frame *f) {
  // TODO
}

void ins_stableswitch(Frame *f) {
  u1 *pc = PCGet();
  jshort index = OSPop(&f->operandStack);
  jshort defaultVal = BCReadU2();
  jshort low = BCReadU2();
  jshort high = BCReadU2();
  if (index < low || index > high) {
    PCSet(pc + defaultVal);
  } else {
    u1 *offset = pc + 6 + (index - low) * 2;
    PCSet(pc + ((*offset << 8) | *(offset + 1)));
  }
}

void ins_slookupswitch(Frame *f) {
  u1 *pc = PCGet();
  jshort key = OSPop(&f->operandStack);
  jshort defaultVal = BCReadU2();
  jshort nPairs = BCReadU2();
  for (jshort i = 0; i < nPairs; ++i) {
    jshort match = BCReadU2();
    jshort offset = BCReadU2();
    if (key == match) {
      PCSet(pc + 4 + offset);
      return;
    }
  }
  PCSet(pc + 4 + defaultVal);
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
  u2 index = BCReadU2();
  jshort v = CPGetData(index);
  OSPush(&f->operandStack, v);
}

void ins_putstatic_abs(Frame *f) {
  u2 index = BCReadU2();
  jshort v = OSPop(&f->operandStack);
  CPSetData(index, v);
}

void ins_getfield_abs(Frame *f) {
  u1 index = BCReadU1();
  jshort objRef = OSPop(&f->operandStack);
  jshort value = ODGet(objRef, index);
  OSPush(&f->operandStack, value);
}

void ins_putfield_abs(Frame *f) {
  u1 index = BCReadU1();
  jshort value = OSPop(&f->operandStack);
  jshort objRef = OSPop(&f->operandStack);
  ODSet(objRef, index, value);
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

void ins_sinc_w(Frame *f) {
  u1 index = BCReadU1();
  jshort v = BCReadU2();
  jshort var = VTGet(&f->variableTable, index);
  VTSet(&f->variableTable, index, var + v);
}

void ins_ifeq_w(Frame *f) {
  jshort branch = BCReadU2();
  jshort v = OSPop(&f->operandStack);
  if (v == 0) {
    PCSetOffset(branch);
  }
}

void ins_ifne_w(Frame *f) {
  jshort branch = BCReadU2();
  jshort v = OSPop(&f->operandStack);
  if (v != 0) {
    PCSetOffset(branch);
  }
}

void ins_iflt_w(Frame *f) {
  jshort branch = BCReadU2();
  jshort v = OSPop(&f->operandStack);
  if (v < 0) {
    PCSetOffset(branch);
  }
}

void ins_ifge_w(Frame *f) {
  jshort branch = BCReadU2();
  jshort v = OSPop(&f->operandStack);
  if (v >= 0) {
    PCSetOffset(branch);
  }
}

void ins_ifgt_w(Frame *f) {
  jshort branch = BCReadU2();
  jshort v = OSPop(&f->operandStack);
  if (v > 0) {
    PCSetOffset(branch);
  }
}

void ins_ifle_w(Frame *f) {
  jshort branch = BCReadU2();
  jshort v = OSPop(&f->operandStack);
  if (v <= 0) {
    PCSetOffset(branch);
  }
}

void ins_ifnull_w(Frame *f) {
  jshort branch = BCReadU2();
  u2 v = OSPop(&f->operandStack);
  if (v == 0) {
    PCSetOffset(branch);
  }
}

void ins_ifnonnull_w(Frame *f) {
  jshort branch = BCReadU2();
  u2 v = (u2)OSPop(&f->operandStack);
  if (v > 0) {
    PCSetOffset(branch);
  }
}

void ins_if_acmpeq_w(Frame *f) {
  jshort branch = BCReadU2();
  u2 v2 = (u2)OSPop(&f->operandStack);
  u2 v1 = (u2)OSPop(&f->operandStack);
  if (v1 == v2) {
    PCSetOffset(branch);
  }
}

void ins_if_acmpne_w(Frame *f) {
  jshort branch = BCReadU2();
  u2 v2 = (u2)OSPop(&f->operandStack);
  u2 v1 = (u2)OSPop(&f->operandStack);
  if (v1 != v2) {
    PCSetOffset(branch);
  }
}

void ins_if_scmpeq_w(Frame *f) {
  jshort branch = BCReadU2();
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  if (v1 == v2) {
    PCSetOffset(branch);
  }
}

void ins_if_scmpne_w(Frame *f) {
  jshort branch = BCReadU2();
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  if (v1 != v2) {
    PCSetOffset(branch);
  }
}

void ins_if_scmplt_w(Frame *f) {
  jshort branch = BCReadU2();
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  if (v1 < v2) {
    PCSetOffset(branch);
  }
}

void ins_if_scmpge_w(Frame *f) {
  jshort branch = BCReadU2();
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  if (v1 >= v2) {
    PCSetOffset(branch);
  }
}

void ins_if_scmpgt_w(Frame *f) {
  jshort branch = BCReadU2();
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  if (v1 > v2) {
    PCSetOffset(branch);
  }
}

void ins_if_scmple_w(Frame *f) {
  jshort branch = BCReadU2();
  jshort v2 = OSPop(&f->operandStack);
  jshort v1 = OSPop(&f->operandStack);
  if (v1 <= v2) {
    PCSetOffset(branch);
  }
}

void ins_goto_w(Frame *f) {
  jshort branch = BCReadU2();
  PCSetOffset(branch);
}
