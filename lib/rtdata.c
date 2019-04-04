#include "rtdata.h"


//<editor-fold desc="OperandStackOperations">

jbyte OSPopByte(OperandStack *s) {
    return *(--s->next);
}

void OSPushByte(OperandStack *s, jbyte val) {
    *s->next++ = val;
}

jbyte OSGetByte(OperandStack *s) {
    return *(s->next - 1);
}

jshort OSPopShort(OperandStack *s) {
    uint8_t v1, v2;
    v1 = *(--s->next);
    v2 = *(--s->next);
    return (v1 << 8) | v2;
}

void OSPushShort(OperandStack *s, jshort val) {
    uint8_t v1, v2;
    v1 = val >> 8;
    v2 = val & 0xFF;
    *s->next++ = v2;
    *s->next++ = v1;
}

jshort OSGetShort(OperandStack *s) {
    uint8_t v1, v2;
    v1 = *(s->next - 1);
    v2 = *(s->next - 2);
    return (v1 << 8) | v2;
}

jint OSPopInt(OperandStack *s) {
    uint8_t v1, v2, v3, v4;
    v1 = *(--s->next);
    v2 = *(--s->next);
    v3 = *(--s->next);
    v4 = *(--s->next);
    return (v1 << 24) | (v2 << 16) | (v3 << 8) | v4;
}

void OSPushInt(OperandStack *s, jint val) {
    uint8_t v1, v2, v3, v4;
    v1 = val >> 24;
    v2 = (val >> 16) & 0xFF;
    v3 = (val >> 8) & 0xFF;
    v4 = val & 0xFF;
    *s->next++ = v4;
    *s->next++ = v3;
    *s->next++ = v2;
    *s->next++ = v1;
}

jint OSGetInt(OperandStack *s) {
    uint8_t v1, v2, v3, v4;
    v1 = *(s->next - 1);
    v2 = *(s->next - 2);
    v3 = *(s->next - 3);
    v4 = *(s->next - 4);
    return (v1 << 24) | (v2 << 16) | (v3 << 8) | v4;
}

//</editor-fold>
