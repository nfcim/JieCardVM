#include "vm.h"
#include "bd/lfs_filebd.h"
#include "context.h"
#include "globals.h"
#include "lfs.h"
#include <catch.hpp>
#include <unistd.h>

static struct lfs_config cfg;
static lfs_filebd_t bd;
static package_t pkg;
static bool first = true;

static void init() {
  if (first) {
    unlink("testctx");
    first = false;
  }

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
  lfs_filebd_create(&cfg, "test_vm");
  context_init(&cfg);

  strcpy(pkg.aid_hex, "F00001");
  pkg.aid_hex_length = 6;
}

static void finalize() { lfs_filebd_destroy(&cfg); }

TEST_CASE("vm_create_cap", "[context]") {
  init();
  int ret = context_create_cap(&pkg);
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}

TEST_CASE("vm_load_method", "[context]") {
  init();
  u1 buffer[1024];
  FILE *fp = fopen("test/Method.cap", "rb");
  REQUIRE(fp != NULL);
  u4 length = fread(buffer, 1, sizeof(buffer), fp);
  int ret = vm_load_method(buffer, length);
  REQUIRE(ret == 0);
  fclose(fp);
  finalize();
}

TEST_CASE("vm_load_constant_pool", "[context]") {
  init();
  u1 buffer[1024];
  FILE *fp = fopen("test/ConstantPool.cap", "rb");
  REQUIRE(fp != NULL);
  u4 length = fread(buffer, 1, sizeof(buffer), fp);
  int ret = vm_load_constant_pool(buffer, length);
  REQUIRE(ret == 0);
  fclose(fp);
  finalize();
}

TEST_CASE("vm_load_applet", "[context]") {
  init();
  u1 buffer[1024];
  FILE *fp = fopen("test/Applet.cap", "rb");
  REQUIRE(fp != NULL);
  u4 length = fread(buffer, 1, sizeof(buffer), fp);
  int ret = vm_load_applet(buffer, length);
  REQUIRE(ret == 0);
  fclose(fp);
  finalize();
}

TEST_CASE("vm_load_classes", "[context]") {
  init();
  u1 buffer[1024];
  FILE *fp = fopen("test/Class.cap", "rb");
  REQUIRE(fp != NULL);
  u4 length = fread(buffer, 1, sizeof(buffer), fp);
  int ret = vm_load_class(buffer, length);
  REQUIRE(ret == 0);
  fclose(fp);
  finalize();
}

TEST_CASE("vm_install_applet_empty", "[context]") {
  init();
  // EmptyApplet
  u1 aid[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00, 0x02};
  int res = vm_install_applet(aid, sizeof(aid));
  REQUIRE(res == 0);
  finalize();
}

TEST_CASE("vm_install_applet_simple", "[context]") {
  init();
  // SimpleApplet
  u1 aid[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00, 0x03};
  int res = vm_install_applet(aid, sizeof(aid));
  REQUIRE(res == 0);
  finalize();
}

TEST_CASE("vm_delete_cap", "[context]") {
  init();
  int ret = context_delete_cap(&pkg);
  REQUIRE(ret == CONTEXT_ERR_OK);
  finalize();
}
