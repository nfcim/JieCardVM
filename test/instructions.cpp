#include <catch.hpp>
#include <context.h>
#include <cstring>
#include <emubd/lfs_emubd.h>
#include <globals.h>
#include <instructions.h>
#include <rtda.h>
#include <vm.h>

static u2 operands[100];
static jshort vt[100];
static u1 bytecodes[100];
static frame_t frame;
static struct lfs_config cfg;
static lfs_emubd_t bd;

static void init() {
  frame.operand_stack.base = operands;
  frame.operand_stack.index = 0;
  frame.variable_table.base = vt;
  bytecode_set(bytecodes);

  memset(&cfg, 0, sizeof(cfg));
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
  lfs_emubd_create(&cfg, "testins");
  context_init(&cfg);

  strcpy(current_package.aid_hex, "F00001");
  current_package.aid_hex_length = 6;
}

static void finalize() {
  lfs_unmount(&g_lfs);
  lfs_emubd_destroy(&cfg);
}

TEST_CASE("aconst_null", "[instructions]") {
  init();
  ins_aconst_null(&frame);
  REQUIRE((u2)operand_stack_get(&frame.operand_stack) == JNULL);
  finalize();
}

TEST_CASE("sconst_m1", "[instructions]") {
  init();
  ins_sconst_m1(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == -1);
  finalize();
}

TEST_CASE("sconst_0", "[instructions]") {
  init();
  ins_sconst_0(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 0);
  finalize();
}

TEST_CASE("sconst_1", "[instructions]") {
  init();
  ins_sconst_1(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 1);
  finalize();
}

TEST_CASE("sconst_2", "[instructions]") {
  init();
  ins_sconst_2(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 2);
  finalize();
}

TEST_CASE("sconst_3", "[instructions]") {
  init();
  ins_sconst_3(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 3);
  finalize();
}

TEST_CASE("sconst_4", "[instructions]") {
  init();
  ins_sconst_4(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 4);
  finalize();
}

TEST_CASE("sconst_5", "[instructions]") {
  init();
  ins_sconst_5(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 5);
  finalize();
}

TEST_CASE("bspush", "[instructions]") {
  init();
  bytecodes[0] = 0x5A;
  ins_bspush(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 0x5A);
  finalize();
}

TEST_CASE("sspush", "[instructions]") {
  init();
  bytecodes[0] = 0x5A;
  bytecodes[1] = 0xA5;
  ins_sspush(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 0x5AA5);
  finalize();
}

TEST_CASE("asload", "[instructions]") {
  init();
  bytecodes[0] = 2;
  vt[2] = 0x5A5A;
  ins_asload(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 0x5A5A);
  finalize();
}

TEST_CASE("asload_0", "[instructions]") {
  init();
  vt[0] = 0x5A5A;
  ins_asload_0(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 0x5A5A);
  finalize();
}

TEST_CASE("asload_1", "[instructions]") {
  init();
  vt[1] = 0x5A5A;
  ins_asload_1(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 0x5A5A);
  finalize();
}

TEST_CASE("asload_2", "[instructions]") {
  init();
  vt[2] = 0x5A5A;
  ins_asload_2(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 0x5A5A);
  finalize();
}

TEST_CASE("asload_3", "[instructions]") {
  init();
  vt[3] = 0x5A5A;
  ins_asload_3(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 0x5A5A);
  finalize();
}

TEST_CASE("aaload", "[instructions]") {
  init();
  context_create_cap(&current_package);
  int ref = context_create_array(&current_package, ARRAY_T_REFERENCE, 0, 10);
  context_write_array(&current_package, ref, ARRAY_T_REFERENCE, 2, 0x5A);
  operand_stack_push(&frame.operand_stack, ref);
  operand_stack_push(&frame.operand_stack, 2);
  ins_aaload(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 0x5A);
  context_delete_cap(&current_package);
  finalize();
}

TEST_CASE("baload", "[instructions]") {
  // TODO
}

TEST_CASE("saload", "[instructions]") {
  // TODO
}

TEST_CASE("asstore", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0xA55A);
  bytecodes[0] = 4;
  ins_asstore(&frame);
  REQUIRE(variable_table_get(&frame.variable_table, 4) == (jshort)0xA55A);
  finalize();
}

TEST_CASE("asstore_0", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0xA55A);
  ins_asstore_0(&frame);
  REQUIRE(variable_table_get(&frame.variable_table, 0) == (jshort)0xA55A);
  finalize();
}

TEST_CASE("asstore_1", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0xA55A);
  ins_asstore_1(&frame);
  REQUIRE(variable_table_get(&frame.variable_table, 1) == (jshort)0xA55A);
  finalize();
}

TEST_CASE("asstore_2", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0xA55A);
  ins_asstore_2(&frame);
  REQUIRE(variable_table_get(&frame.variable_table, 2) == (jshort)0xA55A);
  finalize();
}

TEST_CASE("asstore_3", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0xA55A);
  ins_asstore_3(&frame);
  REQUIRE(variable_table_get(&frame.variable_table, 3) == (jshort)0xA55A);
  finalize();
}

TEST_CASE("aastore", "[instructions]") {
  // TODO
}

TEST_CASE("bastore", "[instructions]") {
  // TODO
}

TEST_CASE("sastore", "[instructions]") {
  // TODO
}

TEST_CASE("pop", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0xA55A);
  ins_pop(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("pop2", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0xA55A);
  operand_stack_push(&frame.operand_stack, (jshort)0xA55A);
  ins_pop2(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("dup", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0xA55A);
  operand_stack_push(&frame.operand_stack, (jshort)0xA55A);
  ins_dup(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0xA55A);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0xA55A);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0xA55A);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("dup2", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0x5A5A);
  operand_stack_push(&frame.operand_stack, (jshort)0xA55A);
  ins_dup2(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0xA55A);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x5A5A);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0xA55A);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x5A5A);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}
