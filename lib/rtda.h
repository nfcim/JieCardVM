#ifndef JIECARDVM_RTDA_H
#define JIECARDVM_RTDA_H

#include <stdint.h>
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

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

jshort OSPop(OperandStack *s);

void OSPush(OperandStack *s, jshort val);

jshort OSGet(OperandStack *s);

jshort VTGet(VariableTable *t, u1 index);

void VTSet(VariableTable *t, u1 index, jshort val);

u1 BCReadU1(void);

u2 BCReadU2(void);

u1 *PCGet(void);

void PCSet(u1 *new_pc);

void PCSetOffset(int16_t offset);

u2 CPGetData(u2 index);

void CPSetData(u2 index, u2 val);

jshort ODGet(jshort objRef, jshort index);

void ODSet(jshort objRef, jshort index, jshort value);

#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_RTDA_H
