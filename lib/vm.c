#include "globals.h"
#include "instructions.h"
#include "library.h"
#include "rtda.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <vm.h>

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

int vm_load_class(u1 *data, u4 length) {
  int res = context_write_classes(&current_package, data, length);
  if (res < 0)
    return VM_ERR_UNKNOWN;
  return VM_ERR_OK;
}

int vm_load_import(u1 *data, u4 length) {
  int res = context_write_imports(&current_package, data, length);
  if (res < 0)
    return VM_ERR_UNKNOWN;
  return VM_ERR_OK;
}

int vm_install_applet(u1 *target_aid, u2 aid_length, u1 *params, u2 length,
                      u2 offset_arg, u1 length_arg) {
  u1 count;
  int res = context_read_applet(&current_package, &count, 0, sizeof(count));
  if (res != 1)
    return VM_ERR_UNKNOWN;
  u2 offset = 1;
  for (u1 i = 0; i < count; i++) {
    u1 cur_aid_length;
    res = context_read_applet(&current_package, &cur_aid_length, offset,
                              sizeof(cur_aid_length));
    if (res != 1)
      return VM_ERR_UNKNOWN;
    offset += 1;

    u1 aid[64];
    res = context_read_applet(&current_package, aid, offset, cur_aid_length);
    if (res != cur_aid_length)
      return VM_ERR_UNKNOWN;
    offset += cur_aid_length;
    if (cur_aid_length == aid_length &&
        memcmp(aid, target_aid, aid_length) == 0) {
      // found
      DBG_MSG("Found applet at index %d\n", i);
      u2 install_method_offset;
      res = context_read_applet(&current_package, (u1 *)&install_method_offset,
                                offset, sizeof(install_method_offset));
      if (res != sizeof(install_method_offset))
        return VM_ERR_UNKNOWN;
      install_method_offset = htobe16(install_method_offset);
      // u1 tag, u2 size
      DBG_MSG("Install method is at %d\n", install_method_offset);

      init_frame(install_method_offset);

      // initialize params
      if (params != NULL) {
        // first arg is params byte[]
        u2 ref =
            context_create_array(&current_package, ARRAY_T_BYTE, 0, length);
        // TODO: optimize
        for (u2 i = 0; i < length; i++) {
          context_write_array(&current_package, ref, ARRAY_T_BYTE, i,
                              params[i]);
        }
        variable_table_set(&frames[current_frame].variable_table, 0, ref);
      } else {
        // null
        variable_table_set(&frames[current_frame].variable_table, 0, 0);
      }
      // offset
      variable_table_set(&frames[current_frame].variable_table, 1, offset_arg);
      // length
      variable_table_set(&frames[current_frame].variable_table, 2, length_arg);

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

int vm_process_apdu(u2 ref, u1 *cmd_apdu, u2 cmd_len, u1 *resp_apdu,
                    u2 resp_len) {

  u2 class_index = context_get_object_class(&current_package, ref);
  class_info info;
  context_read_class(&current_package, (u1 *)&info, class_index, sizeof(info));
  DBG_MSG("Class at %d, interface count %d, public base %d count %d, package "
          "base %d "
          "count %d\n",
          class_index, info.interface_count, info.public_method_table_base,
          info.public_method_table_count, info.package_method_table_base,
          info.package_method_table_count);

  // find method overriding javacard/framework/Applet."process"
  if (info.public_method_table_base <=
          JAVACARD_FRAMEWORK_APPLET_PROCESS_TOKEN &&
      info.public_method_table_base + info.public_method_table_count >
          JAVACARD_FRAMEWORK_APPLET_PROCESS_TOKEN) {
    u2 i =
        JAVACARD_FRAMEWORK_APPLET_PROCESS_TOKEN - info.public_method_table_base;
    u2 method_index;
    context_read_class(&current_package, (u1 *)&method_index,
                       class_index + sizeof(info) + 2 * i,
                       sizeof(method_index));
    method_index = htobe16(method_index);
    DBG_MSG("Found process() method %d\n", method_index);
  }
  return VM_ERR_OK;
}