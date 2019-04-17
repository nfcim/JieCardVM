#include "context.h"
#include "globals.h"
#include "lfs.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

static package_metadata_t package_metadata;
static u1 array_buffer[ARRAY_BUFFER_SIZE];

static int read_metadata(package_t *pkg) {
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/m");
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_RDONLY);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  err = lfs_file_read(&g_lfs, &f, &package_metadata, sizeof(package_metadata));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  return CONTEXT_ERR_OK;
}

static int write_metadata(package_t *pkg) {
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/m");
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex,
                          LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  err = lfs_file_write(&g_lfs, &f, &package_metadata, sizeof(package_metadata));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  return CONTEXT_ERR_OK;
}

void context_init(const struct lfs_config *lfs_cfg) {
  int err = lfs_mount(&g_lfs, lfs_cfg);
  if (err) { // the first boot
    lfs_format(&g_lfs, lfs_cfg);
    lfs_mount(&g_lfs, lfs_cfg);
  }
}

int context_create_cap(package_t *pkg) {
  int err = lfs_mkdir(&g_lfs, pkg->aid_hex);
  if (err == LFS_ERR_EXIST)
    return CONTEXT_ERR_EXIST;
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  package_metadata.array_cnt = 0;
  package_metadata.object_cnt = 0;
  err = write_metadata(pkg);
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
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/b");
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex,
                          LFS_O_WRONLY | LFS_O_CREAT | LFS_O_APPEND);
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

int context_read_method(package_t *pkg, u1 *target, u2 offset, u2 length) {
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/b");
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_RDONLY);
  if (err != 0)
    return CONTEXT_ERR_UNKNOWN;
  lfs_file_seek(&g_lfs, &f, offset, LFS_SEEK_SET);
  int read = lfs_file_read(&g_lfs, &f, target, length);
  err = lfs_file_close(&g_lfs, &f);
  if (err != 0)
    return CONTEXT_ERR_UNKNOWN;
  return read;
}

int context_create_array(package_t *pkg, u2 length) {
  int err = read_metadata(pkg);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  sprintf(pkg->aid_hex + pkg->aid_hex_length, "/a%d",
          ++package_metadata.array_cnt);
  lfs_file_t f;
  err = lfs_file_open(&g_lfs, &f, pkg->aid_hex,
                      LFS_O_WRONLY | LFS_O_CREAT | LFS_O_EXCL);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  memset(array_buffer, 0, sizeof(array_buffer));
  for (u2 i = 0; i < length; i += ARRAY_BUFFER_SIZE) {
    u2 size = length - i;
    if (size > ARRAY_BUFFER_SIZE)
      size = ARRAY_BUFFER_SIZE;
    err = lfs_file_write(&g_lfs, &f, array_buffer, size);
    if (err < 0)
      return CONTEXT_ERR_UNKNOWN;
  }
  err = lfs_file_close(&g_lfs, &f);
  if (err != 0)
    return CONTEXT_ERR_UNKNOWN;
  err = write_metadata(pkg);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  return package_metadata.array_cnt;
}
