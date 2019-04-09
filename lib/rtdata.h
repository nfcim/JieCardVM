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

jshort OSPop(OperandStack *s);

void OSPush(OperandStack *s, jshort val);

jshort OSGet(OperandStack *s);

//</editor-fold>

//<editor-fold desc="VariableTableOperations">

jshort VTGet(VariableTable *t, u1 index);

void VTSet(VariableTable *t, u1 index, jshort val);

//</editor-fold>

#endif // JIECARDVM_RTDATA_H
