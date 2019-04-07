#include "rtdata.h"


//<editor-fold desc="OperandStackOperations">

jshort OSPopShort(OperandStack *s) {
    return *(--s->next);
}

void OSPushShort(OperandStack *s, jshort val) {
    *s->next++ = val;
}

jshort OSGetShort(OperandStack *s) {
    return *(s->next - 1);
}

jint OSPopInt(OperandStack *s) {
    uint16_t v1, v2;
    v1 = *(--s->next);
    v2 = *(--s->next);
    return (v1 << 16) | v2;
}

void OSPushInt(OperandStack *s, jint val) {
    uint8_t v1, v2;
    v1 = (val >> 16) & 0xFFFF;
    v2 = val & 0xFFFF;
    *s->next++ = v2;
    *s->next++ = v1;
}

jint OSGetInt(OperandStack *s) {
    uint8_t v1, v2;
    v1 = *(s->next - 1);
    v2 = *(s->next - 2);
    return (v1 << 16) | v2;
}

//</editor-fold>

//<editor-fold desc="VariableTableOperations">

jshort VTGetShort(VariableTable *t, uint8_t index) {
    return t->base[index];
}

jint VTGetInt(VariableTable *t, uint8_t index) {
    return (t->base[index] << 16) | t->base[index + 1];
}

void VTSetShort(VariableTable *t, uint8_t index, jshort val) {
    t->base[index] = val;
}

void VTSetInt(VariableTable *t, uint8_t index, jint val) {
    t->base[index] = val >> 16;
    t->base[index + 1] = val & 0xFFFF;
}

//</editor-fold>
