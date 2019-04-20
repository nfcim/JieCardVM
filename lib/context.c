#include <context.h>
#include <globals.h>
#include <lfs.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>

static package_metadata_t package_metadata;
static array_metadata_t array_metadata;
static u1 array_buffer[ARRAY_BUFFER_SIZE];

static int read_package_metadata(package_t *pkg) {
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  int err = lfs_getattr(&g_lfs, pkg->aid_hex, LFS_ATTR_METADATA,
                        &package_metadata, sizeof(package_metadata));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return CONTEXT_ERR_OK;
}

static int write_package_metadata(package_t *pkg) {
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  int err = lfs_setattr(&g_lfs, pkg->aid_hex, LFS_ATTR_METADATA,
                        &package_metadata, sizeof(package_metadata));
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
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  int err = lfs_mkdir(&g_lfs, pkg->aid_hex);
  if (err == LFS_ERR_EXIST)
    return CONTEXT_ERR_EXIST;
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  package_metadata.array_cnt = 0;
  package_metadata.object_cnt = 0;
  err = write_package_metadata(pkg);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return CONTEXT_ERR_OK;
}

int context_delete_cap(package_t *pkg) {
  pkg->aid_hex[pkg->aid_hex_length] = 0;
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

  err = lfs_dir_close(&g_lfs, &dir);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

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
                          LFS_O_WRONLY | LFS_O_CREAT | LFS_O_APPEND);
  if (err < 0)
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
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/m");
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_RDONLY);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_seek(&g_lfs, &f, offset, LFS_SEEK_SET);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  int read = lfs_file_read(&g_lfs, &f, target, length);
  if (read < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return read;
}

int context_create_array(package_t *pkg, u1 type, u2 class_ref, u2 length) {
  int err = read_package_metadata(pkg);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  sprintf(pkg->aid_hex + pkg->aid_hex_length, "/a%u",
          ++package_metadata.array_cnt);
  lfs_file_t f;
  err = lfs_file_open(&g_lfs, &f, pkg->aid_hex,
                      LFS_O_WRONLY | LFS_O_CREAT | LFS_O_EXCL);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  array_metadata.type = type;
  array_metadata.class_ref = class_ref;
  array_metadata.length = length;
  err = lfs_setattr(&g_lfs, pkg->aid_hex, LFS_ATTR_METADATA, &array_metadata,
                    sizeof(array_metadata));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  if (type == ARRAY_T_SHORT || type == ARRAY_T_REFERENCE)
    length *= 2;
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
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = write_package_metadata(pkg);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return package_metadata.array_cnt;
}

// TODO: add cache
int context_read_array(package_t *pkg, u2 ref, u1 type, u2 index, u1 *val) {
  sprintf(pkg->aid_hex + pkg->aid_hex_length, "/a%u", ref);
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_RDONLY);
  // TODO: what if the file does not exist
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_getattr(&g_lfs, pkg->aid_hex, LFS_ATTR_METADATA, &array_metadata,
                    sizeof(array_metadata));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  // TODO: new return value
  if ((type == ARRAY_T_BYTE && array_metadata.type != ARRAY_T_BYTE &&
       array_metadata.type != ARRAY_T_BOOLEAN) ||
      type != array_metadata.type)
    return CONTEXT_ERR_UNKNOWN;
  if (index >= array_metadata.length)
    return CONTEXT_ERR_NOENT;

  u2 offset = index;
  if (type == ARRAY_T_SHORT || type == ARRAY_T_REFERENCE)
    offset *= 2;
  err =
      lfs_file_seek(&g_lfs, &f, offset & ARRAY_BUFFER_SIZE_UMASK, LFS_SEEK_CUR);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_read(&g_lfs, &f, array_buffer, ARRAY_BUFFER_SIZE);
  offset &= ARRAY_BUFFER_SIZE_MASK;
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  *val = array_buffer[offset];
  if (type == ARRAY_T_SHORT || type == ARRAY_T_REFERENCE)
    *(val + 1) = array_buffer[offset + 1];

  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return CONTEXT_ERR_OK;
}

// TODO: add cache
int context_write_array(package_t *pkg, u2 ref, u1 type, u2 index, u2 val) {
  sprintf(pkg->aid_hex + pkg->aid_hex_length, "/a%u", ref);
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_RDWR);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_getattr(&g_lfs, pkg->aid_hex, LFS_ATTR_METADATA, &array_metadata,
                    sizeof(array_metadata));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  if ((type == ARRAY_T_BYTE && array_metadata.type != ARRAY_T_BYTE &&
       array_metadata.type != ARRAY_T_BOOLEAN) ||
      type != array_metadata.type)
    return CONTEXT_ERR_UNKNOWN;
  // TODO: check ref type
  u2 offset = index;
  if (type == ARRAY_T_SHORT || type == ARRAY_T_REFERENCE)
    offset *= 2;
  err = lfs_file_seek(&g_lfs, &f, offset, LFS_SEEK_CUR);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  if (type == ARRAY_T_SHORT || type == ARRAY_T_REFERENCE) {
    err = lfs_file_write(&g_lfs, &f, &val, 2);
  } else {
    u1 data = (u1)val;
    err = lfs_file_write(&g_lfs, &f, &data, 1);
  }
  if (err <= 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return CONTEXT_ERR_OK;
}

int context_array_meta(package_t *pkg, u2 ref, array_metadata_t *metadata) {
  sprintf(pkg->aid_hex + pkg->aid_hex_length, "/a%u", ref);
  int err = lfs_getattr(&g_lfs, pkg->aid_hex, LFS_ATTR_METADATA, metadata,
                        sizeof(array_metadata));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return CONTEXT_ERR_OK;
}
