#ifndef JIECARDVM_INTERPRETER_H
#define JIECARDVM_INTERPRETER_H

#include "types.h"

u1 ByteCodeReadU1(void);

u2 ByteCodeReadU2(void);

u4 ByteCodeReadU4(void);

void ByteCodeOffset(int16_t offset);

u1 *ByteCodeGetPC(void);

void ByteCodeSetPC(u1 *new_pc);

u4 ContextGetData(u2 index);

void ContextSetData(u2 index, u4 val);

#endif // JIECARDVM_INTERPRETER_H
