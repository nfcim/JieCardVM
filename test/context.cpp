#include "context.h"
#include "emubd/lfs_emubd.h"
#include "globals.h"
#include "lfs.h"
#include <catch.hpp>

static struct lfs_config cfg;
static lfs_emubd_t bd;
static package_t pkg;

static void init() {
  cfg.context = &bd;
  cfg.read = &lfs_emubd_read;
  cfg.prog = &lfs_emubd_prog;
  cfg.erase = &lfs_emubd_erase;
  cfg.sync = &lfs_emubd_sync;
  cfg.read_size = 16;
  cfg.prog_size = 16;
  cfg.block_size = 512;
  cfg.block_count = 400;
  cfg.block_cycles = 50000;
  cfg.cache_size = 128;
  cfg.lookahead_size = 16;
  lfs_emubd_create(&cfg, "testctx");
  context_init(&cfg);

  strcpy(pkg.aid_hex, "F00001");
  pkg.aid_hex_length = 6;
}

static void finalize() { lfs_emubd_destroy(&cfg); }

TEST_CASE("context_create_cap", "[context]") {
  init();
  int ret = context_create_cap(&pkg);
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}

TEST_CASE("context_append_method", "[context]") {
  init();
  u1 bytecodes[] = {0x7B, 0x30, 0x11, 0x01, 0xC8, 0x31, 0x1D, 0x1E,
                    0x41, 0x32, 0x1F, 0x1E, 0x43, 0x30, 0x1D, 0x78};
  int ret = context_append_method(&pkg, bytecodes, sizeof(bytecodes));
  REQUIRE(ret == CONTEXT_ERR_OK);
  context_append_method(&pkg, bytecodes, sizeof(bytecodes));
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}

TEST_CASE("context_read_method", "[context]") {
  init();
  u1 bytecodes[100];
  int ret = context_read_method(&pkg, bytecodes, 0, 256);
  REQUIRE(ret == 32);
  REQUIRE(bytecodes[0] == 0x7B);
  REQUIRE(bytecodes[10] == 0x1F);
  REQUIRE(bytecodes[16] == 0x7B);
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

TEST_CASE("context_delete_cap", "[context]") {
  init();
  int ret = context_delete_cap(&pkg);
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}
