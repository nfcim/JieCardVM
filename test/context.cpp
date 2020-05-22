#include "context.h"
#include "bd/lfs_filebd.h"
#include "globals.h"
#include "lfs.h"
#include <catch.hpp>
#include "vm.h"

static struct lfs_config cfg;
static lfs_filebd_t bd;
static package_t pkg;

static void init() {
  cfg.context = &bd;
  cfg.read = &lfs_filebd_read;
  cfg.prog = &lfs_filebd_prog;
  cfg.erase = &lfs_filebd_erase;
  cfg.sync = &lfs_filebd_sync;
  cfg.read_size = 16;
  cfg.prog_size = 16;
  cfg.block_size = 512;
  cfg.block_count = 400;
  cfg.block_cycles = 50000;
  cfg.cache_size = 128;
  cfg.lookahead_size = 16;
  lfs_filebd_create(&cfg, "testctx");
  context_init(&cfg);

  strcpy(pkg.aid_hex, "F00001");
  pkg.aid_hex_length = 6;
}

static void finalize() { lfs_filebd_destroy(&cfg); }

TEST_CASE("context_create_cap", "[context]") {
  init();
  int ret = context_create_cap(&pkg);
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}

TEST_CASE("context_load_class", "[context]") {
  init();
  u1 buffer[1024];
  FILE *fp = fopen("test/Simple.class", "rb");
  u4 length = fread(buffer, sizeof(buffer), 1, fp);
  int ret = context_load_class(&pkg, buffer, length);
  REQUIRE(ret == 0);
  REQUIRE(context_count_constant(&pkg) == 18);
  finalize();
}

TEST_CASE("context_read_constant", "[context]") {
  init();
  u1 buffer[100];
  // first constant is a method ref
  int ret = context_read_constant(&pkg, 0, buffer, 100);
  REQUIRE(ret > 0);
  REQUIRE(buffer[0] == CONSTANT_METHOD_REF);
  finalize();
}

TEST_CASE("context_read_method", "[context]") {
  init();
  u1 buffer[100];
  // first method is <init>
  int ret = context_read_method(&pkg, buffer, 0, 100);
  REQUIRE(ret > 0);
  // access flags = ACC_PUBLIC
  REQUIRE(buffer[1] == 0x01);
  // name is <init>
  u2 name_index = *(u2 *)(buffer + 2);
  ret = context_read_utf8_constant(&pkg, name_index, buffer, sizeof(buffer));
  REQUIRE(ret > 0);
  REQUIRE(memcmp(buffer, "<init>", 6) == 0);
  finalize();
}

TEST_CASE("context_find_method", "[context]") {
  init();
  u2 index;
  // first method is <init>
  int ret = context_find_method(&pkg, &index, "im/nfc/testapplet/Simple", "<init>");
  REQUIRE(ret == CONTEXT_ERR_OK);
  REQUIRE(index == 0);
  // second method is empty()
  ret = context_find_method(&pkg, &index, "im/nfc/testapplet/Simple", "empty");
  REQUIRE(ret == CONTEXT_ERR_OK);
  REQUIRE(index == 1);
  // third method is returnByte()
  ret = context_find_method(&pkg, &index, "im/nfc/testapplet/Simple", "returnByte");
  REQUIRE(ret == CONTEXT_ERR_OK);
  REQUIRE(index == 2);
  // not found
  ret = context_find_method(&pkg, &index, "im/nfc/testapplet/Simple", "nada");
  REQUIRE(ret == CONTEXT_ERR_NOENT);
  finalize();
}

TEST_CASE("context_create_array", "[context]") {
  u1 buffer[512];
  init();
  int ret = context_create_array(&pkg, ARRAY_T_BYTE, 0, 128);
  REQUIRE(ret == 1);
  lfs_file_t f;
  ret = lfs_file_open(&g_lfs, &f, "F00001/a1", LFS_O_RDONLY);
  REQUIRE(ret == 0);
  memset(buffer, 0xFF, sizeof(buffer));
  array_metadata_t metadata;
  ret = lfs_getattr(&g_lfs, "F00001/a1", LFS_ATTR_METADATA, &metadata,
                    sizeof(metadata));
  REQUIRE(ret > 0);
  REQUIRE(metadata.length == 128);
  REQUIRE(metadata.type == ARRAY_T_BYTE);
  ret = lfs_file_read(&g_lfs, &f, buffer, 128);
  REQUIRE(ret == 128);
  for (int i = 0; i < 128; ++i)
    REQUIRE(buffer[i] == 0);

  ret = context_create_array(&pkg, ARRAY_T_SHORT, 0, 255);
  REQUIRE(ret == 2);
  ret = lfs_file_open(&g_lfs, &f, "F00001/a2", LFS_O_RDONLY);
  REQUIRE(ret == 0);
  ret = lfs_getattr(&g_lfs, "F00001/a2", LFS_ATTR_METADATA, &metadata,
                    sizeof(metadata));
  REQUIRE(ret > 0);
  REQUIRE(metadata.length == 255);
  REQUIRE(metadata.type == ARRAY_T_SHORT);
  memset(buffer, 0xFF, sizeof(buffer));
  ret = lfs_file_read(&g_lfs, &f, buffer, 512);
  REQUIRE(ret == 510);
  for (int i = 0; i < 510; ++i)
    REQUIRE(buffer[i] == 0);
}

