#ifndef JIECARDVM_RTDATA_H
#define JIECARDVM_RTDATA_H

#include <stdint.h>

#include "types.h"


#define OPERAND_STACK_SIZE 128


typedef struct {
    size_t size;
    uint8_t data[OPERAND_STACK_SIZE];
} OperandStack;

typedef struct {
    OperandStack operandStack;
} Frame;


//<editor-fold desc="OperandStackOperations">

jbyte OSPopByte(OperandStack *s);
void OSPushByte(OperandStack *s, jbyte val);
jshort OSPopShort(OperandStack *s);
void OSPushShort(OperandStack *s, jshort val);
jint OSPopInt(OperandStack *s);
void OSPushInt(OperandStack *s, jint val);

//</editor-fold>

#endif // JIECARDVM_RTDATA_H
