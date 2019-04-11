#include "context.h"
#include "emubd/lfs_emubd.h"
#include "lfs.h"
#include <catch.hpp>

struct lfs_config cfg;
lfs_emubd_t bd;

static void init() {
  cfg.context = &bd;
  cfg.read =  &lfs_emubd_read;
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

static void finalize() {
  lfs_emubd_destroy(&cfg);
}

TEST_CASE("CTXCreateCAP", "[context]") {
  init();
  bool ret = CTXCreateCAP((u1 *)"\xF0\x00\x01", 3);
  REQUIRE(ret == true);
  finalize();
}

TEST_CASE("CTXDeleteCAP", "[context]") {
  init();
  bool ret = CTXDeleteCAP((u1 *)"\xF0\x00\x01", 3);
  REQUIRE(ret == true);
  finalize();
}
