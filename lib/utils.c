#include "utils.h"

void btox(char *xp, const uint8_t *bb, int n) {
  const char xx[] = "0123456789ABCDEF";
  int nn = n * 2;
  while (--nn >= 0)
    xp[nn] = xx[(bb[nn >> 1] >> ((1 - (nn & 1)) << 2)) & 0xF];
  xp[n * 2] = 0;
}
