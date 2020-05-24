#ifndef JIECARDVM_RTDA_H
#define JIECARDVM_RTDA_H

#include "lfs.h"
#include "types.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BYTECODE_WINDOW_SIZE 16
#define TOTAL_FRAMES 100

typedef struct {
  u2 *base; // base memory
  u1 index;
  u1 max_stack;
} operand_stack_t;

typedef struct {
  i2 *base; // base memory
  u1 max_locals;
} variable_table_t;

typedef struct {
  operand_stack_t operand_stack;
  variable_table_t variable_table;
  u2 method_offset;   // method offset in Method.cap
  u2 bytecode_offset; // bytecode offset in Method.cap
} frame_t;

typedef struct {
  u1 is_external;
  union {
    // read bytecode from Method.cap
    // a sliding window of Method.cap
    struct {
      u1 buffer[BYTECODE_WINDOW_SIZE];
      u2 method_offset;   // offset of bytecode in Method.cap
    } method;
    // read bytecode from external buffer
    struct {
      u1 *buffer;
    } external;
  };
  u2 index;
} bytecode_t;

// index of current frame
extern int current_frame;
// whether vm is running
extern int running;

i2 operand_stack_pop(operand_stack_t *s);

void operand_stack_push(operand_stack_t *s, i2 val);

i2 operand_stack_get(operand_stack_t *s);

i2 variable_table_get(variable_table_t *t, u1 index);

void variable_table_set(variable_table_t *t, u1 index, i2 val);

u1 bytecode_read_u1(void);

u2 bytecode_read_u2(void);

void bytecode_jump_offset(int16_t offset);

/**
 * Use bytecode from Method.cap
 *
 * @param method_offset Offset of bytecode in Method.cap
 */
void bytecode_set_method(u2 method_offset);

/**
 * Use bytecode from buffer
 *
 * @param bytecode Buffer of bytecode
 */
void bytecode_set_buffer(u1 *bytecode);

/**
 * Init the first frame
 *
 * @param method_offset Method offset of root frame in Method.cap
 */
int init_frame(u2 method_offset);

/**
 * Init a new frame on top
 *
 * @param method_offset Method offset of new frame in Method.cap
 */
int push_frame(u2 method_offset);

/**
 * Pop current frame
 *
 * @return Pointer to new frame
 */
frame_t *pop_frame();

u2 _bytecode_get_index(void);

u2 constant_pool_get(u2 index);

void constant_pool_set(u2 index, u2 val);

i2 object_data_get(i2 objRef, i2 index);

void object_data_set(i2 objRef, i2 index, i2 value);

void run();

#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_RTDA_H
