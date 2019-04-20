#ifndef JIECARDVM_INSTRUCTIONS_H
#define JIECARDVM_INSTRUCTIONS_H

#include "rtda.h"

#ifdef __cplusplus
extern "C" {
#endif

void ins_invalid(frame_t *f);
void ins_nop(frame_t *f);
void ins_aconst_null(frame_t *f);
void ins_sconst_m1(frame_t *f);
void ins_sconst_0(frame_t *f);
void ins_sconst_1(frame_t *f);
void ins_sconst_2(frame_t *f);
void ins_sconst_3(frame_t *f);
void ins_sconst_4(frame_t *f);
void ins_sconst_5(frame_t *f);
void ins_bspush(frame_t *f);
void ins_sspush(frame_t *f);
void ins_asload(frame_t *f);
void ins_asload_0(frame_t *f);
void ins_asload_1(frame_t *f);
void ins_asload_2(frame_t *f);
void ins_asload_3(frame_t *f);
void ins_aaload(frame_t *f);
void ins_saload(frame_t *f);
void ins_baload(frame_t *f);
void ins_asstore(frame_t *f);
void ins_asstore_0(frame_t *f);
void ins_asstore_1(frame_t *f);
void ins_asstore_2(frame_t *f);
void ins_asstore_3(frame_t *f);
void ins_aastore(frame_t *f);
void ins_bastore(frame_t *f);
void ins_sastore(frame_t *f);
void ins_pop(frame_t *f);
void ins_pop2(frame_t *f);
void ins_dup(frame_t *f);
void ins_dup2(frame_t *f);
void ins_dup_x(frame_t *f);
void ins_swap_x(frame_t *f);
void ins_sadd(frame_t *f);
void ins_ssub(frame_t *f);
void ins_smul(frame_t *f);
void ins_sdiv(frame_t *f);
void ins_srem(frame_t *f);
void ins_sneg(frame_t *f);
void ins_sshl(frame_t *f);
void ins_sshr(frame_t *f);
void ins_sushr(frame_t *f);
void ins_sand(frame_t *f);
void ins_sor(frame_t *f);
void ins_sxor(frame_t *f);
void ins_sinc(frame_t *f);
void ins_s2b(frame_t *f);
void ins_ifeq(frame_t *f);
void ins_ifne(frame_t *f);
void ins_iflt(frame_t *f);
void ins_ifge(frame_t *f);
void ins_ifgt(frame_t *f);
void ins_ifle(frame_t *f);
void ins_ifnull(frame_t *f);
void ins_ifnonnull(frame_t *f);
void ins_if_ascmpeq(frame_t *f);
void ins_if_ascmpne(frame_t *f);
void ins_if_scmplt(frame_t *f);
void ins_if_scmpge(frame_t *f);
void ins_if_scmpgt(frame_t *f);
void ins_if_scmple(frame_t *f);
void ins_goto(frame_t *f);
void ins_jsr(frame_t *f);
void ins_ret(frame_t *f);
void ins_stableswitch(frame_t *f);
void ins_slookupswitch(frame_t *f);
void ins_areturn(frame_t *f);
void ins_sreturn(frame_t *f);
void ins_return(frame_t *f);
void ins_getstatic_abs(frame_t *f);
void ins_putstatic_abs(frame_t *f);
void ins_getfield_abs(frame_t *f);
void ins_putfield_abs(frame_t *f);
void ins_invokevirtual(frame_t *f);
void ins_invokespecial(frame_t *f);
void ins_invokestatic(frame_t *f);
void ins_invokeinterface(frame_t *f);
void ins_new(frame_t *f);
void ins_newarray(frame_t *f);
void ins_anewarray(frame_t *f);
void ins_arraylength(frame_t *f);
void ins_athrow(frame_t *f);
void ins_checkcast(frame_t *f);
void ins_instanceof(frame_t *f);
void ins_sinc_w(frame_t *f);
void ins_ifeq_w(frame_t *f);
void ins_ifne_w(frame_t *f);
void ins_iflt_w(frame_t *f);
void ins_ifge_w(frame_t *f);
void ins_ifgt_w(frame_t *f);
void ins_ifle_w(frame_t *f);
void ins_ifnull_w(frame_t *f);
void ins_ifnonnull_w(frame_t *f);
void ins_if_acmpeq_w(frame_t *f);
void ins_if_acmpne_w(frame_t *f);
void ins_if_scmpeq_w(frame_t *f);
void ins_if_scmpne_w(frame_t *f);
void ins_if_scmplt_w(frame_t *f);
void ins_if_scmpge_w(frame_t *f);
void ins_if_scmpgt_w(frame_t *f);
void ins_if_scmple_w(frame_t *f);
void ins_goto_w(frame_t *f);
void ins_getfield_abs_w(frame_t *f);
void ins_getfield_abs_this(frame_t *f);
void ins_putfield_abs_w(frame_t *f);
void ins_putfield_abs_this(frame_t *f);

extern void (*opcodes[256])(frame_t *);

#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_INSTRUCTIONS_H
