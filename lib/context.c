#include "context.h"
#include "utils.h"
#include "../littlefs/lfs.h"

static char aidBuffer[33];

static lfs_t lfs;
static struct lfs_config lfsConfig = {};

void CTXInit(void) {
  int err = lfs_mount(&lfs, &lfsConfig);
  if (err) {  // the first boot
    lfs_format(&lfs, &lfsConfig);
    lfs_mount(&lfs, &lfsConfig);
  }
}

bool CTXCreateCAP(u1 *aid, u1 aidLength) {
  if (aidLength > 16)
    return false;
  btox(aidBuffer, aid, aidLength);
  int err = lfs_mkdir(&lfs, aidBuffer);
  return err == 0;
}
