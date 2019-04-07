#ifndef JIECARDVM_RTDATA_H
#define JIECARDVM_RTDATA_H

#include <stdint.h>

#include "types.h"

typedef struct {
  u2 *base; // base memory
  u2 *next; // next slot
} OperandStack;

typedef struct {
  jshort *base; // base memory
} VariableTable;

typedef struct {
  OperandStack operandStack;
  VariableTable variableTable;
} Frame;

//<editor-fold desc="OperandStackOperations">

jshort OSPopShort(OperandStack *s);

void OSPushShort(OperandStack *s, jshort val);

jshort OSGetShort(OperandStack *s);

jint OSPopInt(OperandStack *s);

void OSPushInt(OperandStack *s, jint val);

jint OSGetInt(OperandStack *s);

//</editor-fold>

//<editor-fold desc="VariableTableOperations">

jshort VTGetShort(VariableTable *t, u1 index);

jint VTGetInt(VariableTable *t, u1 index);

void VTSetShort(VariableTable *t, u1 index, jshort val);

void VTSetInt(VariableTable *t, u1 index, jint val);

//</editor-fold>

#endif // JIECARDVM_RTDATA_H
