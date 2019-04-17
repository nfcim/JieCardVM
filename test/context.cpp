#include "context.h"
#include "emubd/lfs_emubd.h"
#include "globals.h"
#include "lfs.h"
#include <catch.hpp>

struct lfs_config cfg;
lfs_emubd_t bd;
package_t pkg;

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
  lfs_emubd_create(&cfg, "testdata");
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
  int ret = context_create_array(&pkg, 128);
  REQUIRE(ret == 1);
  lfs_file_t f;
  ret = lfs_file_open(&g_lfs, &f, "F00001/a1", LFS_O_RDONLY);
  REQUIRE(ret == 0);
  memset(buffer, 0xFF, sizeof(buffer));
  ret = lfs_file_read(&g_lfs, &f, buffer, 128);
  REQUIRE(ret == 128);
  for (int i = 0; i < 128; ++i)
    REQUIRE(buffer[i] == 0);

  ret = context_create_array(&pkg, 511);
  REQUIRE(ret == 2);
  ret = lfs_file_open(&g_lfs, &f, "F00001/a2", LFS_O_RDONLY);
  REQUIRE(ret == 0);
  memset(buffer, 0xFF, sizeof(buffer));
  ret = lfs_file_read(&g_lfs, &f, buffer, 512);
  REQUIRE(ret == 511);
  for (int i = 0; i < 511; ++i)
    REQUIRE(buffer[i] == 0);
}

TEST_CASE("context_read_array", "[context]") {
  init();
  jbyte data;
  int ret = context_read_array(&pkg, 1, ARRAY_T_BYTE, 1, (u1 *) &data);
  REQUIRE(ret == CONTEXT_ERR_OK);
  REQUIRE(data == 0);
  ret = context_read_array(&pkg, 1, ARRAY_T_BYTE, 256, (u1 *) &data);
  REQUIRE(ret == CONTEXT_ERR_NOENT);
}

TEST_CASE("context_delete_cap", "[context]") {
  init();
  int ret = context_delete_cap(&pkg);
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}
