#ifndef JIECARDVM_LIBRARY_H
#define JIECARDVM_LIBRARY_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*library_function_handler)();

// well known aid
const u1 JAVACARD_FRAMEWORK_AID[] = {0xa0, 0x00, 0x00, 0x00, 0x62, 0x01, 0x01};
const u1 JAVA_LANG_AID[] = {0xa0, 0x00, 0x00, 0x00, 0x62, 0x00, 0x01};

const u2 JAVACARD_FRAMEWORK_APPLET_TOKEN = 3;

struct library_function {
  const u2 aid_length;
  const u1 *aid;
  const u2 class_token;
  const u2 token;
  const library_function_handler handler;
};

void javacard_framework_applet_init();
void javacard_framework_applet_register();

const struct library_function LIBRARY_FUNCTIONS[] = {
    // javacard/framework/Applet."<init>"
    {sizeof(JAVACARD_FRAMEWORK_AID), JAVACARD_FRAMEWORK_AID,
     JAVACARD_FRAMEWORK_APPLET_TOKEN, 0, javacard_framework_applet_init},
    // javacard/framework/Applet."register"
    {sizeof(JAVACARD_FRAMEWORK_AID), JAVACARD_FRAMEWORK_AID,
     JAVACARD_FRAMEWORK_APPLET_TOKEN, 1, javacard_framework_applet_register},
};

const int LIBRARY_FUNCTION_COUNT =
    sizeof(LIBRARY_FUNCTIONS) / sizeof(struct library_function);

#ifdef __cplusplus
};
#endif

#endif
