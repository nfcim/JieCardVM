#include "../lib/instruction.h"
#include "../lib/rtda.h"
#include <catch.hpp>

u2 operands[100];
jshort vt[100];

Frame frame;

void init() {
  frame.operandStack.base = operands;
  frame.operandStack.next = operands;
  frame.variableTable.base = vt;
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
  ins_bspush(&frame);
  REQUIRE(OSGet(&frame.operandStack) == 5);
}