TEST_CASE("context_write_array", "[context]") {
  init();
  int ret = context_write_array(&pkg, 1, ARRAY_T_BYTE, 1, 6);
  REQUIRE(ret == CONTEXT_ERR_OK);

  ret = context_write_array(&pkg, 2, ARRAY_T_SHORT, 128, 167);
  REQUIRE(ret == CONTEXT_ERR_OK);
}

TEST_CASE("context_read_array", "[context]") {
  init();
  jbyte b;
  jshort s;
  int ret = context_read_array(&pkg, 1, ARRAY_T_BYTE, 1, (u1 *)&b);
  REQUIRE(ret == CONTEXT_ERR_OK);
  REQUIRE(b == 6);

  ret = context_read_array(&pkg, 1, ARRAY_T_BYTE, 256, (u1 *)&b);
  REQUIRE(ret == CONTEXT_ERR_NOENT);
  ret = context_read_array(&pkg, 2, ARRAY_T_SHORT, 128, (u1 *)&s);
  REQUIRE(ret == CONTEXT_ERR_OK);
  REQUIRE(s == 167);

  ret = context_read_array(&pkg, 2, ARRAY_T_SHORT, 300, (u1 *)&s);
  REQUIRE(ret == CONTEXT_ERR_NOENT);
}

/*
TEST_CASE("context_create_constant_pool", "[context]") {
  init();
  u1 data[64] = {0x02, 0x00, 0x0E, 0x00, 0x02, 0x00, 0x0E, 0x01, 0x06, 0x00,
                 0x00, 0x16, 0x06, 0x81, 0x03, 0x00, 0x06, 0x81, 0x08, 0x0D,
                 0x06, 0x80, 0x00, 0x00, 0x06, 0x82, 0x0E, 0x00, 0x01, 0x00,
                 0x0E, 0x00, 0x03, 0x81, 0x03, 0x01, 0x03, 0x81, 0x0A, 0x01,
                 0x03, 0x82, 0x0E, 0x01, 0x06, 0x81, 0x10, 0x02, 0x03, 0x81,
                 0x0A, 0x08, 0x03, 0x00, 0x0E, 0x08, 0x03, 0x00, 0x0E, 0x0A,
                 0x05, 0x00, 0x00, 0x00};
  int ret = context_append_constant(&pkg, data, sizeof(data));
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}

TEST_CASE("context_read_constant_pool", "[context]") {
  init();
  cp_info info;
  int ret = context_read_constant(&pkg, 0, (u1 *)&info, sizeof(cp_info));
  REQUIRE(ret == sizeof(cp_info));
  REQUIRE(info.tag == CONSTANT_INSTANCE_FIELD_REF);
  REQUIRE(info.instance_field.token == 0);
  finalize();
}

TEST_CASE("context_create_static_image", "[context]") {
  init();
  u1 data[2] = {0x00, 0x00};
  int ret = context_create_static_image(&pkg, data, sizeof(data));
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}

TEST_CASE("context_write_static_image", "[context]") {
  init();
  int ret = context_write_static_image(&pkg, 0, ELEM_SIZE_2, 12345);
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}

TEST_CASE("context_read_static_image", "[context]") {
  init();
  jshort val;
  int ret = context_read_static_image(&pkg, 0, ELEM_SIZE_2, (u1 *)&val);
  REQUIRE(ret == CONTEXT_ERR_OK);
  REQUIRE(val == 12345);
  finalize();
}

TEST_CASE("context_resolve_static_field", "[context]") {
  init();
  jshort val;
  int ret = context_resolve_static_field(&pkg, 15, ELEM_SIZE_2, (u1 *)&val);
  REQUIRE(ret == CONTEXT_ERR_OK);
  REQUIRE(val == 12345);
  finalize();
}
*/

TEST_CASE("vm_execute_static_method", "[context]") {
  init();
  int ret = vm_execute_static_method(&pkg, "im/nfc/testapplet/Simple", "returnByte");
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}

TEST_CASE("context_delete_cap", "[context]") {
  init();
  int ret = context_delete_cap(&pkg);
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}
