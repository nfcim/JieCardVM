#include "instructions.h"
#include "rtda.h"
#include "types.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <vm.h>

package_t current_package;

u1 bytecode_buffer[TOTAL_FRAMES][16];

typedef struct __attribute__((__packed__)) {
  u1 tag;
  u2 size;
  u1 count;
} applet_component_compat_header;

int vm_set_current_package(char *aid) {
  size_t len = strlen(aid);
  if (len + 1 >= sizeof(current_package.aid_hex)) {
    return VM_ERR_INVALID_ARG;
  }
  strcpy(current_package.aid_hex, aid);
  current_package.aid_hex_length = len;
  return VM_ERR_OK;
}

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

int vm_install_applet(u1 *target_aid, u2 length) {
  u1 count;
  int res = context_read_applet(&current_package, &count, 3, sizeof(count));
  if (res != 1)
    return VM_ERR_UNKNOWN;
  u2 offset = 4;
  for (u1 i = 0; i < count; i++) {
    u1 aid_length;
    res = context_read_applet(&current_package, &aid_length, offset,
                              sizeof(aid_length));
    if (res != 1)
      return VM_ERR_UNKNOWN;
    offset += 1;

    u1 aid[64];
    res = context_read_applet(&current_package, aid, offset, aid_length);
    if (res != aid_length)
      return VM_ERR_UNKNOWN;
    offset += aid_length;
    if (aid_length == length && memcmp(aid, target_aid, length) == 0) {
      // found
      DBG_MSG("Found applet at index %d\n", i);
      u2 install_method_offset;
      res = context_read_applet(&current_package, (u1 *)&install_method_offset,
                                offset, sizeof(install_method_offset));
      if (res != sizeof(install_method_offset))
        return VM_ERR_UNKNOWN;
      install_method_offset = htobe16(install_method_offset);
      // u1 tag, u2 size
      install_method_offset;
      DBG_MSG("Install method is at %d\n", install_method_offset);

      init_frame(install_method_offset);
      run();
      return VM_ERR_OK;
    } else {
      // skip
      offset += 2;
      continue;
    }
  }
  return VM_ERR_NO_ENT;
}