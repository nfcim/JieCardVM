#include "instructions.h"
#include "rtda.h"
#include "types.h"
#include <stdbool.h>
#include <vm.h>
#include <stdio.h>

frame_t frames[100], *currentFrame;
u2 frameData[1024];
package_t current_package;

bool step(void) {
  u1 opcode = bytecode_read_u1();
  if (opcode == 0) return false;
  opcodes[opcode](currentFrame);
  return true;
}

int vm_init(void) {
  currentFrame = frames;
  return 0;
}
