#ifndef JIECARDVM_RTDA_H
#define JIECARDVM_RTDA_H

#include "lfs.h"
#include "types.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_BYTECODE_INDEX 512
#define TOTAL_FRAMES 100

typedef struct {
  u2 *base; // base memory
  u1 index;
  u1 max_stack;
} operand_stack_t;

typedef struct {
  jshort *base; // base memory
  u1 max_locals;
} variable_table_t;

typedef struct {
  operand_stack_t operand_stack;
  variable_table_t variable_table;
  u2 method_offset;
} frame_t;

typedef struct {
  u1 *base;
  u2 index;
} bytecode_t;

jshort operand_stack_pop(operand_stack_t *s);

void operand_stack_push(operand_stack_t *s, jshort val);

jshort operand_stack_get(operand_stack_t *s);

jshort variable_table_get(variable_table_t *t, u1 index);

void variable_table_set(variable_table_t *t, u1 index, jshort val);

u1 bytecode_read_u1(void);

u2 bytecode_read_u2(void);

void bytecode_jump_offset(int16_t offset);

void bytecode_set(u1 *base);

u2 _bytecode_get_index(void);

u2 constant_pool_get(u2 index);

void constant_pool_set(u2 index, u2 val);

jshort object_data_get(jshort objRef, jshort index);

void object_data_set(jshort objRef, jshort index, jshort value);

#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_RTDA_H
