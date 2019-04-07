#ifndef JIECARDVM_INTERPRETER_H
#define JIECARDVM_INTERPRETER_H

#include "types.h"

u1 ByteCodeReadU1(void);

u2 ByteCodeReadU2(void);

u4 ByteCodeReadU4(void);

void ByteCodeBranch(int16_t offset);

#endif // JIECARDVM_INTERPRETER_H
