#include "context.h"
#include "lfs.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

static char path[64];

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
  btox(path, aid, aidLength);
  int err = lfs_mkdir(&lfs, path);
  return err == 0;
}

bool CTXDeleteCAP(u1 *aid, u1 aidLength) {
  if (aidLength > 16)
    return false;
  btox(path, aid, aidLength);

  lfs_dir_t dir;
  int err = lfs_dir_open(&lfs, &dir, path);
  if (err)
    return false;
  struct lfs_info info;
  while (true) {
    err = lfs_dir_read(&lfs, &dir, &info);
    if (err < 0)
      return false;
    if (err == 0)
      break;
    if (info.name[0] == '.')
      continue;
    sprintf(path + aidLength * 2, "/%s", info.name);
    err = lfs_remove(&lfs, path);
    if (err < 0)
      return false;
  }
  lfs_dir_close(&lfs, &dir);
  path[aidLength * 2] = 0;
  err = lfs_remove(&lfs, path);
  return err == 0;
}
