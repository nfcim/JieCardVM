#include "rtdata.h"


//<editor-fold desc="OperandStackOperations">

jbyte OSPopByte(OperandStack *s) {
    return (jbyte) s->data[--s->size];
}

void OSPushByte(OperandStack *s, jbyte val) {
    s->data[s->size++] = val;
}

jshort OSPopShort(OperandStack *s) {
    uint8_t v1, v2;
    v1 = s->data[--s->size];
    v2 = s->data[--s->size];
    return (v1 << 8) | v2;
}

void OSPushShort(OperandStack *s, jshort val) {
    uint8_t v1, v2;
    v1 = val >> 8;
    v2 = val & 0xFF;
    s->data[s->size++] = v2;
    s->data[s->size++] = v1;
}

jint OSPopInt(OperandStack *s) {
    uint8_t v1, v2, v3, v4;
    v1 = s->data[--s->size];
    v2 = s->data[--s->size];
    v3 = s->data[--s->size];
    v4 = s->data[--s->size];
    return (v1 << 24) | (v2 << 16) | (v3 << 8) | v4;
}

void OSPushInt(OperandStack *s, jint val) {
    uint8_t v1, v2, v3, v4;
    v1 = val >> 24;
    v2 = (val >> 16) & 0xFF;
    v3 = (val >> 8) & 0xFF;
    v4 = val & 0xFF;
    s->data[s->size++] = v4;
    s->data[s->size++] = v3;
    s->data[s->size++] = v2;
    s->data[s->size++] = v1;
}

//</editor-fold>
