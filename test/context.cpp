#include "context.h"
#include "emubd/lfs_emubd.h"
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

TEST_CASE("CTXCreateCAP", "[context]") {
  init();
  bool ret = context_create_cap(&pkg);
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}

TEST_CASE("CTXAppendMethods", "[context]") {
  init();
  u1 bytecodes[] = {0x7B, 0x30, 0x11, 0x01, 0xC8, 0x31, 0x1D, 0x1E,
                    0x41, 0x32, 0x1F, 0x1E, 0x43, 0x30, 0x1D, 0x78};
  bool ret = context_append_method(&pkg, bytecodes, sizeof(bytecodes));
  REQUIRE(ret == CONTEXT_ERR_OK);
  context_append_method(&pkg, bytecodes, sizeof(bytecodes));
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}

TEST_CASE("CTXReadMethods", "[context]") {
  init();
  u1 bytecodes[100];
  ssize_t ret = context_read_method(&pkg, bytecodes, 0, 256);
  REQUIRE(ret == 32);
  REQUIRE(bytecodes[0] == 0x7B);
  REQUIRE(bytecodes[10] == 0x1F);
  REQUIRE(bytecodes[16] == 0x7B);
  finalize();
}

TEST_CASE("CTXDeleteCAP", "[context]") {
  init();
  bool ret = context_delete_cap(&pkg);
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}
