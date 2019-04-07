#ifndef JIECARDVM_INTERPRETER_H
#define JIECARDVM_INTERPRETER_H

#include "types.h"

jbyte ByteCodeReadByte(void);

jshort ByteCodeReadShort(void);

jint ByteCodeReadInt(void);

void ByteCodeBranch(int16_t offset);

#endif //JIECARDVM_INTERPRETER_H
