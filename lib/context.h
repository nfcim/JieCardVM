#ifndef JIECARDVM_CONTEXT_H
#define JIECARDVM_CONTEXT_H

#include "lfs.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ARRAY_BUFFER_SIZE 256
#define ARRAY_BUFFER_SIZE_UMASK 0xFFFFFF00u
#define ARRAY_BUFFER_SIZE_MASK 0xFFu

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
 * @param pkg Package Info
 * @return CONTEXT_ERR_NOENT if the AID does not exist;
 * CONTEXT_ERR_OK if the package is created.
 */
int context_delete_cap(package_t *pkg);

int context_append_method(package_t *pkg, u1 *data, u2 length);

int context_read_method(package_t *pkg, u1 *target, u2 offset, u2 length);

int context_create_array(package_t *pkg, u1 type, u2 class_ref, u2 length);

int context_read_array(package_t *pkg, u2 ref, u1 type, u2 index, u1 *val);

int context_write_array(package_t *pkg, u2 ref, u1 type, u2 index, u2 val);

#ifdef __cplusplus
}
#endif

#endif // JIECARDVM_CONTEXT_H
