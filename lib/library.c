#include "library.h"
#include "common.h"
#include "globals.h"
#include "rtda.h"
#include "utils.h"
#include "vm.h"

void javacard_framework_Applet_init() {
  // no args, only this
  u2 this = operand_stack_pop(&current_operand_stack);
  DBG_MSG("Applet.<init>(%d)\n", this);
}

void javacard_framework_Applet_register() {
  // no args, only this
  u2 this = operand_stack_pop(&current_operand_stack);
  DBG_MSG("Applet.register(%d)\n", this);
}

void javacard_framework_APDU_setOutgoingAndSend() {
  // two args and this
  u2 length = operand_stack_pop(&current_operand_stack);
  u2 offset = operand_stack_pop(&current_operand_stack);
  u2 this = operand_stack_pop(&current_operand_stack);
  DBG_MSG("APDU.getBuffer(%d, %d, %d)\n", this, offset, length);
  // no return
}

// well known aid
const u1 JAVACARD_FRAMEWORK_AID[] = {0xa0, 0x00, 0x00, 0x00, 0x62, 0x01, 0x01};
const u1 JAVA_LANG_AID[] = {0xa0, 0x00, 0x00, 0x00, 0x62, 0x00, 0x01};

// class tokens
const u2 JAVACARD_FRAMEWORK_APPLET_TOKEN = 3;
const u2 JAVACARD_FRAMEWORK_APDU_TOKEN = 10;

const struct library_function LIBRARY_FUNCTIONS[] = {
    // javacard/framework/Applet."<init>"
    {sizeof(JAVACARD_FRAMEWORK_AID), JAVACARD_FRAMEWORK_AID,
     JAVACARD_FRAMEWORK_APPLET_TOKEN, 0, javacard_framework_Applet_init},
    // javacard/framework/Applet."register"
    {sizeof(JAVACARD_FRAMEWORK_AID), JAVACARD_FRAMEWORK_AID,
     JAVACARD_FRAMEWORK_APPLET_TOKEN, 1, javacard_framework_Applet_register},
    // javacard/framework/APDU."setOutgoingAndSend"
    {sizeof(JAVACARD_FRAMEWORK_AID), JAVACARD_FRAMEWORK_AID,
     JAVACARD_FRAMEWORK_APDU_TOKEN, 8,
     javacard_framework_APDU_setOutgoingAndSend},
};

const int JAVACARD_FRAMEWORK_APPLET_PROCESS_TOKEN = 7;

const int LIBRARY_FUNCTION_COUNT =
    sizeof(LIBRARY_FUNCTIONS) / sizeof(struct library_function);

const struct library_virtual_function LIBRARY_VIRTUAL_FUNCTIONS[] = {
    // javacard/framework/Applet."select"
    {sizeof(JAVACARD_FRAMEWORK_AID), JAVACARD_FRAMEWORK_AID,
     JAVACARD_FRAMEWORK_APPLET_TOKEN, 6},
    // javacard/framework/Applet."process"
    {sizeof(JAVACARD_FRAMEWORK_AID), JAVACARD_FRAMEWORK_AID,
     JAVACARD_FRAMEWORK_APPLET_TOKEN, JAVACARD_FRAMEWORK_APPLET_PROCESS_TOKEN},
};
