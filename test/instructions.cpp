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
  init();
  context_create_cap(&current_package);
  int ref = context_create_array(&current_package, ARRAY_T_BYTE, 0, 10);
  context_write_array(&current_package, ref, ARRAY_T_BYTE, 3, 0x5A);
  operand_stack_push(&frame.operand_stack, ref);
  operand_stack_push(&frame.operand_stack, 3);
  ins_baload(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 0x5A);
  context_delete_cap(&current_package);
  finalize();
}

TEST_CASE("saload", "[instructions]") {
  init();
  context_create_cap(&current_package);
  int ref = context_create_array(&current_package, ARRAY_T_SHORT, 0, 10);
  context_write_array(&current_package, ref, ARRAY_T_SHORT, 5, 0x52A);
  operand_stack_push(&frame.operand_stack, ref);
  operand_stack_push(&frame.operand_stack, 5);
  ins_saload(&frame);
  REQUIRE(operand_stack_get(&frame.operand_stack) == 0x52A);
  context_delete_cap(&current_package);
  finalize();
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

TEST_CASE("dup_x", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0x1);
  operand_stack_push(&frame.operand_stack, (jshort)0x2);
  operand_stack_push(&frame.operand_stack, (jshort)0x3);
  operand_stack_push(&frame.operand_stack, (jshort)0x4);
  bytecodes[0] = 0x14;
  ins_dup_x(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x4);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x3);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x2);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x1);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x4);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("swap_x", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0x1);
  operand_stack_push(&frame.operand_stack, (jshort)0x2);
  operand_stack_push(&frame.operand_stack, (jshort)0x3);
  operand_stack_push(&frame.operand_stack, (jshort)0x4);
  bytecodes[0] = 0x12;
  ins_swap_x(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x3);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x2);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x4);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x1);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("sadd", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0x1);
  operand_stack_push(&frame.operand_stack, (jshort)0x2);
  ins_sadd(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x3);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("ssub", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0x1);
  operand_stack_push(&frame.operand_stack, (jshort)0x2);
  ins_ssub(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)-0x1);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("smul", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0x10);
  operand_stack_push(&frame.operand_stack, (jshort)0x20);
  ins_smul(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x200);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("sdiv", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0x10);
  operand_stack_push(&frame.operand_stack, (jshort)0x20);
  ins_sdiv(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x0);
  REQUIRE(frame.operand_stack.index == 0);
  operand_stack_push(&frame.operand_stack, (jshort)-32768);
  operand_stack_push(&frame.operand_stack, (jshort)-1);
  ins_sdiv(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)-32768);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("srem", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)10);
  operand_stack_push(&frame.operand_stack, (jshort)20);
  ins_srem(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)10);
  REQUIRE(frame.operand_stack.index == 0);
  operand_stack_push(&frame.operand_stack, (jshort)-21);
  operand_stack_push(&frame.operand_stack, (jshort)-4);
  ins_srem(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)-1);
  REQUIRE(frame.operand_stack.index == 0);
  operand_stack_push(&frame.operand_stack, (jshort)-32768);
  operand_stack_push(&frame.operand_stack, (jshort)-1);
  ins_srem(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("sneg", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0);
  ins_sneg(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0);
  REQUIRE(frame.operand_stack.index == 0);
  operand_stack_push(&frame.operand_stack, (jshort)-10);
  ins_sneg(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)10);
  REQUIRE(frame.operand_stack.index == 0);
  operand_stack_push(&frame.operand_stack, (jshort)-32768);
  ins_sneg(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)-32768);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("sshl", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0x10);
  operand_stack_push(&frame.operand_stack, (jshort)1);
  ins_sshl(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x20);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("sshr", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0x21);
  operand_stack_push(&frame.operand_stack, (jshort)1);
  ins_sshr(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x10);
  REQUIRE(frame.operand_stack.index == 0);
  operand_stack_push(&frame.operand_stack, (jshort)-129);
  operand_stack_push(&frame.operand_stack, (jshort)1);
  ins_sshr(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)-65);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("sushr", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0x21);
  operand_stack_push(&frame.operand_stack, (jshort)1);
  ins_sushr(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x10);
  REQUIRE(frame.operand_stack.index == 0);
  operand_stack_push(&frame.operand_stack, (jshort)-129);
  operand_stack_push(&frame.operand_stack, (jshort)1);
  ins_sushr(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)32703);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("sand", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0x21);
  operand_stack_push(&frame.operand_stack, (jshort)0x1F);
  ins_sand(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x1);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("sor", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0x21);
  operand_stack_push(&frame.operand_stack, (jshort)0x1F);
  ins_sor(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x3F);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("sxor", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)0x21);
  operand_stack_push(&frame.operand_stack, (jshort)0x1F);
  ins_sxor(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jshort)0x3E);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("sinc", "[instructions]") {
  init();
  bytecodes[0] = 2;
  bytecodes[1] = 100;
  vt[2] = 123;
  ins_sinc(&frame);
  REQUIRE(variable_table_get(&frame.variable_table, 2) == (jshort)223);
  finalize();
}

