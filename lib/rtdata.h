#ifndef JIECARDVM_RTDATA_H
#define JIECARDVM_RTDATA_H

#include <stdint.h>

#include "types.h"


typedef struct {
    uint8_t *base;  // base memory
    uint8_t *next;  // next slot
} OperandStack;

typedef struct {
    jshort *base;   // base memory
} VariableTable;

typedef struct {
    OperandStack operandStack;
    VariableTable variableTable;
} Frame;


jbyte FReadByte(Frame *f);
jshort FReadShort(Frame *f);
jint FReadInt(Frame *f);


//<editor-fold desc="OperandStackOperations">

jbyte OSPopByte(OperandStack *s);
void OSPushByte(OperandStack *s, jbyte val);
jbyte OSGetByte(OperandStack *s);
jshort OSPopShort(OperandStack *s);
void OSPushShort(OperandStack *s, jshort val);
jshort OSGetShort(OperandStack *s);
jint OSPopInt(OperandStack *s);
void OSPushInt(OperandStack *s, jint val);
jint OSGetInt(OperandStack *s);

//</editor-fold>

#endif // JIECARDVM_RTDATA_H
