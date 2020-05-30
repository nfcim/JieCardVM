#ifndef JIECARDVM_RTDA_H
#define JIECARDVM_RTDA_H

#include "common.h"
#include "context.h"
#include "lfs.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

i2 operand_stack_pop(operand_stack_t *s);

void operand_stack_push(operand_stack_t *s, i2 val);

i2 operand_stack_get(operand_stack_t *s);

i2 variable_table_get(variable_table_t *t, u1 index);

void variable_table_set(variable_table_t *t, u1 index, i2 val);

u1 bytecode_read_u1(void);

u2 bytecode_read_u2(void);

/**
 * Jump relative offset in bytecode
 *
 * @param offset Offset of bytecode to jump
 */
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

// get index of bytecode, for testing only
u2 _bytecode_get_index(void);

u2 constant_pool_get(u2 index);

void constant_pool_set(u2 index, u2 val);

i2 object_data_get(i2 objRef, i2 index);

void object_data_set(i2 objRef, i2 index, i2 value);

/** 
 * Start run loop from root frame
*/
void run();

#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_RTDA_H
