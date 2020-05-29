#ifndef JIECARDVM_COMMON_H
#define JIECARDVM_COMMON_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BYTECODE_WINDOW_SIZE 16
#define TOTAL_FRAMES 100

typedef int8_t i1;
typedef int16_t i2;

typedef uint8_t u1;
typedef uint16_t u2;

typedef uint32_t u4;

enum ARRAY_TYPE {
  ARRAY_T_BOOLEAN = 10,
  ARRAY_T_BYTE = 11,
  ARRAY_T_SHORT = 12,
  ARRAY_T_REFERENCE = 14,
};

typedef struct {
  char aid_hex[64]; // will be reused as path
  u1 aid_hex_length;
} package_t;

typedef struct {
  u2 array_cnt;  // amount of arrays in the package
  u2 object_cnt; // amount of objects in the package
} package_metadata_t;

typedef struct {
  u1 type;      // type of array
  u2 class_ref; // valid if type is T_REFERENCE
  u2 length;    // length of array
} array_metadata_t;

typedef struct __attribute__((__packed__)) {
  union {
    u2 internal_ref;
    struct __attribute__((__packed__)) {
      u1 package_token;
      u1 class_token;
    } external_ref;
  };
} class_ref;

typedef struct __attribute__((__packed__)) {
  class_ref klass;
  u1 token;
} instance_field_ref;

typedef instance_field_ref virtual_method_ref;

typedef instance_field_ref super_method_ref;

typedef struct __attribute__((__packed__)) {
  union {
    struct __attribute__((__packed__)) {
      u1 padding;
      u2 offset;
    } internal_ref;
    struct __attribute__((__packed__)) {
      u1 package_token;
      u1 class_token;
      u1 token;
    } external_ref;
  };
} static_ref;

typedef struct __attribute__((__packed__)) {
  u1 interface_count : 4;
  u1 flags : 4;
  class_ref super_class_ref;
  u1 declared_instance_size;
  u1 first_reference_token;
  u1 reference_count;
  u1 public_method_table_base;
  u1 public_method_table_count;
  u1 package_method_table_base;
  u1 package_method_table_count;
} class_info;

#define CONSTANT_CLASS_REF 1
#define CONSTANT_INSTANCE_FIELD_REF 2
#define CONSTANT_VIRTUAL_METHOD_REF 3
#define CONSTANT_SUPER_METHOD_REF 4
#define CONSTANT_STATIC_FIELD_REF 5
#define CONSTANT_STATIC_METHOD_REF 6

#define COMPONENT_ConstantPool 5
#define COMPONENT_Method 7

typedef struct __attribute__((__packed__)) {
  u1 tag;
  union {
    class_ref klass;
    instance_field_ref instance_field;
    virtual_method_ref virtual_method;
    super_method_ref super_method;
    // both static field and static method
    static_ref static_elem;
  };
} cp_info;

typedef struct __attribute__((__packed__)) {
  u1 max_stack : 4;
  u1 flags : 4;
  u1 max_locals : 4;
  u1 nargs : 4;
} method_header_info;

typedef struct __attribute__((__packed__)) {
  u1 minor_version;
  u1 major_version;
  u1 aid_length;
  u1 aid[0];
} package_info;

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
  u2 method_offset;        // method offset in Method.cap
  u2 bytecode_offset;      // bytecode offset in Method.cap
  method_header_info info; // method info
} frame_t;

typedef struct {
  u1 is_external;
  union {
    // read bytecode from Method.cap
    // a sliding window of Method.cap
    struct {
      u1 buffer[BYTECODE_WINDOW_SIZE];
      u2 method_offset; // offset of bytecode in Method.cap
    } method;
    // read bytecode from external buffer
    struct {
      u1 *buffer;
    } external;
  };
  u2 index;
} bytecode_t;

#ifdef __cplusplus
};
#endif

#endif