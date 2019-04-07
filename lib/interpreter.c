#include <stdint.h>
#include "interpreter.h"


static uint8_t *pc;

jbyte ByteCodeReadByte(void) {
    return (jbyte) *pc++;
}

jshort ByteCodeReadShort(void) {
    uint8_t v1 = *pc++;
    uint8_t v2 = *pc++;
    return (jshort) ((v1 << 8) | v2);
}

jint ByteCodeReadInt(void) {
    uint8_t v1 = *pc++;
    uint8_t v2 = *pc++;
    uint8_t v3 = *pc++;
    uint8_t v4 = *pc++;
    return (jint) ((v1 << 24) | (v2 << 16) | (v3 << 8) | v4);
}
