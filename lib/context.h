#ifndef JIECARDVM_CONTEXT_H
#define JIECARDVM_CONTEXT_H

#include <lfs.h>
#include <rtda.h>
#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LFS_ATTR_METADATA 0

enum data_size {
  ELEM_SIZE_1 = 1,
  ELEM_SIZE_2 = 2,
};

enum context_error {
  CONTEXT_ERR_OK = 0,         // No error
  CONTEXT_ERR_EXIST = -1,     // Package already exists
  CONTEXT_ERR_NOENT = -2,     // Entry does not exist
  CONTEXT_ERR_UNKNOWN = -128, // Unknown error
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

#define CONSTANT_CLASS_REF 1
#define CONSTANT_INSTANCE_FIELD_REF 2
#define CONSTANT_VIRTUAL_METHOD_REF 3
#define CONSTANT_SUPER_METHOD_REF 4
#define CONSTANT_STATIC_FIELD_REF 5
#define CONSTANT_STATIC_METHOD_REF 6

typedef struct __attribute__((__packed__)) {
  u1 tag;
  union {
    class_ref klass;
    instance_field_ref instance_field;
    virtual_method_ref virtual_method;
    super_method_ref super_method;
    static_ref static_elem;
  };
} cp_info;

/**
 * Initialize the context
 *
 * @param lfs_cfg Config of little fs
 */
void context_init(const struct lfs_config *lfs_cfg);

/**
 * Create a new cap
 *
 * @param pkg Package Info
 * @return CONTEXT_ERR_EXIST if the AID exists;
 * CONTEXT_ERR_OK if the package is created.
 */
int context_create_cap(package_t *pkg);

/**
 * Delete a cap
 *
 * @param pkg Package info
 * @return CONTEXT_ERR_NOENT if the AID does not exist;
 * CONTEXT_ERR_OK if the package is created.
 */
int context_delete_cap(package_t *pkg);

/**
 * Append method info to a cap
 *
 * @param pkg Package info
 * @param data Method info
 * @param length Length of method info
 * @return method index on success
 */
int context_append_method(package_t *pkg, u1 *data, u2 length);

/**
 * Read method info from a cap
 *
 * @param pkg Package info
 * @param target Buffer to store method info
 * @param index Index of method
 * @param length Length to read
 * @return bytes read
 */
int context_read_method(package_t *pkg, u1 *target, u2 index, u2 length);

/**
 * Create an array
 *
 * @param pkg Package info
 * @param type Array type
 * @param class_ref If the array is of type reference, the class_ref should be
 * provided
 * @param length Length of the array
 * @return Arrayref
 */
int context_create_array(package_t *pkg, u1 type, u2 class_ref, u2 length);

int context_read_array(package_t *pkg, u2 ref, u1 type, u2 index, u1 *val);

int context_write_array(package_t *pkg, u2 ref, u1 type, u2 index, u2 val);

int context_array_meta(package_t *pkg, u2 ref, array_metadata_t *metadata);

int context_create_constant_pool(package_t *pkg, u1 *data, u2 length);

int context_read_constant_pool(package_t *pkg, u2 index, cp_info *info);

int context_create_static_image(package_t *pkg, u1 *data, u2 length);

int context_read_static_image(package_t *pkg, u2 offset, u1 size, u1 *val);

int context_write_static_image(package_t *pkg, u2 offset, u1 size, u2 val);

int context_resolve_static_method(package_t *pkg, u2 index,
                                  bytecode_t *bytecode);

int context_resolve_static_field(package_t *pkg, u2 index, u1 size, u1 *val);

#ifdef __cplusplus
}
#endif

#endif // JIECARDVM_CONTEXT_H
