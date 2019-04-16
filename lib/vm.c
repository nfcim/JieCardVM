#include "instructions.h"
#include "rtda.h"
#include "types.h"
#include <stdbool.h>
#include <vm.h>
#include <stdio.h>

Frame frames[100], *currentFrame;
u2 frameData[1024];

bool step(void) {
  u1 opcode = BCReadU1();
  if (opcode == 0) return false;
  opcodes[opcode](currentFrame);
  return true;
}

int vm_init(void) {
  currentFrame = frames;
  return 0;
}
