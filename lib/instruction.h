#ifndef JIECARDVM_INSTRUCTION_H
#define JIECARDVM_INSTRUCTION_H

#ifdef __cplusplus
extern "C" {
#endif

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
void ins_asload(Frame *f);
void ins_asload_0(Frame *f);
void ins_asload_1(Frame *f);
void ins_asload_2(Frame *f);
void ins_asload_3(Frame *f);
void ins_aaload(Frame *f);
void ins_baload(Frame *f);
void ins_saload(Frame *f);
void ins_asstore(Frame *f);
void ins_asstore_0(Frame *f);
void ins_asstore_1(Frame *f);
void ins_asstore_2(Frame *f);
void ins_asstore_3(Frame *f);
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
void ins_getstatic_abs(Frame *f);
void ins_putstatic_abs(Frame *f);
void ins_getfield_abs(Frame *f);
void ins_putfield_abs(Frame *f);
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
void ins_getfield_abs_w(Frame *f);
void ins_getfield_abs_this(Frame *f);
void ins_putfield_abs_w(Frame *f);
void ins_putfield_abs_this(Frame *f);

extern void (*opcodes[256])(Frame *);

#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_INSTRUCTION_H
