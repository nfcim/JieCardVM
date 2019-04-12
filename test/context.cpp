#include "context.h"
#include "emubd/lfs_emubd.h"
#include "lfs.h"
#include <catch.hpp>

struct lfs_config cfg;
lfs_emubd_t bd;

static void init() {
  cfg.context = &bd;
  cfg.read = &lfs_emubd_read;
  cfg.prog = &lfs_emubd_prog;
  cfg.erase = &lfs_emubd_erase;
  cfg.sync = &lfs_emubd_sync;
  cfg.read_size = 128;
  cfg.prog_size = 128;
  cfg.block_size = 1024;
  cfg.block_count = 1024;
  cfg.lookahead = 32;
  lfs_emubd_create(&cfg, "testdata");
  CTXInit(&cfg);
}

static void finalize() { lfs_emubd_destroy(&cfg); }

TEST_CASE("CTXCreateCAP", "[context]") {
  init();
  bool ret = CTXCreateCAP((u1 *)"\xF0\x00\x01", 3);
  REQUIRE(ret == true);
  finalize();
}

TEST_CASE("CTXAppendMethods", "[context]") {
  init();
  u1 bytecodes[] = {0x7B, 0x30, 0x11, 0x01, 0xC8, 0x31, 0x1D, 0x1E,
                    0x41, 0x32, 0x1F, 0x1E, 0x43, 0x30, 0x1D, 0x78};
  bool ret = CTXAppendMethods((u1 *)"\xF0\x00\x01", 3, bytecodes, sizeof(bytecodes));
  REQUIRE(ret == true);
  CTXAppendMethods((u1 *)"\xF0\x00\x01", 3, bytecodes, sizeof(bytecodes));
  REQUIRE(ret == true);
  finalize();
}

TEST_CASE("CTXReadMethods", "[context]") {
  init();
  u1 bytecodes[100];
  ssize_t ret = CTXReadMethods((u1 *)"\xF0\x00\x01", 3, bytecodes, 0, 256);
  REQUIRE(ret == 32);
  REQUIRE(bytecodes[0] == 0x7B);
  REQUIRE(bytecodes[10] == 0x1F);
  REQUIRE(bytecodes[16] == 0x7B);
  finalize();
}

TEST_CASE("CTXDeleteCAP", "[context]") {
  init();
  bool ret = CTXDeleteCAP((u1 *)"\xF0\x00\x01", 3);
  REQUIRE(ret == true);
  finalize();
}
