#include "common.h"
#include "rtda.h"
#include "utils.h"

void javacard_framework_applet_init() {
  // no args, only this
  u2 this = operand_stack_pop(&frames[current_frame].operand_stack);
  DBG_MSG("Applet.<init>\n");
}

void javacard_framework_applet_register() {
  // no args, only this
  u2 this = operand_stack_pop(&frames[current_frame].operand_stack);
  DBG_MSG("Applet.register\n");
}