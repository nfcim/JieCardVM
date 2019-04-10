#include "utils.h"

void btox(char *xp, const uint8_t *bb, int n) {
  const char xx[] = "0123456789ABCDEF";
  int nn = n << 1;
  while (--nn >= 0)
    xp[n] = xx[(bb[n >> 1] >> ((1 - (n & 1)) << 2)) & 0xF];
  xp[(n << 1) + 1] = 0;
}
