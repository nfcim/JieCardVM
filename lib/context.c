#include <context.h>
#include <globals.h>
#include <lfs.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>

#define ARRAY_BUFFER_SIZE 16
static package_metadata_t package_metadata;
static array_metadata_t array_metadata;

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

int context_write_general(package_t *pkg, u1 *data, u2 length,
                          const char *postfix) {
  // open file
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  strcpy(pkg->aid_hex + pkg->aid_hex_length, postfix);
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_WRONLY | LFS_O_CREAT);
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

int context_read_general(package_t *pkg, u1 *target, u2 offset, u2 length,
                         const char *postfix) {
  // open file
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  strcpy(pkg->aid_hex + pkg->aid_hex_length, postfix);
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_RDONLY);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  // skip u1 tag, u2 size
  offset += 3;
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

int context_write_methods(package_t *pkg, u1 *data, u2 length) {
  return context_write_general(pkg, data, length, "/m");
}

int context_read_method(package_t *pkg, u1 *target, u2 offset, u2 length) {
  return context_read_general(pkg, target, offset, length, "/m");
}

int context_create_array(package_t *pkg, u1 type, u2 class_ref, u2 length) {
  // get allocated array count
  u1 array_buffer[ARRAY_BUFFER_SIZE];
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

  // save array metadata
  array_metadata.type = type;
  array_metadata.class_ref = class_ref;
  array_metadata.length = length;
  err = lfs_setattr(&g_lfs, pkg->aid_hex, LFS_ATTR_METADATA, &array_metadata,
                    sizeof(array_metadata));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  // fill array content with zeros
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

int context_read_array(package_t *pkg, u2 ref, u1 type, u2 index, u1 *val) {
  sprintf(pkg->aid_hex + pkg->aid_hex_length, "/a%u", ref);
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_RDONLY);
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
  if (index >= array_metadata.length)
    return CONTEXT_ERR_NOENT;

  u2 offset = index;
  if (type == ARRAY_T_SHORT || type == ARRAY_T_REFERENCE)
    offset *= 2;
  err = lfs_file_seek(&g_lfs, &f, offset, LFS_SEEK_SET);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  if (type == ARRAY_T_SHORT || type == ARRAY_T_REFERENCE)
    err = lfs_file_read(&g_lfs, &f, val, ELEM_SIZE_2);
  else
    err = lfs_file_read(&g_lfs, &f, val, ELEM_SIZE_1);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return CONTEXT_ERR_OK;
}

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

int context_write_constants(package_t *pkg, u1 *data, u2 length) {
  return context_write_general(pkg, data, length, "/c");
}

int context_read_constant(package_t *pkg, u2 index, u1 *info, u2 length) {
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/c");
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_RDONLY);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  u4 file_size = lfs_file_size(&g_lfs, &f);
  u4 header_length = 5;
  if (index * sizeof(cp_info) + header_length >= file_size)
    return CONTEXT_ERR_NOENT;

  // seek to index
  err = lfs_file_seek(&g_lfs, &f, index * sizeof(cp_info) + header_length,
                      LFS_SEEK_CUR);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  int read = lfs_file_read(&g_lfs, &f, info, length);
  if (read < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return read;
}

int context_count_constant(package_t *pkg) {
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/c");
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_RDONLY);
  if (err == LFS_ERR_NOENT)
    return 0;
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  int file_size = lfs_file_size(&g_lfs, &f);

  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  // skip 4-byte header
  return file_size / sizeof(cp_info) - 1;
}


int context_write_applets(package_t *pkg, u1 *data, u2 length) {
  return context_write_general(pkg, data, length, "/a");
}

int context_read_applet(package_t *pkg, u1 *target, u2 offset, u2 length) {
  return context_read_general(pkg, target, offset, length, "/a");
}

int context_write_classes(package_t *pkg, u1 *data, u2 length) {
  return context_write_general(pkg, data, length, "/C");
}

int context_read_class(package_t *pkg, u1 *target, u2 offset, u2 length) {
  return context_read_general(pkg, target, offset, length, "/C");
}

int context_create_object(package_t *pkg, u2 class_index) {
  // get allocated object count
  int err = read_package_metadata(pkg);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  pkg->aid_hex[pkg->aid_hex_length] = 0;
  sprintf(pkg->aid_hex + pkg->aid_hex_length, "/c%u",
          ++package_metadata.object_cnt);
  lfs_file_t f;
  err = lfs_file_open(&g_lfs, &f, pkg->aid_hex,
                      LFS_O_WRONLY | LFS_O_CREAT | LFS_O_EXCL);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  // metadata: class index
  err = lfs_setattr(&g_lfs, pkg->aid_hex, LFS_ATTR_METADATA, &class_index,
                    sizeof(class_index));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = write_package_metadata(pkg);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return package_metadata.object_cnt;
}

int context_write_imports(package_t *pkg, u1 *data, u2 length) {
  return context_write_general(pkg, data, length, "/i");
}

int context_read_import(package_t *pkg, u1 *target, u2 offset, u2 length) {
  offset += 1; // u1 count
  return context_read_general(pkg, target, offset, length, "/i");
}

int context_get_object_class(package_t *pkg, u2 ref) {
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  sprintf(pkg->aid_hex + pkg->aid_hex_length, "/c%u",
          ref);
  u2 class_index;
  // metadata: class index
  int err = lfs_getattr(&g_lfs, pkg->aid_hex, LFS_ATTR_METADATA, &class_index,
                    sizeof(class_index));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  return class_index;
}