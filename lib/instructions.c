#include "rtdata.h"

// ins 00

void ins_nop(Frame *f) {
}

void ins_aconst_null(Frame *f) {
    // TODO
}

static void ins_sconst_number(Frame *f, jshort val) {
    OperandStack *s;
    s = &f->operandStack;
    OSPushShort(s, val);
}

void ins_sconst_m1(Frame *f) {
    ins_sconst_number(f, -1);
}

void ins_sconst_0(Frame *f) {
    ins_sconst_number(f, 0);
}

void ins_sconst_1(Frame *f) {
    ins_sconst_number(f, 1);
}

void ins_sconst_2(Frame *f) {
    ins_sconst_number(f, 2);
}

void ins_sconst_3(Frame *f) {
    ins_sconst_number(f, 3);
}

void ins_sconst_4(Frame *f) {
    ins_sconst_number(f, 4);
}

void ins_sconst_5(Frame *f) {
    ins_sconst_number(f, 5);
}

static void ins_iconst_number(Frame *f, jint val) {
    OperandStack *s;
    s = &f->operandStack;
    OSPushInt(s, val);
}

void ins_iconst_m1(Frame *f) {
    ins_iconst_number(f, -1);
}

void ins_iconst_0(Frame *f) {
    ins_iconst_number(f, 0);
}

void ins_iconst_1(Frame *f) {
    ins_iconst_number(f, 1);
}

void ins_iconst_2(Frame *f) {
    ins_iconst_number(f, 2);
}

void ins_iconst_3(Frame *f) {
    ins_iconst_number(f, 3);
}

void ins_iconst_4(Frame *f) {
    ins_iconst_number(f, 4);
}

void ins_iconst_5(Frame *f) {
    ins_iconst_number(f, 5);
}

// ins 10

void ins_bspush(Frame *f) {
    jbyte val;
    val = FReadByte(f);
    OSPushShort(&f->operandStack, val);
}

void ins_sspush(Frame *f) {
    jshort val;
    val = FReadShort(f);
    OSPushShort(&f->operandStack, val);
}

void ins_bipush(Frame *f) {
    jbyte val;
    val = FReadByte(f);
    OSPushInt(&f->operandStack, val);
}

void ins_sipush(Frame *f) {
    jshort val;
    val = FReadShort(f);
    OSPushInt(&f->operandStack, val);
}

void ins_iipush(Frame *f) {
    jint val;
    val = FReadInt(f);
    OSPushInt(&f->operandStack, val);
}

void ins_aload(Frame *f) {
    // TODO
}

void ins_sload(Frame *f) {
    // TODO
}

void ins_iload(Frame *f) {
    // TODO
}

void ins_aload_0(Frame *f) {
    // TODO
}

void ins_aload_1(Frame *f) {
    // TODO
}

void ins_aload_2(Frame *f) {
    // TODO
}

void ins_aload_3(Frame *f) {
    // TODO
}

void ins_sload_0(Frame *f) {
    // TODO
}

void ins_sload_1(Frame *f) {
    // TODO
}

void ins_sload_2(Frame *f) {
    // TODO
}

void ins_sload_3(Frame *f) {
    // TODO
}

// ins 20

void ins_iload_0(Frame *f) {
    // TODO
}

void ins_iload_1(Frame *f) {
    // TODO
}

void ins_iload_2(Frame *f) {
    // TODO
}

void ins_iload_3(Frame *f) {
    // TODO
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

void ins_iaload(Frame *f) {
    // TODO
}

void ins_astore(Frame *f) {
    // TODO
}

void ins_sstore(Frame *f) {
    // TODO
}

void ins_istore(Frame *f) {
    // TODO
}

void ins_astore_0(Frame *f) {
    // TODO
}

void ins_astore_1(Frame *f) {
    // TODO
}

void ins_astore_2(Frame *f) {
    // TODO
}

void ins_astore_3(Frame *f) {
    // TODO
}

void ins_sstore_0(Frame *f) {
    // TODO
}

// ins 30

void ins_sstore_1(Frame *f) {
    // TODO
}

void ins_sstore_2(Frame *f) {
    // TODO
}

void ins_sstore_3(Frame *f) {
    // TODO
}

void ins_istore_0(Frame *f) {
    // TODO
}

void ins_istore_1(Frame *f) {
    // TODO
}

void ins_istore_2(Frame *f) {
    // TODO
}

void ins_istore_3(Frame *f) {
    // TODO
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

void ins_iastore(Frame *f) {
    // TODO
}

void ins_sadd(Frame *f) {
    OperandStack *s;
    jshort v1, v2, result;
    s = &f->operandStack;
    v2 = OSPopShort(s);
    v1 = OSPopShort(s);
    result = v1 + v2;
    OSPushShort(s, result);
}

void ins_iadd(Frame *f) {
    OperandStack *s;
    jint v1, v2, result;
    s = &f->operandStack;
    v2 = OSPopInt(s);
    v1 = OSPopInt(s);
    result = v1 + v2;
    OSPushInt(s, result);
}

void ins_ssub(Frame *f) {
    OperandStack *s;
    jshort v1, v2, result;
    s = &f->operandStack;
    v2 = OSPopShort(s);
    v1 = OSPopShort(s);
    result = v1 - v2;
    OSPushShort(s, result);
}

void ins_isub(Frame *f) {
    OperandStack *s;
    jint v1, v2, result;
    s = &f->operandStack;
    v2 = OSPopInt(s);
    v1 = OSPopInt(s);
    result = v1 - v2;
    OSPushInt(s, result);
}
