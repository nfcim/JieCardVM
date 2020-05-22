#include "instructions.h"
#include "rtda.h"
#include "types.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <vm.h>

package_t current_package;

typedef struct __attribute__((__packed__)) {
  u1 tag;
  u2 size;
  u1 count;
} applet_component_compat_header;

bool step(void) {
  u1 opcode = bytecode_read_u1();
  if (opcode == 0)
    return false;
  return true;
}

int vm_set_current_package(char *aid) {
  size_t len = strlen(aid);
  if (aid + 1 >= sizeof(current_package.aid_hex)) {
    return VM_ERR_INVALID_ARG;
  }
  strcpy(current_package.aid_hex, aid);
  current_package.aid_hex_length = len;
  return VM_ERR_OK;
}

int vm_execute_static_method(int index) { return 0; }

int vm_init(void) { return 0; }

int vm_load_method(u1 *data, u4 length) {
  int res = context_write_methods(&current_package, data, length);
  if (res < 0)
    return VM_ERR_UNKNOWN;
  return VM_ERR_OK;
}

int vm_load_constant_pool(u1 *data, u4 length) {
  int res = context_write_constants(&current_package, data, length);
  if (res < 0)
    return VM_ERR_UNKNOWN;
  DBG_MSG("Loaded %d constants\n", length / 4 - 1);
  return VM_ERR_OK;
}

int vm_load_applet(u1 *data, u4 length) {
  int res = context_write_applets(&current_package, data, length);
  if (res < 0)
    return VM_ERR_UNKNOWN;
  applet_component_compat_header *header =
      (applet_component_compat_header *)data;
  DBG_MSG("Loaded %d applets\n", header->count);
  return VM_ERR_OK;
}