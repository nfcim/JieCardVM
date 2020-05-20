#include "instructions.h"
#include "rtda.h"
#include "types.h"
#include <stdbool.h>
#include <stdio.h>
#include <vm.h>

package_t current_package;

bool step(void) {
  u1 opcode = bytecode_read_u1();
  if (opcode == 0)
    return false;
  return true;
}

int vm_execute_static_method(package_t *package, char *class_name, char *method_name) {
  return 0;
}

int vm_init(void) { return 0; }
