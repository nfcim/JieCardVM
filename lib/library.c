#include "common.h"
#include "rtda.h"
#include "utils.h"
#include "vm.h"
#include "globals.h"

void javacard_framework_applet_init() {
  // no args, only this
  u2 this = operand_stack_pop(&frames[current_frame].operand_stack);
  DBG_MSG("Applet.<init>(%d)\n", this);
}

void javacard_framework_applet_register() {
  // no args, only this
  u2 this = operand_stack_pop(&frames[current_frame].operand_stack);
  DBG_MSG("Applet.register(%d)\n", this);
  u2 class_index = context_get_object_class(&current_package, this);
  class_info info;
  context_read_class(&current_package, (u1 *)&info, class_index, sizeof(info));
  DBG_MSG("Class at %d, interface count %d, public base %d count %d, package "
          "base %d "
          "count %d\n",
          class_index, info.interface_count, info.public_method_table_base,
          info.public_method_table_count, info.package_method_table_base,
          info.package_method_table_count);
  for (u1 i = 0; i < info.public_method_table_count; i++) {
    u2 method_index;
    context_read_class(&current_package, (u1 *)&method_index,
                       class_index + sizeof(info) + 2 * i,
                       sizeof(method_index));
    method_index = htobe16(method_index);
    DBG_MSG("Method %d at %d\n", i, method_index);
  }
}