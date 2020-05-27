#ifndef JIECARDVM_LIBRARY_H
#define JIECARDVM_LIBRARY_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*library_function_handler)();

const u1 JAVACARD_FRAMEWORK_AID[] = {0xa0, 0x00, 0x00, 0x00, 0x62, 0x01, 0x01};

struct library_function {
  const u2 aid_length;
  const u1 *aid;
  const u2 class_token;
  const u2 token;
  const library_function_handler handler;
};

void javacard_framework_applet_init();

const struct library_function LIBRARY_FUNCTIONS[] = {
    {sizeof(JAVACARD_FRAMEWORK_AID), JAVACARD_FRAMEWORK_AID, 3, 0,
     javacard_framework_applet_init}};

const int LIBRARY_FUNCTION_COUNT =
    sizeof(LIBRARY_FUNCTIONS) / sizeof(struct library_function);

#ifdef __cplusplus
};
#endif

#endif
