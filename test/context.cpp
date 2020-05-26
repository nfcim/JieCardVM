#include "context.h"
#include "bd/lfs_filebd.h"
#include "globals.h"
#include "lfs.h"
#include <catch.hpp>
#include <unistd.h>
#include "vm.h"

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
  REQUIRE(context_count_constant(&current_package) == 11);
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

TEST_CASE("context_read_constant", "[context]") {
  init();
  u1 buffer[100];
  int ret = context_read_constant(&pkg, 0, buffer, 100);
  REQUIRE(ret > 0);
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
  i1 b;
  i2 s;
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
