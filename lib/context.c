#include "context.h"
#include "lfs.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

static char path[64];

static lfs_t lfs;

void context_init(const struct lfs_config *cfg) {
  int err = lfs_mount(&lfs, cfg);
  if (err) { // the first boot
    lfs_format(&lfs, cfg);
    lfs_mount(&lfs, cfg);
  }
}

bool context_create_cap(u1 *aid, u1 aidLength) {
  if (aidLength > 16)
    return false;
  bytes_to_hexstring(path, aid, aidLength);
  int err = lfs_mkdir(&lfs, path);
  return err == 0;
}

bool context_delete_cap(u1 *aid, u1 aidLength) {
  if (aidLength > 16)
    return false;
  bytes_to_hexstring(path, aid, aidLength);

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
    strcpy(path + aidLength * 2, "/");
    strcpy(path + aidLength * 2 + 1, info.name);
    err = lfs_remove(&lfs, path);
    if (err < 0)
      return false;
  }
  lfs_dir_close(&lfs, &dir);
  path[aidLength * 2] = 0;
  err = lfs_remove(&lfs, path);
  return err == 0;
}

bool context_append_method(u1 *aid, u1 aidLength, u1 *bytecodes,
                           u2 bytecodesLength) {
  if (aidLength > 16)
    return false;
  bytes_to_hexstring(path, aid, aidLength);
  strcpy(path + aidLength * 2, "/m");
  lfs_file_t f;
  int err =
      lfs_file_open(&lfs, &f, path, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND);
  if (err != 0)
    return false;
  err = lfs_file_write(&lfs, &f, bytecodes, bytecodesLength);
  if (err < 0)
    return false;
  err = lfs_file_close(&lfs, &f);
  return err == 0;
}

ssize_t context_read_method(u1 *aid, u1 aidLength, u1 *target, u2 offset,
                            u2 length) {
  if (aidLength > 16)
    return false;
  bytes_to_hexstring(path, aid, aidLength);
  strcpy(path + aidLength * 2, "/m");
  lfs_file_t f;
  int err = lfs_file_open(&lfs, &f, path, LFS_O_RDONLY);
  if (err != 0)
    return err;
  ssize_t read = lfs_file_read(&lfs, &f, target, length);
  err = lfs_file_close(&lfs, &f);
  if (err != 0)
    return err;
  return read;
}
