#include "context.h"
#include "globals.h"
#include "lfs.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

void context_init(const struct lfs_config *cfg) {
  int err = lfs_mount(&g_lfs, cfg);
  if (err) { // the first boot
    lfs_format(&g_lfs, cfg);
    lfs_mount(&g_lfs, cfg);
  }
}

int context_create_cap(package_t *pkg) {
  int err = lfs_mkdir(&g_lfs, pkg->aid_hex);
  if (err == LFS_ERR_EXIST)
    return CONTEXT_ERR_EXIST;
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  return CONTEXT_ERR_OK;
}

int context_delete_cap(package_t *pkg) {
  lfs_dir_t dir;
  int err = lfs_dir_open(&g_lfs, &dir, pkg->aid_hex);
  if (err == LFS_ERR_NOENT)
    return CONTEXT_ERR_NOENT;
  struct lfs_info info;
  while (true) {
    err = lfs_dir_read(&g_lfs, &dir, &info);
    if (err < 0)
      return CONTEXT_ERR_UNKNOWN;
    if (err == 0)
      break;
    if (info.name[0] == '.')
      continue;
    strcpy(pkg->aid_hex + pkg->aid_hex_length, "/");
    strcpy(pkg->aid_hex + pkg->aid_hex_length + 1, info.name);
    err = lfs_remove(&g_lfs, pkg->aid_hex);
    if (err < 0)
      return CONTEXT_ERR_UNKNOWN;
  }
  lfs_dir_close(&g_lfs, &dir);
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  err = lfs_remove(&g_lfs, pkg->aid_hex);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  return CONTEXT_ERR_OK;
}

int context_append_method(package_t *pkg, u1 *data, u2 length) {
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/m");
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex,
                          LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND);
  if (err != 0)
    return CONTEXT_ERR_UNKNOWN;
  err = lfs_file_write(&g_lfs, &f, data, length);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  return CONTEXT_ERR_OK;
}

ssize_t context_read_method(package_t *pkg, u1 *target, u2 offset, u2 length) {
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/m");
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_RDONLY);
  if (err != 0)
    return CONTEXT_ERR_UNKNOWN;
  ssize_t read = lfs_file_read(&g_lfs, &f, target, length);
  err = lfs_file_close(&g_lfs, &f);
  if (err != 0)
    return CONTEXT_ERR_UNKNOWN;
  return read;
}