TEST_CASE("s2b", "[instructions]") {
  init();
  operand_stack_push(&frame.operand_stack, (jshort)999);
  ins_s2b(&frame);
  REQUIRE(operand_stack_pop(&frame.operand_stack) == (jbyte)0xE7);
  REQUIRE(frame.operand_stack.index == 0);
  finalize();
}

TEST_CASE("ifeq", "[instructions]") {
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)0);
  ins_ifeq(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 2);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)2);
  ins_ifeq(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
}

TEST_CASE("ifne", "[instructions]") {
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)0);
  ins_ifne(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)2);
  ins_ifne(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 2);
  finalize();
}

TEST_CASE("iflt", "[instructions]") {
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)-5);
  ins_iflt(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)2);
  ins_iflt(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
}

TEST_CASE("ifge", "[instructions]") {
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)0);
  ins_ifge(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)3);
  ins_ifge(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)-1);
  ins_ifge(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
}

TEST_CASE("ifgt", "[instructions]") {
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)8);
  ins_ifgt(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)0);
  ins_ifgt(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
}

TEST_CASE("ifle", "[instructions]") {
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)0);
  ins_ifle(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)-3);
  ins_ifle(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)1);
  ins_ifle(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
}

TEST_CASE("ifnull", "[instructions]") {
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)0);
  ins_ifnull(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)10);
  ins_ifnull(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
}

TEST_CASE("ifnonnull", "[instructions]") {
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)8);
  ins_ifnonnull(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)0);
  ins_ifnonnull(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
}

TEST_CASE("if_ascmpeq", "[instructions]") {
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)8);
  operand_stack_push(&frame.operand_stack, (jshort)8);
  ins_if_ascmpeq(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)0);
  operand_stack_push(&frame.operand_stack, (jshort)8);
  ins_if_ascmpeq(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
}

TEST_CASE("if_ascmpne", "[instructions]") {
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)0);
  operand_stack_push(&frame.operand_stack, (jshort)8);
  ins_if_ascmpne(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)8);
  operand_stack_push(&frame.operand_stack, (jshort)8);
  ins_if_ascmpne(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
}

TEST_CASE("if_scmplt", "[instructions]") {
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)0);
  operand_stack_push(&frame.operand_stack, (jshort)8);
  ins_if_scmplt(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)8);
  operand_stack_push(&frame.operand_stack, (jshort)8);
  ins_if_scmplt(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
}

TEST_CASE("if_scmpge", "[instructions]") {
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)8);
  operand_stack_push(&frame.operand_stack, (jshort)8);
  ins_if_scmpge(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)8);
  operand_stack_push(&frame.operand_stack, (jshort)9);
  ins_if_scmpge(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
}

TEST_CASE("if_scmpgt", "[instructions]") {
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)9);
  operand_stack_push(&frame.operand_stack, (jshort)8);
  ins_if_scmpgt(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)8);
  operand_stack_push(&frame.operand_stack, (jshort)8);
  ins_if_scmpgt(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
}

TEST_CASE("if_scmple", "[instructions]") {
  init();
  bytecodes[0] = 3;
  operand_stack_push(&frame.operand_stack, (jshort)8);
  operand_stack_push(&frame.operand_stack, (jshort)8);
  ins_if_scmple(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 3);
  finalize();
  init();
  bytecodes[0] = 2;
  operand_stack_push(&frame.operand_stack, (jshort)8);
  operand_stack_push(&frame.operand_stack, (jshort)0);
  ins_if_scmple(&frame);
  REQUIRE(frame.operand_stack.index == 0);
  REQUIRE(_bytecode_get_index() == 1);
  finalize();
}

TEST_CASE("goto", "[instructions]") {
  init();
  bytecodes[0] = 5;
  ins_goto(&frame);
  REQUIRE(_bytecode_get_index() == 5);
  finalize();
}

TEST_CASE("jsr", "[instructions]") {
  // TODO
}

TEST_CASE("ret", "[instructions]") {
  // TODO
}
