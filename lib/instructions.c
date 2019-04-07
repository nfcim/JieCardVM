#include <string.h>

#include "rtdata.h"
#include "interpreter.h"

// ins 00

void ins_nop(Frame *f) {
}

void ins_aconst_null(Frame *f) {
    OSPushShort(&f->operandStack, 0);
}

void ins_sconst_m1(Frame *f) {
    OSPushShort(&f->operandStack, -1);
}

void ins_sconst_0(Frame *f) {
    OSPushShort(&f->operandStack, 0);
}

void ins_sconst_1(Frame *f) {
    OSPushShort(&f->operandStack, 1);
}

void ins_sconst_2(Frame *f) {
    OSPushShort(&f->operandStack, 2);
}

void ins_sconst_3(Frame *f) {
    OSPushShort(&f->operandStack, 3);
}

void ins_sconst_4(Frame *f) {
    OSPushShort(&f->operandStack, 4);
}

void ins_sconst_5(Frame *f) {
    OSPushShort(&f->operandStack, 5);
}

void ins_iconst_m1(Frame *f) {
    OSPushInt(&f->operandStack, -1);
}

void ins_iconst_0(Frame *f) {
    OSPushInt(&f->operandStack, 0);
}

void ins_iconst_1(Frame *f) {
    OSPushInt(&f->operandStack, 1);
}

void ins_iconst_2(Frame *f) {
    OSPushInt(&f->operandStack, 2);
}

void ins_iconst_3(Frame *f) {
    OSPushInt(&f->operandStack, 3);
}

void ins_iconst_4(Frame *f) {
    OSPushInt(&f->operandStack, 4);
}

void ins_iconst_5(Frame *f) {
    OSPushInt(&f->operandStack, 5);
}

// ins 10

void ins_bspush(Frame *f) {
    jbyte val = ByteCodeReadByte();
    OSPushShort(&f->operandStack, val);
}

void ins_sspush(Frame *f) {
    jshort val = ByteCodeReadShort();
    OSPushShort(&f->operandStack, val);
}

void ins_bipush(Frame *f) {
    jbyte val = ByteCodeReadByte();
    OSPushInt(&f->operandStack, val);
}

void ins_sipush(Frame *f) {
    jshort val = ByteCodeReadShort();
    OSPushInt(&f->operandStack, val);
}

void ins_iipush(Frame *f) {
    jint val = ByteCodeReadInt();
    OSPushInt(&f->operandStack, val);
}

void ins_aload(Frame *f) {
    uint8_t index = ByteCodeReadByte();
    jshort addr = VTGetShort(&f->variableTable, index);
    OSPushShort(&f->operandStack, addr);
}

void ins_sload(Frame *f) {
    uint8_t index = ByteCodeReadByte();
    jshort val = VTGetShort(&f->variableTable, index);
    OSPushShort(&f->operandStack, val);
}

void ins_iload(Frame *f) {
    uint8_t index = ByteCodeReadByte();
    jint val = VTGetInt(&f->variableTable, index);
    OSPushInt(&f->operandStack, val);
}

void ins_aload_0(Frame *f) {
    jshort addr = VTGetShort(&f->variableTable, 0);
    OSPushShort(&f->operandStack, addr);
}

void ins_aload_1(Frame *f) {
    jshort addr = VTGetShort(&f->variableTable, 1);
    OSPushShort(&f->operandStack, addr);
}

void ins_aload_2(Frame *f) {
    jshort addr = VTGetShort(&f->variableTable, 2);
    OSPushShort(&f->operandStack, addr);
}

void ins_aload_3(Frame *f) {
    jshort addr = VTGetShort(&f->variableTable, 3);
    OSPushShort(&f->operandStack, addr);
}

void ins_sload_0(Frame *f) {
    jshort val = VTGetShort(&f->variableTable, 0);
    OSPushShort(&f->operandStack, val);
}

void ins_sload_1(Frame *f) {
    jshort val = VTGetShort(&f->variableTable, 1);
    OSPushShort(&f->operandStack, val);
}

