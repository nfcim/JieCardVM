#include "context.h"
#include "lfs.h"
#include "utils.h"

static char aidBuffer[33];

static lfs_t lfs;

void CTXInit(const struct lfs_config *cfg) {
  int err = lfs_mount(&lfs, cfg);
  if (err) { // the first boot
    lfs_format(&lfs, cfg);
    lfs_mount(&lfs, cfg);
  }
}

bool CTXCreateCAP(u1 *aid, u1 aidLength) {
  if (aidLength > 16)
    return false;
  btox(aidBuffer, aid, aidLength);
  int err = lfs_mkdir(&lfs, aidBuffer);
  return err == 0;
}
