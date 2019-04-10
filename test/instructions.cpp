#include "../lib/instruction.h"
#include "../lib/rtda.h"
#include <catch.hpp>
#include <cstring>

u2 operands[100];
jshort vt[100];
u1 bytecodes[100];

Frame frame;

void init() {
  frame.operandStack.base = operands;
  frame.operandStack.next = operands;
  frame.variableTable.base = vt;
  PCSet(bytecodes);
}

TEST_CASE("aconst_null", "[instructions]") {
  init();
  ins_aconst_null(&frame);
  REQUIRE((u2)OSGet(&frame.operandStack) == JNULL);
}

TEST_CASE("sconst_m1", "[instructions]") {
  init();
  ins_sconst_m1(&frame);
  REQUIRE(OSGet(&frame.operandStack) == -1);
}

TEST_CASE("sconst_0", "[instructions]") {
  init();
  ins_sconst_0(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 0);
}

TEST_CASE("sconst_1", "[instructions]") {
  init();
  ins_sconst_1(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 1);
}

TEST_CASE("sconst_2", "[instructions]") {
  init();
  ins_sconst_2(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 2);
}

TEST_CASE("sconst_3", "[instructions]") {
  init();
  ins_sconst_3(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 3);
}

TEST_CASE("sconst_4", "[instructions]") {
  init();
  ins_sconst_4(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 4);
}

TEST_CASE("sconst_5", "[instructions]") {
  init();
  ins_sconst_5(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 5);
}

TEST_CASE("bspush", "[instructions]") {
  init();
  bytecodes[0] = 0x5A;
  ins_bspush(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 0x5A);
}

TEST_CASE("sspush", "[instructions]") {
  init();
  bytecodes[0] = 0x5A;
  bytecodes[1] = 0xA5;
  ins_sspush(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 0x5AA5);
}

TEST_CASE("asload", "[instructions]") {
  init();
  bytecodes[0] = 2;
  vt[2] = 0x5A5A;
  ins_asload(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 0x5A5A);
}

TEST_CASE("asload_0", "[instructions]") {
  init();
  vt[0] = 0x5A5A;
  ins_asload_0(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 0x5A5A);
}

TEST_CASE("asload_1", "[instructions]") {
  init();
  vt[1] = 0x5A5A;
  ins_asload_1(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 0x5A5A);
}

TEST_CASE("asload_2", "[instructions]") {
  init();
  vt[2] = 0x5A5A;
  ins_asload_2(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 0x5A5A);
}

TEST_CASE("asload_3", "[instructions]") {
  init();
  vt[3] = 0x5A5A;
  ins_asload_3(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 0x5A5A);
}

TEST_CASE("aaload", "[instructions]") {
  // TODO
}

TEST_CASE("baload", "[instructions]") {
  // TODO
}

TEST_CASE("saload", "[instructions]") {
  // TODO
}

TEST_CASE("asstore", "[instructions]") {
  init();
  OSPush(&frame.operandStack, (jshort) 0xA55A);
  bytecodes[0] = 4;
  ins_asstore(&frame);
  REQUIRE(VTGet(&frame.variableTable, 4) == (jshort) 0xA55A);
}

TEST_CASE("asstore_0", "[instructions]") {
  init();
  OSPush(&frame.operandStack, (jshort) 0xA55A);
  ins_asstore_0(&frame);
  REQUIRE(VTGet(&frame.variableTable, 0) == (jshort) 0xA55A);
}

TEST_CASE("asstore_1", "[instructions]") {
  init();
  OSPush(&frame.operandStack, (jshort) 0xA55A);
  ins_asstore_1(&frame);
  REQUIRE(VTGet(&frame.variableTable, 1) == (jshort) 0xA55A);
}

TEST_CASE("asstore_2", "[instructions]") {
  init();
  OSPush(&frame.operandStack, (jshort) 0xA55A);
  ins_asstore_2(&frame);
  REQUIRE(VTGet(&frame.variableTable, 2) == (jshort) 0xA55A);
}

TEST_CASE("asstore_3", "[instructions]") {
  init();
  OSPush(&frame.operandStack, (jshort) 0xA55A);
  ins_asstore_3(&frame);
  REQUIRE(VTGet(&frame.variableTable, 3) == (jshort) 0xA55A);
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
  OSPush(&frame.operandStack, (jshort) 0xA55A);
  ins_pop(&frame);
  REQUIRE(frame.operandStack.base == frame.operandStack.next);
}

TEST_CASE("pop2", "[instructions]") {
  init();
  OSPush(&frame.operandStack, (jshort) 0xA55A);
  OSPush(&frame.operandStack, (jshort) 0xA55A);
  ins_pop2(&frame);
  REQUIRE(frame.operandStack.base == frame.operandStack.next);
}

TEST_CASE("dup", "[instructions]") {
  init();
  OSPush(&frame.operandStack, (jshort) 0xA55A);
  OSPush(&frame.operandStack, (jshort) 0xA55A);
  ins_dup(&frame);
  REQUIRE(OSPop(&frame.operandStack) == (jshort) 0xA55A);
  REQUIRE(OSPop(&frame.operandStack) == (jshort) 0xA55A);
  REQUIRE(OSPop(&frame.operandStack) == (jshort) 0xA55A);
  REQUIRE(frame.operandStack.base == frame.operandStack.next);
}

TEST_CASE("dup2", "[instructions]") {
  init();
  OSPush(&frame.operandStack, (jshort) 0x5A5A);
  OSPush(&frame.operandStack, (jshort) 0xA55A);
  ins_dup2(&frame);
  REQUIRE(OSPop(&frame.operandStack) == (jshort) 0xA55A);
  REQUIRE(OSPop(&frame.operandStack) == (jshort) 0x5A5A);
  REQUIRE(OSPop(&frame.operandStack) == (jshort) 0xA55A);
  REQUIRE(OSPop(&frame.operandStack) == (jshort) 0x5A5A);
  REQUIRE(frame.operandStack.base == frame.operandStack.next);
}
