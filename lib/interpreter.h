#ifndef JIECARDVM_INTERPRETER_H
#define JIECARDVM_INTERPRETER_H

#include "types.h"

u1 ByteCodeReadU1(void);

u2 ByteCodeReadU2(void);

u4 ByteCodeReadU4(void);

void ByteCodeOffset(int16_t offset);

u1 *ByteCodeGetPC(void);

void ByteCodeSetPC(u1 *new_pc);

#endif // JIECARDVM_INTERPRETER_H
