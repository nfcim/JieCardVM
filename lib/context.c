#include "context.h"
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

int context_append_method(package_t *pkg, u1 *data, u2 length) {
  // open method file
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/m");
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex,
                          LFS_O_WRONLY | LFS_O_CREAT | LFS_O_APPEND);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  // open method lookup file
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/M");
  lfs_file_t lookup_f;
  err = lfs_file_open(&g_lfs, &lookup_f, pkg->aid_hex,
                      LFS_O_WRONLY | LFS_O_CREAT | LFS_O_APPEND);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  // update lookup file
  u4 offset = lfs_file_size(&g_lfs, &f);
  u4 lookup_file_size = lfs_file_size(&g_lfs, &lookup_f);
  err = lfs_file_write(&g_lfs, &lookup_f, &offset, sizeof(offset));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_write(&g_lfs, &f, data, length);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_close(&g_lfs, &lookup_f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return lookup_file_size / sizeof(u4);
}

int context_read_method(package_t *pkg, u1 *target, u2 index, u2 length) {
  // open lookup file
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/M");
  lfs_file_t lookup_f;
  int err = lfs_file_open(&g_lfs, &lookup_f, pkg->aid_hex, LFS_O_RDONLY);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  // seek to index
  lfs_file_seek(&g_lfs, &lookup_f, index * sizeof(u4), LFS_SEEK_SET);

  // read offset
  u4 offset = 0;
  err = lfs_file_read(&g_lfs, &lookup_f, &offset, sizeof(offset));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  // open method file
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/m");
  lfs_file_t f;
  err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_RDONLY);
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

  err = lfs_file_close(&g_lfs, &lookup_f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return read;
}

