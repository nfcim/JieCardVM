#ifndef JIECARDVM_CONTEXT_H
#define JIECARDVM_CONTEXT_H

#include <stdio.h>

#include "lfs.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

enum context_error {
  CONTEXT_ERR_OK = 0,         // No error
  CONTEXT_ERR_EXIST = -1,     // Package already exists
  CONTEXT_ERR_NOENT = -2,     // Package does not exist
  CONTEXT_ERR_UNKNOWN = -128, // Unknown error
};

typedef struct {
  char aid_hex[64]; // will be reused as path
  u1 aid_hex_length;
} package_t;

void context_init(const struct lfs_config *cfg);
int context_create_cap(package_t *pkg);
int context_delete_cap(package_t *pkg);
int context_append_method(package_t *pkg, u1 *data, u2 length);
ssize_t context_read_method(package_t *pkg, u1 *target, u2 offset, u2 length);
int context_create_array(package_t *pkg);

#ifdef __cplusplus
}
#endif

#endif // JIECARDVM_CONTEXT_H
