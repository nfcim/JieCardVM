#include "instructions.h"
#include "rtda.h"
#include "types.h"
#include <stdbool.h>
#include <vm.h>
#include <stdio.h>

package_t current_package;

bool step(void) {
  u1 opcode = bytecode_read_u1();
  if (opcode == 0) return false;
  return true;
}

int vm_init(void) {
  return 0;
}
