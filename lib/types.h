#ifndef JIECARDVM_TYPES_H
#define JIECARDVM_TYPES_H

#include <stdint.h>

#define JNULL 0xFFFF

#ifdef __cplusplus
extern "C" {
#endif

typedef int8_t jbyte;
typedef int16_t jshort;

typedef uint8_t u1;
typedef uint16_t u2;

enum ARRAY_TYPE {
  ARRAY_T_BOOLEAN = 10,
  ARRAY_T_BYTE = 11,
  ARRAY_T_SHORT = 12,
  ARRAY_T_REFERENCE = 14,
};

#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_TYPES_H
