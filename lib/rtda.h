#ifndef JIECARDVM_RTDA_H
#define JIECARDVM_RTDA_H

#include <stdint.h>
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_BC_INDEX 512

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

typedef struct {
  u1 *base;
  u2 index;
} ByteCode;

jshort OSPop(OperandStack *s);

void OSPush(OperandStack *s, jshort val);

jshort OSGet(OperandStack *s);

jshort VTGet(VariableTable *t, u1 index);

void VTSet(VariableTable *t, u1 index, jshort val);

u1 BCReadU1(void);

u2 BCReadU2(void);

void BCJump(int16_t offset);

void BCSet(u1 *base);

u2 CPGetData(u2 index);

void CPSetData(u2 index, u2 val);

jshort ODGet(jshort objRef, jshort index);

void ODSet(jshort objRef, jshort index, jshort value);

#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_RTDA_H