void ins_sload_2(Frame *f) {
    jshort val = VTGetShort(&f->variableTable, 2);
    OSPushShort(&f->operandStack, val);
}

void ins_sload_3(Frame *f) {
    jshort val = VTGetShort(&f->variableTable, 3);
    OSPushShort(&f->operandStack, val);
}

// ins 20

void ins_iload_0(Frame *f) {
    jint val = VTGetInt(&f->variableTable, 0);
    OSPushInt(&f->operandStack, val);
}

void ins_iload_1(Frame *f) {
    jint val = VTGetInt(&f->variableTable, 1);
    OSPushInt(&f->operandStack, val);
}

void ins_iload_2(Frame *f) {
    jint val = VTGetInt(&f->variableTable, 2);
    OSPushInt(&f->operandStack, val);
}

void ins_iload_3(Frame *f) {
    jint val = VTGetInt(&f->variableTable, 3);
    OSPushInt(&f->operandStack, val);
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
    uint8_t index = ByteCodeReadByte();
    jshort addr = OSPopShort(&f->operandStack);
    VTSetShort(&f->variableTable, index, addr);
}

void ins_sstore(Frame *f) {
    uint8_t index = ByteCodeReadByte();
    jshort val = OSPopShort(&f->operandStack);
    VTSetShort(&f->variableTable, index, val);
}

void ins_istore(Frame *f) {
    uint8_t index = ByteCodeReadByte();
    jint val = OSPopInt(&f->operandStack);
    VTSetInt(&f->variableTable, index, val);
}

void ins_astore_0(Frame *f) {
    jshort addr = OSPopShort(&f->operandStack);
    VTSetShort(&f->variableTable, 0, addr);
}

void ins_astore_1(Frame *f) {
    jshort addr = OSPopShort(&f->operandStack);
    VTSetShort(&f->variableTable, 1, addr);
}

void ins_astore_2(Frame *f) {
    jshort addr = OSPopShort(&f->operandStack);
    VTSetShort(&f->variableTable, 2, addr);
}

void ins_astore_3(Frame *f) {
    jshort addr = OSPopShort(&f->operandStack);
    VTSetShort(&f->variableTable, 3, addr);
}

void ins_sstore_0(Frame *f) {
    jshort val = OSPopShort(&f->operandStack);
    VTSetShort(&f->variableTable, 0, val);
}

// ins 30

void ins_sstore_1(Frame *f) {
    jshort val = OSPopShort(&f->operandStack);
    VTSetShort(&f->variableTable, 1, val);
}

void ins_sstore_2(Frame *f) {
    jshort val = OSPopShort(&f->operandStack);
    VTSetShort(&f->variableTable, 2, val);
}

void ins_sstore_3(Frame *f) {
    jshort val = OSPopShort(&f->operandStack);
    VTSetShort(&f->variableTable, 3, val);
}

void ins_istore_0(Frame *f) {
    jint val = OSPopInt(&f->operandStack);
    VTSetInt(&f->variableTable, 0, val);
}

void ins_istore_1(Frame *f) {
    jint val = OSPopInt(&f->operandStack);
    VTSetInt(&f->variableTable, 1, val);
}

void ins_istore_2(Frame *f) {
    jint val = OSPopInt(&f->operandStack);
    VTSetInt(&f->variableTable, 2, val);
}

