#ifndef JIECARDVM_INSTRUCTION_H
#define JIECARDVM_INSTRUCTION_H

#include "rtda.h"

void ins_invalid(Frame *f);
void ins_nop(Frame *f);
void ins_aconst_null(Frame *f);
void ins_sconst_m1(Frame *f);
void ins_sconst_0(Frame *f);
void ins_sconst_1(Frame *f);
void ins_sconst_2(Frame *f);
void ins_sconst_3(Frame *f);
void ins_sconst_4(Frame *f);
void ins_sconst_5(Frame *f);
void ins_bspush(Frame *f);
void ins_sspush(Frame *f);
void ins_aload(Frame *f);
void ins_sload(Frame *f);
void ins_aload_0(Frame *f);
void ins_aload_1(Frame *f);
void ins_aload_2(Frame *f);
void ins_aload_3(Frame *f);
void ins_sload_0(Frame *f);
void ins_sload_1(Frame *f);
void ins_sload_2(Frame *f);
void ins_sload_3(Frame *f);
void ins_aaload(Frame *f);
void ins_baload(Frame *f);
void ins_saload(Frame *f);
void ins_astore(Frame *f);
void ins_sstore(Frame *f);
void ins_astore_0(Frame *f);
void ins_astore_1(Frame *f);
void ins_astore_2(Frame *f);
void ins_astore_3(Frame *f);
void ins_sstore_0(Frame *f);
void ins_sstore_1(Frame *f);
void ins_sstore_2(Frame *f);
void ins_sstore_3(Frame *f);
void ins_aastore(Frame *f);
void ins_bastore(Frame *f);
void ins_sastore(Frame *f);
void ins_pop(Frame *f);
void ins_pop2(Frame *f);
void ins_dup(Frame *f);
void ins_dup2(Frame *f);
void ins_dup_x(Frame *f);
void ins_swap_x(Frame *f);
void ins_sadd(Frame *f);
void ins_ssub(Frame *f);
void ins_smul(Frame *f);
void ins_sdiv(Frame *f);
void ins_srem(Frame *f);
void ins_sneg(Frame *f);
void ins_sshl(Frame *f);
void ins_sshr(Frame *f);
void ins_sushr(Frame *f);
void ins_sand(Frame *f);
void ins_sor(Frame *f);
void ins_sxor(Frame *f);
void ins_sinc(Frame *f);
void ins_s2b(Frame *f);
void ins_ifeq(Frame *f);
void ins_ifne(Frame *f);
void ins_iflt(Frame *f);
void ins_ifge(Frame *f);
void ins_ifgt(Frame *f);
void ins_ifle(Frame *f);
void ins_ifnull(Frame *f);
void ins_ifnonnull(Frame *f);
void ins_if_acmpeq(Frame *f);
void ins_if_acmpne(Frame *f);
void ins_if_scmpeq(Frame *f);
void ins_if_scmpne(Frame *f);
void ins_if_scmplt(Frame *f);
void ins_if_scmpge(Frame *f);
void ins_if_scmpgt(Frame *f);
void ins_if_scmple(Frame *f);
void ins_goto(Frame *f);
void ins_jsr(Frame *f);
void ins_ret(Frame *f);
void ins_stableswitch(Frame *f);
void ins_slookupswitch(Frame *f);
void ins_areturn(Frame *f);
void ins_sreturn(Frame *f);
void ins_return(Frame *f);
void ins_getstatic_a(Frame *f);
void ins_getstatic_b(Frame *f);
void ins_getstatic_s(Frame *f);
void ins_putstatic_a(Frame *f);
void ins_putstatic_b(Frame *f);
void ins_putstatic_s(Frame *f);
void ins_getfield_a(Frame *f);
void ins_getfield_b(Frame *f);
void ins_getfield_s(Frame *f);
void ins_getfield_i(Frame *f);
void ins_putfield_a(Frame *f);
void ins_putfield_b(Frame *f);
void ins_putfield_s(Frame *f);
void ins_putfield_i(Frame *f);
void ins_invokevirtual(Frame *f);
void ins_invokespecial(Frame *f);
void ins_invokestatic(Frame *f);
void ins_invokeinterface(Frame *f);
void ins_new(Frame *f);
void ins_newarray(Frame *f);
void ins_anewarray(Frame *f);
void ins_arraylength(Frame *f);
void ins_athrow(Frame *f);
void ins_checkcast(Frame *f);
void ins_instanceof(Frame *f);
void ins_sinc_w(Frame *f);
void ins_ifeq_w(Frame *f);
void ins_ifne_w(Frame *f);
void ins_iflt_w(Frame *f);
void ins_ifge_w(Frame *f);
void ins_ifgt_w(Frame *f);
void ins_ifle_w(Frame *f);
void ins_ifnull_w(Frame *f);
void ins_ifnonnull_w(Frame *f);
void ins_if_acmpeq_w(Frame *f);
void ins_if_acmpne_w(Frame *f);
void ins_if_scmpeq_w(Frame *f);
void ins_if_scmpne_w(Frame *f);
void ins_if_scmplt_w(Frame *f);
void ins_if_scmpge_w(Frame *f);
void ins_if_scmpgt_w(Frame *f);
void ins_if_scmple_w(Frame *f);
void ins_goto_w(Frame *f);
void ins_getfield_a_w(Frame *f);
void ins_getfield_b_w(Frame *f);
void ins_getfield_s_w(Frame *f);
void ins_getfield_a_this(Frame *f);
void ins_getfield_b_this(Frame *f);
void ins_getfield_s_this(Frame *f);
void ins_putfield_a_w(Frame *f);
void ins_putfield_b_w(Frame *f);
void ins_putfield_s_w(Frame *f);
void ins_putfield_a_this(Frame *f);
void ins_putfield_b_this(Frame *f);
void ins_putfield_s_this(Frame *f);

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
    ins_aload,
    ins_sload,
    ins_invalid,
    ins_aload_0,
    ins_aload_1,
    ins_aload_2,
    ins_aload_3,
    ins_sload_0,
    ins_sload_1,
    ins_sload_2,
    ins_sload_3,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_invalid,
    ins_aaload,
    ins_baload,
    ins_saload,
    ins_invalid,
    ins_astore,
    ins_sstore,
    ins_invalid,
    ins_astore_0,
    ins_astore_1,
    ins_astore_2,
    ins_astore_3,
    ins_sstore_0,
    ins_sstore_1,
    ins_sstore_2,
    ins_sstore_3,
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
    ins_getstatic_a,
    ins_getstatic_b,
    ins_getstatic_s,
    ins_invalid,
    ins_putstatic_a,
    ins_putstatic_b,
    ins_putstatic_s,
    ins_invalid,
    ins_getfield_a,
    ins_getfield_b,
    ins_getfield_s,
    ins_getfield_i,
    ins_putfield_a,
    ins_putfield_b,
    ins_putfield_s,
    ins_putfield_i,
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
    ins_getfield_a_w,
    ins_getfield_b_w,
    ins_getfield_s_w,
    ins_invalid,
    ins_getfield_a_this,
    ins_getfield_b_this,
    ins_getfield_s_this,
    ins_invalid,
    ins_putfield_a_w,
    ins_putfield_b_w,
    ins_putfield_s_w,
    ins_invalid,
    ins_putfield_a_this,
    ins_putfield_b_this,
    ins_putfield_s_this,
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

#endif // JIECARDVM_INSTRUCTION_H