int context_create_array(package_t *pkg, u1 type, u2 class_ref, u2 length) {
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

int context_append_constant(package_t *pkg, u1 *data, u2 length) {
  // open constant file
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/c");
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex,
                          LFS_O_WRONLY | LFS_O_CREAT | LFS_O_APPEND);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  // open constant lookup file
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/C");
  lfs_file_t lookup_f;
  err = lfs_file_open(&g_lfs, &lookup_f, pkg->aid_hex,
                      LFS_O_WRONLY | LFS_O_CREAT | LFS_O_APPEND);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  // update lookup file
  u4 offset = lfs_file_size(&g_lfs, &f);
  u4 lookup_file_size = lfs_file_size(&g_lfs, &lookup_f);
  err = lfs_file_write(&g_lfs, &lookup_f, &offset, sizeof(offset));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_write(&g_lfs, &f, data, length);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_close(&g_lfs, &lookup_f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return lookup_file_size / sizeof(u4);
}

int context_read_constant(package_t *pkg, u2 index, u1 *info, u2 length) {
  // open lookup file
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/C");
  lfs_file_t lookup_f;
  int err = lfs_file_open(&g_lfs, &lookup_f, pkg->aid_hex, LFS_O_RDONLY);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  // seek to index
  lfs_file_seek(&g_lfs, &lookup_f, index * sizeof(u4), LFS_SEEK_SET);

  // read offset
  u4 offset = 0;
  err = lfs_file_read(&g_lfs, &lookup_f, &offset, sizeof(offset));
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  // open constant file
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/c");
  lfs_file_t f;
  err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_RDONLY);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_seek(&g_lfs, &f, offset, LFS_SEEK_CUR);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  int read = lfs_file_read(&g_lfs, &f, info, length);
  if (read < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_close(&g_lfs, &lookup_f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return read;
}

int context_count_constant(package_t *pkg) {
  // open lookup file
  pkg->aid_hex[pkg->aid_hex_length] = 0;
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/C");
  lfs_file_t lookup_f;
  int err = lfs_file_open(&g_lfs, &lookup_f, pkg->aid_hex, LFS_O_RDONLY);
  if (err == LFS_ERR_NOENT)
    return 0;
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;
  int file_size = lfs_file_size(&g_lfs, &lookup_f);

  err = lfs_file_close(&g_lfs, &lookup_f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return file_size / sizeof(u4);
}

int context_create_static_image(package_t *pkg, u1 *data, u2 length) {
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/s");
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

int context_read_static_image(package_t *pkg, u2 offset, u1 size, u1 *val) {
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/s");
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_RDONLY);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  if (offset + size > lfs_file_size(&g_lfs, &f))
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_read(&g_lfs, &f, val, size);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return CONTEXT_ERR_OK;
}

int context_write_static_image(package_t *pkg, u2 offset, u1 size, u2 val) {
  strcpy(pkg->aid_hex + pkg->aid_hex_length, "/s");
  lfs_file_t f;
  int err = lfs_file_open(&g_lfs, &f, pkg->aid_hex, LFS_O_WRONLY);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  if (offset + size > lfs_file_size(&g_lfs, &f))
    return CONTEXT_ERR_UNKNOWN;

  if (size == 1) {
    u1 data = (u1)val;
    err = lfs_file_write(&g_lfs, &f, &data, 1);
  } else if (size == 2)
    err = lfs_file_write(&g_lfs, &f, &val, 2);
  else
    return CONTEXT_ERR_UNKNOWN;
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  err = lfs_file_close(&g_lfs, &f);
  if (err < 0)
    return CONTEXT_ERR_UNKNOWN;

  return CONTEXT_ERR_OK;
}

int context_resolve_static_method(package_t *pkg, u2 index,
                                  bytecode_t *bytecode) {
  cp_info info;
  // FIXME
  int err = context_read_constant(pkg, index, (u1 *)&info, sizeof(cp_info));
  if (err < 0 || info.tag != CONSTANT_STATIC_METHOD_REF)
    return CONTEXT_ERR_UNKNOWN;
  if (info.static_elem.external_ref.package_token > 0x7F) {
    // TODO: external package
    return CONTEXT_ERR_OK;
  } else {
    return context_read_method(pkg, bytecode->base,
                               info.static_elem.internal_ref.offset,
                               MAX_BYTECODE_INDEX);
  }
}

int context_resolve_static_field(package_t *pkg, u2 index, u1 size, u1 *val) {
  cp_info info;
  // FIXME
  int err = context_read_constant(pkg, index, (u1 *)&info, sizeof(cp_info));
  if (err < 0 || info.tag != CONSTANT_STATIC_FIELD_REF)
    return CONTEXT_ERR_UNKNOWN;
  if (info.static_elem.external_ref.package_token > 0x7F) {
    return CONTEXT_ERR_OK;
  } else {
    return context_read_static_image(pkg, info.static_elem.internal_ref.offset,
                                     size, val);
  }
}

// load .class file into current context
int context_load_class(package_t *package, u1 *data, u4 length) {
  u1 *p = data;
  u4 magic = ntohl(*(u4 *)p);
  if (magic != 0xCAFEBABE)
    return CONTEXT_ERR_UNKNOWN;
  p += 4;
  // minor, major
  p += 4;

  // constants
  u2 constant_offset = context_count_constant(package);
  u2 constant_pool_count = ntohs(*(u2 *)p);
  p += 2;
  // constant_pool[constant_pool_count-1]
  for (u2 i = 0; i < constant_pool_count - 1; i++) {
    u1 tag = *p;
    // see how long it is and relocate offsets
    u2 size = 0;
    u2 inner_length = 0;
    u2 *index = NULL;
    switch (tag) {
    case CONSTANT_UTF8:
      // UTF-8
      inner_length = htons(*(u2 *)(p + 1));
      size = 1 + 2 + inner_length;
      break;
    case CONSTANT_INTEGER:
    case CONSTANT_FLOAT:
      // Integer and float
      size = 1 + 4;
      break;
    case CONSTANT_LONG:
    case CONSTANT_DOUBLE:
      // Long and double
      size = 1 + 8;
      break;
    case CONSTANT_CLASS:
    case CONSTANT_STRING:
      // class/string
      // one byte tag and two bytes index
      index = (u2 *)(p + 1);
      // relocate
      *index = htons(ntohs(*index) + constant_offset);
      size = 1 + 2;
      break;
    case CONSTANT_FIELD_REF:
    case CONSTANT_METHOD_REF:
    case CONSTANT_INTERFACE_METHOD_REF:
    case CONSTANT_NAME_AND_TYPE:
      // field ref, method ref, interface method ref or name and type
      // one byte tag, two bytes class index, two bytes name and type index
      // relocate
      index = (u2 *)(p + 1);
      *index = htons(ntohs(*index) + constant_offset);
      // relocate
      index = (u2 *)(p + 3);
      *index = htons(ntohs(*index) + constant_offset);
      size = 1 + 2 + 2;
      break;

    default:
      return CONTEXT_ERR_UNKNOWN;
      break;
    }

    context_append_constant(package, p, size);
    // advance
    p += size;
  }

  u2 access_flags = ntohs(*(u2 *)p);
  p += 2;
  u2 this_class = ntohs(*(u2 *)p);
  p += 2;
  u2 super_class = ntohs(*(u2 *)p);
  p += 2;
  u2 interface_count = ntohs(*(u2 *)p);
  p += 2;
  // TODO: interfaces
  u2 fields_count = ntohs(*(u2 *)p);
  p += 2;
  // TODO: fields
  u2 methods_count = ntohs(*(u2 *)p);
  p += 2;
  for (u2 i = 0; i < methods_count; i++) {
    u1 *method = p;
    u2 access_flags = ntohs(*(u2 *)p);
    p += 2;
    u2 name_index = ntohs(*(u2 *)p);
    // TODO: relocate
    p += 2;
    u2 descriptor_index = ntohs(*(u2 *)p);
    // TODO: relocate
    p += 2;
    u2 attributes_count = ntohs(*(u2 *)p);
    p += 2;
    for (u2 i = 0; i < attributes_count; i++) {
      u2 attributes_name_index = ntohs(*(u2 *)p);
      p += 2;
      u4 attributes_length = ntohl(*(u4 *)p);
      p += 4;
      // skip attribute
      p += attributes_length;
    }
    context_append_method(package, method, p - method);
  }
  return CONTEXT_ERR_OK;
}