void ins_istore_3(Frame *f) {
    jint val = OSPopInt(&f->operandStack);
    VTSetInt(&f->variableTable, 3, val);
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

void ins_pop(Frame *f) {
    OSPopShort(&f->operandStack);
}

void ins_pop2(Frame *f) {
    OSPopInt(&f->operandStack);
}

void ins_dup(Frame *f) {
    jshort val = OSGetShort(&f->operandStack);
    OSPushShort(&f->operandStack, val);
}

void ins_dup2(Frame *f) {
    jint val = OSGetInt(&f->operandStack);
    OSPushInt(&f->operandStack, val);
}

void ins_dup_x(Frame *f) {
    uint8_t mn = ByteCodeReadByte();
    uint8_t m = (mn >> 4) * 2;
    uint8_t n = (mn & 0xF) * 2;
    memmove(f->operandStack.next - n + m, f->operandStack.next - n, n);
    memcpy(f->operandStack.next - n, f->operandStack.next, m);
    f->operandStack.next += m;
}

// ins 40

void ins_swap_x(Frame *f) {
    uint8_t buf[2];
    uint8_t mn = ByteCodeReadByte();
    uint8_t m = (mn >> 4) * 2;
    uint8_t n = (mn & 0xF) * 2;
    memcpy(buf, f->operandStack.next - m, m);
    memmove(f->operandStack.next - n, f->operandStack.next - n - m, n);
    memcpy(f->operandStack.next - n - m, buf, m);
}

void ins_sadd(Frame *f) {
    jshort v2 = OSPopShort(&f->operandStack);
    jshort v1 = OSPopShort(&f->operandStack);
    jshort result = v1 + v2;
    OSPushShort(&f->operandStack, result);
}

void ins_iadd(Frame *f) {
    jint v2 = OSPopInt(&f->operandStack);
    jint v1 = OSPopInt(&f->operandStack);
    jint result = v1 + v2;
    OSPushInt(&f->operandStack, result);
}

void ins_ssub(Frame *f) {
    jshort v2 = OSPopShort(&f->operandStack);
    jshort v1 = OSPopShort(&f->operandStack);
    jshort result = v1 - v2;
    OSPushShort(&f->operandStack, result);
}

void ins_isub(Frame *f) {
    jint v2 = OSPopInt(&f->operandStack);
    jint v1 = OSPopInt(&f->operandStack);
    jint result = v1 - v2;
    OSPushInt(&f->operandStack, result);
}

void ins_smul(Frame *f) {
    jshort v2 = OSPopShort(&f->operandStack);
    jshort v1 = OSPopShort(&f->operandStack);
    jshort result = v1 * v2;
    OSPushShort(&f->operandStack, result);
}

void ins_imul(Frame *f) {
    jint v2 = OSPopInt(&f->operandStack);
    jint v1 = OSPopInt(&f->operandStack);
    jint result = v1 * v2;
    OSPushInt(&f->operandStack, result);
}

void ins_sdiv(Frame *f) {
    jshort v2 = OSPopShort(&f->operandStack);
    jshort v1 = OSPopShort(&f->operandStack);
    jshort result = v1 / v2;
    OSPushShort(&f->operandStack, result);
}

void ins_idiv(Frame *f) {
    jint v2 = OSPopInt(&f->operandStack);
    jint v1 = OSPopInt(&f->operandStack);
    jint result = v1 / v2;
    OSPushInt(&f->operandStack, result);
}

void ins_srem(Frame *f) {
    jshort v2 = OSPopShort(&f->operandStack);
    jshort v1 = OSPopShort(&f->operandStack);
    jshort result = v1 % v2;
    OSPushShort(&f->operandStack, result);
}

void ins_irem(Frame *f) {
    jint v2 = OSPopInt(&f->operandStack);
    jint v1 = OSPopInt(&f->operandStack);
    jint result = v1 % v2;
    OSPushInt(&f->operandStack, result);
}

void ins_sneg(Frame *f) {
    jshort v = OSPopShort(&f->operandStack);
    OSPushShort(&f->operandStack, -v);
}

void ins_ineg(Frame *f) {
    jint v = OSPopInt(&f->operandStack);
    OSPushInt(&f->operandStack, -v);
}

void ins_sshl(Frame *f) {
    jshort v2 = OSPopShort(&f->operandStack);
    jshort v1 = OSPopShort(&f->operandStack);
    v2 &= 0x1F;
    OSPushShort(&f->operandStack, v1 << v2);
}

void ins_ishl(Frame *f) {
    jint v2 = OSPopInt(&f->operandStack);
    jint v1 = OSPopInt(&f->operandStack);
    v2 &= 0x1F;
    OSPushInt(&f->operandStack, v1 << v2);
}

void ins_sshr(Frame *f) {
    jshort v2 = OSPopShort(&f->operandStack);
    jshort v1 = OSPopShort(&f->operandStack);
    v2 &= 0x1F;
    OSPushShort(&f->operandStack, v1 >> v2);
}

// ins 50

void ins_ishr(Frame *f) {
    jint v2 = OSPopInt(&f->operandStack);
    jint v1 = OSPopInt(&f->operandStack);
    v2 &= 0x1F;
    OSPushInt(&f->operandStack, v1 >> v2);
}

void ins_sushr(Frame *f) {
    jshort v2 = OSPopShort(&f->operandStack);
    jshort v1 = OSPopShort(&f->operandStack);
    v2 &= 0x1F;
    OSPushShort(&f->operandStack, (uint16_t) v1 >> v2);
}

void ins_iushr(Frame *f) {
    jint v2 = OSPopInt(&f->operandStack);
    jint v1 = OSPopInt(&f->operandStack);
    v2 &= 0x1F;
    OSPushInt(&f->operandStack, (uint32_t) v1 >> v2);
}

void ins_sand(Frame *f) {
    jshort v2 = OSPopShort(&f->operandStack);
    jshort v1 = OSPopShort(&f->operandStack);
    OSPushShort(&f->operandStack, v1 & v2);
}

void ins_iand(Frame *f) {
    jint v2 = OSPopInt(&f->operandStack);
    jint v1 = OSPopInt(&f->operandStack);
    OSPushInt(&f->operandStack, v1 & v2);
}

void ins_sor(Frame *f) {
    jshort v2 = OSPopShort(&f->operandStack);
    jshort v1 = OSPopShort(&f->operandStack);
    OSPushShort(&f->operandStack, v1 | v2);
}

void ins_ior(Frame *f) {
    jint v2 = OSPopInt(&f->operandStack);
    jint v1 = OSPopInt(&f->operandStack);
    OSPushInt(&f->operandStack, v1 | v2);
}

void ins_sxor(Frame *f) {
    jshort v2 = OSPopShort(&f->operandStack);
    jshort v1 = OSPopShort(&f->operandStack);
    OSPushShort(&f->operandStack, v1 ^ v2);
}

void ins_ixor(Frame *f) {
    jint v2 = OSPopInt(&f->operandStack);
    jint v1 = OSPopInt(&f->operandStack);
    OSPushInt(&f->operandStack, v1 ^ v2);
}

void ins_sinc(Frame *f) {
    uint8_t index = ByteCodeReadByte();
    jbyte val = ByteCodeReadByte();
    jshort var = VTGetShort(&f->variableTable, index);
    VTSetShort(&f->variableTable, index, var + val);
}

void ins_iinc(Frame *f) {
    uint8_t index = ByteCodeReadByte();
    jbyte val = ByteCodeReadByte();
    jint var = VTGetInt(&f->variableTable, index);
    VTSetInt(&f->variableTable, index, var + val);
}

void ins_s2b(Frame *f) {
    jshort v = OSPopShort(&f->operandStack);
    OSPushShort(&f->operandStack, (jbyte) v);
}

void ins_s2i(Frame *f) {
    jshort v = OSPopShort(&f->operandStack);
    OSPushInt(&f->operandStack, (jint) v);
}

void ins_i2b(Frame *f) {
    jint v = OSPopInt(&f->operandStack);
    OSPushShort(&f->operandStack, (jbyte) v);
}

void ins_i2s(Frame *f) {
    jint v = OSPopInt(&f->operandStack);
    OSPushShort(&f->operandStack, (jshort) v);
}

void ins_icmp(Frame *f) {
    jint v2 = OSPopInt(&f->operandStack);
    jint v1 = OSPopInt(&f->operandStack);
    OSPushShort(&f->operandStack, v1 < v2 ? -1 : (v1 == v2 ? 0 : 1));
}
